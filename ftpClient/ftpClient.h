// ftpClient.h : ftpClient DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "MasterCtrl.h"

// CftpClientApp
// �йش���ʵ�ֵ���Ϣ������� ftpClient.cpp
//

class CftpClientApp : public CWinApp
{
public:
	CftpClientApp();
	CMasterCtrl m_MasterCtrl;

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
