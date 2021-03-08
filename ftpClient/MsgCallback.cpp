#include "StdAfx.h"
#include "MsgCallback.h"

CMsgCallback::CMsgCallback(void) 
{
}

CMsgCallback::~CMsgCallback(void) 
{
}

void CMsgCallback::sendMsg(int percent, int ftpcode, const char* guid) {
	PMSGCALLBACK pMsg = new MSGCALLBACK;
	memset(pMsg, 0, sizeof(MSGCALLBACK));

	pMsg->m_iPercent = percent;
	pMsg->m_iFtpCode = ftpcode;
	strcpy(pMsg->m_szGuid, guid);

	PostThreadMessage(WM_MSG_CALLBACK, (WPARAM)pMsg, NULL);
}

int CMsgCallback::OnThreadMessage(UINT Msg, WPARAM wParam, LPARAM lParam) {
	if (WM_MSG_CALLBACK == Msg && NULL != wParam) {
		PMSGCALLBACK pMsg = (PMSGCALLBACK)wParam;
		PMsgCallback pCallback = NULL;
		std::list<PMsgCallback>::iterator it = lst_callback.begin();
		for (; it != lst_callback.end(); ++it) {
			pCallback = *it;
			if (NULL != pCallback) pCallback(pMsg->m_szGuid, pMsg->m_iFtpCode, pMsg->m_iPercent);
		}

		delete pMsg;
		pMsg = NULL;
	}

	return 0;
}

BOOL CMsgCallback::InitInstance() {
	return TRUE;
}

DWORD CMsgCallback::ExitInstance() {
	return 0;
}

void CMsgCallback::registerCallback(PMsgCallback msg_callback) {
	lst_callback.push_back(msg_callback);
}
