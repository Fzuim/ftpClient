#pragma once

#include "MsgCallback.h"

class CMasterCtrl
{
public:
	CMasterCtrl(void);
	~CMasterCtrl(void);

	void init_startup();
	void stop_release();

	int uploadFile(const char* host, int port, const char* username, const char* password, const char* local_file, const char* remote_file, const char* guid);
	int downloadFile(const char* host, int port, const char* username, const char* password, const char* remote_file, const char* local_file, const char* guid);
	void registerCallback(PMsgCallback msg_callback);
	int checkOnline(const char* host, int port);

protected:
	void sendMsg(int percent, int ftpcode, const char* guid);

private:
	CMsgCallback msg_callback_obj;
};
