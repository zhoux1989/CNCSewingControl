//===================================================================================================
// Summary:
//		IO检测视图类
// Date:
//		2018-07-07
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#pragma once

// IO检测视图工具栏类
class CIOCheckToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

// IO检测视图类
class CIOCheckView : public CDockablePane
{
// 构造
public:
	CIOCheckView();
	virtual ~CIOCheckView();

// 属性
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_ctrlItemList.SetVSDotNetLook(bSet);
		m_ctrlItemList.SetGroupNameFullWidth(bSet);
	}

	void InitData();
	void ReceiveIOData(const vector<int>& arrBin);
	// 输入检测列表
	void UpdateInputList();
	
	BOOL m_bTimerStart;
	void StartTimer();
	void EndTimer();

protected:
	CFont m_fntPropList;
	CMFCPropertyGridCtrl m_ctrlItemList;
	//CIOCheckToolBar m_wndToolBar;
	CComboBox m_wndIOCombo;
	//CIOCheckToolBar m_wndSearchBar;
	//CEdit m_wndSeartchCtrl;
	IOCheckDataArray m_arrInputCheck;
	IOCheckDataArray m_arrOutputCheck;

// 实现
public:

protected:
	// 布局调整
	void AdjustLayout();
	// 输入检测列表
	void SetListInputCheck();
	// 输出检测列表
	void SetListOutputCheck();
	// 设置信息列表字体
	void SetSewingInfoListFont();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnIOCheckSelChange();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	//afx_msg void OnCmdSearch();
	afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

