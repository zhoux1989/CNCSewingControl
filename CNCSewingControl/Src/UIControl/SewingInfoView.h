//===================================================================================================
// Summary:
//		缝纫信息视图类
// Date:
//		2018-07-07
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#pragma once

#include "UserPropertyGridCtrl.h"

// 缝纫机信息视图工具栏类
class CSewingInfoToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

// 缝纫机信息视图属性类
class CDoubleClickedPropertyCtrl : public CMFCPropertyGridCtrl
{
public:
	CDoubleClickedPropertyCtrl(){};
	virtual ~CDoubleClickedPropertyCtrl(){};

protected:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

// 缝纫机信息视图类
class CSewingInfoView : public CDockablePane
{
// 构造
public:
	CSewingInfoView();
	virtual ~CSewingInfoView();

// 属性
protected:
	CFont m_fntPropList;
	CSewingInfoToolBar m_wndToolBar;
	SewingInfo m_infoSewing;

public:
	CUserPropertyGridCtrl m_wndInfoList;

// 实现
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndInfoList.SetVSDotNetLook(bSet);
		m_wndInfoList.SetGroupNameFullWidth(bSet);
	}

	// 更新
	void UpdateSewingInfo(SewingInfo infoSewing);

	void InitData();	// 初始化

protected:
	// 布局调整
	void AdjustLayout();
	// 初始化缝纫信息列表
	void InitSewingInfoList();
	// 设置信息列表字体
	void SetSewingInfoListFont();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnSaveValue(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

