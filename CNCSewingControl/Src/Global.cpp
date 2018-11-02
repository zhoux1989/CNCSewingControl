//===================================================================================================
// Summary:
//		SmartDoctor内部使用的公共函数
// Date:
//		2010-12-08
// Author:
//		YangXuefeng(yxf4640084@sina.com.cn)
//===================================================================================================

#include "stdafx.h"
#include "Global.h"
#include <math.h>
#include "CNCSewingControl.h"

//===================================================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//===================================================================================================

BOOL GetFileExtName(const CString& strFilePathName, CString& strRetExt)
{	
	int nPos = strFilePathName.ReverseFind(_T('.'));
	if (nPos == -1)
		return FALSE;
	strRetExt = strFilePathName.Right(strFilePathName.GetLength() - nPos - 1);
	return TRUE;
}

// 检查是否存在指定的目录
BOOL IsFolderExist(LPCTSTR pszFolderName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	WIN32_FIND_DATA wfd;
	BOOL bRet = FALSE;
	HANDLE hFind = FindFirstFile(pszFolderName, &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		bRet = TRUE;   
	FindClose(hFind);
	return bRet;
}

BOOL IsFileExist(LPCTSTR pszFileName)
{
	CFile file;
	if (file.Open(pszFileName, CFile::modeRead|CFile::shareDenyNone))
	{
		file.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL IsFileOpened(LPCTSTR pszFileName)
{
	try
	{
		CFile cf;
		if (cf.Open(pszFileName, CFile::shareExclusive))
		{
			cf.Close();
			return FALSE;
		}
	}
	catch (...)
	{
		return FALSE;
	}

	return TRUE;			
}

void SafeDeleteFile(LPCTSTR pszFileName)
{
	try
	{
		CFile::Remove(pszFileName);		
	}
	catch (CException* e)
	{
		if (e != NULL)
			e->Delete();
	}
}

// 删除指定目录下的文件和文件夹
BOOL DeleteDirectoryFiles(LPCTSTR pszFolderName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CFileFind fileFind;
	CString strFileName = CString(pszFolderName) + L"\\*.*";
	BOOL IsFinded = (BOOL)fileFind.FindFile(strFileName.GetBuffer());
	CString strFullFileName;
	while(IsFinded)
	{
		IsFinded = (BOOL)fileFind.FindNextFile();
		if(!fileFind.IsDots())
		{
			strFullFileName = CString(pszFolderName) + L"\\" + fileFind.GetFileName();
			if(fileFind.IsDirectory())
				DeleteDirectoryFiles(strFullFileName.GetBuffer());
			else
				SafeDeleteFile(strFullFileName.GetBuffer());
		}
	}

	fileFind.Close();

	return TRUE;
}

BOOL GetTempFile(CString& strTempFileName, const CString &strExt /*= */)
{
	TCHAR szTempFileName[_MAX_PATH];
	TCHAR szTempPath[_MAX_PATH];

	GetTempPath(_MAX_PATH, szTempPath);
	if (GetTempFileName(szTempPath, _T("tmp"), 0, szTempFileName) == 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	strTempFileName = szTempFileName;

	if (strExt.CompareNoCase(__T("tmp")) != 0)
	{
		strTempFileName = strTempFileName.Left(strTempFileName.GetLength() - 3) + strExt;
		SafeDeleteFile(szTempFileName);
	}

	return TRUE;
}

// 得到文件的二进制数据
BOOL GetFileData(const CString &strFilePath, BYTE *&pFileData, DWORD &dwLength)
{
	CFile file;
	if (!file.Open(strFilePath, CFile::modeRead|CFile::shareDenyNone))
		return FALSE;

	dwLength = (DWORD)file.GetLength();
	if (dwLength == 0)
		return FALSE;

	pFileData = new BYTE[dwLength];
	memset(pFileData, 0, dwLength);
	file.Read(pFileData, dwLength);
	file.Close();

	return TRUE;
}

BOOL RecursiveCreateDirectory(LPCTSTR szPath)
{	
	if (_taccess(szPath, 0) == 0)
		return TRUE;

	CString strPath = szPath;
	strPath += _T('\\');
	CString strTempPath;
	int nPos = 0;
	while (1)
	{
		nPos++;
		nPos = strPath.Find(_T('\\'), nPos);
		if (nPos == -1)
			break;
		strTempPath = strPath.Left(nPos);
		if (_taccess(strTempPath, 0) != 0)
		{
			if (!CreateDirectory(strTempPath, NULL))
			{
				ASSERT(FALSE);
				return FALSE;
			}
		}		
	}
	return TRUE;
}

void GetMainModulePath(HMODULE hModule, CString& strPath)
{
	TCHAR szFileName[_MAX_PATH];
	GetModuleFileName(hModule, szFileName, _MAX_PATH);

	strPath = szFileName;
	int nLoc = strPath.ReverseFind(_T('\\'));
	if (nLoc != -1)
		strPath = strPath.Left(nLoc + 1);
}

// 获取路径的上一层路径
CString GetPrevPath(const CString &strPath)
{
	CString strPrevPath = strPath;
	int nFindBackslash = strPrevPath.ReverseFind(L'\\');
	if (nFindBackslash == strPrevPath.GetLength()-1)
	{
		strPrevPath = strPrevPath.Left(nFindBackslash);
		nFindBackslash = strPrevPath.ReverseFind(L'\\');
		if (nFindBackslash >= 0)
			strPrevPath = strPrevPath.Left(nFindBackslash);
	}
	else
		strPrevPath = strPrevPath.Left(nFindBackslash);
	strPrevPath += L"\\";
	return strPrevPath;
}

void CStringToConvertByte(const CString &strSrcText, BYTE *pDestText, int &nDestLen)
{
	if (strSrcText.GetLength() == 0)
	{
		nDestLen = 0;
		return;
	}
	int nLen = WideCharToMultiByte(936, 0, (LPCWSTR)strSrcText, -1, NULL, 0, NULL, NULL);
	if (nLen >= nDestLen)
		WideCharToMultiByte(936, 0, (LPCWSTR)strSrcText, -1, (char*)pDestText, nDestLen-1, NULL, NULL);
	else
	{
		nDestLen = nLen;
		WideCharToMultiByte(936, 0, (LPCWSTR)strSrcText, -1, (char*)pDestText, nLen, NULL, NULL);
	}
}

void ConvertByteToCString(const BYTE *pSrcText, CString &strDesText)
{
	if (pSrcText == NULL)
		return;

	strDesText.Empty();	
	int nLen = MultiByteToWideChar(936, 0, (char*)pSrcText, -1, NULL, 0);
	WCHAR *lpTempbuffer = (WCHAR *)(LPTSTR)strDesText.GetBuffer(nLen - 1);
	MultiByteToWideChar(936, 0, (char*)pSrcText, -1, lpTempbuffer, nLen);
	strDesText.ReleaseBuffer(nLen - 1);
}

// 获取屏幕DPI
double GetScreenDPI()
{
	static double dScreenDPI = 0;
	if ((int)dScreenDPI == 0)
	{
		HDC hScreenDc = ::GetDC(NULL);
		if (hScreenDc != NULL)
		{
			dScreenDPI = GetDeviceCaps(hScreenDc, LOGPIXELSX);
			// m_dYDPI = GetDeviceCaps(hScreenDc, LOGPIXELSY);
			::ReleaseDC(NULL, hScreenDc);
		}
	}
	return dScreenDPI;
}

// 获取非模态对话框的左上角位置
CRect GetSceenCenterRect(int nOffsetX, int nOffsetY, const CRect &rcRect)
{
	CRect rcResult;
	if (nOffsetX != -1 && nOffsetY != -1)
	{
		rcResult.left = nOffsetX;
		rcResult.top = nOffsetY;
		rcResult.right = nOffsetX + rcRect.right-rcRect.left;
		rcResult.bottom = nOffsetY + rcRect.bottom-rcRect.top;
	}
	else
	{
		int nWidth = GetSystemMetrics(SM_CXSCREEN); 
		int nHeight = GetSystemMetrics(SM_CYSCREEN);

		rcResult.left = nWidth/2 - (rcRect.right-rcRect.left)/2;
		rcResult.top = nHeight/2 - (rcRect.bottom-rcRect.top)/2;
		rcResult.right = nWidth/2 + (rcRect.right-rcRect.left)/2;
		rcResult.bottom = nHeight/2 + (rcRect.bottom-rcRect.top)/2;
	}
	return rcResult;
}

// 替换字符串中的子串
void SwapString(CString &str, const CString &strFrom, const CString &strTo)
{
	int nLoc = -1;
	while (TRUE)
	{
		nLoc = str.Find(strFrom);
		if (nLoc < 0)
			break;
		str.Delete(nLoc, strFrom.GetLength());
		str.Insert(nLoc, strTo);
	}
}

// 查找字符串数组中输入字符串的位置
int FindInCStringArray(const CStringArray &arrString, const CString &strIn)
{
	int nFind = -1;
	for (int i=0; i<arrString.GetSize(); i++)
	{
		if (strIn.CompareNoCase(arrString[i]) == 0)
		{
			nFind = i;
			break;
		}
	}
	return nFind;
}

// 按关键字把字符串拆分
BOOL CStringToCStringArray(const CString &str, CStringArray &arr, const CString &strKeyword/* = L" "*/)
{
	if (str.IsEmpty())
		return FALSE;

	CString strSub = str;
	arr.RemoveAll();
	int nFind = -1;
	CString strTemp;
	while (!strSub.IsEmpty())
	{
		nFind = strSub.Find(strKeyword);
		if (nFind >= 0)
		{
			strTemp = strSub.Left(nFind);
			if (!strTemp.IsEmpty())
				arr.Add(strTemp);
			strSub = strSub.Mid(nFind+1);
		}
		else
		{
			arr.Add(strSub);
			break;
		}
	}

	return TRUE;
}

// 按关键字把字符串组合
BOOL CStringArrayToCString(const CStringArray &arr, CString &str, const CString &strKeyword/* = L" "*/)
{
	str = L"";
	if (arr.IsEmpty())
		return FALSE;
	str += arr[0];
	for (int i=1; i<arr.GetSize(); i++)
	{
		str += strKeyword;
		str += arr[i];
	}

	return TRUE;
}

// 判断字符串是否为数字
BOOL IsNumber(const CString &str)
{
	BOOL bRet = TRUE;
	int nDotIndex = -1;
	for (int i=0; i<str.GetLength(); i++)
	{
		if (IsNumber(str[i]))
			continue;
		if (str[i] == L'.')
		{
			if (nDotIndex < 0)
			{
				nDotIndex = i;
				continue;
			}
			else
			{
				bRet = FALSE;
				break;
			}
		}
		if ((str[i] == L'+' || str[i] == L'-') && i == 0)
			continue;
		bRet = FALSE;
		break;
	}

	return bRet;
}

// 判断字符串是否为正数
BOOL IsPoistive(const CString &str)
{
	if (str.IsEmpty() || !IsNumber(str) || DEQUAL(_wtof(str), 0.) || _wtof(str) < 0)
		return FALSE;
	return TRUE;
}

// 获取文件名,不包含扩展名
CString GetFileNameNoExtByPath(const CString& strFullPath)
{
	if (strFullPath.IsEmpty())
		return _T("");

	TCHAR szDrive[256];
	TCHAR szDir[256];
	TCHAR szFileName[256];
	TCHAR szExt[256]; 
	_wsplitpath_s(strFullPath, szDrive, 256, szDir, 256, szFileName, 256, szExt, 256);
	return CString(szFileName);
}

// 获取转换的临时文件夹
CString GetFileTempPath()
{
	TCHAR szTempPath[_MAX_PATH];
	GetTempPath(_MAX_PATH, szTempPath);
	TCHAR szTempFileName[_MAX_PATH];
	GetTempFileName(szTempPath, _T("tmp"), 0, szTempFileName);
	SafeDeleteFile(szTempFileName);

	CString strTempPath = CString(szTempPath) + GetFileNameNoExtByPath(szTempFileName);
	CreateDirectory(strTempPath, NULL);
	strTempPath += L"\\";

	return strTempPath;
}

// 得到数值的字符串
void GetValueString(double dValue, CString &strValue, BOOL bWithFlag/* = FALSE*/)
{
	if (dValue > DFE_ZERO)
	{
		if (bWithFlag)
			strValue.Format(L"+%g", dValue);
		else
			strValue.Format(L"%g", dValue);
	}
	else if (dValue < -DFE_ZERO)
		strValue.Format(L"-%g", -dValue);
	else
		strValue = L"0";
}

// 删除字符串前后多余的空格
void DelSpace(CString &strText)
{
	// 删除前导空格
	while (!strText.IsEmpty())
	{
		if (strText[0] != L' ')
			break;
		strText.Delete(0);
	}

	// 删除后置空格
	while (!strText.IsEmpty())
	{
		if (strText[strText.GetLength()-1] != L' ')
			break;
		strText.Delete(strText.GetLength()-1);
	}
}

// 获取我的文档文件夹
CString GetMyDocuments()
{
	wchar_t szDefaultDir[MAX_PATH] = {0};
	wchar_t szDocument[MAX_PATH] = {0};
	memset(szDefaultDir, 0, _MAX_PATH);
	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_PERSONAL, &pidl);
	if (pidl != NULL && SHGetPathFromIDList(pidl, szDocument))
		GetShortPathName(szDocument, szDefaultDir, _MAX_PATH);

	return CString(szDocument);
}

CString GetConfigFilePath()
{
	CString strPath;
	GetMainModulePath(theApp.m_hInstance, strPath);
	strPath += L"config.ini";
	return strPath;
}

CString GetParamXmlFilePath()
{
	CString strPath;
	GetMainModulePath(theApp.m_hInstance, strPath);
	strPath += L"Param.xml";
	return strPath;
}

CString GetIOXmlFilePath(BOOL bInput)
{
	CString strPath;
	GetMainModulePath(theApp.m_hInstance, strPath);
	if (bInput)
		strPath += L"InputCheck.xml";
	else
		strPath += L"OutputCheck.xml";

	return strPath;
}

// 十六进制转十进制
int Hex2Dec(const ByteArray& arrByte)
{
	int nDec = 0;
	for (int i=0; i<(int)arrByte.size(); i++)
	{
		nDec += (int)pow(256.0, (double)i)*arrByte[i];
	}
	return nDec;
}

// 十进制转十六进制
void DecToHex(int nDec, ByteArray& arrByte)
{
	arrByte.clear();

	while (1)
	{
		if (nDec/256)
		{
			arrByte.push_back(BYTE(nDec%256));
			nDec = nDec/256;
		}
		else
		{
			arrByte.push_back(nDec);
			break;
		}
	}
}

// 十六进制转二进制
void HexToBin(const ByteArray& arrHex, vector<int>&arrBin)
{
	int nBinSize = (int)arrHex.size()*8;
	arrBin.resize(nBinSize);

	// 先转为十进制
	int nDec = Hex2Dec(arrHex);
	for (int i=0; i<nBinSize; i++)
	{
		if (nDec == 0)
		{
			arrBin[i] = 0;
			continue;
		}

		if (nDec/2)
		{
			arrBin[i] = nDec%2;
			nDec = nDec/2;
		}
		else
		{
			arrBin[i] = nDec;
			break;
		}
	}
}

// 求短整型十六进制的补码
void HexToCmt(const ByteArray& arrHex, ByteArray& arrCmt)
{
	WORD sum = 0;
	for (int i = 0; i<arrHex.size(); i++)
	{
		sum += arrHex[i];
		sum = sum & 0x0FFFF;
	}
	DecToHex(0x010000-sum, arrCmt);
}

// 求短整型十六进制的补码
int HexToCmt(BYTE byteHigh, BYTE byteLow)
{
	WORD dData = byteHigh*256+byteLow;
	short sData = dData;
	int nData = sData;
	return nData;
}

// 获取选项值
CString GetConfigOption(const CString& strSectionName, const CString &strOptionName)
{
	CString strConfigFile = GetConfigFilePath();
	if (!IsFileExist(strConfigFile))
		return L"";

	TCHAR lpValue[MAX_PATH] = {0};
	GetPrivateProfileString(strSectionName, strOptionName, L"", lpValue, MAX_PATH, strConfigFile);

	return lpValue;
}

// 获取选项值
int GetConfigOptionINT(const CString& strSectionName, const CString &strOptionName)
{
	CString strConfigFile = GetConfigFilePath();
	if (!IsFileExist(strConfigFile))
		return 0;

	TCHAR lpValue[MAX_PATH] = {0};
	GetPrivateProfileString(strSectionName, strOptionName, L"", lpValue, MAX_PATH, strConfigFile);

	return _wtoi(lpValue);
}

// 设置选项值
void SetConfigOption(const CString& strSectionName, const CString &strOptionName, const CString& strVal)
{
	CString strConfigFile = GetConfigFilePath();
	if (IsFileExist(strConfigFile))
		WritePrivateProfileString(strSectionName, strOptionName, strVal, strConfigFile);
}

// 设置选项值
void SetConfigOption(const CString& strSectionName, const CString &strOptionName, int nVal)
{
	CString strConfigFile = GetConfigFilePath();
	CString strVal;
	strVal.Format(L"%d", nVal);
	if (IsFileExist(strConfigFile))
		WritePrivateProfileString(strSectionName, strOptionName, strVal, strConfigFile);
}

// 读取二进制
BOOL ReadSewFile(const CString& strFile, ByteArray& arrByte)
{
	if (!IsFileExist(strFile))
		return FALSE;

	CFile file;
	if (!file.Open(strFile, CFile::modeRead | CFile::typeBinary))
	{
		return FALSE;
	}

	arrByte.clear();
	int iLen = (int)file.GetLength();
	BYTE* pData = new BYTE[iLen];
	file.SeekToBegin();
	file.Read(pData, iLen);
	file.Close();

	for (int i=0; i<iLen; i++)
	{
		arrByte.push_back(pData[i]);
	}

	return TRUE;
}

// 写入二进制
BOOL SaveSewFile(const CString& strFile, const ByteArray& arrByte)
{
	//if (IsFileExist(strFile))
	//	return FALSE;

	int nByteSize = (int)arrByte.size();
	if (nByteSize == 0)
		return FALSE;

	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		return FALSE;
	}

	BYTE* buff = new BYTE[nByteSize];
	for (int i=0; i<nByteSize; i++)
	{
		buff[i] = arrByte[i];
	}
	file.SeekToBegin();
	file.Write(buff, nByteSize);

	SAFE_DELETE_ARRAY(buff);
	
	file.Close();

	return TRUE;
}