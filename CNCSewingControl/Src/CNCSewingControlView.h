//===================================================================================================
// Summary:
//		����ͼ��
// Date:
//		2018-07-07
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#pragma once


class CCNCSewingControlView : public CView
{
protected: // �������л�����
	CCNCSewingControlView();
	DECLARE_DYNCREATE(CCNCSewingControlView)

// ����
	PreviewControlData m_previewCtrl;	// Ԥ���������ݣ����ڱ仯ʱ����£�

public:
	CCNCSewingControlDoc* GetDocument() const;

// ����
public:

protected:
	void PreviewSewfile(const NeedleTrailData& trailToPreview);
	// ��������ʶ
	void DrawStartEndFlag(CDC* pDC, CPoint pnt, BOOL bStart);

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CCNCSewingControlView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // CNCSewingControlView.cpp �еĵ��԰汾
inline CCNCSewingControlDoc* CCNCSewingControlView::GetDocument() const
   { return reinterpret_cast<CCNCSewingControlDoc*>(m_pDocument); }
#endif

