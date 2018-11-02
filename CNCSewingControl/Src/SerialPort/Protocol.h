#pragma once

/* Protocol���� */

enum SPCommand
{
	//����ͨѶ
	MAIN_ERROR, //���屨�� 0x5C
	MAIN_READY, //PC��Ӧ�� 0x03
	PC_READY,   //0x01
	MAIN_BACK_PC_READY, // 0x01
	PC_ASK_PARAM,// 0x1C
	MAIN_BACK_PC_ASK_PARAM, // 0x1D

	//�궨��ͨѶ
	PC_ENTER_PRO, //0x2A
	MAIN_BACK_PC_ENTER_PRO, //0x2A
	PC_MOVE_ZERO,//0x1A
	MAIN_BACK_PC_MOVE_ZERO,//0x1A
	MAIN_MOVE_ZERO_FINISH,//0x1B
	PC_BACK_MAIN_MOVE_ZERO_FINISH,//0x1B
	//PC_MOVE_PREPARE, //0x06
	//MAIN_BACK_PC_MOVE_PREPARE, //0x06
	PC_MOVE, //0x4D
	MAIN_BACK_PC_MOVE, //0x4D
	MAIN_MOVE_FINISH, //0x4E
	PC_BACK_MAIN_MOVE_FINISH, //0x4E 
	PC_EXIT_PRO, //0x2B
	MAIN_BACK_PC_EXIT_PRO, //0x2B

	//�㺸��ʱ/Ԥ��/��λ����������ָ��
	PC_HEAT_SETTING,  //0x9E CA 27
	MAIN_BACK_PC_HEAT_SETTING, //0x9E CA 27

	//���Ҳ�����
	LEFT_START, // 0xA0
	RIGHT_START, // 0xA0
	PC_SEND_CONTROL, //0x9E  CA 20
	MAIN_BACK_PC_SEND_CONTROL, //0x9E   CA 20
	PC_ASK_EMPTY,  //PCѯ�ʻ����ļ��Ƿ����  0x74
	MAIN_BACK_EMPTY_TRUE, //MAIN �޴��ļ����� 0x75
	MAIN_BACK_EMPTY_FALSE, //�д��ļ�����
	PC_SEND_POINTS_TO_CREATE_PATTERN, // 0x78
	MAIN_BACK_PC_SEND_POINTS_CREATE_PATTERN_TRUE, //0x79
	PC_SEND_PATTERN_FINSH, //0x57
	MAIN_BACK_SEND_PATTERN_FINISH, //0x57
	//����
	//PC_AFTER_SEND_PATTERN_FINSIH, //0x9E
	//MAIN_BACK_AFTER_SEND_PATTERN_FINISH, //0x9E
	UNDEFINED_STATU
};


class CProtocol
{
public:
	CProtocol();
	CProtocol::CProtocol(const BYTE* pData, const BYTE& dataLength, const BYTE& command, const BYTE& param, const BYTE& request);
	CProtocol::CProtocol(const BYTE* pData, const BYTE& dataLength, const BYTE& command, const BYTE& param, const BYTE& request, const BYTE& receive);
	~CProtocol();

	BYTE GenCheckCode(CProtocol* oneCommand);
	static BYTE GenCheckCodeByByte(const BYTE* data, const BYTE& length);
	void SetRequestCode(const BYTE& requestCode);
	void SetCommandCode(const BYTE& commandCode);
	void SetParamCode(const BYTE& paramCode);
	void SetDataAndLength(int length, const BYTE* data);
	SPCommand AnalyseReceive(BYTE * pBuffer, int size);
	BOOL GenBuffer();

public:
	BYTE  m_requestCode;//������
	BYTE  m_length;//ָ���
	BYTE  m_commandCode;//ָ����
	BYTE  m_paramCode;//������
	BYTE  m_data[1024];//ָ������
	BYTE  m_checkCode;//У����

	BYTE m_receiveCode;	// ���յ�ָ���루������֤NC��������Ϣ��

	BYTE  m_buffer[1024];
};

