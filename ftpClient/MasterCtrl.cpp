#include "StdAfx.h"
#include "MasterCtrl.h"

CMasterCtrl::CMasterCtrl(void) {
}

CMasterCtrl::~CMasterCtrl(void) {
}

void CMasterCtrl::init_startup() {
	TCHAR tszConfigIni[2 * MAX_PATH] = {0};
	_stprintf_s(tszConfigIni, _T("%s\\Config.ini"), global_config.m_tszCurExePath);
	
	global_config.m_iFlag = GetPrivateProfileInt(_T("Config"), _T("Flag"), 0, tszConfigIni);

	msg_callback_obj.Create();
}

void CMasterCtrl::stop_release() {
	msg_callback_obj.PostThreadMessage(WM_QUIT, NULL, NULL);
}

///	@brief �ϴ��ļ�
///	@param host ��������ַ
///	@param port �������˿�
///	@param username ��¼�û���
///	@param password ��¼����
///	@param local_file Ҫ�ϴ��ı����ļ�
///	@param remote_file �ϴ���Զ�̷��������ļ�����
///	@param guid 
///	@return FTPCode
int CMasterCtrl::uploadFile(const char* host, int port, const char* username, const char* password, 
							const char* local_file, const char* remote_file, const char* guid) {
	std::string strRemoteFile(remote_file);
	std::string strRemotePath	= "";
	std::string strResponse		= "";
	std::string strTmp			= "";
	std::string strRemoteSize	= "";
	
	int returnCode = -1;
	int serverStatus = 0;
	int pos	= 0;
	int percent = 0;
	int prePercent = percent;
	UINT64 localSize = 0;
	UINT64 remoteSize = 0;

	HANDLE hSrcFile = INVALID_HANDLE_VALUE;

	Poco::Net::FTPClientSession *uploadSession = NULL;

	try {
		CLogModule::Odprintf(_T("[%s]->->->->->->->->->->->->->->->->->->"), __FUNCTIONW__);
		uploadSession = new Poco::Net::FTPClientSession(host, port);
		uploadSession->login(username, password);

		// ·���жϣ��Ƿ񴴽�Ŀ��·�����ǷŸ�Ŀ¼
		if (strRemoteFile.find('/') != std::string::npos) {
			strRemotePath = strRemoteFile.substr(0, strRemoteFile.rfind('/') + 1);
			serverStatus = uploadSession->sendCommand("MKD", strRemotePath, strResponse);
		}

		// ��ȡԶ���ļ���С
		serverStatus = uploadSession->sendCommand("MLST", strRemoteFile, strResponse);
		if(serverStatus / 100 == 2) { //2����������Ӧ��ȷ����Ŀ���ļ�����
			pos = strResponse.find("size=");
			strTmp = strResponse.substr(pos);
			pos = strTmp.find(";");
			strRemoteSize = strTmp.substr(5, pos-5);
			remoteSize = _atoi64(strRemoteSize.c_str()); // Զ���ļ���С
		}

		hSrcFile = CreateFileA(local_file, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
		if (INVALID_HANDLE_VALUE != hSrcFile) {
			localSize = CPubFunc::MyGetFileSize(local_file);
			if (localSize != remoteSize) { // ���������ļ���С�ͱ��ز�һ�£��Ž����ϴ�
				if (1 == (1 & global_config.m_iFlag) && (0 != remoteSize && remoteSize < localSize)) { // �ϵ�����
					serverStatus = uploadSession->sendCommand("REST", strRemoteSize, strResponse);
					if (3 != serverStatus / 100) remoteSize = 0;
				} else remoteSize = 0; // �����ϴ�

				// ���ñ����ļ�ƫ��
				LARGE_INTEGER li;
				li.QuadPart = remoteSize;
				SetFilePointerEx(hSrcFile, li, NULL, FILE_BEGIN);

				CLogModule::Odprintf(_T("[%s]SetFilePointerEx=%d"), __FUNCTIONW__, remoteSize);
				CLogModule::Odprintf("[%s]beginUpload=%s", __FUNCTION__, strRemoteFile.c_str());
				// ��ʼִ���ϴ�
				std::ostream& ostr = uploadSession->beginUpload(strRemoteFile);
				Poco::Buffer<char> buffer(8192);
				UINT64 totalSize = remoteSize;
				DWORD dwReadLen = 0;
				BOOL bRet = ReadFile(hSrcFile, buffer.begin(), 8192, &dwReadLen, NULL);
				while(bRet && dwReadLen > 0) {
					totalSize += dwReadLen;
					ostr.write(buffer.begin(), dwReadLen);
					if (ostr) bRet = ReadFile(hSrcFile, buffer.begin(), 8192, &dwReadLen, NULL);
					else dwReadLen = 0;

					// �ϴ����Ȱٷֱ�
					percent = (int)((totalSize * 100) / localSize);
					if (percent > 0 && 1 == percent % 2 && percent != prePercent) {
						prePercent = percent;
						msg_callback_obj.sendMsg(percent, FTP_UPLOADING, guid);
					}
				}

				returnCode = FTP_SUCCESS;
				uploadSession->endUpload();

				CLogModule::Odprintf("[%s]endUpload=%s", __FUNCTION__, strRemoteFile.c_str());
			} else {
				returnCode = FTP_ALREAD_EXIST;
				CLogModule::Odprintf(_T("[%s]remote file has been exist."), __FUNCTIONW__);
			}

			CloseHandle(hSrcFile);
			hSrcFile = INVALID_HANDLE_VALUE;
		} else {
			returnCode = FTP_OPEN_ERROR;
			CLogModule::Odprintf("[%s]open file=%s error=%d", __FUNCTION__, strRemoteFile.c_str(), GetLastError());
		}
	}
	catch(FTPException& e) {
		returnCode = FTP_FTPEXCEPTION;
		CLogModule::Odprintf("[%s]FTPException=%s", __FUNCTION__, e.displayText().c_str());
	}
	catch(Poco::Net::NetException& e) {
		returnCode = FTP_NETEXCEPTION;
		CLogModule::Odprintf("[%s]NetException=%s", __FUNCTION__, e.displayText().c_str());
	}
	catch(Poco::Exception& e) {
		returnCode = FTP_EXCEPTION;
		CLogModule::Odprintf("[%s]Exception=%s", __FUNCTION__, e.displayText().c_str());
	}
	catch(...) {
		returnCode = FTP_UNKNOW_ERROR;
		CLogModule::Odprintf("[%s]unknow error", __FUNCTION__);
	}

	if (uploadSession != NULL) {
		delete uploadSession;
		uploadSession = NULL;
	}

	if (FTP_SUCCESS == returnCode || FTP_ALREAD_EXIST == returnCode) {
		msg_callback_obj.sendMsg(100, returnCode, guid);
	} else {
		msg_callback_obj.sendMsg(0, returnCode, guid);
	}

	return returnCode;
}

///	@brief �����ļ�
///	@param host ��������ַ
///	@param port �������˿�
///	@param username ��¼�û���
///	@param password ��¼����
///	@param remote_file Զ�̷�������Ҫ���ص��ļ�
///	@param local_file ���ص����ص��ļ�
///	@param guid 
///	@return FTPCode
int CMasterCtrl::downloadFile(const char* host, int port, const char* username, const char* password, 
							  const char* remote_file, const char* local_file, const char* guid) {
	std::string strResponse		= "";
	std::string strTmp			= "";
	std::string strRemoteSize	= "";

	int returnCode = -1;
	int serverStatus = 0;
	int pos	= 0;
	int percent = 0;
	int prePercent = percent;
	UINT64 localSize = 0;
	UINT64 remoteSize = 0;
	DWORD dwWriteType = std::ios::out | std::ios::binary;

	Poco::Net::FTPClientSession *downloadSession = NULL;
	
	try {
		CLogModule::Odprintf(_T("[%s]-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<"), __FUNCTIONW__);
		downloadSession = new Poco::Net::FTPClientSession(host, port);
		downloadSession->login(username, password);

		// ��ȡ��Զ���ļ���С
		serverStatus = downloadSession->sendCommand("MLST", remote_file, strResponse);
		if (2 == serverStatus / 100) { // 2-Զ���ļ�����
			pos = strResponse.find("size=");
			strTmp = strResponse.substr(pos);
			pos = strTmp.find(";");
			strRemoteSize = strTmp.substr(5, pos-5);
			remoteSize = _atoi64(strRemoteSize.c_str()); // Զ���ļ���С

			localSize = CPubFunc::MyGetFileSize(local_file);
			if (1 == (1 & global_config.m_iFlag) && (0 != remoteSize && remoteSize > localSize)) { // Զ���ļ���С�ȱ��ش�
				char szLocalSize[64] = {0};
				_i64toa(localSize, szLocalSize, 10);
				std::string strLocalSize(szLocalSize);
				serverStatus = downloadSession->sendCommand("REST", strLocalSize, strResponse);
				if (3 == serverStatus / 100) dwWriteType |= std::ios::app;
				else localSize = 0;
			}else localSize = 0;
		
			CLogModule::Odprintf("[%s]beginDownload=%s", __FUNCTION__, remote_file);
			std::istream& istr = downloadSession->beginDownload(remote_file);
			std::ofstream outFile(local_file, dwWriteType);

			if (outFile.is_open()) {
				Poco::Buffer<char> buffer(8192);
				UINT64 totalSize = localSize;

				istr.read(buffer.begin(), 8192);
				std::streamsize size = istr.gcount();

				while(size > 0) {
					totalSize += size;
					outFile.write(buffer.begin(), size);
					if (istr && outFile){
						istr.read(buffer.begin(), 8192);
						size = istr.gcount();
					} else size = 0;

					percent = (int)((totalSize * 100) / remoteSize);
					if (percent > 0 && 1 == percent % 2 && percent != prePercent) {
						prePercent = percent;
						msg_callback_obj.sendMsg(percent, FTP_DOWNLOADING, guid);
					}
				}

				returnCode = FTP_SUCCESS;
			} else { // �򿪱����ļ�ʧ��
				returnCode = FTP_OPEN_ERROR;
				CLogModule::Odprintf("[%s]open file=%s error=%d", __FUNCTION__, local_file, GetLastError());
			}

			outFile.close();
			downloadSession->endDownload();
			CLogModule::Odprintf("[%s]endDownload=%s", __FUNCTION__, remote_file);
		} else { // Զ���ļ�������
			returnCode = FTP_NOT_EXIST;
			CLogModule::Odprintf("[%s]remote file[%s] not exist!", __FUNCTION__, remote_file);
		}
	}
	catch(FTPException& e) {
		returnCode = FTP_FTPEXCEPTION;
		CLogModule::Odprintf("[%s]FTPException=%s", __FUNCTION__, e.displayText().c_str());
	}
	catch(Poco::Net::NetException& e) {
		returnCode = FTP_NETEXCEPTION;
		CLogModule::Odprintf("[%s]NetException=%s", __FUNCTION__, e.displayText().c_str());
	}
	catch(Poco::Exception& e) {
		returnCode = FTP_EXCEPTION;
		CLogModule::Odprintf("[%s]Exception=%s", __FUNCTION__, e.displayText().c_str());
	}
	catch(...) {
		returnCode = FTP_UNKNOW_ERROR;
		CLogModule::Odprintf("[%s]unknow error", __FUNCTION__);
	}

	if (NULL != downloadSession) {
		delete downloadSession;
		downloadSession = NULL;
	}

	if (FTP_SUCCESS == returnCode || FTP_ALREAD_EXIST == returnCode) {
		msg_callback_obj.sendMsg(100, returnCode, guid);
	} else {
		msg_callback_obj.sendMsg(0, returnCode, guid);
	}

	return returnCode;
}

void CMasterCtrl::registerCallback(PMsgCallback msg_callback) {
	msg_callback_obj.registerCallback(msg_callback);
}

///	@brief �жϷ������Ƿ�����
///	@param host ��ַ
///	@param port �˿�
///	@return 1 ���ߣ���������
int CMasterCtrl::checkOnline(const char* host, int port)
{
	int iRet = 0;
	Poco::Net::StreamSocket ss;
	try {
		Poco::Timespan timeout(5, 0);
		ss.connect(Poco::Net::SocketAddress(host, port), timeout);
		iRet = 1;
	}
	catch (Poco::TimeoutException& e)
	{
		iRet = -1;
		CLogModule::Odprintf("[%s]connect server[%s][%d] timeout[%s]", __FUNCTION__, host, port, e.displayText().c_str());
	}
	catch (Poco::Net::ConnectionRefusedException& e)
	{
		iRet = -2;
		CLogModule::Odprintf("[%s]connect server[%s][%d] exception[%s]", __FUNCTION__, host, port, e.displayText().c_str());
	}
	catch (Poco::Net::NetException& e)
	{
		iRet = -3;
		CLogModule::Odprintf("[%s]connect server[%s][%d] netException[%s]", __FUNCTION__, host, port, e.displayText().c_str());
	}
	catch(...)
	{
		iRet = -4;
		CLogModule::Odprintf("[%s]connect server[%s][%d] unknow error", __FUNCTION__, host, port);
	}

	ss.close();

	return iRet;
}
