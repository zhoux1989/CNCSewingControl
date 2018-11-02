//===================================================================================================
// Summary:
//		对话框：连接缝纫机
// Date:
//		2018-08-25
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#pragma once
#include "afxwin.h"
#include "resource.h"

class CDlgConnect : public CDialog
{
public:
	CDlgConnect(CWnd* pParent = NULL);
	virtual ~CDlgConnect();

	enum { IDD = IDD_CONNECT };

public:
	int m_nPort;
	CComboBox m_cmbPort;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL OnInitDialog();

public:
	DECLARE_MESSAGE_MAP()
};
