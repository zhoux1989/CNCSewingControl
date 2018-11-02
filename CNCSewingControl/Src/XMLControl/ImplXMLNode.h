//===================================================================================================
// Summary:
//		XML�ڵ����
// Usage:
//		XML�ڵ�����ṩXML�ڵ�����ԡ��ӽڵ�����ݵĲ�����
// Remarks:
//		Ŀǰ��ʾElement�ڵ㣬ע�ⷵ��ָ����ͷ�
//		�ͷ�CBpXMLNodeָ�롢CBpXMLDocumentָ�벢����Ӱ��DOM���Ľṹ
// Date:
//		2008-12-3
// Author:
//		Yang Xuefeng (yang_xuefeng@founder.com.cn)
//===================================================================================================

#ifndef	__Bp_XMLCodec_ImplXMLNode__
#define __Bp_XMLCodec_ImplXMLNode__

//===================================================================================================

class CBpXMLNode : public IBpXMLNode
{
public:
	CBpXMLNode();
	CBpXMLNode(CBpXMLNode* pNode);
	virtual ~CBpXMLNode();

public:
	virtual IBpXMLNode *GetParent() const;
	virtual int GetName(CString &strNodeName) const;		
	virtual int GetText(CString &strNodeText) const;
	virtual int GetElementCount() const;
	virtual IBpXMLNode *GetElementChildByIndex(int nIndex) const;
	virtual IBpXMLNode *GetFirstElementChild() const;
	virtual IBpXMLNode *GetLastElementChild() const;
	virtual IBpXMLNode *GetElementChildByName(const wchar_t *szChildNodeName, int nIndex = 1) const;
	virtual IBpXMLNode *GetPreviousElementSibling() const;
	virtual IBpXMLNode *GetNextElementSibling() const;		
	virtual int GetAttributeCount() const;
	virtual int GetAttributeByName(const wchar_t *szAttributeName, CString &strAttributeValue) const;
	virtual int GetAttributeByIndex(int nIndex, CString &strAttributeValue) const;

public:
	virtual bool HasChildNodes() const;
	virtual bool HasElementChildNodes() const;
	virtual bool IsEmpty() const;

public:
	virtual IBpXMLNode* AppendNewElementChild(const wchar_t *szElementName);
	virtual int DeleteChild(const IBpXMLNode *pNodeChild);

public:
	virtual int SetText(const wchar_t *szText);
	virtual int SetAttribute(const wchar_t *szAttrName, const wchar_t *szAttrValue);

protected:
	/*// �������ڵ����ӽڵ㣬���Ƿ�����������Ҫ���Ŀ��
	virtual DOMNode* SearchNodeByNameAndText(DOMNode* pNode, const OUString& strChildNodeName, const OUString& strChildNodeText) const;*/

private:
	MSXML2::IXMLDOMNodePtr m_pDOMNode;	// DOM���������
	int m_nCurrentParseIndex;			// ��ǰ����������

	friend class CBpXMLParser;	// ��Ԫ��
	friend class CBpXMLDocument;
};

//===================================================================================================

#endif // #ifndef__Bp_XMLCodec_ImplXMLNode__

