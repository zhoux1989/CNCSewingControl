//===================================================================================================
// Summary:
//		XML��document�ڵ�
// Usage:
//		�õ�CBpXMLDocumentָ���ɽ���XPath����������XML�����������ڵ��
// Remarks:
//		ע�ⷵ��ָ����ͷ�
//		�ͷ�CBpXMLNodeָ�롢CBpXMLDocumentָ�벢����Ӱ��DOM���Ľṹ
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
	// XML�ĵ�����ָ��
	MSXML2::IXMLDOMDocumentPtr	m_pDOMDocument;	
	// ��Ԫ				
	friend class CBpXMLNode;	
	int m_nLastError;
	CString m_strFileName;
};

//===================================================================================================

#endif	// #ifndef	__Bp_XMLCodec_ImplXMLDocument__