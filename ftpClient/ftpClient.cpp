// ftpClient.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "ftpClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CftpClientApp

BEGIN_MESSAGE_MAP(CftpClientApp, CWinApp)
END_MESSAGE_MAP()


// CftpClientApp ����

CftpClientApp::CftpClientApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CftpClientApp ����

CftpClientApp theApp;


// CftpClientApp ��ʼ��

BOOL CftpClientApp::InitInstance()
{
	CWinApp::InitInstance();

	CPubFunc::GetCurExePath(global_config.m_tszCurExePath);

	theApp.m_MasterCtrl.init_startup();

	return TRUE;
}

// �ϴ��ļ�
int WINAPI ftpUpload(const char* host, int port, const char* username, const char* password, 
					 const char* local_file, const char* remote_file, const char* guid)
{
	return theApp.m_MasterCtrl.uploadFile(host, port, username, password, local_file, remote_file, guid);
}

// �����ļ�
int WINAPI ftpDownload(const char* host, int port, const char* username, const char* password, 
					   const char* remote_file, const char* local_file, const char* guid)
{
	return theApp.m_MasterCtrl.downloadFile(host, port, username, password, remote_file, local_file, guid);
}

// ע��ص�
void WINAPI ftpRegisterCallback(PMsgCallback msg_callback)
{
	theApp.m_MasterCtrl.registerCallback(msg_callback);
}

// �������Ƿ������ж�
int WINAPI ftpOnline(const char* host, int port)
{
	return theApp.m_MasterCtrl.checkOnline(host, port);
}