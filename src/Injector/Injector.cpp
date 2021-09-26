// Injector.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <cstdio>
#include <Windows.h>
#include <detours.h>
#include "Injector.h"
#pragma comment(lib,"detours.lib")

int main(int argc, char* argv[])
{
    if (argc == 1 || argc > 2) {
        return -1;
    }
    WCHAR EXE[MAX_PATH + 1] = { 0 };
    swprintf_s(EXE, L"%hs", argv[1]);
    wprintf(L"%s", EXE);
    //std::cout << "Hello World!\n";
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    si.cb = sizeof(STARTUPINFO);
    WCHAR DirPath[MAX_PATH + 1];
    wcscpy_s(DirPath, MAX_PATH, L"S:\\softwareSecurityTask\\src\\DetoursDll3\\Debug");
    char DLLPath[MAX_PATH + 1] = "S:\\softwareSecurityTask\\src\\DetoursDll3\\Debug\\DetoursDll3.dll";

    if (DetourCreateProcessWithDllEx(EXE, NULL, NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, NULL, DirPath, &si, &pi, DLLPath, NULL)) {
        printf("Success Hooked!\n");
        ResumeThread(pi.hThread);
        WaitForSingleObject(pi.hProcess, INFINITE);
    }
    else {
        return GetLastError();
    }
    return 0;
}
