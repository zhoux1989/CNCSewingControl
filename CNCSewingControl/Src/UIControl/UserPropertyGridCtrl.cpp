//===================================================================================================
// Summary:
//		自定义属性控件
// Date:
//		2018-10-30
// Author:
//		
//===================================================================================================

#include "stdafx.h"
#include "UserPropertyGridCtrl.h"

//===================================================================================================
/*
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
*/
//===================================================================================================

CUserPropertyGridProperty::CUserPropertyGridProperty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr):CMFCPropertyGridProperty(strName, varValue, lpszDescr)
{
	m_nStep = 1;
}

void CUserPropertyGridProperty::OnDrawValue(CDC* pDC, CRect rect)
{
	// 比较当前值为默认值
	COleVariant originVal = GetOriginalValue();
	COleVariant currVal = GetValue();

	if (!(originVal == currVal))
	{
		pDC->SetTextColor(RGB(255, 0, 0));
	}

	CMFCPropertyGridProperty::OnDrawValue(pDC, rect);
	pDC->SetTextColor(RGB(0, 0, 0));
}

//===================================================================================================

CUserPropertyGridCtrl::CUserPropertyGridCtrl()
{
	
}

BEGIN_MESSAGE_MAP(CUserPropertyGridCtrl, CMFCPropertyGridCtrl)
	//{{AFX_MSG_MAP(CTreeEdit)
	/*ON_WM_MEASUREITEM_REFLECT()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_LBUTTONDBLCLK()
	*/
	//ON_WM_KILLFOCUS()
	ON_COMMAND_RANGE(IDC_FIRSTBTN_PLUS_ID, IDC_FIRSTBTN_PLUS_ID+9998, OnBtnClickedPlus)
	ON_COMMAND_RANGE(IDC_FIRSTBTN_MINUS_ID, IDC_FIRSTBTN_MINUS_ID+9998, OnBtnClickedMinus)
	ON_COMMAND_RANGE(IDC_FIRSTBTN_SAVE_ID, IDC_FIRSTBTN_SAVE_ID+9998, OnBtnClickedSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//void CUserPropertyGridCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
//{
//	TCHAR lpBuffer[256];
//
//	LV_ITEM lvi;
//
//	lvi.mask = LVIF_TEXT | LVIF_PARAM ;
//	lvi.iItem = lpDrawItemStruct->itemID; 
//	lvi.iSubItem = 0;
//	lvi.pszText = lpBuffer ;
//	lvi.cchTextMax = sizeof(lpBuffer);
//	VERIFY(GetItem(&lvi));
//
//	LV_COLUMN lvc, lvcprev ;
//	::ZeroMemory(&lvc, sizeof(lvc));
//	::ZeroMemory(&lvcprev, sizeof(lvcprev));
//	lvc.mask = LVCF_WIDTH | LVCF_FMT;
//	lvcprev.mask = LVCF_WIDTH | LVCF_FMT;
//
//	CRect textRect;
//	//lpDrawItemStruct->rcItem.top += 4;
//	for ( int nCol=0; GetColumn(nCol, &lvc); nCol++)
//	{
//		if (nCol > 0) 
//		{
//			// Get Previous Column Width in order to move the next display item
//			GetColumn(nCol-1, &lvcprev);
//			lpDrawItemStruct->rcItem.left += lvcprev.cx;
//			lpDrawItemStruct->rcItem.right = lpDrawItemStruct->rcItem.left + lvc.cx; 
//		}
//
//		// Get the text 
//		::ZeroMemory(&lvi, sizeof(lvi));
//		lvi.iItem = lpDrawItemStruct->itemID;
//		lvi.mask = LVIF_TEXT | LVIF_PARAM;
//		lvi.iSubItem = nCol;
//		lvi.pszText = lpBuffer;
//		lvi.cchTextMax = sizeof(lpBuffer);
//		VERIFY(GetItem(&lvi));
//
//		CDC* pDC;
//		pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
//
//		// pDC->Rectangle(&lpDrawItemStruct->rcItem);
//		if (lpDrawItemStruct->itemState & ODS_SELECTED && nCol == m_nCol)
//		{
//			pDC->FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_HIGHLIGHT)) ; 
//			pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
//		}
//		else
//		{
//			pDC->FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_WINDOW)) ;
//			pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
//		}
//
//		pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));
//
//		UINT uFormat = DT_LEFT;
//
//		textRect = lpDrawItemStruct->rcItem;
//		textRect.left += 4;
//		textRect.top += 4;
//		::DrawText(lpDrawItemStruct->hDC, lpBuffer, (int)wcslen(lpBuffer), 
//			&textRect, uFormat);
//		pDC->SelectStockObject(SYSTEM_FONT);
//	}
//}

//BOOL CUserPropertyGridCtrl::PreTranslateMessage(MSG* pMsg)
//{
//	BOOL bRet = FALSE;
//	if (pMsg->hwnd == this->m_hWnd && pMsg->message == WM_IME_ENDCOMPOSITION)
//	{
//// 		HIMC hIMC = ImmGetContext(this->m_hWnd);
//// 		WCHAR pBuf[1000] = {0};
//// 		ImmGetCompositionString(hIMC, GCS_RESULTSTR, pBuf, 1000);
//// 		ImmReleaseContext(this->m_hWnd, hIMC);
//// 		StartEditItem(CString(pBuf), TRUE);
//// 		bRet = TRUE;
//	}
//
//	HWND pWnd = NULL;
//	if (m_subCtrl.nType == LIECT_EDITCOMBOBOX)
//	{
//		if (m_subCtrl.pCtrl != NULL)
//			pWnd = ::GetWindow(m_subCtrl.pCtrl->m_hWnd, GW_CHILD);
//	}
//	else
//	{
//		if (m_subCtrl.pCtrl != NULL)
//			pWnd = m_subCtrl.pCtrl->m_hWnd;
//	}
//
//	if(pMsg->hwnd == pWnd)
//	{
//		switch (pMsg->message)
//		{
//		case WM_KEYDOWN:
//			{
//				switch (pMsg->wParam)
//				{
//				case VK_RETURN:		//回车
//					if (FinishEditItem())
//						MoveNextRowHead();
//					bRet = TRUE;
//					break;
//// 				case VK_TAB:		//TAB键
//// 					if (FinishEditItem())
//// 						bRet = MoveRight();
//// 					break;
//				case VK_ESCAPE:		//ESC键
//					if (m_subCtrl.pCtrl != NULL)
//					{
//						m_bEdit = FALSE;
//						m_subCtrl.pCtrl->SetWindowText(GetItemText(m_nRow, m_nCol));
//						m_subCtrl.pCtrl->ShowWindow(SW_HIDE);
//					}
//					bRet = TRUE;
//					break;
//				default:
//					break;
//				}
//			}
//		default:
//			break;
//		}
//	}
//
//	if (!bRet)
//		bRet = CListCtrl::PreTranslateMessage(pMsg);
//
//	return bRet;
//}

VOID CUserPropertyGridCtrl::PostNcDestroy()
{
	DestroySubCtrl();
}

//void CUserPropertyGridCtrl::SetItemHeight(UINT nHeight)
//{
//	m_nItemHeight = nHeight;
//	CRect rcWin;
//	GetWindowRect(&rcWin);
//	WINDOWPOS wp;
//	wp.hwnd = m_hWnd;
//	wp.cx = rcWin.Width();
//	wp.cy = rcWin.Height();
//	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
//
//	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
//}

//void CUserPropertyGridCtrl::CreateSubCtrl(int nCtrlType, int nIndex/* = 0*/)
//{
//	if (m_subCtrl.pCtrl != NULL && m_subCtrl.nType == nCtrlType)
//		return;
//
//	DestroySubCtrl();
//	RECT rect;
//	rect.left = rect.top = 0;
//	rect.bottom = 200;
//	rect.right = 100;
//	if (nCtrlType == LIECT_EDIT)
//	{
//		CEdit *pEdit = new CEdit;
//		pEdit->Create(WS_CHILD | ES_LEFT | WS_BORDER | ES_AUTOHSCROLL | ES_WANTRETURN, 
//			rect, this, IDC_EDIT_LIST_EDIT);
//		pEdit->SetFont(this->GetFont(), FALSE);
//		m_subCtrl.nType = LIECT_EDIT;
//		m_subCtrl.pCtrl = pEdit;
//	}
//	else if (nCtrlType == LIECT_COMBOBOX)
//	{
//		CComboBox *pCombo = new CComboBox;
//		pCombo->Create(CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP, 
//			rect, this, IDC_CMB_LIST_EDIT_DROPLIST);
//		pCombo->SetFont(this->GetFont(), FALSE);
//		m_subCtrl.nType = LIECT_COMBOBOX;
//		m_subCtrl.pCtrl = pCombo;
//	}
//	else if (nCtrlType == LIECT_EDITCOMBOBOX)
//	{
//		CComboBox *pCombo = new CComboBox;
//		pCombo->Create(CBS_DROPDOWN | CBS_AUTOHSCROLL | WS_VSCROLL | WS_TABSTOP, 
//			rect, this, IDC_CMB_LIST_EDIT_DROPDOWN);
//		pCombo->SetFont(this->GetFont(), FALSE);
//		m_subCtrl.nType = LIECT_EDITCOMBOBOX;
//		m_subCtrl.pCtrl = pCombo;
//	}
//	else if (nCtrlType == LIECT_BUTTON)
//	{
//		CButton *pBtn = new CButton;
//		pBtn->Create(L"测试", WS_CHILD|WS_VISIBLE,
//			rect, this, IDC_BTN_FIRST_ID + nIndex);
//		pBtn->SetFont(this->GetFont(), FALSE);
//		m_subCtrl.nType = LIECT_BUTTON;
//		m_subCtrl.pCtrl = pBtn;
//	}
//}

//// 初始化编辑控件
//void CUserPropertyGridCtrl::InitSubCtrl(const CString &strText, LPRECT rect)
//{
//	if (m_subCtrl.pCtrl == NULL)
//		return;
//	if (m_subCtrl.nType == LIECT_EDIT)
//	{
//		CEdit *pEdit = (CEdit *)m_subCtrl.pCtrl;
//		pEdit->SetWindowText(strText);
//		pEdit->MoveWindow(rect, TRUE);
//		pEdit->ShowWindow(SW_SHOW);
//		BYTE strByte[256];
//		int nLength;
//		CStringToConvertByte(strText, strByte, nLength);
//		pEdit->SetSel(nLength, nLength);
//		pEdit->SetFocus();
//	}
//	else if (m_subCtrl.nType == LIECT_COMBOBOX)
//	{
//		CComboBox *pCombo = (CComboBox *)m_subCtrl.pCtrl;
//		CStringArray *pEditData = m_GetListEditData(*this, m_nRow, m_nCol);
//		if (pEditData == NULL)
//			return;
//		CStringArray &arrData = *pEditData;
//		// 初始化数据
//		BOOL bFindName = FALSE;
//		pCombo->ResetContent();
//		for (int i = 0; i < arrData.GetSize(); i++)
//		{
//			pCombo->AddString(arrData[i]);
//			if (strText.CompareNoCase(arrData[i]) == 0)
//			{
//				pCombo->SetCurSel(i);
//				bFindName = TRUE;
//			}
//		}
//		if (!bFindName)
//			pCombo->SetCurSel(0);
//
//		pCombo->MoveWindow(rect, TRUE);
//		pCombo->ShowWindow(SW_SHOW);
//		BYTE strByte[256];
//		int nLength;
//		CStringToConvertByte(strText, strByte, nLength);
//		pCombo->SetEditSel(nLength, nLength);
//		pCombo->SetFocus();
//	}
//	else if (m_subCtrl.nType == LIECT_EDITCOMBOBOX)
//	{
//		CComboBox *pCombo = (CComboBox *)m_subCtrl.pCtrl;
//		CStringArray *pEditData = m_GetListEditData(*this, m_nRow, m_nCol);
//		if (pEditData == NULL)
//			return;
//		CStringArray &arrData = *pEditData;
//		// 初始化数据
//		BOOL bFindName = FALSE;
//		pCombo->ResetContent();
//		for (int i = 0; i < arrData.GetSize(); i++)
//		{
//			pCombo->AddString(arrData[i]);
//			if (strText.CompareNoCase(arrData[i]) == 0)
//			{
//				pCombo->SetCurSel(i);
//				bFindName = TRUE;
//			}
//		}
//		if (!bFindName)
//			pCombo->SetWindowText(strText);
//
//		pCombo->MoveWindow(rect, TRUE);
//		pCombo->ShowWindow(SW_SHOW);
//		BYTE strByte[256];
//		int nLength;
//		CStringToConvertByte(strText, strByte, nLength);
//		pCombo->SetEditSel(nLength, nLength);
//		pCombo->SetFocus();
//	}
//	else if (m_subCtrl.nType == LIECT_BUTTON)
//	{
//		CButton *pBtn = (CButton *)m_subCtrl.pCtrl;
//		pBtn->SetWindowText(strText);
//		pBtn->MoveWindow(rect, TRUE);
//		pBtn->ShowWindow(SW_SHOW);
//	}
//}

// 销毁编辑控件
void CUserPropertyGridCtrl::DestroySubCtrl()
{
	/*if (m_subCtrl.pCtrl != NULL)
	{
		if (m_subCtrl.pCtrl->GetSafeHwnd() != NULL)
			m_subCtrl.pCtrl->DestroyWindow();

		delete m_subCtrl.pCtrl;
		m_subCtrl.pCtrl = NULL;
	}*/

	for (int i=0; i<m_arrButtonGroups.size(); i++)
	{
		for (int j=0; j<m_arrButtonGroups[i].size(); j++)
		{
			if (m_arrButtonGroups[i][j] != NULL)
			{
				if (m_arrButtonGroups[i][j]->GetSafeHwnd() != NULL)
				{
					m_arrButtonGroups[i][j]->DestroyWindow();
				}

				delete m_arrButtonGroups[i][j];
				m_arrButtonGroups[i][j] = NULL;
			}
		}
		m_arrButtonGroups[i].clear();
	}
	m_arrButtonGroups.clear();
}

// 增加按钮
void CUserPropertyGridCtrl::AddButtonGroup(int nItem, BOOL bSwitchVal, BOOL bSaveButton, BOOL bResetButton)
{
	CStringArray arrButtonText;
	vector<int> arrCtrlID;
	if (bSwitchVal)
	{
		arrButtonText.Add(L"开");
		arrButtonText.Add(L"关");
	}
	else
	{
		arrButtonText.Add(L"加");
		arrButtonText.Add(L"减");
	}
	arrCtrlID.push_back(IDC_FIRSTBTN_PLUS_ID);
	arrCtrlID.push_back(IDC_FIRSTBTN_MINUS_ID);

	if (bResetButton)
	{
		arrButtonText.Add(L"重置");
		arrCtrlID.push_back(IDC_FIRSTBTN_RESET_ID);
	}
	if (bSaveButton)
	{
		arrButtonText.Add(L"保存");
		arrCtrlID.push_back(IDC_FIRSTBTN_SAVE_ID);
	}

	ButtonGroup btnGroup;
	for (int i=0; i<arrButtonText.GetSize(); i++)
	{
		CButton *pBtn = new CButton;
		pBtn->Create(arrButtonText[i], WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, arrCtrlID[i]+nItem);
		pBtn->SetFont(this->GetFont(), FALSE);
		btnGroup.push_back(pBtn);
	}
	m_arrButtonGroups.push_back(btnGroup);

	CMFCPropertyGridProperty* pProp = this->GetProperty(nItem);
	CRect rect = pProp->GetRect();
	int nLeftWidth = abs(CMFCPropertyGridCtrl::GetLeftColumnWidth());
	rect.DeflateRect(nLeftWidth, 0, 0, 0);
	rect.DeflateRect(rect.Width()/3, 0, 0, 0);

	int nCount = (int)arrButtonText.GetSize();
	int nBtnWidth = rect.Width()/nCount;
	int nSpace = 5;
	CRect rectSub;
	rectSub.top = rect.top;
	rectSub.bottom = rect.bottom;
	for (int i=0, nLeft=0, nRight=0; i<nCount; i++)
	{
		rectSub.left = rect.right-nBtnWidth*(i+1)-nSpace*i;
		rectSub.right = rectSub.left+nBtnWidth;
		btnGroup[nCount-i-1]->MoveWindow(&rectSub, TRUE);
		btnGroup[nCount-i-1]->ShowWindow(SW_SHOW);
	}
}

void CUserPropertyGridCtrl::UpdateSubCtrlPos()
{
	for (int i=0; i<m_arrButtonGroups.size(); i++)
	{
		UpdateBtnPos(i);
	}
}

// 更新按钮的位置
void CUserPropertyGridCtrl::UpdateBtnPos(int nItem)
{
	CMFCPropertyGridProperty* pProp = this->GetProperty(nItem);
	CRect rect = pProp->GetRect();
	int nLeftWidth = abs(CMFCPropertyGridCtrl::GetLeftColumnWidth());
	rect.DeflateRect(nLeftWidth, 0, 0, 0);
	rect.DeflateRect(rect.Width()/3, 0, 0, 0);

	int nCount = (int)m_arrButtonGroups[nItem].size();
	int nBtnWidth = rect.Width()/nCount;
	int nSpace = 5;
	CRect rectSub;
	rectSub.top = rect.top;
	rectSub.bottom = rect.bottom;
	for (int i=0; i<nCount; i++)
	{
		rectSub.left = rect.right-nBtnWidth*(i+1)-nSpace*i;
		rectSub.right = rectSub.left+nBtnWidth;
		m_arrButtonGroups[nItem][nCount-i-1]->MoveWindow(&rectSub, TRUE);
	}
}

//void CUserPropertyGridCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
//{
//	lpMeasureItemStruct->itemHeight = m_nItemHeight;
//}

//void CUserPropertyGridCtrl::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	CListCtrl::OnLButtonDown(nFlags, point);
//	LVHITTESTINFO hi;
//	hi.pt = point;
//	if(SubItemHitTest(&hi) != -1)
//	{
//		if (m_nRow < 0 || m_nCol < 0)
//		{
//			m_nRow = hi.iItem;
//			m_nCol = hi.iSubItem;
//			Invalidate();
//			return;
//		}
//		if (m_nRow == hi.iItem)
//		{
//			if (m_nCol != hi.iSubItem)
//			{
//				BOOL bEditComplete = TRUE;
//				if (m_bEdit)
//					bEditComplete = FinishEditItem();
//				if (bEditComplete)
//				{
//					m_nCol = hi.iSubItem;
//					Invalidate();
//				}
//			}
//			else
//			{
//				m_nRow = hi.iItem;
//				m_nCol = hi.iSubItem;
//				if (!m_bEdit)
//					StartEditItem();
//			}
//		}
//		else
//		{
//			BOOL bEditComplete = TRUE;
//			if (m_bEdit)
//				bEditComplete = FinishEditItem();
//			if (bEditComplete)
//			{
//				m_nRow = hi.iItem;
//				m_nCol = hi.iSubItem;
//				Invalidate();
//			}
//		}
//	}
//}
//
//void CUserPropertyGridCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	BOOL bUsefulKey = FALSE;
//	// 处理Ctrl+C事件
//	if (GetKeyState(VK_CONTROL) < 0 && nChar == 'C')
//	{
//		m_bClipboard = TRUE;
//		if(m_subCtrl.pCtrl == NULL)
//			CreateSubCtrl(LIECT_EDITCOMBOBOX);
//		m_subCtrl.pCtrl->SetWindowText(GetItemText(m_nRow, m_nCol));
//		return;
//	}
//// 	// 处理Ctrl+X事件
//// 	if (GetKeyState(VK_CONTROL) < 0 && nChar == 'X')
//// 	{
//// 		m_bClipboard = TRUE;
//// 		m_cmbItem.SetWindowText(GetItemText(m_nRow, m_nCol));
//// 		DeleteItemText();
//// 		return;
//// 	}
//	// 处理Ctrl+V事件
//	if (GetKeyState(VK_CONTROL) < 0 && nChar == 'V')
//	{
//		if (m_bClipboard)
//			FinishEditItem();
//		return;
//	}
//
//	switch (nChar)
//	{
//	case VK_SHIFT:
//	case VK_CONTROL:
//	case VK_CAPITAL:
//	case VK_INSERT:
//	case VK_NUMLOCK:
//		break;
//	case VK_HOME:
//	case VK_END:
//	case VK_DELETE:
//		bUsefulKey = TRUE;
//		break;
//	case VK_F2:
//		StartEditItem();
//		bUsefulKey = TRUE;
//		break;
//	case VK_LEFT:
//		bUsefulKey = MoveLeft();
//		break;
//	case VK_RIGHT:
//		bUsefulKey = MoveRight();
//		break;
//	case VK_UP:
//		bUsefulKey = MoveUp();
//		break;
//	case VK_DOWN:
//		bUsefulKey = MoveDown();
//		break;
//	default:
//		break;
//	}
//
//	if (!bUsefulKey)
//		CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
//}
//
//void CUserPropertyGridCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	nRepCnt;
//	nFlags;
//	if (nChar >= 32 && nChar <= 126)
//	{
//		CString str;
//		str.AppendChar((TCHAR)nChar);
//		StartEditItem(str, TRUE);
//	}
//}
//
//void CUserPropertyGridCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
//{
//	LVHITTESTINFO hi;
//	hi.pt = point;
//	if(SubItemHitTest(&hi) != -1)
//	{
//		m_nRow = hi.iItem;
//		m_nCol = hi.iSubItem;
//		StartEditItem();
//	}
//	CListCtrl::OnLButtonDblClk(nFlags, point);
//}
//
//void CUserPropertyGridCtrl::OnKillFocus(CWnd *pNewWnd)
//{
//	HWND hSubCtrlWnd = NULL, hNewWnd = NULL;
//	if (m_subCtrl.nType == LIECT_EDITCOMBOBOX)
//	{
//		if (m_subCtrl.pCtrl != NULL)
//			hSubCtrlWnd = ::GetWindow(m_subCtrl.pCtrl->m_hWnd, GW_CHILD);
//		if (pNewWnd != NULL)
//			hNewWnd = ::GetWindow(pNewWnd->m_hWnd, GW_CHILD);
//	}
//	else
//	{
//		if (m_subCtrl.pCtrl != NULL)
//			hSubCtrlWnd = m_subCtrl.pCtrl->m_hWnd;
//		if (pNewWnd != NULL)
//			hNewWnd = pNewWnd->m_hWnd;
//	}
//
//	if (hSubCtrlWnd == hNewWnd)
//		return;
//
//	if (m_bEdit)
//		FinishEditItem();
//}
//

void CUserPropertyGridCtrl::OnBtnClickedPlus(UINT nBtnID)
{
	int nItem = 0;
	nItem = nBtnID - IDC_FIRSTBTN_PLUS_ID;
	CUserPropertyGridProperty* pProp = (CUserPropertyGridProperty*)GetProperty(nItem);
	COleVariant var;
	var = pProp->GetValue();
	var.ChangeType(VT_I4);
	var.llVal += pProp->m_nStep;
	pProp->SetValue(var);
}

void CUserPropertyGridCtrl::OnBtnClickedMinus(UINT nBtnID)
{
	int nItem = 0;
	nItem = nBtnID - IDC_FIRSTBTN_MINUS_ID;
	CUserPropertyGridProperty* pProp = (CUserPropertyGridProperty*)GetProperty(nItem);
	COleVariant var;
	var = pProp->GetValue();
	var.ChangeType(VT_I4);
	var.llVal -= pProp->m_nStep;
	pProp->SetValue(var);
}

void CUserPropertyGridCtrl::OnBtnClickedSave(UINT nBtnID)
{
	int nItem = 0;
	nItem = nBtnID - IDC_FIRSTBTN_SAVE_ID;
	CMFCPropertyGridProperty* pProp = this->GetProperty(nItem);
	COleVariant var;
	var = pProp->GetValue();
	var.ChangeType(VT_I4);
	this->GetParent()->SendMessage(ID_CMD_SAVE_VALUE, pProp->GetData(), var.lVal);
}