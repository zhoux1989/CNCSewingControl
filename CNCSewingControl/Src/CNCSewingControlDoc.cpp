//===================================================================================================
// Summary:
//		主文档类
// Date:
//		2018-07-07
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#include "stdafx.h"
#include "CNCSewingControl.h"

#include "CNCSewingControlDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCNCSewingControlDoc

IMPLEMENT_DYNCREATE(CCNCSewingControlDoc, CDocument)

BEGIN_MESSAGE_MAP(CCNCSewingControlDoc, CDocument)
END_MESSAGE_MAP()


// CCNCSewingControlDoc 构造/析构

CCNCSewingControlDoc::CCNCSewingControlDoc()
{
	// TODO: 在此添加一次性构造代码

}

CCNCSewingControlDoc::~CCNCSewingControlDoc()
{
}

BOOL CCNCSewingControlDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}

// CCNCSewingControlDoc 序列化
void CCNCSewingControlDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CCNCSewingControlDoc 诊断
#ifdef _DEBUG
void CCNCSewingControlDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCNCSewingControlDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCNCSewingControlDoc 命令
