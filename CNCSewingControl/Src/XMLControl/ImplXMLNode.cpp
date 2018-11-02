//===================================================================================================
// Summary:
//		XMLNode��ʵ���ļ�
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

// ��ȡ��ǰ���ĸ����
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

// ��ȡ��ǰ�ڵ������
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

// ��ȡ��ǰ�ڵ㼰���ӽڵ������
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

// ��ȡ��ǰ�ڵ������Element�ӽڵ����
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

// ��ȡ��ǰ�ڵ��ָ��λ�õ�Element�ӽڵ�
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

// ��ȡ��ǰ�ڵ�ĵ�һ��Element�ӽڵ�
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

// ��ȡ��ǰ�ڵ�����һ��Element�ӽڵ�
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

// ��ȡ��ǰ��㣬ָ�����Ƶĵ�index��Element�ӽ��
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

// ��ȡ��ǰ����ǰһ��Element�ֵܽ��
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

// ��ȡ��ǰ������һ��Element�ֵܽ��
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

// ��ȡ��ǰ�������Եĸ���
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

// ��ȡ��ǰ����ָ�����Ե�ֵ			
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

// ��ȡ��ǰ����ָ����ŵ����Ե�����
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

// �жϵ�ǰ�ڵ��Ƿ�����ӽ��
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

// �жϵ�ǰ�ڵ��Ƿ����Element�ӽ��
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

// �жϵ�ǰ�ڵ��Ƿ�Ϊ�սڵ㣬����m_pDOMNode�Ƿ�Ϊ��
bool CBpXMLNode::IsEmpty() const
{
	if (m_pDOMNode == NULL)
		return true;
	else
		return false;
}

// ���ݸ��������֣�����Element�ڵ㣬�����丽��Ϊ��ǰ�ڵ���ӽڵ�
IBpXMLNode* CBpXMLNode::AppendNewElementChild(const wchar_t *szElementName)
{
	if (m_pDOMNode == NULL)	
		return NULL;

	MSXML2::IXMLDOMDocument	*pDOMDocPtr = NULL;
	m_pDOMNode->get_ownerDocument(&pDOMDocPtr);
	if (pDOMDocPtr == NULL)
		return NULL;

	// ����Element
	VARIANT vtTemp;
	vtTemp.vt = VT_I2;
	vtTemp.iVal = 1;	

	MSXML2::IXMLDOMNodePtr pDOMNodeCreated = pDOMDocPtr->createNode(vtTemp, _bstr_t(szElementName), L"");
	if (pDOMNodeCreated == NULL)
		return NULL;

	// ����ָ�򱻸��Ӻ�Ľڵ�
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

// ɾ���ӽ��
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


// ����ڵ�������ӽڵ㣬����strText��Ϊ�ı��ڵ����Ϊ���ڵ���ӽڵ�
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

// ��������ֵ
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