//===========================================================================
// Summary:
//		µÈ´ý¶Ô»°¿ò¡£
// Usage:
//		
// Remarks:
//		Null
// Date:
//		2009-04-23
// Author:
//		
//===========================================================================

#include "stdafx.h"
#include "DlgWaiting.h"

//===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//===========================================================================


CDlgWaiting::CDlgWaiting(CWnd* pParent /*=NULL*/) : CDialog(CDlgWaiting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgWaiting)
	//}}AFX_DATA_INIT
}


void CDlgWaiting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgWaiting)
	DDX_Control(pDX, IDC_WAITING_INFO, m_StaticInfo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgWaiting, CDialog)
	//{{AFX_MSG_MAP(CDlgWaiting)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//===========================================================================

BOOL CDlgWaiting::Create()
{
	BOOL bRet = CDialog::Create(CDlgWaiting::IDD);
	CenterWindow();
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	ShowWindow(SW_SHOW);
	UpdateWindow();
	return bRet;
}

CDlgWaiting::~CDlgWaiting()
{
	DestroyWindow();
}

void CDlgWaiting::SetInfo(LPCTSTR lpInfo)
{
	m_StaticInfo.SetWindowText(lpInfo);
}

BOOL CDlgWaiting::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}

BOOL CDlgWaiting::OnEraseBkgnd(CDC* pDC)
{
	CDC memDC;
	CBitmap *pOldBitmap;
	BITMAP bm;
	CRect rect;

	GetWindowRect(&rect);

	CBitmap m_bmSkin;
	
	m_bmSkin.LoadBitmap(IDB_BMP_WAITTING);

	memDC.CreateCompatibleDC(pDC);
	GetObject(m_bmSkin.GetSafeHandle(), sizeof(bm), &bm);

	pOldBitmap = memDC.SelectObject(&m_bmSkin);
	pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, 311, 50, SRCCOPY);
	memDC.SelectObject(pOldBitmap);

	m_bmSkin.DeleteObject();

	return TRUE;
}
