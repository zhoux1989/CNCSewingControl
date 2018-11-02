#include "stdafx.h"
#include "Protocol.h"

//初始化或者不输入值
CProtocol::CProtocol()
{
	m_requestCode = 0xE5;//请求码标志
	m_length = 0x02;//指令长度
	m_commandCode = 0x01;//指令码
	m_paramCode = 0x00;//指令参数
					   //memcpy(m_data, 0, sizeof(m_data));
					   //m_checkCode = 0x18;//校验码
					   //memcpy(m_buffer, 0, sizeof(m_buffer));
}

CProtocol::CProtocol(const BYTE* pData, const BYTE& dataLength,
	const BYTE& command, const BYTE& param, const BYTE& request)
{
	if (pData == NULL)
	{
		m_length = 2;
	}
	else
	{
		//		length > 126 ? m_nLength = 128 : m_nLength = length + 2;
		m_length = dataLength + 2;
		for (int i = 0; i < m_length - 2; i++)
		{
			m_data[i] = *pData;
			*pData++;
		}
	}

	m_paramCode = param;
	m_commandCode = command;
	m_requestCode = request;

	m_receiveCode = command;
}

CProtocol::CProtocol(const BYTE* pData, const BYTE& dataLength,
					 const BYTE& command, const BYTE& param, const BYTE& request, const BYTE& receive)
{
	if (pData == NULL)
	{
		m_length = 2;
	}
	else
	{
		//		length > 126 ? m_nLength = 128 : m_nLength = length + 2;
		m_length = dataLength + 2;
		for (int i = 0; i < m_length - 2; i++)
		{
			m_data[i] = *pData;
			*pData++;
		}
	}

	m_paramCode = param;
	m_commandCode = command;
	m_requestCode = request;

	m_receiveCode = receive;
}


CProtocol::~CProtocol()
{

}

BYTE CProtocol::GenCheckCode(CProtocol* oneCommand)
{
	WORD sum = 0;
	sum += oneCommand->m_requestCode;
	sum = sum & 0x00FF;
	sum += oneCommand->m_length;
	sum = sum & 0x00FF;
	sum += oneCommand->m_commandCode;
	sum = sum & 0x00FF;
	sum += oneCommand->m_paramCode;
	sum = sum & 0x00FF;
	for (int i = 0; i < oneCommand->m_length - 2; i++) 
	{
		sum += oneCommand->m_data[i];
		sum = sum & 0x00FF;
	}
	return(0x0100 - sum);
}

BYTE CProtocol::GenCheckCodeByByte(const BYTE * data, const BYTE & length)
{
	WORD sum = 0;
	for (int i = 0; i < length; i++)
	{
		sum += data[i];
		sum = sum & 0x00FF;
	}
	return(0x0100 - sum);
}

void CProtocol::SetRequestCode(const BYTE& requestCode)
{
	m_requestCode = requestCode;
}

void CProtocol::SetCommandCode(const BYTE& commandCode)
{
	m_commandCode = commandCode;
}

void CProtocol::SetParamCode(const BYTE & paramCode)
{
	m_paramCode = paramCode;
}


void CProtocol::SetDataAndLength(int length, const BYTE * data)
{
	m_length = 0xFF & length;
	for (int i = 0; i < length - 2; i++) {
		m_data[i] = data[i];
	}
}

SPCommand CProtocol::AnalyseReceive(BYTE * pBuffer, int size)
{
	/*SPCommand result = UNDEFINED_STATU;
	CProtocol tem;
	CString cs;
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
	tem.m_checkCode = GenCheckCode(&tem);
	if (tem.m_checkCode != pBuffer[size - 1]) {
		return UNDEFINED_STATU;
	}
	else {
		switch (tem.m_commandCode) {
		case 0x03:
			result = MAIN_READY;
			cs.Format(_T("MAIN_READY"));
			AfxMessageBox(cs);
			break;
		case 0x01:
			result = MAIN_BACK_PC_READY;
			cs.Format(_T("MAIN_BACK_PC_READY"));
			AfxMessageBox(cs);
			break;
		case 0x1d:
			result = MAIN_BACK_PC_ASK_PARAM;
			cs.Format(_T("MAIN_BACK_PC_ASK_PARAM"));
			AfxMessageBox(cs);
			break;
		case 0x11:
			if (tem.m_data[1] == 0x01) {
				result = MAIN_ASK_TAKE_PICTURE;
				cs.Format(_T("MAIN_ASK_TAKE_PICTURE"));
				AfxMessageBox(cs);
			}
			else if (tem.m_data[1] == 0x02) {
				result = MAIN_ASK_POINTS;
				cs.Format(_T("MAIN_ASK_POINTS"));
				AfxMessageBox(cs);
			}
			else if (tem.m_data[1] == 0x03) {
				result = MAIN_ASK_PICTURE_AND_POINTS;
				cs.Format(_T("MAIN_ASK_PICTURE_AND_POINTS"));
				AfxMessageBox(cs);
			}
			break;
		case 0x75:
			result = MAIN_BACK_EMPTY_TRUE;
			cs.Format(_T("MAIN_BACK_EMPTY_TRUE"));
			AfxMessageBox(cs);
			break;
		case 0x79:
			result = MAIN_BACK_SEND_PATTERN_FINISH;
			cs.Format(_T("MAIN_BACK_SEND_PATTERN_FINISH"));
			AfxMessageBox(cs);
			break;
		default:
			cs.Format(_T("Default"));
			AfxMessageBox(cs);
			break;
		}
	}*/
	SPCommand result = UNDEFINED_STATU;
	return result;
}

BOOL CProtocol::GenBuffer()
{
	BYTE* pData = &m_buffer[0];
	*pData++ = m_requestCode;
	*pData++ = m_length;
	*pData++ = m_commandCode;
	*pData++ = m_paramCode;
	memcpy(pData, &m_data[0], m_length - 2);
	BYTE checkCode = GenCheckCodeByByte(m_buffer, m_length + 2);
	*(pData + m_length - 2) = checkCode;

	return TRUE;
}
