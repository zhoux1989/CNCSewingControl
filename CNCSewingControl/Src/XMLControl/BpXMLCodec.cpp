//===================================================================================================
// Summary:
//		
// Date:
//		2008-12-3
// Author:
//		Yang Xuefeng (yang_xuefeng@founder.com.cn)
//===================================================================================================

#include "stdafx.h"
#include "BaseMacro.h"
#include "ImplXMLDocument.h"
#include "ImplXMLNode.h"

//===================================================================================================

bool InitXMLCodec()
{
	try
	{
		CoInitialize(NULL);
	}
	catch(...)
	{
	}
	return true;
}

bool UnInitXMLCodec()
{
	CoUninitialize();
	return true;
}

IBpXMLDocument *CreateXMLDocument()
{
	CBpXMLDocument *pXMLDocument = new CBpXMLDocument();
	if (pXMLDocument == NULL)
		return NULL;

	return (IBpXMLDocument *)pXMLDocument;
}

bool DestroyXMLDocument(IBpXMLDocument *pBpXMLDocument)
{
	CBpXMLDocument *pDocument = (CBpXMLDocument *)pBpXMLDocument;
	SAFE_DELETE_POINTER(pDocument);
	return true;
}

bool DestroyXMLNode(IBpXMLNode *pBpXMLNode)
{
	CBpXMLNode *pNode = (CBpXMLNode *)pBpXMLNode;
	SAFE_DELETE_POINTER(pNode);
	return true;
}