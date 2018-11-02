// TransparentStatic.h: interface for the CTransparentStatic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSPARENTSTATIC_H__E2779322_272C_4C05_B61F_F09355ACE5C0__INCLUDED_)
#define AFX_TRANSPARENTSTATIC_H__E2779322_272C_4C05_B61F_F09355ACE5C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTransparentStatic : public CStatic  
{
public:
	CTransparentStatic();
	virtual ~CTransparentStatic();

protected:
	afx_msg LRESULT OnSetText(WPARAM,LPARAM);
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

private:
	CBitmap m_Bmp;

};

#endif // !defined(AFX_TRANSPARENTSTATIC_H__E2779322_272C_4C05_B61F_F09355ACE5C0__INCLUDED_)
