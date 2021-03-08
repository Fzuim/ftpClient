
// testSuiteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "testSuite.h"
#include "testSuiteDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CtestSuiteDlg 对话框




CtestSuiteDlg::CtestSuiteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CtestSuiteDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestSuiteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CtestSuiteDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CtestSuiteDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CtestSuiteDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CtestSuiteDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CtestSuiteDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CtestSuiteDlg 消息处理程序

BOOL CtestSuiteDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CtestSuiteDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtestSuiteDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtestSuiteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

typedef int (*PMsgCallback)(const char *, int, int);
typedef int (WINAPI* PFTPUPLOAD)(const char*,int,const char*,const char*,const char*,const char*,const char*);
typedef int (WINAPI* PFTPDOWNLOAD)(const char*,int,const char*,const char*,const char*,const char*,const char*);
typedef int (WINAPI* PREGISTERCB)(PMsgCallback);
typedef int (WINAPI* PFTPONLINE)(const char*,int);

PFTPUPLOAD ftpUpload = NULL;
PFTPDOWNLOAD ftpDownLoad = NULL;
PREGISTERCB registerCB = NULL;
PFTPONLINE ftpOnline = NULL;

BOOL inited = FALSE;

int msgCallbak(const char* guid, int code, int percent)
{
	printf("guid[%s],code[%d],percent[%d]\n", guid, code, percent);

	return code;
}

void CtestSuiteDlg::OnBnClickedButton1()
{
	if (inited)
	{
		AfxMessageBox(_T("ftp传输Dll已初始化"));
		return;
	}

	HMODULE hDll = ::LoadLibrary(_T("ftpClient.dll"));
	if (NULL != hDll)
	{
		ftpUpload = (PFTPUPLOAD)::GetProcAddress(hDll, "ftpUpload");
		ftpDownLoad = (PFTPDOWNLOAD)::GetProcAddress(hDll, "ftpDownload");
		registerCB = (PREGISTERCB)::GetProcAddress(hDll, "ftpRegisterCallback");
		ftpOnline = (PFTPONLINE)::GetProcAddress(hDll, "ftpOnline");

		if (ftpUpload && ftpDownLoad && registerCB && ftpOnline)
		{
			inited = TRUE;
			registerCB(msgCallbak);
			AfxMessageBox(_T("初始化ftp传输组件成功"));
		}
	}
	else
	{
		CString cstrMsg;
		cstrMsg.Format(_T("加载ftp传输Dll失败，错误码：%d"), GetLastError());
		AfxMessageBox(cstrMsg);
	}
}

void CtestSuiteDlg::OnBnClickedButton2()
{
	if (inited && ftpUpload)
	{
		int iRet = ftpUpload("127.0.0.1", 21001, "fzuim", "fzuim1234", "C:\\upload.txt", "upload.txt", "12312312");
	}
}

void CtestSuiteDlg::OnBnClickedButton3()
{
	if (inited && ftpDownLoad)
	{
		int iRet = ftpDownLoad("127.0.0.1", 21001, "fzuim", "fzuim1234", "download.txt", "C:\\download.txt", "2323112323");
	}
}

void CtestSuiteDlg::OnBnClickedButton4()
{
	if (inited && ftpOnline)
	{
		int iRet = ftpOnline("127.0.0.1", 21001);
		CString cstrMsg;
		cstrMsg.Format(_T("ftp在线测试，返回[%d]"), iRet);
		AfxMessageBox(cstrMsg);
	}
}
