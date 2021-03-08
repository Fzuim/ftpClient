#include "stdafx.h"
#include "PubFunc.h"

void CPubFunc::GetCurExePath(TCHAR* v_tszExePath)
{
	/// 获取当前工作目录
	GetModuleFileNameW(NULL, v_tszExePath, MAX_PATH);
	int iLen = _tcslen( v_tszExePath );

	for(int i = iLen - 1; i >= 0; i--)
	{
		if(_T('\\') == v_tszExePath[i])
		{
			v_tszExePath[i] = _T('\0');
			break;
		}
	}
}

UINT64 CPubFunc::MyGetFileSize(const std::string& v_szSrcFile)
{
	UINT64 nFileSize = 0;
	DWORD dwSizeLow = 0;
	DWORD dwSizeHigh = 0;
	HANDLE hFile = CreateFileA(v_szSrcFile.c_str(), FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		dwSizeLow = GetFileSize(hFile, &dwSizeHigh);
		nFileSize = dwSizeHigh;
		nFileSize <<= 32;
		nFileSize += dwSizeLow;
		CloseHandle(hFile);
	}

	return nFileSize;
}

void CPubFunc::GetGuid(char* v_szGuid)
{
	GUID		SmtpFileGuid;	//!< 获取的GUID
	if(S_OK == CoCreateGuid(&SmtpFileGuid))
	{
		sprintf(v_szGuid, "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x"
			,SmtpFileGuid.Data1
			,SmtpFileGuid.Data2
			,SmtpFileGuid.Data3
			,SmtpFileGuid.Data4[0]
			,SmtpFileGuid.Data4[1]
			,SmtpFileGuid.Data4[2]
			,SmtpFileGuid.Data4[3]
			,SmtpFileGuid.Data4[4]
			,SmtpFileGuid.Data4[5]
			,SmtpFileGuid.Data4[6]
			,SmtpFileGuid.Data4[7]
		);
	}
}
