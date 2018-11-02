//===================================================================================================
// Summary:
//		������Ϣ��ͼ��
// Date:
//		2018-07-07
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#pragma once

#include "UserPropertyGridCtrl.h"

// ���һ���Ϣ��ͼ��������
class CSewingInfoToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

// ���һ���Ϣ��ͼ������
class CDoubleClickedPropertyCtrl : public CMFCPropertyGridCtrl
{
public:
	CDoubleClickedPropertyCtrl(){};
	virtual ~CDoubleClickedPropertyCtrl(){};

protected:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

// ���һ���Ϣ��ͼ��
class CSewingInfoView : public CDockablePane
{
// ����
public:
	CSewingInfoView();
	virtual ~CSewingInfoView();

// ����
protected:
	CFont m_fntPropList;
	CSewingInfoToolBar m_wndToolBar;
	SewingInfo m_infoSewing;

public:
	CUserPropertyGridCtrl m_wndInfoList;

// ʵ��
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndInfoList.SetVSDotNetLook(bSet);
		m_wndInfoList.SetGroupNameFullWidth(bSet);
	}

	// ����
	void UpdateSewingInfo(SewingInfo infoSewing);

	void InitData();	// ��ʼ��

protected:
	// ���ֵ���
	void AdjustLayout();
	// ��ʼ��������Ϣ�б�
	void InitSewingInfoList();
	// ������Ϣ�б�����
	void SetSewingInfoListFont();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnSaveValue(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

