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

#ifndef __DLGWAITING_HEADERFILE__
#define __DLGWAITING_HEADERFILE__

//===========================================================================

#include "Resource.h"
#include "TransparentStatic.h"

//===========================================================================

class CDlgWaiting : public CDialog
{
public:
	CDlgWaiting(CWnd* pParent = NULL); 
	virtual	~CDlgWaiting();

public:
	BOOL Create();
	void SetInfo(LPCTSTR lpInfo);

	//{{AFX_DATA(CDlgWaiting)
	enum { IDD = IDD_DLG_WAITING };
	CTransparentStatic	m_StaticInfo;
	//}}AFX_DATA

protected:
	//{{AFX_VIRTUAL(CDlgWaiting)
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_VIRTUAL


protected:
	//{{AFX_MSG(CDlgWaiting)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//===========================================================================

#endif // !defined(AFX_DLGWAITING_H__5C6922D5_DF7F_4A02_92E5_8B4AB701CE31__INCLUDED_)
