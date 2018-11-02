//===================================================================================================
// Summary:
//		SmartDoctor内部使用的公共函数
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

// 指令类别
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

// 功能模块
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

// 预览
typedef enum __tagPreviewState
{
	PS_CLOSE,		// 关闭预览
	PS_PREPARE,		// 准备中
	PS_RECEIVE,		// 接收中
	PS_VALID,		// 有效（可以显示预览）
	PS_INVALID,		// 无效（预览失败）
	PS_NOCURRENT,	// 未设置当前花样
} PreviewState;

typedef vector<BYTE> ByteArray;

typedef BYTE FILEDATABYTE[126];

// 参数信息
typedef struct __tagParaInfo
{
	int nID;						// 参数编号
	CString strParaName;			// 参数名
	int nType;						// 参数值类型(0:OFF/ON)
	int nCurrentValue;				// 当前值
	int nDefaultValue;				// 初始值
	int nMinValue;					// 最小值
	int nMaxValue;					// 最大值
	int nStep;						// 步进值
	CString strDescription;			// 参数说明

	__tagParaInfo()
	{
		nID = -1;
		strParaName = L"未知参数";
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

// 花样文件信息
typedef struct __tagSewFileData
{
	int nID;				// 文件ID
	ByteArray arrByte;		// 数据
	ByteArray arrBlockSize;	// 块长度
	CString strPath;		// 文件路径（仅对本地文件有效）
	
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

// 缝制相关信息
typedef struct __tagSewingInfo
{
	int nBottomLineCounter;	// 底线计数器
	int nWorkpiectCounter;	// 工件计数器
	int nCurrentSewFile;	// 当前花样号
	int nSewMode;			// 缝制模式
	int nSpindleSpeed;		// 主轴速度
	int nMediumLegHeight;	// 中压脚高度
	int nLineTensionA;		// 线张力1
	int nLineTensionB;		// 线张力2

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

// IO检测数据
typedef struct __tagIOCheckData
{
	int nID;						// 检测项编号
	CString strDesc;				// 检测项描述
	int nType;						// 检测项类型（1或2）
	int nMode;						// 查询模式（0为待定、1为专用输入口、2为通用输出口、3为输出口、11为通用输出口、12为输出电磁铁端口、10表示电机输出测试）
	int nBit;						// 端口号（仅当nType = 1有效）
	int nState;						// 当前状态
	int nStep;						// 步进值（仅当nMode = 10有效）

	__tagIOCheckData()
	{
		nID = -1;
		strDesc = L"待定";
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

// 指令记录信息
typedef struct __tagCodeRecode
{
	BYTE m_byteQACode;	// 请求码或应答码
	BYTE m_byteCmdCode;	// 指令码
	BOOL bAnswered;		// 执行状态

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

// 花样针步数据结构
typedef struct __tagNeedleStepData
{
	long lPulseOffsetX;		// X轴脉冲增量
	long lPulseOffsetY;		// Y轴脉冲增量
	BOOL bEmptyMoveFlag;	// 空移或下针--D15 0--下针 1--空移

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

// 花样轨迹数据结构
typedef struct __tagNeedleTrailData
{
	CString strDesc;
	int nStartPulseX;	// 起点X轴脉冲
	int nStartPulseY;	// 起点Y轴脉冲
	int nPlusMaxX;		// X轴正向最大值
	int nPlusMaxY;		// Y轴正向最大值
	int nMinusMaxX;	// X轴负向最大值
	int nMinusMaxY;	// Y轴负向最大值
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

// 预览控制数据结构
typedef struct __tagPreviewControlData
{
	CPoint ptOrigin;	// 客户区预览坐标原点
	CRect rectPreview;	// 客户区预览范围
	CString strText;	// 客户区要显示的文字
	BOOL bShowEmptyMoveTrail;	// 显示空移轨迹

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

// 获取文件打开类型，返回值参考READERFILETYPE枚举值
int GetFileOpenType(const CString& strFileName);
// 获取文件扩展名
BOOL GetFileExtName(const CString& strFilePathName, CString& strRetExt);
// 检查是否存在指定的目录
BOOL IsFolderExist(LPCTSTR pszFolderName);
// 判断文件是否存在。
BOOL IsFileExist(LPCTSTR pszFileName);
// 判断文件是否被打开。
BOOL IsFileOpened(LPCTSTR pszFileName);
// 安全删除文件。
void SafeDeleteFile(LPCTSTR pszFileName);
// 删除指定目录下的文件和文件夹
BOOL DeleteDirectoryFiles(LPCTSTR pszFolderName);
// 获取一个临时文件名。
BOOL GetTempFile(CString& strTempFileName, const CString &strExt = _T("tmp"));
// 得到文件的二进制数据
BOOL GetFileData(const CString &strFilePath, BYTE *&pFileData, DWORD &nLength);
// 创建多层嵌套的目录。
BOOL RecursiveCreateDirectory(LPCTSTR szPath);
// 获取主程序所在路径。
void GetMainModulePath(HMODULE hModule, CString& strPath);
// 获取路径的上一层路径
CString GetPrevPath(const CString &strPath);
// 获取主程序的版本信息。
BOOL GetApabiVersion(DWORD& dwMajorVersion, DWORD& dwMinorVersion);
// 判断字符是否字母
inline BOOL IsLetter(TCHAR ch) { return (ch >= L'A' && ch <= L'Z') || (ch >= L'a' && ch <= L'z'); }
// 判断字符是否数字
inline BOOL IsNumber(TCHAR ch) { return (ch >= L'0' && ch <= L'9'); }

void CStringToConvertByte(const CString &strSrcText, BYTE *pDestText, int &nDestLen);
void ConvertByteToCString(const BYTE *pSrcText, CString &strDesText);
void BSTRToCString(BSTR pBFName, wchar_t *szNodeName, int &nLen);

BOOL GetSdVersion(DWORD& dwMajorVersion, DWORD& dwMinorVersion);
// 获取屏幕DPI
double GetScreenDPI();
// 获取非模态对话框的左上角位置
CRect GetSceenCenterRect(int nOffsetX, int nOffsetY, const CRect &rcRect);
// 查找字符串数组中输入字符串的位置
int FindInCStringArray(const CStringArray &arrString, const CString &strIn);
// 按关键字把字符串拆分
BOOL CStringToCStringArray(const CString &str, CStringArray &arr, const CString &strKeyword = L" ");
// 按关键字把字符串组合
BOOL CStringArrayToCString(const CStringArray &arr, CString &str, const CString &strKeyword = L" ");
// 判断字符串是否为数字
BOOL IsNumber(const CString &str);
// 判断字符串是否为正数
BOOL IsPoistive(const CString &str);
// 获取文件名,不包含扩展名
CString GetFileNameNoExtByPath(const CString& strFullPath);
// 获取转换的临时文件夹
CString GetFileTempPath();
// 得到数值的字符串
void GetValueString(double dValue, CString &strValue, BOOL bWithFlag = FALSE);
// 删除字符串前后多余的空格
void DelSpace(CString &strText);
// 获取我的文档文件夹
CString GetMyDocuments();

CString GetConfigFilePath();
CString GetParamXmlFilePath();
CString GetIOXmlFilePath(BOOL bInput);
// 十六进制转十进制
int Hex2Dec(const ByteArray& arrByte);
void DecToHex(int nDec, ByteArray& arrByte);
// 十六进制转二进制
void HexToBin(const ByteArray& arrHex, vector<int>&arrBin);
// 求十六进制的补码
void HexToCmt(const ByteArray& arrHex, ByteArray& arrCmt);
// 求短整型十六进制的补码
int HexToCmt(BYTE byteHigh, BYTE byteLow);
// 获取选项值
CString GetConfigOption(const CString& strSectionName, const CString &strOptionName);
// 获取选项值
int GetConfigOptionINT(const CString& strSectionName, const CString &strOptionName);
// 设置选项值
void SetConfigOption(const CString& strSectionName, const CString &strOptionName, const CString& strVal);
// 设置选项值
void SetConfigOption(const CString& strSectionName, const CString &strOptionName, int nVal);
// 读取二进制
BOOL ReadSewFile(const CString& strFile, ByteArray& arrByte);
// 写入二进制
BOOL SaveSewFile(const CString& strFile, const ByteArray& arrByte);

//===================================================================================================

#endif