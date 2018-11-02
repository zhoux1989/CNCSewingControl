//===================================================================================================
// Summary:
//		�Զ������Կؼ�
// Date:
//		2018-10-30
// Author:
//		
//===================================================================================================

#ifndef __UserPropertyGridCtrl_h__
#define __UserPropertyGridCtrl_h__

//===================================================================================================

#include "Resource.h"

//===================================================================================================

#define IDC_FIRSTBTN_PLUS_ID		20001
#define IDC_FIRSTBTN_MINUS_ID		30001
#define IDC_FIRSTBTN_RESET_ID		40001
#define IDC_FIRSTBTN_SAVE_ID		50001

typedef vector<CButton*> ButtonGroup;

// �Զ�������������
class CUserPropertyGridProperty : public CMFCPropertyGridProperty
{
public:
	CUserPropertyGridProperty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL);
	virtual ~CUserPropertyGridProperty(){};

	int m_nStep;	// �Ӽ��Ĳ���ֵ
	BOOL m_bNeedSave;

	//BOOL IsValueModified(const COleVariant& var1, const COleVariant& var2);
	virtual void OnDrawValue(CDC* pDC, CRect rect);
};

class CUserPropertyGridCtrl : public CMFCPropertyGridCtrl
{
public:
	CUserPropertyGridCtrl();
	virtual ~CUserPropertyGridCtrl() { }

public:

private:
	vector<ButtonGroup> m_arrButtonGroups;


public:
	void UpdateSubCtrlPos();
	// �ƶ�ѡ�е�Ԫ��
	/*BOOL MoveLeft();
	BOOL MoveRight();
	BOOL MoveUp();
	BOOL MoveDown();
	BOOL MoveNextRowHead();
	int m_nRow;
	int m_nCol;
	BOOL m_bEdit;
	ListItemEditControlInfo m_subCtrl;*/

protected:
	//{{AFX_VIRTUAL(CDlgPartTree)
	//virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual VOID PostNcDestroy();
	//}}AFX_VIRTUAL

	UINT m_nItemHeight;

public:
	// �����༭�ؼ�
	//void CreateSubCtrl(int nCtrlType, int nIndex = 0);
	// ��ʼ���༭�ؼ�
	//void InitSubCtrl(const CString &strText, LPRECT rect);
	// ���ٱ༭�ؼ�
	void DestroySubCtrl();
	// ���Ӱ�ť
	void AddButtonGroup(int nItem, BOOL bSwitchVal=FALSE, BOOL bSaveButton=FALSE, BOOL bResetButton=FALSE);
	// ���°�ť��λ��
	void UpdateBtnPos(int nItem);

protected:
	//{{AFX_MSG(CSPLList)
	//afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//afx_msg void OnKillFocus(CWnd *pNewWnd);
	afx_msg void OnBtnClickedPlus(UINT nBtnID);
	afx_msg void OnBtnClickedMinus(UINT nBtnID);
	afx_msg void OnBtnClickedSave(UINT nBtnID);
	afx_msg void OnUpdateSaveBtn(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
};

//===================================================================================================

#endif