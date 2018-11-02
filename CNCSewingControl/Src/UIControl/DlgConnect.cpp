//===================================================================================================
// Summary:
//		对话框：连接缝纫机
// Date:
//		2018-08-25
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#include "stdafx.h"
#include "DlgConnect.h"

//===================================================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//===================================================================================================

CDlgConnect::CDlgConnect(CWnd* pParent /*=NULL*/) : CDialog(CDlgConnect::IDD, pParent)
{
	m_nPort = 0;
}

CDlgConnect::~CDlgConnect()
{
}

void CDlgConnect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_SET_PORT, m_cmbPort);
}


BEGIN_MESSAGE_MAP(CDlgConnect, CDialog)
END_MESSAGE_MAP()

BOOL CDlgConnect::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cmbPort.AddString(_T("COM1"));
	m_cmbPort.AddString(_T("COM2"));
	m_cmbPort.AddString(_T("COM3"));
	m_cmbPort.AddString(_T("COM4"));
	m_cmbPort.AddString(_T("COM5"));
	m_cmbPort.AddString(_T("COM6"));
	m_cmbPort.AddString(_T("COM7"));
	m_cmbPort.AddString(_T("COM8"));
	m_cmbPort.AddString(_T("COM9"));
	m_cmbPort.SetCurSel(0);

	return TRUE;
}

void CDlgConnect::OnOK()
{
	UpdateData(TRUE);

	m_nPort = m_cmbPort.GetCurSel()+1;

	CDialog::OnOK();
}
