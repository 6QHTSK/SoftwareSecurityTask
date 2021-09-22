
// Target4Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Target4.h"
#include "Target4Dlg.h"
#include "afxdialogex.h"
#include <exception>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTarget4Dlg 对话框



CTarget4Dlg::CTarget4Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TARGET4_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTarget4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTarget4Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_MESSAGEBOXA, &CTarget4Dlg::OnBnClickedButtonOpenMessageboxa)
	ON_BN_CLICKED(IDC_BUTTON2, &CTarget4Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTarget4Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTarget4Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CTarget4Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(ID_BUTTON6, &CTarget4Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CTarget4Dlg::OnBnClickedButton7)
END_MESSAGE_MAP()


// CTarget4Dlg 消息处理程序

BOOL CTarget4Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTarget4Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTarget4Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTarget4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTarget4Dlg::OnBnClickedButton1()
{
	//MessageBoxA(NULL, "The MessageBox is Not Hooked","MessageBoxA", MB_OK);
}


void CTarget4Dlg::OnBnClickedButtonOpenMessageboxa()
{
	MessageBoxA(NULL, "The MessageBox is Not Hooked", "MessageBoxA", MB_OK);
}

void CallMsgBox(LPCWSTR message) {
	MessageBoxW(NULL, message, L"debug", MB_OK);
}

void CTarget4Dlg::OnBnClickedButton2()
{
	HANDLE hHeap = HeapCreate(HEAP_NO_SERIALIZE, 100, 100);
	CallMsgBox(L"HeapCreate执行");
	if (hHeap != NULL){
		auto hptest = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 100);
		CallMsgBox(L"HeapAlloc执行");
		HeapFree(hHeap, 0, hptest);
		CallMsgBox(L"HeapFree执行");
		HeapDestroy(hHeap);
		CallMsgBox(L"HeapDestroy执行");
	}
}


void CTarget4Dlg::OnBnClickedButton3()
{
	HANDLE hFile = CreateFileA(
		"test.txt",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	CallMsgBox(L"CreateFile执行");
	DWORD sz = 0;
	char buf1[4096];
	memcpy(buf1, "Hello World",12);
	WriteFile(hFile, buf1, 12, &sz, NULL);
	FlushFileBuffers(hFile);
	CallMsgBox(L"WriteFile执行");
	LONG lDistance = 0;
	SetFilePointer(hFile, lDistance, NULL, FILE_BEGIN);
	char buf[20];
	DWORD dw = 0;
	ReadFile(hFile, buf, 12, &dw, NULL);
	CallMsgBox(L"ReadFile执行");
	CloseHandle(hFile);
	CallMsgBox(L"CloseHandle执行");
}


void CTarget4Dlg::OnBnClickedButton4()
{
	HKEY hk = NULL;
	HKEY hk2 = NULL;
	LPCSTR keyname = "SOFTWARE\\HUST\\RJAQ";
	auto dwType = REG_SZ;
	char buffer[257] = { 0 };
	DWORD dwLen = 256;
	auto res = RegCreateKeyExA(HKEY_LOCAL_MACHINE, keyname , 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hk, NULL);
	CallMsgBox(L"RegCreateKeyExA执行");
	RegCloseKey(hk);
	RegOpenKeyExA(HKEY_LOCAL_MACHINE, keyname, 0, KEY_ALL_ACCESS, &hk2);
	CallMsgBox(L"RegOpenKeyExA执行");
	RegSetValueExA(hk2, "Hello", 0, REG_SZ, (PBYTE)"Hello HUST", 11);
	CallMsgBox(L"RegSetValueExA执行");
	RegQueryValueExA(hk2, "Hello", NULL, &dwType, (BYTE*)buffer, &dwLen);
	CallMsgBox(L"RegQueryValueExA执行");
	RegCloseKey(hk2);
	CallMsgBox(L"RegCloseKey执行");
}


void CTarget4Dlg::OnBnClickedButton5()
{
	HANDLE hHeap = HeapCreate(HEAP_NO_SERIALIZE, 100, 100);
	HANDLE hHeap2 = HeapCreate(HEAP_NO_SERIALIZE, 100, 100);	// WARNING 114
	if (hHeap != NULL) {
		auto hptest = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 100);
		auto hptest2 = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 100);
		HeapFree(hHeap, 0, hptest);
		HeapDestroy(hHeap);	// WARNING 113
		CallMsgBox(L"Destroy 堆时不释放Alloc");
	}
}


void CTarget4Dlg::OnBnClickedButton6()
{
	TCHAR exeFullName[MAX_PATH];
	GetModuleFileName(NULL, exeFullName, MAX_PATH);
	HANDLE hFile = CreateFileA(
		CW2A(exeFullName),
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	char buf[20];
	DWORD dw = 0;
	ReadFile(hFile, buf, 12, &dw, NULL);
	CloseHandle(hFile);
}


void CTarget4Dlg::OnBnClickedButton7()
{
	HKEY hRoot = HKEY_LOCAL_MACHINE;
	char* szSubKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	HKEY hKey;

	//  打开指定子键
	DWORD dwDisposition = REG_OPENED_EXISTING_KEY;     //  如果不存在不创建
	LONG lRet = RegCreateKeyExA(hRoot, szSubKey, 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
	wchar_t valueName[1000];
}
