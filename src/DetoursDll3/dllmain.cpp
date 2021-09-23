// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "framework.h"
#include "windows.h"
#include "detours.h"
#include "stdio.h"
#include "stdarg.h"
#include <iostream>
#include <json.hpp>
#include <atlbase.h>
#include <direct.h>
#include <sys/timeb.h>
#include <winsock.h>
#include <string>
#include "base64.h"


using json = nlohmann::json;
using namespace std;
#pragma comment(lib,"detours.lib")
#pragma comment(lib,"ws2_32.lib")
SYSTEMTIME st;
HANDLE hPipe;
BOOL Disable_Detours = FALSE;
BOOL Trace[10000] = { 0 };
string currentFileName,currentWorkDir;
//HANDLE currentHeap;

void trace(HANDLE hHandle) {
    for (int i = 0; i < 10000; i++) {
        if (Trace[i] <= 0)
            Trace[i] = int(hHandle);
    }
}

void detrace(HANDLE hHandle) {
    for (int i = 0; i < 10000; i++) {
        if (Trace[i] == int(hHandle))
            Trace[i] = -1;
        if (Trace[i] == 0)
            break;
    }
}

BOOL isTrace(HANDLE hHandle) {
    for (int i = 0; i < 10000; i++) {
        if (Trace[i] == int(hHandle))
            return TRUE;
        if (Trace[i] == 0)
            break;
    }
    return FALSE;
}

void formatError(DWORD errorcode, LPSTR buffer) {
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        errorcode,
        0,
        buffer,
        0,
        NULL
    );
}

static int (WINAPI* OldMessageBoxA)(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType) = MessageBoxA;

static HANDLE(WINAPI* OldHeapCreate)(DWORD fIOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize) = HeapCreate;
static BOOL(WINAPI* OldHeapDestroy)(HANDLE hHeap) = HeapDestroy;
static LPVOID(WINAPI* OldHeapAlloc)(HANDLE hHeap, DWORD dwFLAGS, SIZE_T dwBytes) = HeapAlloc;
static BOOL(WINAPI* OldHeapFree)(HANDLE hHeap, DWORD dwFlags, _Frees_ptr_opt_ LPVOID lpMem) = HeapFree;

static HANDLE(WINAPI* OldCreateFileA)(LPCSTR pFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) = CreateFileA;
// OpenFile 函数已过时？ 无法查阅到相关使用方法
static HFILE(WINAPI* OldOpenFile)(LPCSTR lpFileName, LPOFSTRUCT lpReOpenBuff, UINT uStyle) = OpenFile;
static BOOL(WINAPI* OldWriteFile)(HANDLE hFile,LPCVOID lpBuffer,DWORD nNumberOfBytesToWrite,LPDWORD lpNumberOfBytesWritten,LPOVERLAPPED lpOverlapped) = WriteFile;
static BOOL(WINAPI* OldReadFile)(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped) = ReadFile;
static BOOL(WINAPI* _Oldlclose)(HFILE hfile) = _lclose;
static BOOL(WINAPI* OldCloseHandle)(HANDLE hObject) = CloseHandle;

static LSTATUS(WINAPI* OldRegCreateKeyExA)(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved, LPSTR lpClass, DWORD dwOptions, REGSAM samDesired, const LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition) = RegCreateKeyExA;
static LSTATUS(WINAPI* OldRegOpenKeyExA)(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult) = RegOpenKeyExA;
static LSTATUS(WINAPI* OldRegCloseKey)(HKEY hKey) = RegCloseKey;
static LSTATUS(WINAPI* OldRegQueryValueExA)(HKEY hKey, LPCSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData) = RegQueryValueExA;
static LSTATUS(WINAPI* OldRegSetValueExA)(HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, const BYTE* lpData, DWORD cbData) = RegSetValueExA;

static unsigned int(WSAAPI* Oldsocket)(int af,int type,int protocol) = socket;
static int(WSAAPI* OldWSAStartup)(WORD wVersionRequired, LPWSADATA lpWSAData) = WSAStartup;
static int(WSAAPI* Oldlisten)(SOCKET s, int backlog) = listen;
static int(WSAAPI* Oldconnect)(SOCKET s, const sockaddr* name, int namelen) = connect;
static int(WSAAPI* Oldrecv)(SOCKET s, char* buf, int len, int flags) = recv;
static int(WSAAPI* Oldsend)(SOCKET s, const char* buf, int len, int flags) = send;
static int(WSAAPI* Oldclosesocket)(SOCKET s) = closesocket;
static int(WSAAPI* OldWSACleanup)() = WSACleanup;

static void* (_cdecl *OldMemcpy)(void* destination, const void* source, size_t num) = memcpy;


extern "C" __declspec(dllexport)int WINAPI NewMessageBoxA(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType);

extern "C" __declspec(dllexport)HANDLE WINAPI NewHeapCreate(DWORD fIOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize);
extern "C" __declspec(dllexport)BOOL WINAPI NewHeapDestroy(HANDLE hHeap);
extern "C" __declspec(dllexport)LPVOID WINAPI NewHeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);
extern "C" __declspec(dllexport)BOOL WINAPI NewHeapFree(HANDLE hHeap, DWORD dwFlags, _Frees_ptr_opt_ LPVOID lpMem);

extern "C" __declspec(dllexport) HANDLE WINAPI NewCreateFileA(LPCSTR pFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
extern "C" __declspec(dllexport) HFILE WINAPI NewOpenFile(LPCSTR lpFileName, LPOFSTRUCT lpReOpenBuff, UINT uStyle);
extern "C" __declspec(dllexport) BOOL WINAPI NewWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
extern "C" __declspec(dllexport) BOOL WINAPI NewReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
extern "C" __declspec(dllexport) BOOL WINAPI _Newlclose(HFILE hFile);

extern "C" __declspec(dllexport) BOOL WINAPI NewCloseHandle(HANDLE hObject);
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegCreateKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved, LPSTR lpClass, DWORD dwOptions, REGSAM samDesired, const LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition);
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult);
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegCloseKey(HKEY hKey);
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegQueryValueExA(HKEY hKey, LPCSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegSetValueExA(HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, const BYTE * lpData, DWORD cbData);

// Only Client
extern "C" __declspec(dllexport) SOCKET WSAAPI Newsocket(int af, int type, int protocol);
extern "C" __declspec(dllexport) int WSAAPI NewWSAStartup(WORD wVersionRequired, LPWSADATA lpWSAData);
extern "C" __declspec(dllexport) int WSAAPI Newconnect(SOCKET s, const sockaddr* name, int namelen);
extern "C" __declspec(dllexport) int WSAAPI Newrecv(SOCKET s, char* buf, int len, int flags);
extern "C" __declspec(dllexport) int WSAAPI Newsend(SOCKET s, const char* buf, int len, int flags);
extern "C" __declspec(dllexport) int WSAAPI Newclosesocket(SOCKET s);
extern "C" __declspec(dllexport) int WSAAPI NewWSACleanup();

void DisableDetours() {
    DetourDetach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);

    DetourDetach(&(PVOID&)OldHeapCreate, NewHeapCreate);
    DetourDetach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
    DetourDetach(&(PVOID&)OldHeapDestroy, NewHeapDestroy);
    DetourDetach(&(PVOID&)OldHeapFree, NewHeapFree);

    DetourDetach(&(PVOID&)OldCreateFileA, NewCreateFileA);
    DetourDetach(&(PVOID&)OldOpenFile, NewOpenFile);
    DetourDetach(&(PVOID&)OldReadFile, NewReadFile);
    DetourDetach(&(PVOID&)OldWriteFile, NewWriteFile);
    DetourDetach(&(PVOID&)_Oldlclose, _Newlclose);

    DetourDetach(&(PVOID&)OldCloseHandle, NewCloseHandle);

    DetourDetach(&(PVOID&)OldRegCreateKeyExA, NewRegCreateKeyExA);
    DetourDetach(&(PVOID&)OldRegOpenKeyExA, NewRegOpenKeyExA);
    DetourDetach(&(PVOID&)OldRegQueryValueExA, NewRegQueryValueExA);
    DetourDetach(&(PVOID&)OldRegSetValueExA, NewRegSetValueExA);
    DetourDetach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);

    DetourDetach(&(PVOID&)Oldsocket, Newsocket);
    DetourDetach(&(PVOID&)OldWSAStartup, NewWSAStartup);
    DetourDetach(&(PVOID&)Oldconnect, Newconnect);
    DetourDetach(&(PVOID&)Oldrecv, Newrecv);
    DetourDetach(&(PVOID&)Oldsend, Newsend);
    DetourDetach(&(PVOID&)Oldclosesocket, Newclosesocket);
    DetourDetach(&(PVOID&)OldWSACleanup, NewWSACleanup);

    DetourTransactionCommit();
}

void EnableDetours() {
    DetourAttach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);

    DetourAttach(&(PVOID&)OldHeapCreate, NewHeapCreate);
    DetourAttach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
    DetourAttach(&(PVOID&)OldHeapDestroy, NewHeapDestroy);
    DetourAttach(&(PVOID&)OldHeapFree, NewHeapFree);

    DetourAttach(&(PVOID&)OldCreateFileA, NewCreateFileA);
    DetourAttach(&(PVOID&)OldOpenFile, NewOpenFile);
    DetourAttach(&(PVOID&)OldReadFile, NewReadFile);
    DetourAttach(&(PVOID&)OldWriteFile, NewWriteFile);
    DetourAttach(&(PVOID&)_Oldlclose, _Newlclose);

    DetourAttach(&(PVOID&)OldCloseHandle, NewCloseHandle);

    DetourAttach(&(PVOID&)OldRegCreateKeyExA, NewRegCreateKeyExA);
    DetourAttach(&(PVOID&)OldRegOpenKeyExA, NewRegOpenKeyExA);
    DetourAttach(&(PVOID&)OldRegQueryValueExA, NewRegQueryValueExA);
    DetourAttach(&(PVOID&)OldRegSetValueExA, NewRegSetValueExA);
    DetourAttach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);

    DetourAttach(&(PVOID&)Oldsocket, Newsocket);
    DetourAttach(&(PVOID&)OldWSAStartup, NewWSAStartup);
    DetourAttach(&(PVOID&)Oldconnect, Newconnect);
    DetourAttach(&(PVOID&)Oldrecv, Newrecv);
    DetourAttach(&(PVOID&)Oldsend, Newsend);
    DetourAttach(&(PVOID&)Oldclosesocket, Newclosesocket);
    DetourAttach(&(PVOID&)OldWSACleanup, NewWSACleanup);

    DetourTransactionCommit();
}

void SendPipe(const char* eventID, json &event) {
    DWORD wlen = 0;
    if (hPipe != INVALID_HANDLE_VALUE) {
        json req;
        timeb t;
        ftime(&t);//获取毫秒
        req["eventID"] = eventID;
        req["time"] = t.time*1000 + t.millitm;
        req["event"] = event;
        req["pid"] = GetCurrentProcessId();
        req["threadID"] = GetCurrentThreadId();
        if (strcmp(eventID, "File") == 0 || strcmp(eventID, "HookFunc") == 0) {
            req["file"] = currentFileName;
            req["cwd"] = currentWorkDir;
        }
        auto info = req.dump();
        OldWriteFile(hPipe, info.c_str(), info.length()+1, &wlen, 0);
    }
}

// MessageBox 部分

//MessageBoxA 代替hook函数
extern "C" __declspec(dllexport)int WINAPI NewMessageBoxA(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType) {
    auto result = OldMessageBoxA(NULL, "newMessageBoxA", "Hooked", MB_OK);
    DisableDetours();
    json info; 
    info["Operation"] = "MessageBoxA";
    info["hWnd"] = (int)hWnd;
    if (lpText != NULL)
        info["lpText"] = string(lpText).substr(0, 2000);
    else
        info["lpText"] = "";
    if (lpCaption != NULL)
        info["lpCaption"] = string(lpCaption).substr(0, 7000);
    else
        info["lpCaption"] = "";
    info["uType"] = uType;
    info["return"] = result;
    SendPipe("MessageBox",info);
    EnableDetours();
    return result;
}

// Heap 操作部分


// HeapCreate 代替函数
extern "C" __declspec(dllexport)HANDLE WINAPI NewHeapCreate(DWORD fIOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize) {
    auto hHeap = OldHeapCreate(fIOptions, dwInitialSize, dwMaximumSize);
    DisableDetours();
    trace(hHeap);
    json info;
    info["Operation"] = "HeapCreate";
    info["hHeap"] = int(hHeap);
    info["fIOptions"] = fIOptions;
    info["dwInitialSize"] = dwInitialSize;
    info["dwMaximumSize"] = dwMaximumSize;
    info["DefaultHeap"] = int(GetProcessHeap());
    if (hHeap == NULL) {
        info["errorCode"] = GetLastError();
    }
    SendPipe("Heap", info);
    EnableDetours();
    return hHeap;
}

// HeapDestroy 代替函数
extern "C" __declspec(dllexport)BOOL WINAPI NewHeapDestroy(HANDLE hHeap) {
    DisableDetours();
    json info;
    BOOL result;
    DWORD flags = 0;
    result = OldHeapDestroy(hHeap);
    detrace(hHeap);
    info["return"] = result;
    if (result == 0) {
        info["errorCode"] = GetLastError();
    }
    info["Operation"] = "HeapDestroy";
    info["hHeap"] = int(hHeap);
    SendPipe("Heap", info);
    EnableDetours();
    return result;
}


// HeapAlloc 代替函数
extern "C" __declspec(dllexport)LPVOID WINAPI NewHeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes) {
    DisableDetours();
    LPVOID address = OldHeapAlloc(hHeap, dwFlags, dwBytes);
    DWORD flags = 0;
    if (!isTrace(hHeap)) {
        EnableDetours();
        return address;
    }
    DWORD wlen = 0;
    timeb t;
    ftime(&t);//获取毫秒
    char buf[1000];
    sprintf_s(buf, "{\"eventID\":\"Heap\",\"time\":%llu, \"pid\":%lu, \"threadID\":%lu ,\"event\":{\"Operation\":\"HeapAlloc\",\"hHeap\":%d,\"dwFlags\":%ld,\"dwBytes\":%ld,\"address\":%d}}", t.time * 1000 + t.millitm, GetCurrentProcessId(),GetCurrentThreadId(), int(hHeap),dwFlags,dwBytes,int(address));
    OldWriteFile(hPipe, buf, strlen(buf) + 1 , &wlen, 0);
    EnableDetours();
    return address;
}

// HeapFree 代替函数
extern "C" __declspec(dllexport)BOOL WINAPI NewHeapFree(HANDLE hHeap, DWORD dwFlags, _Frees_ptr_opt_ LPVOID lpMem) {
    DisableDetours();
    BOOL result;
    json info;
    DWORD flags = 0;
    if (!isTrace(hHeap)) {
        EnableDetours();
        return OldHeapFree(hHeap, dwFlags, lpMem);
    }// TODO: 解决默认堆上的噪音问题
    if (HeapValidate(hHeap, 0, lpMem) != 0) {
        result = OldHeapFree(hHeap, dwFlags, lpMem);
        if (result == 0) {
            info["errorCode"] = GetLastError();
        }
    }
    else {
        result = 0;
    }
    info["Operation"] = "HeapFree";
    info["hHeap"] = int(hHeap);
    info["dwFlags"] = dwFlags;
    info["lpMem"] = int(lpMem);
    info["return"] = result;
    SendPipe("Heap", info);
    EnableDetours();
    return result;
}

// File 操作部分


// CreateFileA 代替函数
extern "C" __declspec(dllexport) HANDLE WINAPI NewCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
    auto handle = OldCreateFileA(lpFileName, dwFlagsAndAttributes, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    DisableDetours();
    json info;
    info["Operation"] = "CreateFileA";
    info["lpFileName"] = string(lpFileName); // 文件名最长256字节
    info["dwDesiredAccess"] = dwDesiredAccess;
    info["dwShareMode"] = dwShareMode;
    info["lpSecurityAttributes"] = int(lpSecurityAttributes);
    info["dwCreationDisposition"] = dwCreationDisposition;
    info["dwFlagsAndAttributes"] = dwFlagsAndAttributes;
    info["hTemplateFile"] = int(hTemplateFile);
    info["handle"] = int(handle);
    WCHAR FileName[MAX_PATH];
    info["pathSuccess"] = GetFinalPathNameByHandle(handle, FileName, MAX_PATH, 0);
    info["path"] = (char*)(CW2A(FileName) + 4);   // 删除 \\?\ 符号
    if (handle == INVALID_HANDLE_VALUE) {
        info["errorCode"] = GetLastError();
    }
    SendPipe("File", info);
    trace(handle);
    EnableDetours();
    return handle;
}

// OpenFile 代替函数
extern "C" __declspec(dllexport) HFILE WINAPI NewOpenFile(LPCSTR lpFileName, LPOFSTRUCT lpReOpenBuff, UINT uStyle) {
    auto hfile = OldOpenFile(lpFileName, lpReOpenBuff, uStyle);
    DisableDetours();
    json info;
    info["Operation"] = "OpenFile";
    info["lpFileName"] = string(lpFileName);
    info["lpReOpenBuff"] = int(lpReOpenBuff);
    info["uStyle"] = uStyle;
    info["hFile"] = hfile;
    if (hfile == HFILE_ERROR) {
        info["errorCode"] = GetLastError();
    }
    SendPipe("File", info);
    EnableDetours();
    return hfile;
}

// WriteFile 代替函数
extern "C" __declspec(dllexport) BOOL WINAPI NewWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped) {
    auto result = OldWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
    DisableDetours();
    json info;
    info["Operation"] = "WriteFile";
    info["hFile"] = int(hFile);
    WCHAR FileName[MAX_PATH];
    info["pathLen"] = GetFinalPathNameByHandle(hFile, FileName, MAX_PATH, 0);
    if (info["pathLen"] > 6)
        info["path"] = (char*)(CW2A(FileName) + 4);   // 删除 \\?\ 符号
    else
        info["path"] = "";
    info["lpBuffer"] = int(lpBuffer);
    info["nNumberOfBytesToWrite"] = nNumberOfBytesToWrite;
    info["lpNumberOfBytesWritten"] = int(lpNumberOfBytesWritten);
    if (lpNumberOfBytesWritten != NULL) {
        info["lpNumberOfBytesWrittenValue"] = *lpNumberOfBytesWritten;
        info["lpBufferValue"] = base64_encode((const unsigned char*)lpBuffer, (unsigned int)*lpNumberOfBytesWritten);
    }
    info["lpOverlapped"] = int(lpOverlapped);
    info["return"] = result;
    if (result == FALSE) {
        info["errorCode"] = GetLastError();
    }
    SendPipe("File", info);
    EnableDetours();
    return result;
}

// ReadFile 代替函数
extern "C" __declspec(dllexport) BOOL WINAPI NewReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped) {
    auto result = OldReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
    DisableDetours();
    json info;
    info["Operation"] = "ReadFile";
    info["hFile"] = int(hFile);
    WCHAR FileName[MAX_PATH];
    info["pathLen"] = GetFinalPathNameByHandle(hFile, FileName, MAX_PATH,0);
    if (info["pathLen"] > 6)
        info["path"] = (char*)(CW2A(FileName) + 4);   // 删除 \\?\ 符号
    else
        info["path"] = "";
    info["lpBuffer"] = int(lpBuffer);
    info["nNumberOfBytesToRead"] = nNumberOfBytesToRead;
    info["lpNumberOfBytesRead"] = int(lpNumberOfBytesRead);
    if(lpNumberOfBytesRead != NULL){
        info["lpNumberOfBytesReadValue"] = *lpNumberOfBytesRead;
        info["lpBufferValue"] = base64_encode((const unsigned char*)lpBuffer, (unsigned int)*lpNumberOfBytesRead);
    }
    info["lpOverlapped"] = int(lpOverlapped);
    info["return"] = result;
    if (result == FALSE) {
        info["errorCode"] = GetLastError();
    }
    SendPipe("File", info);
    EnableDetours();
    return result;
}

// _lclose 代替函数
extern "C" __declspec(dllexport) BOOL WINAPI _Newlclose(HFILE hFile) {
    auto result = _Oldlclose(hFile);
    DisableDetours();
    json info;
    info["Operation"] = "_lclose";
    info["hFile"] = int(hFile);
    info["return"] = result;
    SendPipe("File", info);
    EnableDetours();
    return result;
}

// CloseHandle 代替函数 因为滤除了其它handle所以这里直接认为是关闭文件
extern "C" __declspec(dllexport) BOOL WINAPI NewCloseHandle(HANDLE hObject) {
    DisableDetours();
    json info;
    WCHAR FileName[MAX_PATH];
    info["pathLen"] = GetFinalPathNameByHandle(hObject, FileName, MAX_PATH, 0);
    if (info["pathLen"] > 6)
        info["path"] = (char*)(CW2A(FileName) + 4);   // 删除 \\?\ 符号
    auto result = OldCloseHandle(hObject);
    if (!isTrace(hObject)) {
        EnableDetours();
        return result;
    }
    detrace(hObject);
    info["Operation"] = "CloseHandle";
    info["hFile"] = int(hObject);
    info["return"] = result;
    if (result == 0) {
        info["errorCode"] = GetLastError();
    }
    SendPipe("File", info);
    EnableDetours();
    return result;
}


// Regedit 操作部分

// RegCreateKeyExA 代替函数
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegCreateKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved, LPSTR lpClass, DWORD dwOptions, REGSAM samDesired, const LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition) {
    auto status = OldRegCreateKeyExA(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
    DisableDetours();
    json info;
    info["Operation"] = "RegCreateKeyExA";
    info["hKey"] = int(hKey);
    info["lpSubKey"] = lpSubKey;
    info["Reserved"] = Reserved;
    if (lpClass == NULL) {
        info["lpClass"] = "NULL";
    }
    else {
        info["lpClass"] = lpClass;
    }
    info["dwOptions"] = dwOptions;
    info["samDesired"] = samDesired;
    info["lpSecurityAttributes"] = int(lpSecurityAttributes);
    if(phkResult != NULL)
        info["phkResult"] = int(*phkResult);
    else
        info["phkResult"] = NULL;
    if (lpdwDisposition == NULL) {
        info["lpdwDisposition"] = 0;
    }
    else {
        info["lpdwDisposition"] = *lpdwDisposition;
    } 
    info["status"] = status;
    SendPipe("Regedit", info);
    EnableDetours();
    return status;
}

// RegOpenKeyExA 代替函数
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult) {
    auto status = OldRegOpenKeyExA(hKey, lpSubKey, ulOptions, samDesired, phkResult);
    DisableDetours();
    json info;
    info["Operation"] = "RegOpenKeyExA";
    info["hKey"] = int(hKey);
    info["lpSubKey"] = string(lpSubKey);
    info["ulOptions"] = ulOptions;
    info["samDesired"] = int(samDesired);
    if (phkResult != NULL)
        info["phkResult"] = int(*phkResult);
    else
        info["phkResult"] = NULL;
    info["status"] = status;
    SendPipe("Regedit", info);
    EnableDetours();
    return status;
}

// RegCloseKey 代替函数
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegCloseKey(HKEY hKey) {
    auto status = OldRegCloseKey(hKey);
    DisableDetours();
    json info;
    info["Operation"] = "RegCloseKey";
    info["hKey"] = int(hKey);
    info["status"] = status;
    SendPipe("Regedit", info);
    EnableDetours();
    return status;
}

// RegQueryValueExA 代替函数
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegQueryValueExA(HKEY hKey, LPCSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData) {
    auto status = OldRegQueryValueExA(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
    DisableDetours();
    json info;
    info["Operation"] = "RegQueryValueExA";
    info["hKey"] = int(hKey);
    info["lpValueName"] = string(lpValueName);
    info["lpReserved"] = int(lpReserved);
    info["lpType"] = int(lpType);
    if (lpType != NULL){
        info["lpTypeValue"] = *lpType;
    }
    info["lpData"] = int(lpData);
    if(lpcbData == NULL || lpData == NULL)
        info["lpcbData"] = 0;
    else {
        info["lpcbData"] = *lpcbData;
        info["lpDataValue"] = base64_encode((const unsigned char*)lpData, (unsigned int)*lpcbData);
    }
    info["status"] = status;
    SendPipe("Regedit", info);
    EnableDetours();
    return status;
}

// RegSetValueExA 代替函数
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegSetValueExA(HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, const BYTE * lpData, DWORD cbData) {
    auto status = OldRegSetValueExA(hKey, lpValueName, Reserved, dwType, lpData, cbData);
    DisableDetours();
    json info;
    info["Operation"] = "RegSetValueExA";
    info["hKey"] = int(hKey);
    info["lpValueName"] = string(lpValueName);
    info["Reserved"] = Reserved;
    info["dwType"] = dwType;
    info["lpData"] = int(lpData);
    info["cbData"] = 0;
    if(cbData != 0 && lpData != NULL)
    {
        info["cbData"] = cbData;
        info["lpDataValue"] = base64_encode((const unsigned char*)lpData, cbData);
    }
    info["status"] = status;
    SendPipe("Regedit", info);
    EnableDetours();
    return status;
}

// WebSocket

//socket 代替函数
extern "C" __declspec(dllexport) SOCKET WSAAPI Newsocket(int af, int type, int protocol) {
    auto socket = Oldsocket(af,type,protocol);
    DisableDetours();
    json info;
    info["Operation"] = "socket";
    info["af"] = af;
    info["type"] = type;
    info["protocol"] = protocol;
    info["socket"] = socket;
    if (socket == INVALID_SOCKET) {
        info["errorCode"] = WSAGetLastError();
    }
    SendPipe("Socket", info);
    EnableDetours();
    return socket;
}

// WSAStartup 代替函数
extern "C" __declspec(dllexport) int WSAAPI NewWSAStartup(WORD wVersionRequired, LPWSADATA lpWSAData) {
    auto status = OldWSAStartup(wVersionRequired, lpWSAData);
    DisableDetours();
    json info;
    info["Operation"] = "WSAStartup";
    info["wVersionRequired"] = wVersionRequired;
    info["wVersionRequiredPrimary"] = LOBYTE(wVersionRequired);
    info["wVersionRequiredSecondary"] = HIBYTE(wVersionRequired);
    info["lpWSAData"] = int(lpWSAData);
    if (lpWSAData != NULL && status != WSAEFAULT) {
        info["lpWSADataValue"] = base64_encode((const unsigned char*)lpWSAData, sizeof(WSADATA));
        info["wVersion"] = lpWSAData->wVersion;
        info["wVersionPrimary"] = LOBYTE(lpWSAData->wVersion);
        info["wVersionSecondary"] = HIBYTE(lpWSAData->wVersion);
    }
    info["status"] = status;
    SendPipe("Socket", info);
    EnableDetours();
    return status;
}

// connect 代替函数
extern "C" __declspec(dllexport) int WSAAPI Newconnect(SOCKET s, const sockaddr * name, int namelen) {
    auto status = Oldconnect(s,name,namelen);
    DisableDetours();
    json info;
    info["Operation"] = "connect";
    info["s"] = s;
    info["sockaddr"] = int(name);
    info["namelen"] = namelen;
    if (name != NULL and status != WSAEFAULT) {
        info["nameValue"] = base64_encode((const unsigned char*)name, namelen);
        auto sockaddr = (SOCKADDR*)name;
        info["sa_family"] = sockaddr->sa_family;
        if (sockaddr->sa_family == AF_INET) {
            info["sin_port"] = ntohs(((SOCKADDR_IN*)sockaddr)->sin_port);
            info["sin_addr"] = ntohl(((SOCKADDR_IN*)sockaddr)->sin_addr.s_addr);
        }
    }
    info["status"] = status;
    if (status == SOCKET_ERROR) {
        info["errorCode"] = WSAGetLastError();
    }
    SendPipe("Socket", info);
    EnableDetours();
    return status;
}

// recv 代替函数
extern "C" __declspec(dllexport) int WSAAPI Newrecv(SOCKET s, char* buf, int len, int flags) {
    auto status = Oldrecv(s, buf, len, flags);
    DisableDetours();
    json info;
    info["Operation"] = "recv";
    info["s"] = s;
    info["buf"] = int(buf);
    if (buf != NULL && status > 0) {
        info["bufValue"] = base64_encode((const unsigned char*)buf, status);
    }
    info["len"] = len;
    info["flags"] = flags;
    info["status"] = status;
    if (status == SOCKET_ERROR) {
        info["errorCode"] = WSAGetLastError();
    }   
    SendPipe("Socket", info);
    EnableDetours();
    return status;
}

// send 代替函数
extern "C" __declspec(dllexport) int WSAAPI Newsend(SOCKET s, const char* buf, int len, int flags) {
    auto status = Oldsend(s, buf, len, flags);
    DisableDetours();
    json info;
    info["Operation"] = "send";
    info["s"] = s;
    info["buf"] = int(buf);
    if (buf != NULL && status > 0) {
        info["bufValue"] = base64_encode((const unsigned char*)buf, status);
    }
    info["len"] = len;
    info["flags"] = flags;
    info["status"] = status;
    if (status == SOCKET_ERROR) {
        info["errorCode"] = WSAGetLastError();
    }
    SendPipe("Socket", info);
    EnableDetours();
    return status;
}

// closesocket 代替函数
extern "C" __declspec(dllexport) int WSAAPI Newclosesocket(SOCKET s) {
    auto status = Oldclosesocket(s);
    DisableDetours();
    json info;
    info["Operation"] = "closesocket";
    info["s"] = s;
    info["status"] = status;
    if (status == SOCKET_ERROR) {
        info["errorCode"] = WSAGetLastError();
    }
    SendPipe("Socket", info);
    EnableDetours();
    return status;
}

// WSACleanup 代替函数
extern "C" __declspec(dllexport) int WSAAPI NewWSACleanup() {
    auto status = OldWSACleanup();
    DisableDetours();
    json info;
    info["Operation"] = "WSACleanup";
    info["status"] = status;
    if (status == SOCKET_ERROR) {
        info["errorCode"] = WSAGetLastError();
    }
    SendPipe("Socket", info);
    EnableDetours();
    return status;
};


// 连接函数
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    BOOL bRet;
    json info,infoEnd;
    struct _stat buf;

    TCHAR exeFullName[MAX_PATH];
    GetModuleFileName(NULL, exeFullName, MAX_PATH);
    currentFileName = CW2A(exeFullName);

    TCHAR exeFullPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH,exeFullPath);
    currentWorkDir = CW2A(exeFullPath);

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        bRet = WaitNamedPipe(TEXT("\\\\.\\Pipe\\mypipe"), NMPWAIT_WAIT_FOREVER);
        if (bRet) {
            hPipe = CreateFile(
                TEXT("\\\\.\\Pipe\\mypipe"),
                GENERIC_WRITE,
                0,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );
        }

        info["Operation"] = "StartTrace";
        info["DefaultHeap"] = int(GetProcessHeap());
        if (_stat(currentFileName.c_str(), &buf) == 0) {
            info["st_size"] = buf.st_size;
            info["st_atime"] = buf.st_atime;
            info["st_mtime"] = buf.st_mtime;
            info["st_ctime"] = buf.st_ctime;
        }
        SendPipe("HookFunc", info);

        EnableDetours();

        //DetourTransactionCommit();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        DisableDetours();

        infoEnd["Operation"] = "StopTrace";
        SendPipe("HookFunc", infoEnd);

        //DetourTransactionCommit();
        CloseHandle(hPipe);
        break;
    }
    return TRUE;
}

