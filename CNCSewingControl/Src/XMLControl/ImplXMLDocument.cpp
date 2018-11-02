//===================================================================================================
// Summary:
//		CBpXMLDocument类的实现
// Headers:
//		ImplXMLDocument.h
// Remarks:
//		封装了XPath操作、保存XML操作等
// Date:
//		2008-12-3
// Author:
//		Yang Xuefeng (yang_xuefeng@founder.com.cn)
//===================================================================================================

#include "stdafx.h"
#include "ImplXMLDocument.h"
#include "ImplXMLNode.h"

//===================================================================================================

// 创建XML文件的初始化数据
CString g_strXmlOrig = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<%s>\n</%s>";

//===================================================================================================

CBpXMLDocument::CBpXMLDocument()
{
	m_pDOMDocument = NULL;
}

CBpXMLDocument::~CBpXMLDocument()
{
}

bool CBpXMLDocument::OpenXMLForDocument(const wchar_t *szFilePath)
{
	bool bResult =false;
	try
	{
		m_pDOMDocument.CreateInstance(_T("msxml2.domdocument"));
		if (m_pDOMDocument == NULL)
			return false;

		_variant_t varOut((bool)TRUE);
		varOut = m_pDOMDocument->load(szFilePath); 
		if ((bool)varOut == TRUE)
		{
			m_strFileName = szFilePath;
			bResult = true;
		}
	}
	catch(...)
	{
		bResult = false;
	}
	return bResult;
}

bool CBpXMLDocument::CreateDocument(const wchar_t *szRootElementName)
{
	if (szRootElementName == NULL)
		return false;

	bool bResult =false;
	try
	{
		m_pDOMDocument.CreateInstance(_T("msxml2.domdocument"));
		if (m_pDOMDocument == NULL)
			return false;
		
		CString strXml;
		strXml.Format(g_strXmlOrig, szRootElementName, szRootElementName);
		_bstr_t bstrXmlOrig((LPCTSTR)strXml);
		_variant_t varOut((bool)TRUE);
		varOut = m_pDOMDocument->loadXML(bstrXmlOrig);
		if ((bool)varOut == TRUE)
			bResult = true;		
	}
	catch(...)
	{
		bResult = false;
	}
	return bResult;
}

// 将DOM Tree写入文件
bool CBpXMLDocument::SaveXML(const wchar_t *szFilePath) const
{
	if (szFilePath == NULL || m_pDOMDocument == NULL)
		return false;


	bool bResult = false;
	try
	{
		if (m_pDOMDocument->save(szFilePath) >= 0)
			bResult = true;;
	}
	catch(...)
	{
		bResult = false;
	}
	return bResult;
}

//  仅释放由CreateDocument创建所分配的资源
bool CBpXMLDocument::Release()
{
	if (m_pDOMDocument == NULL)
		return false;
	
	bool bResult = false;
	try
	{
		m_pDOMDocument.Detach();
		bResult = true;
	}
	catch (...)
	{
	}

	return bResult;
}

// 创建一个Element节点
IBpXMLNode* CBpXMLDocument::CreateElementNode(const wchar_t *szElementNodeName)
{
	if (m_pDOMDocument == NULL || szElementNodeName == NULL)
		return NULL;

	// 创建Element
	VARIANT vtTemp;
	vtTemp.vt = VT_I2;
	vtTemp.iVal = 1;	

	MSXML2::IXMLDOMNodePtr pDOMNodeCreated = m_pDOMDocument->createNode(vtTemp, _bstr_t(szElementNodeName), L"");
	if (pDOMNodeCreated == NULL)
		return NULL;

	if (pDOMNodeCreated == NULL)
	{
		pDOMNodeCreated->Release();
		return NULL;
	}

	MSXML2::IXMLDOMNodePtr pNodeAppend = NULL;
	try
	{
		pNodeAppend = m_pDOMDocument->documentElement->appendChild(pDOMNodeCreated);
	}
	catch (...)		
	{
		return NULL;	
	}

	if (pNodeAppend == NULL)
	{
		pDOMNodeCreated->Release();
		return NULL;
	}

	CBpXMLNode* pElemCreated = new CBpXMLNode;
	pElemCreated->m_pDOMNode = pNodeAppend;
	return pElemCreated;
}

// 创建一个Text节点
IBpXMLNode* CBpXMLDocument::CreateTextNode(const wchar_t *szText)
{
	if (m_pDOMDocument == NULL || szText == NULL)
		return NULL;

	// 创建Element
	VARIANT vtTemp;
	vtTemp.vt = VT_I2;
	vtTemp.iVal = 1;	

	MSXML2::IXMLDOMNodePtr pDOMNodeCreated = m_pDOMDocument->createTextNode(szText);
	if (pDOMNodeCreated == NULL)
		return NULL;

	if (pDOMNodeCreated == NULL)
	{
		pDOMNodeCreated->Release();
		return NULL;
	}

	MSXML2::IXMLDOMNodePtr pNodeAppend = NULL;
	try
	{
		pNodeAppend = m_pDOMDocument->documentElement->appendChild(pDOMNodeCreated);
	}
	catch (...)		
	{
		return NULL;	
	}

	if (pNodeAppend == NULL)
	{
		pDOMNodeCreated->Release();
		return NULL;
	}

	CBpXMLNode* pElemCreated = new CBpXMLNode;
	pElemCreated->m_pDOMNode = pNodeAppend;
	return pElemCreated;
}

// 获取根元素
IBpXMLNode *CBpXMLDocument::GetRootElement()
{
	if (m_pDOMDocument == NULL)
		return NULL;

	MSXML2::IXMLDOMElementPtr pDOMElePtr = m_pDOMDocument->documentElement;
	if (pDOMElePtr == NULL)
		return NULL;

	CBpXMLNode* pRootNode = new CBpXMLNode();
	pRootNode->m_pDOMNode = pDOMElePtr;
	return (IBpXMLNode*)pRootNode;
}