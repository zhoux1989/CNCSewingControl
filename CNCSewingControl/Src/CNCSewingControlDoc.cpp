//===================================================================================================
// Summary:
//		���ĵ���
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


// CCNCSewingControlDoc ����/����

CCNCSewingControlDoc::CCNCSewingControlDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CCNCSewingControlDoc::~CCNCSewingControlDoc()
{
}

BOOL CCNCSewingControlDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}

// CCNCSewingControlDoc ���л�
void CCNCSewingControlDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CCNCSewingControlDoc ���
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


// CCNCSewingControlDoc ����
