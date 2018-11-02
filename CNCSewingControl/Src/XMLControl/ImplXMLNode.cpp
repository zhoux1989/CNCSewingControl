//===================================================================================================
// Summary:
//		XMLNode的实现文件
// Headers:
//		ImplXMLNode.h
// Remarks:
//		Null
// Date:
//		2008-12-3
// Author:
//		Yang Xuefeng (yang_xuefeng@founder.com.cn)
//===================================================================================================

#include "stdafx.h"
#include "ImplXMLNode.h"

//===================================================================================================

CBpXMLNode::CBpXMLNode()
{
	m_pDOMNode = NULL;
	m_nCurrentParseIndex = 0;
}

CBpXMLNode::CBpXMLNode(CBpXMLNode* pNode)
{
	if (pNode != NULL)
	{
		m_pDOMNode = pNode->m_pDOMNode;
		m_nCurrentParseIndex = pNode->m_nCurrentParseIndex;
	}
	else
	{
		m_pDOMNode = NULL;	
		m_nCurrentParseIndex = 0;
	}
}

CBpXMLNode::~CBpXMLNode()
{
}

// 获取当前结点的父结点
IBpXMLNode *CBpXMLNode::GetParent() const
{
	if (m_pDOMNode == NULL)
		return NULL;

	MSXML2::IXMLDOMNodePtr pDOMNode = m_pDOMNode->GetparentNode();
	if (pDOMNode != NULL)
	{	
		CBpXMLNode *pNodeCreated  = new CBpXMLNode();	
		pNodeCreated->m_pDOMNode = pDOMNode;
		return (IBpXMLNode*)pNodeCreated;
	}
	return NULL;
}

// 获取当前节点的名字
int CBpXMLNode::GetName(CString &strNodeName) const
{
	if (m_pDOMNode == NULL)
		return XML_ERROR;
	try
	{
		BSTR bstrText = NULL;
		m_pDOMNode->get_nodeName(&bstrText);
		strNodeName = CString(bstrText);
		::SysFreeString(bstrText);		
	}
	catch (...)
	{
		return XML_ERROR;
	}
	return XML_OK;
}

// 获取当前节点及其子节点的内容
int CBpXMLNode::GetText(CString &strNodeText) const
{
	if (m_pDOMNode == NULL)
		return XML_ERROR;
	try
	{
		BSTR bstrName = NULL;
		m_pDOMNode->get_text(&bstrName);
		strNodeText = CString(bstrName);
		::SysFreeString(bstrName);
	}
	catch (...)
	{
		return XML_ERROR;
	}
	return XML_OK;
}

// 获取当前节点的所有Element子节点个数
int CBpXMLNode::GetElementCount() const
{
	if (m_pDOMNode == NULL)
		return 0;

	long nResult = 0;
	MSXML2::IXMLDOMNodeListPtr pNodeList = m_pDOMNode->GetchildNodes();
	if (pNodeList != NULL)
	{
		MSXML2::IXMLDOMNodePtr pNode = NULL;
		long nListLen = 0;
		pNodeList->get_length(&nListLen);
		for (long i = 0; i < nListLen; i++) 
		{ 
			pNodeList->get_item(i, &pNode);
			if (pNode != NULL && pNode->GetnodeType() == MSXML2::NODE_ELEMENT)
				nResult++;
		} 	
	}

	return (int)nResult;
}

// 获取当前节点的指定位置的Element子节点
IBpXMLNode *CBpXMLNode::GetElementChildByIndex(int nIndex) const
{
	if (m_pDOMNode == NULL)
		return NULL;

	if (m_pDOMNode->GetnodeType() != MSXML2::NODE_ELEMENT)
		return NULL;

	MSXML2::IXMLDOMNodeListPtr pNodeList = m_pDOMNode->GetchildNodes();
	if (pNodeList == NULL)
		return NULL;

	long nListLen = 0;
	pNodeList->get_length(&nListLen);
	if (nIndex <= 0 || nIndex > nListLen)
		return NULL;

	MSXML2::IXMLDOMNodePtr pNode = NULL;
	int nRealIndex = 0;
	int nCount = 0;
	for (int i = 0; i < nListLen; i++) 
	{ 
		pNodeList->get_item(i, &pNode);
		if (pNode->GetnodeType() == MSXML2::NODE_ELEMENT) 
		{
			nCount++;
			if (nCount == nIndex)
			{
				nRealIndex = i;	
				break;
			}
		}
	} 	

	if (nRealIndex < 0 || nRealIndex >= nListLen)
		return NULL;

	pNodeList->get_item(nRealIndex, &pNode);
	if (pNode->GetnodeType() == MSXML2::NODE_ELEMENT) 
	{ 
		CBpXMLNode *pNodeCreated  = new CBpXMLNode();	
		pNodeCreated->m_pDOMNode = pNode;
		return (IBpXMLNode*)pNodeCreated;
	} 

	return NULL;
}

// 获取当前节点的第一个Element子节点
IBpXMLNode *CBpXMLNode::GetFirstElementChild() const
{
	if (m_pDOMNode == NULL)
		return NULL;

	if (m_pDOMNode->GetnodeType() == MSXML2::NODE_ELEMENT) 
	{ 
		MSXML2::IXMLDOMNodeListPtr pNodeList = m_pDOMNode->GetchildNodes(); 
		if (pNodeList == NULL)
			return NULL;

		long nListLen = 0;
		pNodeList->get_length(&nListLen); 
		MSXML2::IXMLDOMNodePtr pNode = NULL;
		for (long i = 0; i < nListLen; i++) 
		{ 
			pNodeList->get_item(i, &pNode); 
			if (pNode->GetnodeType() == MSXML2::NODE_ELEMENT) 
			{ 
				CBpXMLNode *pNodeCreated  = new CBpXMLNode();	
				pNodeCreated->m_pDOMNode = pNode;
				return (IBpXMLNode*)pNodeCreated;
			} 
		} 
	}
	return NULL;
}

// 获取当前节点的最后一个Element子节点
IBpXMLNode* CBpXMLNode::GetLastElementChild() const
{
	if (m_pDOMNode == NULL)
		return NULL;

	if (m_pDOMNode->GetnodeType() == MSXML2::NODE_ELEMENT) 
	{ 
		MSXML2::IXMLDOMNodeListPtr pNodeList = m_pDOMNode->GetchildNodes(); 
		if (pNodeList == NULL)
			return NULL;
		long nListLen = 0;
		pNodeList->get_length(&nListLen);;
		MSXML2::IXMLDOMNodePtr pNode = NULL;
		for (long i = 0; i < nListLen; i++) 
		{ 
			pNodeList->get_item(nListLen - 1 - i, &pNode); 
			if (pNode->GetnodeType() == MSXML2::NODE_ELEMENT) 
			{ 
				CBpXMLNode *pNodeCreated  = new CBpXMLNode();	
				pNodeCreated->m_pDOMNode = pNode;
				return (IBpXMLNode*)pNodeCreated;
			} 
		} 
	}
	return NULL;
}

// 获取当前结点，指定名称的第index个Element子结点
IBpXMLNode *CBpXMLNode::GetElementChildByName(const wchar_t *szChildNodeName, int nIndex) const
{
	if (m_pDOMNode == NULL || nIndex <= 0)
		return NULL;

	int nCount = nIndex;
	if (m_pDOMNode->GetnodeType() == MSXML2::NODE_ELEMENT) 
	{ 
		MSXML2::IXMLDOMNodeListPtr pNodeList = m_pDOMNode->GetchildNodes(); 
		if (pNodeList == NULL)
			return NULL;
		long nListLen = 0;
		pNodeList->get_length(&nListLen);; 
		int i = 0;
		MSXML2::IXMLDOMNodePtr pNode = NULL;
		for (;;)
		{ 
			pNodeList->get_item(i, &pNode); 
			if (pNode == NULL)
				break;

			if (pNode->GetnodeType() == MSXML2::NODE_ELEMENT) 
			{ 
				BSTR bstrText = NULL;
				m_pDOMNode->get_nodeName(&bstrText);
				CString strTempNodeName(bstrText);
				if (strTempNodeName.CompareNoCase(szChildNodeName))
				{
					nCount--;
					if(nCount == 0)
					{
						CBpXMLNode *pNodeCreated  = new CBpXMLNode();	
						pNodeCreated->m_pDOMNode = pNode;
						return (IBpXMLNode*)pNodeCreated;
					}
				}
			} 
			i++;
			if(i == nListLen)
				break;
		} 
	}
	return NULL;
}

// 获取当前结点的前一个Element兄弟结点
IBpXMLNode *CBpXMLNode::GetPreviousElementSibling() const
{
	if(m_pDOMNode == NULL)
		return NULL;

	MSXML2::IXMLDOMNode *pTempNode = m_pDOMNode;
	while (pTempNode != NULL)
	{
		pTempNode->get_previousSibling(&pTempNode);
		if (pTempNode == NULL)
			return NULL;

		if (pTempNode->GetnodeType() == MSXML2::NODE_ELEMENT)
		{
			CBpXMLNode *pNodeCreated  = new CBpXMLNode();	
			pNodeCreated->m_pDOMNode = pTempNode;
			return (IBpXMLNode*)pNodeCreated;
		}
	}
	return NULL;
}

// 获取当前结点的下一个Element兄弟结点
IBpXMLNode *CBpXMLNode::GetNextElementSibling() const
{
	if(m_pDOMNode == NULL)
		return NULL;

	MSXML2::IXMLDOMNode *pTempNode = m_pDOMNode;
	while (pTempNode != NULL)
	{
		pTempNode->get_nextSibling(&pTempNode);
		if (pTempNode == NULL)
			return NULL;

		if (pTempNode->GetnodeType() == MSXML2::NODE_ELEMENT)
		{
			CBpXMLNode *pNodeCreated  = new CBpXMLNode();	
			pNodeCreated->m_pDOMNode = pTempNode;
			return (IBpXMLNode*)pNodeCreated;
		}
	}
	return NULL;
}

// 获取当前结点的属性的个数
int CBpXMLNode::GetAttributeCount() const
{
	if (m_pDOMNode == NULL)
		return -1;

	MSXML2::IXMLDOMNamedNodeMap *pAttributeMap = NULL;
	m_pDOMNode->get_attributes(&pAttributeMap);
	if (pAttributeMap == NULL)
		return -1;

	return pAttributeMap->length;
}

// 获取当前结点的指定属性的值			
int CBpXMLNode::GetAttributeByName(const wchar_t *szAttributeName, CString &strAttributeValue) const
{
	if (m_pDOMNode == NULL)
		return XML_ERROR;

	MSXML2::IXMLDOMNamedNodeMap *pAttributeMap = NULL;
	m_pDOMNode->get_attributes(&pAttributeMap);
	if (pAttributeMap == NULL)
		return XML_ERROR;

	MSXML2::IXMLDOMNodePtr pAttNode = pAttributeMap->getNamedItem(szAttributeName);
	if (pAttNode == NULL)
		return XML_ERROR;

	BSTR bstrText = NULL;
	pAttNode->get_text(&bstrText);
	strAttributeValue = bstrText;
	::SysFreeString(bstrText);	

	return XML_OK;
}

// 获取当前结点的指定序号的属性的名称
int CBpXMLNode::GetAttributeByIndex(int nIndex, CString &strAttributeValue) const
{
	if (m_pDOMNode == NULL || nIndex <= 0)
		return XML_ERROR;

	MSXML2::IXMLDOMNamedNodeMap *pAttributeMap = NULL;
	m_pDOMNode->get_attributes(&pAttributeMap);
	if (pAttributeMap == NULL)
		return XML_ERROR;

	MSXML2::IXMLDOMNodePtr pAttNode = pAttributeMap->item[nIndex];
	if (pAttNode == NULL)
		return XML_ERROR;

	BSTR bstrText = NULL;
	pAttNode->get_text(&bstrText);
	strAttributeValue = bstrText;
	::SysFreeString(bstrText);	

	return XML_OK;
}

// 判断当前节点是否存在子结点
bool CBpXMLNode::HasChildNodes() const
{
	if (m_pDOMNode == NULL)
		return false;

	VARIANT_BOOL bRes = m_pDOMNode->hasChildNodes();
	if (bRes == 0)
		return false;
	else
		return true;
}

// 判断当前节点是否存在Element子结点
bool CBpXMLNode::HasElementChildNodes() const
{
	if(m_pDOMNode == NULL)
		return false;
	
	if (m_pDOMNode->GetnodeType() == MSXML2::NODE_ELEMENT) 
	{ 
		MSXML2::IXMLDOMNodeListPtr pNodeList = m_pDOMNode->GetchildNodes(); 
		if (pNodeList == NULL)
			return false;

		long nListLen = 0;
		pNodeList->get_length(&nListLen);
		MSXML2::IXMLDOMNodePtr pNode = NULL;
		for (int i = 0; i < nListLen; i++)
		{
			pNodeList->get_item(i, &pNode);
			if (pNode->GetnodeType() == MSXML2::NODE_ELEMENT)
				return true;			
		}
	}

	return false;
}

// 判断当前节点是否为空节点，即其m_pDOMNode是否为空
bool CBpXMLNode::IsEmpty() const
{
	if (m_pDOMNode == NULL)
		return true;
	else
		return false;
}

// 根据给定的名字，创建Element节点，并将其附加为当前节点的子节点
IBpXMLNode* CBpXMLNode::AppendNewElementChild(const wchar_t *szElementName)
{
	if (m_pDOMNode == NULL)	
		return NULL;

	MSXML2::IXMLDOMDocument	*pDOMDocPtr = NULL;
	m_pDOMNode->get_ownerDocument(&pDOMDocPtr);
	if (pDOMDocPtr == NULL)
		return NULL;

	// 创建Element
	VARIANT vtTemp;
	vtTemp.vt = VT_I2;
	vtTemp.iVal = 1;	

	MSXML2::IXMLDOMNodePtr pDOMNodeCreated = pDOMDocPtr->createNode(vtTemp, _bstr_t(szElementName), L"");
	if (pDOMNodeCreated == NULL)
		return NULL;

	// 用来指向被附加后的节点
	MSXML2::IXMLDOMNodePtr pNodeAppend = NULL;
	try
	{
		pNodeAppend = m_pDOMNode->appendChild(pDOMNodeCreated);
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

// 删除子结点
int CBpXMLNode::DeleteChild(const IBpXMLNode *pNodeChild)
{
	if (m_pDOMNode == NULL || pNodeChild == NULL || ((CBpXMLNode*)pNodeChild)->m_pDOMNode == NULL)
		return XML_ERROR;

	try
	{
		MSXML2::IXMLDOMNodePtr pTempNode = m_pDOMNode->removeChild(((CBpXMLNode*)pNodeChild)->m_pDOMNode);
		if (pTempNode == NULL)
			return XML_ERROR;

		pTempNode->Release();
	}
	catch (...)	
	{
		return XML_ERROR;	
	}
	return XML_OK;
}


// 清除节点的所有子节点，并将strText作为文本节点添加为本节点的子节点
int CBpXMLNode::SetText(const wchar_t *szText)
{
	if (m_pDOMNode == NULL || szText == NULL)
		return XML_ERROR;

	try
	{
		_bstr_t bstrText(szText);
		m_pDOMNode->put_text(bstrText);
	}
	catch (...)
	{
		return XML_ERROR;	
	}
	return XML_OK;
}

// 设置属性值
int CBpXMLNode::SetAttribute(const wchar_t *szAttrName, const wchar_t *szAttrValue)
{
	if (m_pDOMNode == NULL || szAttrName == NULL || szAttrValue == NULL)
		return XML_ERROR;

	if (m_pDOMNode->GetnodeType() != MSXML2::NODE_ELEMENT)
		return XML_ERROR;
	
	try
	{
		_bstr_t bstrName(szAttrName);
		MSXML2::IXMLDOMElementPtr pElemt = (MSXML2::IXMLDOMElementPtr)m_pDOMNode;
		pElemt->setAttribute(_bstr_t(szAttrName),_variant_t(szAttrValue));

	}
	catch (...)
	{
		return XML_ERROR;
	}
	return XML_OK;
}