#include "StdAfx.h"
#include "SerialPort.h"
#include <process.h>
#include <iostream>
#include "resource.h"
// #include "SPLiningDlg.h"
 
//class CProtocol;
/** �߳��˳���־ */


bool CSerialPort::s_bExit = false;
/** ������������ʱ,sleep���´β�ѯ�����ʱ��,��λ:�� */
const UINT SLEEP_TIME_INTERVAL = 1;

CSerialPort::CSerialPort(void)
	: m_hListenThread(INVALID_HANDLE_VALUE)
{
	m_hComm = INVALID_HANDLE_VALUE;
	m_hListenThread = INVALID_HANDLE_VALUE;
	InitializeCriticalSection(&m_csCommunicationSync);
	m_statusSerialPort = UNDEFINED_STATU;
	m_bIsListen = false;
	m_bIsInit = false;
	patternNo = 260;
	flag = -1;
	packetIndex = -1;
	isReadParam = -1;
	markNo = -1;
	isFirstPointing = true;
	leftOrRight = 0;
	m_pMainFrm = NULL;
}

CSerialPort::~CSerialPort(void)
{
	CloseListenTread();
	ClosePort();
	//delete ɾ�����Ƕ�̬�ڴ棬��Ϊbufferread��2048���Ǿ�̬�ڴ� ���Բ�����
	//delete bufferRead;
	/*if (NULL != m_pProtocol)
	{
	delete m_pProtocol;
	m_pProtocol = NULL;
	}*/

	DeleteCriticalSection(&m_csCommunicationSync);
}

bool CSerialPort::InitPort(UINT portNo /*= 1*/, UINT baud /*= CBR_57600*/, char parity /*= 'N'*/,
	UINT databits /*= 8*/, UINT stopsbits /*= 1*/, DWORD dwCommEvents /*= EV_RXCHAR*/)
{

	/*if (NULL == m_pProtocol)
	{
	m_pProtocol = new CProtocol();
	}*/
	//1120
	//m_bIsListen = true;

	/** ��ʱ����,���ƶ�����ת��Ϊ�ַ�����ʽ,�Թ���DCB�ṹ */
	char szDCBparam[50];
	sprintf_s(szDCBparam, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);

	/** ��ָ������,�ú����ڲ��Ѿ����ٽ�������,�����벻Ҫ�ӱ��� */
	if (!openPort(portNo))
	{
		return false;
	}

	/** �����ٽ�� */
	EnterCriticalSection(&m_csCommunicationSync);

	/** �Ƿ��д����� */
	BOOL bIsSuccess = TRUE;

	/** �ڴ˿���������������Ļ�������С,���������,��ϵͳ������Ĭ��ֵ.
	*  �Լ����û�������Сʱ,Ҫע�������Դ�һЩ,���⻺�������
	*/
	/*if (bIsSuccess )
	{
	bIsSuccess = SetupComm(m_hComm,10,10);
	}*/

	/** ���ô��ڵĳ�ʱʱ��,����Ϊ0,��ʾ��ʹ�ó�ʱ���� */
	COMMTIMEOUTS  CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 0;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 0;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 0;
	if (bIsSuccess)
	{
		bIsSuccess = SetCommTimeouts(m_hComm, &CommTimeouts);
	}

	DCB  dcb;
	if (bIsSuccess)
	{
		// ��ANSI�ַ���ת��ΪUNICODE�ַ���    
		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, NULL, 0);
		wchar_t *pwText = new wchar_t[dwNum];
		if (!MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, pwText, dwNum))
		{
			bIsSuccess = TRUE;
		}

		/** ��ȡ��ǰ�������ò���,���ҹ��촮��DCB���� */
		bIsSuccess = GetCommState(m_hComm, &dcb) && BuildCommDCB(pwText, &dcb);
		/** ����RTS flow���� */
		dcb.fRtsControl = RTS_CONTROL_ENABLE;

		/** �ͷ��ڴ�ռ� */
		delete[] pwText;
	}

	if (bIsSuccess)
	{
		/** ʹ��DCB�������ô���״̬ */
		bIsSuccess = SetCommState(m_hComm, &dcb);
	}

	/**  ��մ��ڻ����� */
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	/** �뿪�ٽ�� */
	LeaveCriticalSection(&m_csCommunicationSync);

	return bIsSuccess == TRUE;
}

bool CSerialPort::InitPort(UINT portNo, const LPDCB& plDCB)
{
	/** ��ָ������,�ú����ڲ��Ѿ����ٽ�������,�����벻Ҫ�ӱ��� */
	if (!openPort(portNo))
	{
		return false;
	}
	/*if (NULL == m_pProtocol)
	{
	m_pProtocol = new CProtocol();
	}*/
	m_bIsListen = true;


	/** �����ٽ�� */
	EnterCriticalSection(&m_csCommunicationSync);

	/** ���ô��ڲ��� */
	if (!SetCommState(m_hComm, plDCB))
	{
		/** �뿪�ٽ�� */
		LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	/**  ��մ��ڻ����� */
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	/** �뿪�ٽ�� */
	LeaveCriticalSection(&m_csCommunicationSync);

	return true;
}

void CSerialPort::ClosePort()
{
	/** ����д��ڱ��򿪣��ر��� */
	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
	}
}

bool CSerialPort::openPort(UINT portNo)
{
	/** �����ٽ�� */
	EnterCriticalSection(&m_csCommunicationSync);

	/** �Ѵ��ڵı��ת��Ϊ�豸�� */
	char szPort[50];
	sprintf_s(szPort, "COM%d", portNo);

	/** ��ָ���Ĵ��� */
	m_hComm = CreateFileA(szPort,  /** �豸��,COM1,COM2�� */
		GENERIC_READ | GENERIC_WRITE, /** ����ģʽ,��ͬʱ��д */
		0,                            /** ����ģʽ,0��ʾ������ */
		NULL,                         /** ��ȫ������,һ��ʹ��NULL */
		OPEN_EXISTING,                /** �ò�����ʾ�豸�������,���򴴽�ʧ�� */
		0,
		0);

	/** �����ʧ�ܣ��ͷ���Դ������ */
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		LeaveCriticalSection(&m_csCommunicationSync);
		return false;
	}

	/** �˳��ٽ��� */
	LeaveCriticalSection(&m_csCommunicationSync);

	return true;
}

bool CSerialPort::OpenListenThread()
{
	/** ����߳��Ƿ��Ѿ������� */
	if (m_hListenThread != INVALID_HANDLE_VALUE)
	{
		/** �߳��Ѿ����� */
		return false;
	}

	s_bExit = false;
	/** �߳�ID */
	UINT threadId;
	/** �����������ݼ����߳� */
	m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, ListenThread, this, 0, &threadId);
	//m_hListenThread = _beginthread(ListenThread, 0, this);
	//m_hListenThread = (HANDLE)_beginthread(ListenThread, 0, this);
	if (!m_hListenThread)
	{
		return false;
	}
	/** �����̵߳����ȼ�,������ͨ�߳� */
	if (!SetThreadPriority(m_hListenThread, THREAD_PRIORITY_ABOVE_NORMAL))
	{
		return false;
	}

	return true;
}

bool CSerialPort::CloseListenTread()
{
	if (m_hListenThread != INVALID_HANDLE_VALUE)
	{
		/** ֪ͨ�߳��˳� */
		s_bExit = true;

		/** �ȴ��߳��˳� */
		Sleep(10);

		/** ���߳̾����Ч */
		CloseHandle(m_hListenThread);
		m_hListenThread = INVALID_HANDLE_VALUE;
	}
	return true;
}

UINT CSerialPort::GetBytesInCOM()
{
	DWORD dwError = 0;  /** ������ */
	COMSTAT  comstat;   /** COMSTAT�ṹ��,��¼ͨ���豸��״̬��Ϣ */
	memset(&comstat, 0, sizeof(COMSTAT));

	UINT BytesInQue = 0;
	/** �ڵ���ReadFile��WriteFile֮ǰ,ͨ�������������ǰ�����Ĵ����־ */
	if (ClearCommError(m_hComm, &dwError, &comstat))
	{
		BytesInQue = comstat.cbInQue; /** ��ȡ�����뻺�����е��ֽ��� */
	}

	return BytesInQue;
}
  


UINT WINAPI CSerialPort::ListenThread(void* pParam)
{
	/** �õ������ָ�� */
	CSerialPort *pSerialPort = reinterpret_cast<CSerialPort*>(pParam);
	
	///��ֹ��д����ս��պͷ���
	PurgeComm(pSerialPort->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	// ��һ������մ��ڻ���������
	Sleep(20);
	UINT BytesInQue = pSerialPort->GetBytesInCOM();
	BytesInQue = 0;

	// �߳�ѭ��,��ѯ��ʽ��ȡ��������    
	while (!pSerialPort->s_bExit)
	{
		if (pSerialPort->m_bIsListen)
		{
			UINT BytesInQue = pSerialPort->GetBytesInCOM();
			/** ����������뻺������������,����Ϣһ���ٲ�ѯ */
			if (BytesInQue == 0)
			{
				Sleep(SLEEP_TIME_INTERVAL);
				continue;
			}
			else {
				//��ʱ ��֤��ȡ��������
				if (pSerialPort->isReadParam == 1) {
					Sleep(100);
					BytesInQue = pSerialPort->GetBytesInCOM();
					pSerialPort->isReadParam = 2;
				}
				//��ʱ ��֤��ȡ��������
				Sleep(30);
				BytesInQue = pSerialPort->GetBytesInCOM();
			}

			/** ��ȡ���뻺�����е����ݲ������ʾ */
			//AfxMessageBox(_T("Start reading"));

			//if (pSerialPort->ReadChar(BytesInQue) == true)
			//{
			//	BytesInQue = 0;
			//	//AfxMessageBox(_T("Finish reading"));
			//}
			//���˵���ʼ��00ָ��
			if (BytesInQue > 1) {
				if (pSerialPort->ReadCommandLength()) {
					BytesInQue -= 2;
					int length = pSerialPort->bufferRead[1] + 1;
					if (pSerialPort->ReadCharNew(length)) {
						BytesInQue -= (length);
					}
					else {
						BytesInQue = 0;
					}
				}
			}
			else {
				BytesInQue = 0;
			}
		}
		else
		{
			Sleep(1);
		}
	}
	return 0;
}

SPCommand CSerialPort::AnalyseReceiveInPort(BYTE * pBuffer, int size)
{
	SPCommand result = UNDEFINED_STATU;
	CProtocol tem;
	CString cs;
	/*cs.Format(_T("%d"),size);
	AfxMessageBox(cs);*/
	if (pBuffer == NULL || size < 5) {
		return UNDEFINED_STATU;
	}
	else {
		tem.m_requestCode = pBuffer[0];
		tem.m_length = pBuffer[1];
		tem.m_commandCode = pBuffer[2];
		tem.m_paramCode = pBuffer[3];
	}
	for (int i = 4; i < size - 1; i++) {
		tem.m_data[i - 4] = pBuffer[i];
	}
	tem.m_checkCode = tem.GenCheckCode(&tem);
	if (tem.m_checkCode != pBuffer[size - 1])
	{
		/*if (tem.m_commandCode == 0x1B) {
		m_pDemoDlg->SendBackMovePointFinish();
		markNo++;
		m_pDemoDlg->SendMovePoint(markNo);
		}*/
		cs.Format(_T("Command %d check code wrong"), tem.m_commandCode);
		AfxMessageBox(cs);
		return UNDEFINED_STATU;
	}
	else
	{
		if (m_pMainFrm != NULL)
		{
			::SendMessage(m_pMainFrm->m_hWnd, ID_CMD_RECEIVEDATA, tem.m_commandCode, (LPARAM)tem.m_data);
		}
	}
	return result;
}



bool CSerialPort::ReadChar(int number)
{
	BOOL  bResult = TRUE;
	DWORD BytesRead = 0;
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	/** �ٽ������� */
	EnterCriticalSection(&m_csCommunicationSync);
	//memset(bufferRead, 0, 2048);
	/** �ӻ�������ȡ������ */
	bResult = ReadFile(m_hComm, &bufferRead, number, &BytesRead, NULL);
	//memcpy(bufferUse, bufferRead, number);

	if ((!bResult))
	{
		/** ��ȡ������,���Ը��ݸô�����������ԭ�� */
		DWORD dwError = GetLastError();

		/** ��մ��ڻ����� */
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	/** �뿪�ٽ��� */
	LeaveCriticalSection(&m_csCommunicationSync);
	//CString cs;
	//cs.Format(_T("IT is  "), bufferRead[1]);
	//AfxMessageBox(cs);
	/*CString cs;
	for (int i = 0; i < number; i++) {
	cs.Format(_T("The Status %d"), bufferUse[i]);
	AfxMessageBox(cs);
	}*/
	//m_pProtocol->ReNewProtocol();
	///////////////////0816
	if (bResult && (BytesRead == number) && BytesRead > 0)
	{
		m_statusSerialPort = AnalyseReceiveInPort(bufferRead, BytesRead);
		//m_pDemoDlg->linesIndex++;
		//m_pDemoDlg->WriteComLog(m_pDemoDlg->linesIndex,bufferRead);
	}
	return (BytesRead == number);
}

bool CSerialPort::ReadCharNew(int number)
{
	BOOL  bResult = TRUE;
	DWORD BytesRead = 0;
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	/** �ٽ������� */
	EnterCriticalSection(&m_csCommunicationSync);
	//memset(bufferRead, 0, 2048);
	/** �ӻ�������ȡ������ */
	bResult = ReadFile(m_hComm, &bufferRead[2], number, &BytesRead, NULL);
	//memcpy(bufferUse, bufferRead, number);

	if ((!bResult))
	{
		/** ��ȡ������,���Ը��ݸô�����������ԭ�� */
		DWORD dwError = GetLastError();

		/** ��մ��ڻ����� */
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	/** �뿪�ٽ��� */
	LeaveCriticalSection(&m_csCommunicationSync);
	//CString cs;
	//cs.Format(_T("IT is  "), bufferRead[1]);
	//AfxMessageBox(cs);
	/*CString cs;
	for (int i = 0; i < number; i++) {
	cs.Format(_T("The Status %d"), bufferUse[i]);
	AfxMessageBox(cs);
	}*/
	//m_pProtocol->ReNewProtocol();
	///////////////////0816
	if (bResult && (BytesRead == number) && BytesRead > 0)
	{
		m_statusSerialPort = AnalyseReceiveInPort(bufferRead, BytesRead + 2);
		//m_pDemoDlg->linesIndex++;
		//m_pDemoDlg->WriteComLog(m_pDemoDlg->linesIndex,bufferRead);
	}
	return (BytesRead == number);
}

bool CSerialPort::ReadCommandLength()
{
	BOOL  bResult = TRUE;
	DWORD BytesRead = 0;
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	/** �ٽ������� */
	EnterCriticalSection(&m_csCommunicationSync);
	//memset(bufferRead, 0, 2048);
	/** �ӻ�������ȡ������ */
	bResult = ReadFile(m_hComm, &bufferRead, 2, &BytesRead, NULL);
	//memcpy(bufferUse, bufferRead, number);

	if ((!bResult))
	{
		/** ��ȡ������,���Ը��ݸô�����������ԭ�� */
		DWORD dwError = GetLastError();

		/** ��մ��ڻ����� */
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	/** �뿪�ٽ��� */
	LeaveCriticalSection(&m_csCommunicationSync);
	//CString cs;
	//cs.Format(_T("IT is  "), bufferRead[1]);
	//AfxMessageBox(cs);
	/*CString cs;
	for (int i = 0; i < number; i++) {
	cs.Format(_T("The Status %d"), bufferUse[i]);
	AfxMessageBox(cs);
	}*/
	//m_pProtocol->ReNewProtocol();
	///////////////////0816
	if (bResult && (BytesRead == 2) && BytesRead > 0)
	{
		//m_statusSerialPort = AnalyseReceiveInPort(bufferRead, BytesRead);
		//m_pDemoDlg->linesIndex++;
		//m_pDemoDlg->WriteComLog(m_pDemoDlg->linesIndex,bufferRead);
	}
	return (BytesRead == 2);
}


bool CSerialPort::WriteData(BYTE* pData, unsigned int length)
{
	BOOL   bResult = TRUE;
	DWORD  BytesToSend = 0;
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	/** �ٽ������� */
	EnterCriticalSection(&m_csCommunicationSync);

	/** �򻺳���д��ָ���������� */
	bResult = WriteFile(m_hComm, pData, length, &BytesToSend, NULL);
	if (!bResult)
	{
		DWORD dwError = GetLastError();
		/** ��մ��ڻ����� */
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	/** �뿪�ٽ��� */
	LeaveCriticalSection(&m_csCommunicationSync);

	return true;
}

bool CSerialPort::WriteProtocol(CProtocol* protocol)
{
	BOOL   bResult = TRUE;
	DWORD  BytesToSend = 0;
	protocol->GenBuffer();
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	/** �ٽ������� */
	EnterCriticalSection(&m_csCommunicationSync);

	/** �򻺳���д��ָ���������� */
	bResult = WriteFile(m_hComm, protocol->m_buffer, protocol->m_length + 3, &BytesToSend, NULL);
	if (!bResult)
	{
		DWORD dwError = GetLastError();
		/** ��մ��ڻ����� */
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	/** �뿪�ٽ��� */
	LeaveCriticalSection(&m_csCommunicationSync);

	return true;
}