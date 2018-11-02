//===================================================================================================
// Summary:
//		²ÎÊýXML½âÎö
// Date:
//		2018-08-16
// Author:
//		
//===================================================================================================

#ifndef	__XMLCodec_ParaXMLParse__
#define __XMLCodec_ParaXMLParse__

//===================================================================================================

class IBpXMLDocument;

//===================================================================================================

class CParaXMLParse
{
public:
	CParaXMLParse();
	CParaXMLParse(const CString& strFilePath);
	virtual ~CParaXMLParse();

public:
	BOOL Init();
	BOOL Uninit();

public:
	BOOL ReadParamList(MapParaInf &mapParaList);
	BOOL ReadInputCheckList(IOCheckDataArray &arrInputCheck);
	BOOL ReadOutputCheckList(IOCheckDataArray &arrInputCheck);
	//BOOL Save(const CheckRuleArray &arrCheckRuleList);

private:
	IBpXMLDocument *m_pXMLDocument;
};

//===================================================================================================

#endif  
