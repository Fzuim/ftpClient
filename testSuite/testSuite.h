
// testSuite.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CtestSuiteApp:
// �йش����ʵ�֣������ testSuite.cpp
//

class CtestSuiteApp : public CWinAppEx
{
public:
	CtestSuiteApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CtestSuiteApp theApp;