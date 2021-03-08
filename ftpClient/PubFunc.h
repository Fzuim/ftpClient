#pragma once

class CPubFunc
{
public:
	static void GetCurExePath(TCHAR* v_tszExePath);
	//!<获取文件大小
	static UINT64 MyGetFileSize(const std::string& v_szSrcFile);

	static void GetGuid(char* v_szGuid);
};