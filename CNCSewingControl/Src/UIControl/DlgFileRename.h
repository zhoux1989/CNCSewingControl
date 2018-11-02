//===================================================================================================
// Summary:
//		对话框：重命名
// Date:
//		2018-07-17
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#pragma once
#include "afxwin.h"

class CDlgFileRename : public CDialog
{
public:
	CDlgFileRename(CWnd* pParent = NULL);
	virtual ~CDlgFileRename();

	enum { IDD = IDD_FILE_RENAME };

public:
	CString m_strFileName;
	BOOL m_bCopyDlg;
	int m_nFileID;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()
};
