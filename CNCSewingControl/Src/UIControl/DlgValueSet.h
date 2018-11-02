//===================================================================================================
// Summary:
//		对话框：设置值
// Date:
//		2018-07-07
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#pragma once
#include "afxwin.h"

class CDlgValueSet : public CDialog
{
public:
	CDlgValueSet(CWnd* pParent = NULL);
	virtual ~CDlgValueSet();

	enum { IDD = IDD_VALUE_SET };

public:
	void InitData(
		const CString& strTitle, 
		const COleVariant& var, 
		int nLeftPos, 
		int nTopPos,
		double dIncrement);	// 初始化

public:
	COleVariant m_var;

protected:
	CString m_strValue;
	CStatic m_staticTitle;
	COleVariant m_varOrigin;
	double m_dIncrement;
	int m_nLeftPos;// 初始位置
	int m_nTopPos;
	CString m_strTitle;

protected:
	//{{AFX_VIRTUAL(CDlgConfig)
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDlgConfig)
	afx_msg void OnBnClickedBtnPlus();
	afx_msg void OnBnClickedBtnMinus();
	afx_msg void OnBnClickedBtnReset();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditValue();
};
