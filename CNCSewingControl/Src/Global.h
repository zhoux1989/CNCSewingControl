//===================================================================================================
// Summary:
//		SmartDoctor�ڲ�ʹ�õĹ�������
// Date:
//		2010-12-08
// Author:
//		YangXuefeng(yxf4640084@sina.com.cn)
//===================================================================================================

#ifndef __Global_h__
#define __Global_h__

//===================================================================================================

#include "BaseMacro.h"

//===================================================================================================

// ָ�����
typedef enum __tagFileCMD
{
	CT_NULL,
	CT_START,
	CT_RENAME_SEWFILE,
	CT_DOWNLOAD_SEWFILE,
	CT_UPLOAD_SEWFILE,
	CT_DELETE_SEWFILE,
	CT_CURRENT_SEW,
	CT_PREVIEW_SEW
} FileCMD;

// ����ģ��
typedef enum __tagModeType
{
	MT_NULL,
	MT_SEWINGINFO,
	MT_CURRENT_SEWFILE,
	MT_SEWFILE_MANAGE,
	MT_PARAM_SET,
	MT_IO_CHECK,
	MT_SEWFILE_EDIT
} ModeType;

// Ԥ��
typedef enum __tagPreviewState
{
	PS_CLOSE,		// �ر�Ԥ��
	PS_PREPARE,		// ׼����
	PS_RECEIVE,		// ������
	PS_VALID,		// ��Ч��������ʾԤ����
	PS_INVALID,		// ��Ч��Ԥ��ʧ�ܣ�
	PS_NOCURRENT,	// δ���õ�ǰ����
} PreviewState;

typedef vector<BYTE> ByteArray;

typedef BYTE FILEDATABYTE[126];

// ������Ϣ
typedef struct __tagParaInfo
{
	int nID;						// �������
	CString strParaName;			// ������
	int nType;						// ����ֵ����(0:OFF/ON)
	int nCurrentValue;				// ��ǰֵ
	int nDefaultValue;				// ��ʼֵ
	int nMinValue;					// ��Сֵ
	int nMaxValue;					// ���ֵ
	int nStep;						// ����ֵ
	CString strDescription;			// ����˵��

	__tagParaInfo()
	{
		nID = -1;
		strParaName = L"δ֪����";
		nType = 1;
		nCurrentValue = 0;
		nDefaultValue= 0;
		nMinValue = 0;
		nMaxValue = 1;
		nStep = 1;
		strDescription = L"";
	}
	__tagParaInfo& operator=(const __tagParaInfo &pi)
	{
		if (&pi == this)
			return *this;
		nID = pi.nID;
		strParaName = pi.strParaName;
		nType = pi.nType;
		nCurrentValue = pi.nCurrentValue;
		nDefaultValue = pi.nDefaultValue;
		nMinValue = pi.nMinValue;
		nMaxValue = pi.nMaxValue;
		nStep = pi.nStep;
		strDescription = pi.strDescription;
		return *this;
	}
} ParaInfo;

typedef vector<ParaInfo> ParaInfoArray;

typedef map<int, ParaInfo> MapParaInf;

// �����ļ���Ϣ
typedef struct __tagSewFileData
{
	int nID;				// �ļ�ID
	ByteArray arrByte;		// ����
	ByteArray arrBlockSize;	// �鳤��
	CString strPath;		// �ļ�·�������Ա����ļ���Ч��
	
	__tagSewFileData()
	{
		nID = -1;
		strPath = L"";
	}
	__tagSewFileData& operator=(const __tagSewFileData &si)
	{
		if (&si == this)
			return *this;
		nID = si.nID;
		arrByte = si.arrByte;
		arrBlockSize = si.arrBlockSize;
		strPath = si.strPath;
		
		return *this;
	}
} SewFileData;

typedef vector<SewFileData> SewFileDataArray;

// ���������Ϣ
typedef struct __tagSewingInfo
{
	int nBottomLineCounter;	// ���߼�����
	int nWorkpiectCounter;	// ����������
	int nCurrentSewFile;	// ��ǰ������
	int nSewMode;			// ����ģʽ
	int nSpindleSpeed;		// �����ٶ�
	int nMediumLegHeight;	// ��ѹ�Ÿ߶�
	int nLineTensionA;		// ������1
	int nLineTensionB;		// ������2

	__tagSewingInfo()
	{
		nBottomLineCounter = -1;
		nWorkpiectCounter = -1;
		nCurrentSewFile = -1;
		nSewMode = -1;
		nSpindleSpeed = -1;
		nMediumLegHeight = -1;
		nLineTensionA = -1;
		nLineTensionB = -1;
	}
	__tagSewingInfo& operator=(const __tagSewingInfo &si)
	{
		if (&si == this)
			return *this;
		nBottomLineCounter = si.nBottomLineCounter;
		nWorkpiectCounter = si.nWorkpiectCounter;
		nCurrentSewFile = si.nCurrentSewFile;
		nSewMode = si.nSewMode;
		nSpindleSpeed = si.nSpindleSpeed;
		nMediumLegHeight = si.nMediumLegHeight;
		nLineTensionA = si.nLineTensionA;
		nLineTensionB = si.nLineTensionB;
		return *this;
	}
} SewingInfo;

// IO�������
typedef struct __tagIOCheckData
{
	int nID;						// �������
	CString strDesc;				// ���������
	int nType;						// ��������ͣ�1��2��
	int nMode;						// ��ѯģʽ��0Ϊ������1Ϊר������ڡ�2Ϊͨ������ڡ�3Ϊ����ڡ�11Ϊͨ������ڡ�12Ϊ���������˿ڡ�10��ʾ���������ԣ�
	int nBit;						// �˿ںţ�����nType = 1��Ч��
	int nState;						// ��ǰ״̬
	int nStep;						// ����ֵ������nMode = 10��Ч��

	__tagIOCheckData()
	{
		nID = -1;
		strDesc = L"����";
		nType = 1;
		nMode = 0;
		nBit= 0;
		nState = 0;
		nStep = 0;
	}
	__tagIOCheckData& operator=(const __tagIOCheckData &cd)
	{
		if (&cd == this)
			return *this;
		nID = cd.nID;
		strDesc = cd.strDesc;
		nType = cd.nType;
		nMode = cd.nMode;
		nBit = cd.nBit;
		nState = cd.nState;
		nStep = cd.nStep;
		return *this;
	}
} IOCheckData;

typedef vector<IOCheckData> IOCheckDataArray;

// ָ���¼��Ϣ
typedef struct __tagCodeRecode
{
	BYTE m_byteQACode;	// �������Ӧ����
	BYTE m_byteCmdCode;	// ָ����
	BOOL bAnswered;		// ִ��״̬

	__tagCodeRecode()
	{
		m_byteQACode = 0;
		m_byteCmdCode = 0;
		bAnswered = TRUE;
	}
	__tagCodeRecode(BYTE qa, BYTE cmd, BOOL bA)
	{
		m_byteQACode = qa;
		m_byteCmdCode = cmd;
		bAnswered = bA;
	}
	__tagCodeRecode& operator=(const __tagCodeRecode &cr)
	{
		if (&cr == this)
			return *this;
		m_byteQACode = cr.m_byteQACode;
		m_byteCmdCode = cr.m_byteCmdCode;
		bAnswered = cr.bAnswered;

		return *this;
	}
} CodeRecode;

// �����벽���ݽṹ
typedef struct __tagNeedleStepData
{
	long lPulseOffsetX;		// X����������
	long lPulseOffsetY;		// Y����������
	BOOL bEmptyMoveFlag;	// ���ƻ�����--D15 0--���� 1--����

	__tagNeedleStepData()
	{
		lPulseOffsetX = 0;
		lPulseOffsetY = 0;
		bEmptyMoveFlag = FALSE;
	}

	__tagNeedleStepData& operator=(const __tagNeedleStepData &nsd)
	{
		if (&nsd == this)
			return *this;
		lPulseOffsetX = nsd.lPulseOffsetX;
		lPulseOffsetY = nsd.lPulseOffsetY;
		bEmptyMoveFlag = nsd.bEmptyMoveFlag;
		return *this;
	}
}NeedleStepData;

typedef vector<NeedleStepData> NeedleNodeDataArray;

// �����켣���ݽṹ
typedef struct __tagNeedleTrailData
{
	CString strDesc;
	int nStartPulseX;	// ���X������
	int nStartPulseY;	// ���Y������
	int nPlusMaxX;		// X���������ֵ
	int nPlusMaxY;		// Y���������ֵ
	int nMinusMaxX;	// X�Ḻ�����ֵ
	int nMinusMaxY;	// Y�Ḻ�����ֵ
	NeedleNodeDataArray arrNode;

	__tagNeedleTrailData()
	{
		strDesc = L"";
		nStartPulseX = 0;
		nStartPulseY = 0;
		nPlusMaxX = 0;
		nPlusMaxY = 0;
		nMinusMaxX = 0;
		nMinusMaxY = 0;
	}
	__tagNeedleTrailData& operator=(const __tagNeedleTrailData &ntd)
	{
		if (&ntd == this)
			return *this;
		strDesc = ntd.strDesc;
		nStartPulseX = ntd.nStartPulseX;
		nStartPulseY = ntd.nStartPulseY;
		nPlusMaxX = ntd.nPlusMaxX;
		nPlusMaxY = ntd.nPlusMaxY;
		nMinusMaxX = ntd.nMinusMaxX;
		nMinusMaxY = ntd.nMinusMaxY;
		arrNode = ntd.arrNode;
		return *this;
	}
} NeedleTrailData;

// Ԥ���������ݽṹ
typedef struct __tagPreviewControlData
{
	CPoint ptOrigin;	// �ͻ���Ԥ������ԭ��
	CRect rectPreview;	// �ͻ���Ԥ����Χ
	CString strText;	// �ͻ���Ҫ��ʾ������
	BOOL bShowEmptyMoveTrail;	// ��ʾ���ƹ켣

	__tagPreviewControlData()
	{
		strText = L"";
		bShowEmptyMoveTrail = TRUE;
	}
	__tagPreviewControlData& operator=(const __tagPreviewControlData &pcd)
	{
		if (&pcd == this)
			return *this;
		rectPreview = pcd.rectPreview;
		ptOrigin = pcd.ptOrigin;
		strText = pcd.strText;
		bShowEmptyMoveTrail = pcd.bShowEmptyMoveTrail;
		return *this;
	}
} PreviewControlData;

//===================================================================================================

// ��ȡ�ļ������ͣ�����ֵ�ο�READERFILETYPEö��ֵ
int GetFileOpenType(const CString& strFileName);
// ��ȡ�ļ���չ��
BOOL GetFileExtName(const CString& strFilePathName, CString& strRetExt);
// ����Ƿ����ָ����Ŀ¼
BOOL IsFolderExist(LPCTSTR pszFolderName);
// �ж��ļ��Ƿ���ڡ�
BOOL IsFileExist(LPCTSTR pszFileName);
// �ж��ļ��Ƿ񱻴򿪡�
BOOL IsFileOpened(LPCTSTR pszFileName);
// ��ȫɾ���ļ���
void SafeDeleteFile(LPCTSTR pszFileName);
// ɾ��ָ��Ŀ¼�µ��ļ����ļ���
BOOL DeleteDirectoryFiles(LPCTSTR pszFolderName);
// ��ȡһ����ʱ�ļ�����
BOOL GetTempFile(CString& strTempFileName, const CString &strExt = _T("tmp"));
// �õ��ļ��Ķ���������
BOOL GetFileData(const CString &strFilePath, BYTE *&pFileData, DWORD &nLength);
// �������Ƕ�׵�Ŀ¼��
BOOL RecursiveCreateDirectory(LPCTSTR szPath);
// ��ȡ����������·����
void GetMainModulePath(HMODULE hModule, CString& strPath);
// ��ȡ·������һ��·��
CString GetPrevPath(const CString &strPath);
// ��ȡ������İ汾��Ϣ��
BOOL GetApabiVersion(DWORD& dwMajorVersion, DWORD& dwMinorVersion);
// �ж��ַ��Ƿ���ĸ
inline BOOL IsLetter(TCHAR ch) { return (ch >= L'A' && ch <= L'Z') || (ch >= L'a' && ch <= L'z'); }
// �ж��ַ��Ƿ�����
inline BOOL IsNumber(TCHAR ch) { return (ch >= L'0' && ch <= L'9'); }

void CStringToConvertByte(const CString &strSrcText, BYTE *pDestText, int &nDestLen);
void ConvertByteToCString(const BYTE *pSrcText, CString &strDesText);
void BSTRToCString(BSTR pBFName, wchar_t *szNodeName, int &nLen);

BOOL GetSdVersion(DWORD& dwMajorVersion, DWORD& dwMinorVersion);
// ��ȡ��ĻDPI
double GetScreenDPI();
// ��ȡ��ģ̬�Ի�������Ͻ�λ��
CRect GetSceenCenterRect(int nOffsetX, int nOffsetY, const CRect &rcRect);
// �����ַ��������������ַ�����λ��
int FindInCStringArray(const CStringArray &arrString, const CString &strIn);
// ���ؼ��ְ��ַ������
BOOL CStringToCStringArray(const CString &str, CStringArray &arr, const CString &strKeyword = L" ");
// ���ؼ��ְ��ַ������
BOOL CStringArrayToCString(const CStringArray &arr, CString &str, const CString &strKeyword = L" ");
// �ж��ַ����Ƿ�Ϊ����
BOOL IsNumber(const CString &str);
// �ж��ַ����Ƿ�Ϊ����
BOOL IsPoistive(const CString &str);
// ��ȡ�ļ���,��������չ��
CString GetFileNameNoExtByPath(const CString& strFullPath);
// ��ȡת������ʱ�ļ���
CString GetFileTempPath();
// �õ���ֵ���ַ���
void GetValueString(double dValue, CString &strValue, BOOL bWithFlag = FALSE);
// ɾ���ַ���ǰ�����Ŀո�
void DelSpace(CString &strText);
// ��ȡ�ҵ��ĵ��ļ���
CString GetMyDocuments();

CString GetConfigFilePath();
CString GetParamXmlFilePath();
CString GetIOXmlFilePath(BOOL bInput);
// ʮ������תʮ����
int Hex2Dec(const ByteArray& arrByte);
void DecToHex(int nDec, ByteArray& arrByte);
// ʮ������ת������
void HexToBin(const ByteArray& arrHex, vector<int>&arrBin);
// ��ʮ�����ƵĲ���
void HexToCmt(const ByteArray& arrHex, ByteArray& arrCmt);
// �������ʮ�����ƵĲ���
int HexToCmt(BYTE byteHigh, BYTE byteLow);
// ��ȡѡ��ֵ
CString GetConfigOption(const CString& strSectionName, const CString &strOptionName);
// ��ȡѡ��ֵ
int GetConfigOptionINT(const CString& strSectionName, const CString &strOptionName);
// ����ѡ��ֵ
void SetConfigOption(const CString& strSectionName, const CString &strOptionName, const CString& strVal);
// ����ѡ��ֵ
void SetConfigOption(const CString& strSectionName, const CString &strOptionName, int nVal);
// ��ȡ������
BOOL ReadSewFile(const CString& strFile, ByteArray& arrByte);
// д�������
BOOL SaveSewFile(const CString& strFile, const ByteArray& arrByte);

//===================================================================================================

#endif