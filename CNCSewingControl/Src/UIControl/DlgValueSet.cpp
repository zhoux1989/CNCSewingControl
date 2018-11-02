//===================================================================================================
// Summary:
//		对话框：设置值
// Date:
//		2018-07-07
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#include "stdafx.h"
#include "CNCSewingControl.h"
#include "DlgValueSet.h"

//===================================================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//===================================================================================================

CDlgValueSet::CDlgValueSet(CWnd* pParent /*=NULL*/) : CDialog(CDlgValueSet::IDD, pParent)
{
	m_strValue = L"";
	m_dIncrement = 1;
	m_strTitle = L"";
	m_nLeftPos = -1;
	m_nTopPos = -1;
}

CDlgValueSet::~CDlgValueSet()
{
}

void CDlgValueSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_staticTitle);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_strValue);
}

BEGIN_MESSAGE_MAP(CDlgValueSet, CDialog)
	ON_BN_CLICKED(IDC_BTN_PLUS, OnBnClickedBtnPlus)
	ON_BN_CLICKED(IDC_BTN_MINUS, OnBnClickedBtnMinus)
	ON_BN_CLICKED(IDC_BTN_RESET, OnBnClickedBtnReset)
	ON_EN_CHANGE(IDC_EDIT_VALUE, OnEnChangeEditValue)
END_MESSAGE_MAP()

//===================================================================================================

BOOL CDlgValueSet::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowPos(NULL, m_nLeftPos, m_nTopPos, 0, 0, SWP_NOSIZE);

	CString strTitle;
	strTitle = m_strTitle + L"设置";
	m_staticTitle.SetWindowText(strTitle);

	m_strValue = m_var;

	UpdateData(FALSE);
	

	return TRUE;
}

void CDlgValueSet::OnOK()
{
	UpdateData(TRUE);

	CDialog::OnOK();
}

void CDlgValueSet::OnBnClickedBtnPlus()
{
	if (m_var.vt == VT_INT)
	{
		m_var.intVal += 1;
	}
	else if (m_var.vt == VT_I4)
	{
		m_var.lVal += 1;
	}
	else if (m_var.vt == VT_R8)
	{
		m_var.dblVal += 1.0;
	}
	m_strValue = m_var;
	UpdateData(FALSE);
}

void CDlgValueSet::OnBnClickedBtnMinus()
{
	if (m_var.vt == VT_INT)
	{
		m_var.intVal -= 1;
	}
	else if (m_var.vt == VT_I4)
	{
		m_var.lVal -= 1;
	}
	else if (m_var.vt == VT_R8)
	{
		m_var.dblVal -= 1.0;
	}
	m_strValue = m_var;
	UpdateData(FALSE);
}

void CDlgValueSet::OnBnClickedBtnReset()
{
	m_var = m_varOrigin;
	m_strValue = m_var;
	UpdateData(FALSE);
}

void CDlgValueSet::OnEnChangeEditValue()
{
	if (m_var.vt == VT_INT)
	{
		m_var.intVal = _wtoi(m_strValue);
	}
	else if (m_var.vt == VT_I4)
	{
		m_var.lVal = _wtoi(m_strValue);
	}
	else if (m_var.vt == VT_R8)
	{
		m_var.dblVal = _wtof(m_strValue);
	}
	m_strValue = m_var;
	UpdateData(FALSE);
}

// 初始化
void CDlgValueSet::InitData(
			  const CString& strTitle, 
			  const COleVariant& var, 
			  int nLeftPos, 
			  int nTopPos,
			  double dIncrement)
{
	m_strTitle = strTitle;
	m_var = var;
	m_varOrigin = var;
	m_nLeftPos = nLeftPos;
	m_nTopPos = nTopPos;
	m_dIncrement = dIncrement;
}