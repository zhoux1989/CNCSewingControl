#include "StdAfx.h"
#include "SerialPort.h"
#include <process.h>
#include <iostream>
#include "resource.h"
// #include "SPLiningDlg.h"
 
//class CProtocol;
/** 线程退出标志 */


bool CSerialPort::s_bExit = false;
/** 当串口无数据时,sleep至下次查询间隔的时间,单位:秒 */
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
	//delete 删除的是动态内存，因为bufferread【2048】是静态内存 所以不存在
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

	/** 临时变量,将制定参数转化为字符串形式,以构造DCB结构 */
	char szDCBparam[50];
	sprintf_s(szDCBparam, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);

	/** 打开指定串口,该函数内部已经有临界区保护,上面请不要加保护 */
	if (!openPort(portNo))
	{
		return false;
	}

	/** 进入临界段 */
	EnterCriticalSection(&m_csCommunicationSync);

	/** 是否有错误发生 */
	BOOL bIsSuccess = TRUE;

	/** 在此可以设置输入输出的缓冲区大小,如果不设置,则系统会设置默认值.
	*  自己设置缓冲区大小时,要注意设置稍大一些,避免缓冲区溢出
	*/
	/*if (bIsSuccess )
	{
	bIsSuccess = SetupComm(m_hComm,10,10);
	}*/

	/** 设置串口的超时时间,均设为0,表示不使用超时限制 */
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
		// 将ANSI字符串转换为UNICODE字符串    
		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, NULL, 0);
		wchar_t *pwText = new wchar_t[dwNum];
		if (!MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, pwText, dwNum))
		{
			bIsSuccess = TRUE;
		}

		/** 获取当前串口配置参数,并且构造串口DCB参数 */
		bIsSuccess = GetCommState(m_hComm, &dcb) && BuildCommDCB(pwText, &dcb);
		/** 开启RTS flow控制 */
		dcb.fRtsControl = RTS_CONTROL_ENABLE;

		/** 释放内存空间 */
		delete[] pwText;
	}

	if (bIsSuccess)
	{
		/** 使用DCB参数配置串口状态 */
		bIsSuccess = SetCommState(m_hComm, &dcb);
	}

	/**  清空串口缓冲区 */
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	/** 离开临界段 */
	LeaveCriticalSection(&m_csCommunicationSync);

	return bIsSuccess == TRUE;
}

bool CSerialPort::InitPort(UINT portNo, const LPDCB& plDCB)
{
	/** 打开指定串口,该函数内部已经有临界区保护,上面请不要加保护 */
	if (!openPort(portNo))
	{
		return false;
	}
	/*if (NULL == m_pProtocol)
	{
	m_pProtocol = new CProtocol();
	}*/
	m_bIsListen = true;


	/** 进入临界段 */
	EnterCriticalSection(&m_csCommunicationSync);

	/** 配置串口参数 */
	if (!SetCommState(m_hComm, plDCB))
	{
		/** 离开临界段 */
		LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	/**  清空串口缓冲区 */
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	/** 离开临界段 */
	LeaveCriticalSection(&m_csCommunicationSync);

	return true;
}

void CSerialPort::ClosePort()
{
	/** 如果有串口被打开，关闭它 */
	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
	}
}

bool CSerialPort::openPort(UINT portNo)
{
	/** 进入临界段 */
	EnterCriticalSection(&m_csCommunicationSync);

	/** 把串口的编号转换为设备名 */
	char szPort[50];
	sprintf_s(szPort, "COM%d", portNo);

	/** 打开指定的串口 */
	m_hComm = CreateFileA(szPort,  /** 设备名,COM1,COM2等 */
		GENERIC_READ | GENERIC_WRITE, /** 访问模式,可同时读写 */
		0,                            /** 共享模式,0表示不共享 */
		NULL,                         /** 安全性设置,一般使用NULL */
		OPEN_EXISTING,                /** 该参数表示设备必须存在,否则创建失败 */
		0,
		0);

	/** 如果打开失败，释放资源并返回 */
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		LeaveCriticalSection(&m_csCommunicationSync);
		return false;
	}

	/** 退出临界区 */
	LeaveCriticalSection(&m_csCommunicationSync);

	return true;
}

bool CSerialPort::OpenListenThread()
{
	/** 检测线程是否已经开启了 */
	if (m_hListenThread != INVALID_HANDLE_VALUE)
	{
		/** 线程已经开启 */
		return false;
	}

	s_bExit = false;
	/** 线程ID */
	UINT threadId;
	/** 开启串口数据监听线程 */
	m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, ListenThread, this, 0, &threadId);
	//m_hListenThread = _beginthread(ListenThread, 0, this);
	//m_hListenThread = (HANDLE)_beginthread(ListenThread, 0, this);
	if (!m_hListenThread)
	{
		return false;
	}
	/** 设置线程的优先级,高于普通线程 */
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
		/** 通知线程退出 */
		s_bExit = true;

		/** 等待线程退出 */
		Sleep(10);

		/** 置线程句柄无效 */
		CloseHandle(m_hListenThread);
		m_hListenThread = INVALID_HANDLE_VALUE;
	}
	return true;
}

UINT CSerialPort::GetBytesInCOM()
{
	DWORD dwError = 0;  /** 错误码 */
	COMSTAT  comstat;   /** COMSTAT结构体,记录通信设备的状态信息 */
	memset(&comstat, 0, sizeof(COMSTAT));

	UINT BytesInQue = 0;
	/** 在调用ReadFile和WriteFile之前,通过本函数清除以前遗留的错误标志 */
	if (ClearCommError(m_hComm, &dwError, &comstat))
	{
		BytesInQue = comstat.cbInQue; /** 获取在输入缓冲区中的字节数 */
	}

	return BytesInQue;
}
  


UINT WINAPI CSerialPort::ListenThread(void* pParam)
{
	/** 得到本类的指针 */
	CSerialPort *pSerialPort = reinterpret_cast<CSerialPort*>(pParam);
	
	///终止读写并清空接收和发送
	PurgeComm(pSerialPort->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	// 第一次先清空窗口缓存区内容
	Sleep(20);
	UINT BytesInQue = pSerialPort->GetBytesInCOM();
	BytesInQue = 0;

	// 线程循环,轮询方式读取串口数据    
	while (!pSerialPort->s_bExit)
	{
		if (pSerialPort->m_bIsListen)
		{
			UINT BytesInQue = pSerialPort->GetBytesInCOM();
			/** 如果串口输入缓冲区中无数据,则休息一会再查询 */
			if (BytesInQue == 0)
			{
				Sleep(SLEEP_TIME_INTERVAL);
				continue;
			}
			else {
				//延时 保证收取整串数据
				if (pSerialPort->isReadParam == 1) {
					Sleep(100);
					BytesInQue = pSerialPort->GetBytesInCOM();
					pSerialPort->isReadParam = 2;
				}
				//延时 保证收取整串数据
				Sleep(30);
				BytesInQue = pSerialPort->GetBytesInCOM();
			}

			/** 读取输入缓冲区中的数据并输出显示 */
			//AfxMessageBox(_T("Start reading"));

			//if (pSerialPort->ReadChar(BytesInQue) == true)
			//{
			//	BytesInQue = 0;
			//	//AfxMessageBox(_T("Finish reading"));
			//}
			//过滤掉开始的00指令
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

	/** 临界区保护 */
	EnterCriticalSection(&m_csCommunicationSync);
	//memset(bufferRead, 0, 2048);
	/** 从缓冲区读取的数据 */
	bResult = ReadFile(m_hComm, &bufferRead, number, &BytesRead, NULL);
	//memcpy(bufferUse, bufferRead, number);

	if ((!bResult))
	{
		/** 获取错误码,可以根据该错误码查出错误原因 */
		DWORD dwError = GetLastError();

		/** 清空串口缓冲区 */
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	/** 离开临界区 */
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

	/** 临界区保护 */
	EnterCriticalSection(&m_csCommunicationSync);
	//memset(bufferRead, 0, 2048);
	/** 从缓冲区读取的数据 */
	bResult = ReadFile(m_hComm, &bufferRead[2], number, &BytesRead, NULL);
	//memcpy(bufferUse, bufferRead, number);

	if ((!bResult))
	{
		/** 获取错误码,可以根据该错误码查出错误原因 */
		DWORD dwError = GetLastError();

		/** 清空串口缓冲区 */
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	/** 离开临界区 */
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

	/** 临界区保护 */
	EnterCriticalSection(&m_csCommunicationSync);
	//memset(bufferRead, 0, 2048);
	/** 从缓冲区读取的数据 */
	bResult = ReadFile(m_hComm, &bufferRead, 2, &BytesRead, NULL);
	//memcpy(bufferUse, bufferRead, number);

	if ((!bResult))
	{
		/** 获取错误码,可以根据该错误码查出错误原因 */
		DWORD dwError = GetLastError();

		/** 清空串口缓冲区 */
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	/** 离开临界区 */
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

	/** 临界区保护 */
	EnterCriticalSection(&m_csCommunicationSync);

	/** 向缓冲区写入指定量的数据 */
	bResult = WriteFile(m_hComm, pData, length, &BytesToSend, NULL);
	if (!bResult)
	{
		DWORD dwError = GetLastError();
		/** 清空串口缓冲区 */
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	/** 离开临界区 */
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

	/** 临界区保护 */
	EnterCriticalSection(&m_csCommunicationSync);

	/** 向缓冲区写入指定量的数据 */
	bResult = WriteFile(m_hComm, protocol->m_buffer, protocol->m_length + 3, &BytesToSend, NULL);
	if (!bResult)
	{
		DWORD dwError = GetLastError();
		/** 清空串口缓冲区 */
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	/** 离开临界区 */
	LeaveCriticalSection(&m_csCommunicationSync);

	return true;
}