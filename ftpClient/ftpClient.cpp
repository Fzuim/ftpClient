// ftpClient.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "ftpClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CftpClientApp

BEGIN_MESSAGE_MAP(CftpClientApp, CWinApp)
END_MESSAGE_MAP()


// CftpClientApp 构造

CftpClientApp::CftpClientApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CftpClientApp 对象

CftpClientApp theApp;


// CftpClientApp 初始化

BOOL CftpClientApp::InitInstance()
{
	CWinApp::InitInstance();

	CPubFunc::GetCurExePath(global_config.m_tszCurExePath);

	theApp.m_MasterCtrl.init_startup();

	return TRUE;
}

// 上传文件
int WINAPI ftpUpload(const char* host, int port, const char* username, const char* password, 
					 const char* local_file, const char* remote_file, const char* guid)
{
	return theApp.m_MasterCtrl.uploadFile(host, port, username, password, local_file, remote_file, guid);
}

// 下载文件
int WINAPI ftpDownload(const char* host, int port, const char* username, const char* password, 
					   const char* remote_file, const char* local_file, const char* guid)
{
	return theApp.m_MasterCtrl.downloadFile(host, port, username, password, remote_file, local_file, guid);
}

// 注册回调
void WINAPI ftpRegisterCallback(PMsgCallback msg_callback)
{
	theApp.m_MasterCtrl.registerCallback(msg_callback);
}

// 服务器是否在线判断
int WINAPI ftpOnline(const char* host, int port)
{
	return theApp.m_MasterCtrl.checkOnline(host, port);
}