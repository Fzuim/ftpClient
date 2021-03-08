#pragma once

class CMsgCallback : public CThread
{
public:
	CMsgCallback(void);
	~CMsgCallback(void);

	void sendMsg(int percent, int ftpcode, const char* guid);
	void registerCallback(PMsgCallback msg_callback);

protected:
	virtual int OnThreadMessage(UINT Msg, WPARAM wParam, LPARAM lParam);
	virtual BOOL InitInstance();
	virtual DWORD ExitInstance();

private:
	std::list<PMsgCallback> lst_callback;
};
