//===================================================================================================
// Summary:
//		参数设置视图类
// Date:
//		2018-07-09
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#pragma once

#include "ParaXMLParse.h"

// 自定义网格属性类
class CUDFPropertyGridProperty : public CMFCPropertyGridProperty
{
public:
	CUDFPropertyGridProperty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL);
	virtual ~CUDFPropertyGridProperty(){};

	BOOL IsValueModified(const COleVariant& var1, const COleVariant& var2);

	virtual void OnDrawValue(CDC* pDC, CRect rect);
};

// IO检测视图工具栏类
class CParaConfigToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

// IO检测视图类
class CParaConfigView : public CDockablePane
{
// 构造
public:
	CParaConfigView();
	virtual ~CParaConfigView();

// 属性
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_ctrlParaList.SetVSDotNetLook(bSet);
		m_ctrlParaList.SetGroupNameFullWidth(bSet);
	}

	void SetViewState(BOOL bStartUpdate);

	// 更新参数
	int UpdataParam(ParaInfo para, BOOL bEndFlag);

	void InitData();	// 初始化
	void SetSuccess();	// 设置生效

protected:
	CFont m_fntPropList;
	CMFCPropertyGridCtrl m_ctrlParaList;
	CParaConfigToolBar m_wndToolBar;
	CEdit m_editSearch;
	ParaInfoArray m_paraList;
	int m_nParamInUpdate;	// 更新中的参数
	MapParaInf m_mapParaInfo;

	ParaInfoArray m_paraToUpdate;

protected:
	// 布局调整
	void AdjustLayout();
	// 输入检测列表
	void SetParaList();
	// 设置字体
	void SetViewCtrlFont();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnCmdSave();
	afx_msg void OnCmdSearch();
	afx_msg void OnCmdReturn();
	afx_msg void OnRefresh();
	afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateValid(CCmdUI* pCmdUI);
	afx_msg BOOL OnToolTipsNotify(UINT id,NMHDR * pNMHDR,LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};
