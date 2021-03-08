
// testSuiteDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testSuite.h"
#include "testSuiteDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CtestSuiteDlg �Ի���




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


// CtestSuiteDlg ��Ϣ�������

BOOL CtestSuiteDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CtestSuiteDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
		AfxMessageBox(_T("ftp����Dll�ѳ�ʼ��"));
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
			AfxMessageBox(_T("��ʼ��ftp��������ɹ�"));
		}
	}
	else
	{
		CString cstrMsg;
		cstrMsg.Format(_T("����ftp����Dllʧ�ܣ������룺%d"), GetLastError());
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
		cstrMsg.Format(_T("ftp���߲��ԣ�����[%d]"), iRet);
		AfxMessageBox(cstrMsg);
	}
}
