//===================================================================================================
// Summary:
//		主视图类
// Date:
//		2018-07-07
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#pragma once


class CCNCSewingControlView : public CView
{
protected: // 仅从序列化创建
	CCNCSewingControlView();
	DECLARE_DYNCREATE(CCNCSewingControlView)

// 属性
	PreviewControlData m_previewCtrl;	// 预览控制数据（窗口变化时需更新）

public:
	CCNCSewingControlDoc* GetDocument() const;

// 操作
public:

protected:
	void PreviewSewfile(const NeedleTrailData& trailToPreview);
	// 绘制起点标识
	void DrawStartEndFlag(CDC* pDC, CPoint pnt, BOOL bStart);

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CCNCSewingControlView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // CNCSewingControlView.cpp 中的调试版本
inline CCNCSewingControlDoc* CCNCSewingControlView::GetDocument() const
   { return reinterpret_cast<CCNCSewingControlDoc*>(m_pDocument); }
#endif

