//===================================================================================================
// Summary:
//		对话框：重命名
// Date:
//		2018-07-17
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#include "stdafx.h"
#include "CNCSewingControl.h"
#include "DlgFileRename.h"

//===================================================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//===================================================================================================

CDlgFileRename::CDlgFileRename(CWnd* pParent /*=NULL*/) : CDialog(CDlgFileRename::IDD, pParent)
{
	m_strFileName = L"";
	m_bCopyDlg = FALSE;
	m_nFileID = -1;
}

CDlgFileRename::~CDlgFileRename()
{
}

void CDlgFileRename::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NEWNAME, m_strFileName);
}


BEGIN_MESSAGE_MAP(CDlgFileRename, CDialog)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

BOOL CDlgFileRename::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (m_bCopyDlg)
		this->SetWindowText(L"复制");
	m_strFileName.Format(L"%d", m_nFileID);

	UpdateData(FALSE);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NEWNAME);
	pEdit->SetSel(0, -1, FALSE);

	return TRUE;
}

void CDlgFileRename::OnOK()
{
	UpdateData(TRUE);
	int nTemp = _wtoi(m_strFileName);
	if (nTemp > 65535 || nTemp < 1)
	{
		MessageBox(L"输入值无效!", L"警告", MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	m_nFileID = nTemp;

	CDialog::OnOK();
}

void CDlgFileRename::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NEWNAME);
	pEdit->SetFocus();
}
