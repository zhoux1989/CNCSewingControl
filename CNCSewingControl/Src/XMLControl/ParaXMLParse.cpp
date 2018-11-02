//===================================================================================================
// Summary:
//		参数XML解析
// Date:
//		2018-08-16
// Author:
//
//===================================================================================================

#include "stdafx.h"
#include "BpXMLNode.h"
#include "ParaXMLParse.h"
#include "CNCSewingControl.h"

//===================================================================================================

CParaXMLParse::CParaXMLParse()
{
	m_pXMLDocument = NULL;
}

CParaXMLParse::~CParaXMLParse()
{
}

BOOL CParaXMLParse::Init()
{
	// 初始化xml库
	InitXMLCodec();
	m_pXMLDocument = CreateXMLDocument();
	return TRUE;
}

BOOL CParaXMLParse::Uninit()
{
	if (m_pXMLDocument != NULL)
	{
		m_pXMLDocument->Release();
		DestroyXMLDocument(m_pXMLDocument);
		m_pXMLDocument = NULL;
	}

	// 释放XML库
	UnInitXMLCodec();
	return TRUE;
}

BOOL CParaXMLParse::ReadParamList(MapParaInf &mapParaList)
{
	BOOL bResult = TRUE;

	// 初始化
	Init();

	// 打开xml文件
	if (!m_pXMLDocument->OpenXMLForDocument(GetParamXmlFilePath()))
		bResult = FALSE;

	// 读取数据
	if (bResult)
	{
		mapParaList.clear();
		ParaInfo para;
		CString strValue;
		IBpXMLNode* pRootNode = m_pXMLDocument->GetRootElement();
		pRootNode->GetName(strValue);
		if (strValue.Compare(L"ParamList") == 0)
		{
			IBpXMLNode *pChildNode = NULL;
			int nNodeCount = pRootNode->GetElementCount();
			for (int i = 1; i <= nNodeCount; i++)
			{
				IBpXMLNode *pParaNode = pRootNode->GetElementChildByIndex(i);
				pParaNode->GetAttributeByName(L"id", strValue);
				para.nID = _wtoi(strValue);			// 参数编号
				pParaNode->GetAttributeByName(L"type", strValue);
				para.nType = _wtoi(strValue);		// 参数类型
				pParaNode->GetAttributeByName(L"default", strValue);
				para.nDefaultValue = _wtoi(strValue);	// 参数默认值
				para.nCurrentValue = para.nDefaultValue;
				if (para.nType == 2)
				{
					pParaNode->GetAttributeByName(L"min", strValue);
					para.nMinValue = _wtoi(strValue);	// 参数类型
					pParaNode->GetAttributeByName(L"max", strValue);
					para.nMaxValue = _wtoi(strValue);	// 参数类型
					//pParaNode->GetAttributeByName(L"step", strValue);
					//para.nStep = _wtoi(strValue);	// 参数类型
				}

				pChildNode = pParaNode->GetElementChildByIndex(1);
				pChildNode->GetText(para.strParaName);
				DestroyXMLNode(pChildNode);
				pChildNode = pParaNode->GetElementChildByIndex(2);
				pChildNode->GetText(para.strDescription);
				DestroyXMLNode(pChildNode);
				mapParaList.insert(make_pair(para.nID, para));

				DestroyXMLNode(pParaNode);
			}
		}
		else
		{
			AfxMessageBox(L"XML文件不匹配!");
			bResult = FALSE;
		}
		DestroyXMLNode(pRootNode);
	}

	// 释放资源
	Uninit();

	return bResult;
}

BOOL CParaXMLParse::ReadInputCheckList(IOCheckDataArray &arrInputCheck)
{
	BOOL bResult = TRUE;

	// 初始化
	Init();

	// 打开xml文件
	if (!m_pXMLDocument->OpenXMLForDocument(GetIOXmlFilePath(TRUE)))
		bResult = FALSE;

	// 读取数据
	if (bResult)
	{
		arrInputCheck.clear();
		IOCheckData iodata;
		CString strValue;
		IBpXMLNode* pRootNode = m_pXMLDocument->GetRootElement();
		pRootNode->GetName(strValue);
		if (strValue.Compare(L"InputCheck") == 0)
		{
			IBpXMLNode *pChildNode = NULL;
			int nNodeCount = pRootNode->GetElementCount();
			for (int i = 1; i <= nNodeCount; i++)
			{
				IBpXMLNode *pParaNode = pRootNode->GetElementChildByIndex(i);
				pParaNode->GetAttributeByName(L"id", strValue);
				iodata.nID = _wtoi(strValue);			// 参数编号
				pParaNode->GetAttributeByName(L"type", strValue);
				iodata.nType = _wtoi(strValue);		// 参数类型
				pParaNode->GetAttributeByName(L"mode", strValue);
				iodata.nMode = _wtoi(strValue);	// 模式
				pParaNode->GetAttributeByName(L"mode", strValue);
				iodata.nBit = _wtoi(strValue);	// 端口号

				pChildNode = pParaNode->GetElementChildByIndex(1);
				pChildNode->GetText(iodata.strDesc);
				DestroyXMLNode(pChildNode);
				arrInputCheck.push_back(iodata);
				DestroyXMLNode(pParaNode);
			}
		}
		else
		{
			AfxMessageBox(L"XML文件不匹配!");
			bResult = FALSE;
		}
		DestroyXMLNode(pRootNode);
	}

	// 释放资源
	Uninit();

	return bResult;
}

BOOL CParaXMLParse::ReadOutputCheckList(IOCheckDataArray &arrOutputCheck)
{
	BOOL bResult = TRUE;

	// 初始化
	Init();

	// 打开xml文件
	if (!m_pXMLDocument->OpenXMLForDocument(GetIOXmlFilePath(FALSE)))
		bResult = FALSE;

	// 读取数据
	if (bResult)
	{
		arrOutputCheck.clear();
		IOCheckData iodata;
		CString strValue;
		IBpXMLNode* pRootNode = m_pXMLDocument->GetRootElement();
		pRootNode->GetName(strValue);
		if (strValue.Compare(L"OutputCheck") == 0)
		{
			IBpXMLNode *pChildNode = NULL;
			int nNodeCount = pRootNode->GetElementCount();
			for (int i = 1; i <= nNodeCount; i++)
			{
				IBpXMLNode *pParaNode = pRootNode->GetElementChildByIndex(i);
				pParaNode->GetAttributeByName(L"id", strValue);
				iodata.nID = _wtoi(strValue);			// 参数编号
				pParaNode->GetAttributeByName(L"type", strValue);
				iodata.nType = _wtoi(strValue);		// 参数类型
				pParaNode->GetAttributeByName(L"mode", strValue);
				iodata.nMode = _wtoi(strValue);	// 模式
				pParaNode->GetAttributeByName(L"bit", strValue);
				iodata.nBit = _wtoi(strValue);	// 端口号

				pChildNode = pParaNode->GetElementChildByIndex(1);
				pChildNode->GetText(iodata.strDesc);
				DestroyXMLNode(pChildNode);
				arrOutputCheck.push_back(iodata);
				DestroyXMLNode(pParaNode);
			}
		}
		else
		{
			AfxMessageBox(L"XML文件不匹配!");
			bResult = FALSE;
		}
		DestroyXMLNode(pRootNode);
	}

	// 释放资源
	Uninit();

	return bResult;
}