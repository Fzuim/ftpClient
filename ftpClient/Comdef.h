#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <Windows.h>
#include <list>

//////////////////////////////////////////////////////////////////////////
// poco Í·ÎÄ¼þ
#include "Poco/Buffer.h"
#include "Poco/AutoPtr.h"  
#include "Poco/Thread.h"  
#include "Poco/Runnable.h"  
#include "Poco/Activity.h"
#include "Poco/ActiveMethod.h"
#include "Poco/StreamCopier.h"
#include "Poco/Timespan.h"
#include "Poco/Net/FTPClientSession.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/DialogSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"

#include "Poco/Util/IniFileConfiguration.h"  
#include "Poco/Path.h"
#include "Poco/Event.h"  
#include "Poco/Mutex.h"

using Poco::Mutex;
using Poco::Event; 

using Poco::AutoPtr;  
using Poco::Thread;
using Poco::Activity;
using Poco::ActiveMethod;
using Poco::ActiveResult;
using Poco::Net::FTPClientSession;
using Poco::Net::SocketAddress;
using Poco::Net::FTPException;
using Poco::Util::IniFileConfiguration;
using Poco::StreamCopier;
using Poco::Path;
//////////////////////////////////////////////////////////////////////////
#include "LogModule.h"
#include "PubFunc.h"
#include "Thread.h"

typedef struct GlobalConfig
{
	TCHAR m_tszCurExePath[2 * MAX_PATH];
	int m_iFlag;
}GLOBALCONFIG;

extern GLOBALCONFIG global_config;

typedef int (*PMsgCallback)(const char *, int, int);

typedef struct _tagMsgCallbak
{
	int m_iPercent;
	int m_iFtpCode;
	char m_szGuid[100];
}MSGCALLBACK, *PMSGCALLBACK;

enum _enFtpCode
{
	FTP_SUCCESS = 0,
	FTP_UPLOADING,
	FTP_DOWNLOADING,
	FTP_ALREAD_EXIST,
	FTP_OPEN_ERROR,
	FTP_NET_ERROR,
	FTP_NOT_EXIST,
	FTP_FTPEXCEPTION,
	FTP_NETEXCEPTION,
	FTP_EXCEPTION,

	FTP_UNKNOW_ERROR = 20
};

#define WM_MSG_CALLBACK WM_USER + 100