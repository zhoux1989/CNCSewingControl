//===================================================================================================
// Summary:
//		����XML����
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
	// ��ʼ��xml��
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

	// �ͷ�XML��
	UnInitXMLCodec();
	return TRUE;
}

BOOL CParaXMLParse::ReadParamList(MapParaInf &mapParaList)
{
	BOOL bResult = TRUE;

	// ��ʼ��
	Init();

	// ��xml�ļ�
	if (!m_pXMLDocument->OpenXMLForDocument(GetParamXmlFilePath()))
		bResult = FALSE;

	// ��ȡ����
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
				para.nID = _wtoi(strValue);			// �������
				pParaNode->GetAttributeByName(L"type", strValue);
				para.nType = _wtoi(strValue);		// ��������
				pParaNode->GetAttributeByName(L"default", strValue);
				para.nDefaultValue = _wtoi(strValue);	// ����Ĭ��ֵ
				para.nCurrentValue = para.nDefaultValue;
				if (para.nType == 2)
				{
					pParaNode->GetAttributeByName(L"min", strValue);
					para.nMinValue = _wtoi(strValue);	// ��������
					pParaNode->GetAttributeByName(L"max", strValue);
					para.nMaxValue = _wtoi(strValue);	// ��������
					//pParaNode->GetAttributeByName(L"step", strValue);
					//para.nStep = _wtoi(strValue);	// ��������
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
			AfxMessageBox(L"XML�ļ���ƥ��!");
			bResult = FALSE;
		}
		DestroyXMLNode(pRootNode);
	}

	// �ͷ���Դ
	Uninit();

	return bResult;
}

BOOL CParaXMLParse::ReadInputCheckList(IOCheckDataArray &arrInputCheck)
{
	BOOL bResult = TRUE;

	// ��ʼ��
	Init();

	// ��xml�ļ�
	if (!m_pXMLDocument->OpenXMLForDocument(GetIOXmlFilePath(TRUE)))
		bResult = FALSE;

	// ��ȡ����
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
				iodata.nID = _wtoi(strValue);			// �������
				pParaNode->GetAttributeByName(L"type", strValue);
				iodata.nType = _wtoi(strValue);		// ��������
				pParaNode->GetAttributeByName(L"mode", strValue);
				iodata.nMode = _wtoi(strValue);	// ģʽ
				pParaNode->GetAttributeByName(L"mode", strValue);
				iodata.nBit = _wtoi(strValue);	// �˿ں�

				pChildNode = pParaNode->GetElementChildByIndex(1);
				pChildNode->GetText(iodata.strDesc);
				DestroyXMLNode(pChildNode);
				arrInputCheck.push_back(iodata);
				DestroyXMLNode(pParaNode);
			}
		}
		else
		{
			AfxMessageBox(L"XML�ļ���ƥ��!");
			bResult = FALSE;
		}
		DestroyXMLNode(pRootNode);
	}

	// �ͷ���Դ
	Uninit();

	return bResult;
}

BOOL CParaXMLParse::ReadOutputCheckList(IOCheckDataArray &arrOutputCheck)
{
	BOOL bResult = TRUE;

	// ��ʼ��
	Init();

	// ��xml�ļ�
	if (!m_pXMLDocument->OpenXMLForDocument(GetIOXmlFilePath(FALSE)))
		bResult = FALSE;

	// ��ȡ����
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
				iodata.nID = _wtoi(strValue);			// �������
				pParaNode->GetAttributeByName(L"type", strValue);
				iodata.nType = _wtoi(strValue);		// ��������
				pParaNode->GetAttributeByName(L"mode", strValue);
				iodata.nMode = _wtoi(strValue);	// ģʽ
				pParaNode->GetAttributeByName(L"bit", strValue);
				iodata.nBit = _wtoi(strValue);	// �˿ں�

				pChildNode = pParaNode->GetElementChildByIndex(1);
				pChildNode->GetText(iodata.strDesc);
				DestroyXMLNode(pChildNode);
				arrOutputCheck.push_back(iodata);
				DestroyXMLNode(pParaNode);
			}
		}
		else
		{
			AfxMessageBox(L"XML�ļ���ƥ��!");
			bResult = FALSE;
		}
		DestroyXMLNode(pRootNode);
	}

	// �ͷ���Դ
	Uninit();

	return bResult;
}