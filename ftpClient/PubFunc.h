#pragma once

class CPubFunc
{
public:
	static void GetCurExePath(TCHAR* v_tszExePath);
	//!<��ȡ�ļ���С
	static UINT64 MyGetFileSize(const std::string& v_szSrcFile);

	static void GetGuid(char* v_szGuid);
};