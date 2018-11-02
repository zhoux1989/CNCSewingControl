//===================================================================================================
// Summary:
//		XML的document节点
// Usage:
//		得到CBpXMLDocument指针后可进行XPath操作、保存XML操作、创建节点等
// Remarks:
//		注意返回指针的释放
//		释放CBpXMLNode指针、CBpXMLDocument指针并不会影响DOM树的结构
// Date:
//		2008-12-3
// Author:
//		Yang Xuefeng (yang_xuefeng@founder.com.cn)
//===================================================================================================

#ifndef __Bp_XMLCodec_ImplXMLDocument__
#define __Bp_XMLCodec_ImplXMLDocument__

//===================================================================================================

class CBpXMLNode;

//===================================================================================================

class CBpXMLDocument : public IBpXMLDocument
{
public:
	CBpXMLDocument();
	~CBpXMLDocument();

public:
	virtual bool OpenXMLForDocument(const wchar_t *szFilePath);
	virtual bool CreateDocument(const wchar_t *szRootElementName);	
	virtual bool SaveXML(const wchar_t *szFilePath) const;
	virtual bool Release();

public:
	virtual IBpXMLNode* CreateElementNode(const wchar_t *szElementNodeName);
	virtual IBpXMLNode* CreateTextNode(const wchar_t *szText);
	virtual IBpXMLNode* GetRootElement();

public:
	virtual int GetLastError() const { return m_nLastError; }

private:
	// XML文档对象指针
	MSXML2::IXMLDOMDocumentPtr	m_pDOMDocument;	
	// 友元				
	friend class CBpXMLNode;	
	int m_nLastError;
	CString m_strFileName;
};

//===================================================================================================

#endif	// #ifndef	__Bp_XMLCodec_ImplXMLDocument__