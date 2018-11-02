//===================================================================================================
// Summary:
//		����������ͼ��
// Date:
//		2018-07-09
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#pragma once

#include "ParaXMLParse.h"

// �Զ�������������
class CUDFPropertyGridProperty : public CMFCPropertyGridProperty
{
public:
	CUDFPropertyGridProperty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL);
	virtual ~CUDFPropertyGridProperty(){};

	BOOL IsValueModified(const COleVariant& var1, const COleVariant& var2);

	virtual void OnDrawValue(CDC* pDC, CRect rect);
};

// IO�����ͼ��������
class CParaConfigToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

// IO�����ͼ��
class CParaConfigView : public CDockablePane
{
// ����
public:
	CParaConfigView();
	virtual ~CParaConfigView();

// ����
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_ctrlParaList.SetVSDotNetLook(bSet);
		m_ctrlParaList.SetGroupNameFullWidth(bSet);
	}

	void SetViewState(BOOL bStartUpdate);

	// ���²���
	int UpdataParam(ParaInfo para, BOOL bEndFlag);

	void InitData();	// ��ʼ��
	void SetSuccess();	// ������Ч

protected:
	CFont m_fntPropList;
	CMFCPropertyGridCtrl m_ctrlParaList;
	CParaConfigToolBar m_wndToolBar;
	CEdit m_editSearch;
	ParaInfoArray m_paraList;
	int m_nParamInUpdate;	// �����еĲ���
	MapParaInf m_mapParaInfo;

	ParaInfoArray m_paraToUpdate;

protected:
	// ���ֵ���
	void AdjustLayout();
	// �������б�
	void SetParaList();
	// ��������
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
