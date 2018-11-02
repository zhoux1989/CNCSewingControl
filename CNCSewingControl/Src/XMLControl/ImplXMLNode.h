//===================================================================================================
// Summary:
//		XML节点对象
// Usage:
//		XML节点对象，提供XML节点的属性、子节点等内容的操作。
// Remarks:
//		目前表示Element节点，注意返回指针的释放
//		释放CBpXMLNode指针、CBpXMLDocument指针并不会影响DOM树的结构
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
	/*// 搜索本节点与子节点，看是否有满足搜索要求的目标
	virtual DOMNode* SearchNodeByNameAndText(DOMNode* pNode, const OUString& strChildNodeName, const OUString& strChildNodeText) const;*/

private:
	MSXML2::IXMLDOMNodePtr m_pDOMNode;	// DOM树结点类型
	int m_nCurrentParseIndex;			// 当前解析的索引

	friend class CBpXMLParser;	// 友元类
	friend class CBpXMLDocument;
};

//===================================================================================================

#endif // #ifndef__Bp_XMLCodec_ImplXMLNode__

