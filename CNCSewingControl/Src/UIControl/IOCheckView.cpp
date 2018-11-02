//===================================================================================================
// Summary:
//		IO检测视图类
// Date:
//		2018-07-07
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#include "stdafx.h"

#include "IOCheckView.h"
#include "Resource.h"
#include "MainFrm.h"
#include "CNCSewingControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CIOCheckView

CIOCheckView::CIOCheckView()
{
	InitData();
	m_bTimerStart = FALSE;
}

CIOCheckView::~CIOCheckView()
{
	
}

BEGIN_MESSAGE_MAP(CIOCheckView, CDockablePane)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_CBN_SELCHANGE(IDC_CMB_IOCHECK, OnIOCheckSelChange)
	ON_WM_TIMER()
	//ON_COMMAND(ID_CMD_SEARCH, OnCmdSearch)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIOCheckView 消息处理程序

int CIOCheckView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建组合:
	const DWORD dwCombStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndIOCombo.Create(dwCombStyle, rectDummy, this, IDC_CMB_IOCHECK))
	{
		TRACE0("未能创建输入输出下拉框 \n");
		return -1;
	}

	m_wndIOCombo.AddString(_T("输入测试"));
	m_wndIOCombo.AddString(_T("输出测试"));
	m_wndIOCombo.SetFont(CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT)));
	m_wndIOCombo.SetCurSel(1);

	//// 创建工具栏
	//m_wndSearchBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SEARCH);
	//m_wndSearchBar.LoadToolBar(IDR_SEARCH, 0, 0, TRUE);
	//m_wndSearchBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	//m_wndSearchBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	//m_wndSearchBar.SetOwner(this);
	//m_wndSearchBar.SetRouteCommandsViaFrame(FALSE);

	// 设置输入控件的宽度
	//m_wndSearchBar.SetButtonInfo(1, ID_PLACE_HOLDER, TBBS_SEPARATOR, 160);
	//// 设置控件的位置
	//CRect rect;
	//m_wndSearchBar.GetItemRect(1, &rect);
	//rect.bottom += 50;
	//m_wndSeartchCtrl.Create(WS_VISIBLE|WS_TABSTOP|CBS_AUTOHSCROLL,
	//	rect, &m_wndSearchBar, IDC_EDIT_SEARCH);
	//m_wndSeartchCtrl.SetFont(CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT)));

	// 创建列表
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	if (!m_ctrlItemList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("未能创建属性网格\n");
		return -1;      // 未能创建
	}
	SetSewingInfoListFont();

	// 隐藏表头
	m_ctrlItemList.EnableHeaderCtrl(FALSE);
	// 显示提示区
	m_ctrlItemList.EnableDescriptionArea();
	m_ctrlItemList.SetVSDotNetLook();
	m_ctrlItemList.MarkModifiedProperties();
	m_ctrlItemList.SetBoolLabels(L"ON", L"OFF");

	// 读取参数XML
	CParaXMLParse xmlInput, xmlOutput;
	xmlInput.ReadInputCheckList(m_arrInputCheck);
	xmlOutput.ReadOutputCheckList(m_arrOutputCheck);

	SetListOutputCheck();

	AdjustLayout();

	return 0;
}

void CIOCheckView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CIOCheckView::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case ID_TIMER_IOCHECK:
		{
			this->GetParent()->SendMessage(ID_CMD_REQUEST_INPUTCHECK, 0, 0);
		}
		break;
	default:
		break;
	}

	CWnd::OnTimer(nIDEvent);
}

void CIOCheckView::OnDestroy()
{
	EndTimer();
	CWnd::OnDestroy();
}

// 输入检测列表
void CIOCheckView::SetListInputCheck()
{
	m_ctrlItemList.RemoveAll();

	CMFCPropertyGridProperty* pItem = NULL;
	CString strText, strTips;
	for (int i=0; i<(int)m_arrInputCheck.size(); i++)
	{
		// 过滤待定的检查项
		if (m_arrInputCheck[i].nMode == 0)
			continue;

		strText.Format(L"%02d   %s", m_arrInputCheck[i].nID, m_arrInputCheck[i].strDesc);
		COleVariant varCurrent;
		if (m_arrInputCheck[i].nType == 2)
		{
			varCurrent = (_variant_t) m_arrInputCheck[i].nState;
			varCurrent.ChangeType(VT_I4);
			pItem = new CUDFPropertyGridProperty(strText, varCurrent, strTips);
			pItem->SetData(i);
			pItem->AllowEdit(FALSE);
		}
		else
		{
			varCurrent = (_variant_t) (m_arrInputCheck[i].nState == 1);
			varCurrent.ChangeType(VT_BOOL);
			pItem = new CUDFPropertyGridProperty(strText, (_variant_t) varCurrent, strTips);
			pItem->SetData(i);
		}
		m_ctrlItemList.AddProperty(pItem);
	}
}

// 输出检测列表
void CIOCheckView::SetListOutputCheck()
{
	m_ctrlItemList.RemoveAll();

	CMFCPropertyGridProperty* pItem = NULL;
	CString strText, strTips;
	for (int i=0; i<(int)m_arrOutputCheck.size(); i++)
	{
		// 过滤待定的检查项
		if (m_arrOutputCheck[i].nMode == 0)
			continue;

		strText.Format(L"%02d   %s", m_arrOutputCheck[i].nID, m_arrOutputCheck[i].strDesc);
		COleVariant varCurrent;
		if (m_arrOutputCheck[i].nType == 2)
		{
			varCurrent = (_variant_t) m_arrOutputCheck[i].nState;
			varCurrent.ChangeType(VT_I4);
			pItem = new CUDFPropertyGridProperty(strText, varCurrent, strTips);
			pItem->SetData(i);
			if (m_arrOutputCheck[i].nMode == 10)
			{
				pItem->EnableSpinControl(TRUE);
			}
		}
		else
		{
			varCurrent = (_variant_t) (m_arrOutputCheck[i].nState == 1);
			varCurrent.ChangeType(VT_BOOL);
			pItem = new CUDFPropertyGridProperty(strText, (_variant_t) varCurrent, strTips);
			pItem->SetData(i);
		}
		m_ctrlItemList.AddProperty(pItem);
	}
}

void CIOCheckView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_ctrlItemList.SetFocus();
}

void CIOCheckView::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetSewingInfoListFont();
}

// 设置信息列表字体
void CIOCheckView::SetSewingInfoListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = theApp.m_nFontHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndIOCombo.SetFont(&m_fntPropList);
	m_ctrlItemList.SetFont(&m_fntPropList);
	//m_wndSeartchCtrl.SetFont(&m_fntPropList);
}

// 调整布局
void CIOCheckView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);
	m_wndIOCombo.GetWindowRect(&rectCombo);

	int cyCmb = rectCombo.Size().cy;

	int cyToolbar = 0;
	//cyToolbar = m_wndSearchBar.CalcFixedLayout(FALSE, TRUE).cy;

	/*CRect rectSearch;
	m_wndSearchBar.GetItemRect(0, &rectSearch);
	m_wndSeartchCtrl.SetWindowPos(NULL, rectClient.left, rectClient.top+2, 160, cyToolbar-6, SWP_NOACTIVATE | SWP_NOZORDER);*/

	m_wndIOCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER);
	//m_wndSearchBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyToolbar, SWP_NOACTIVATE | SWP_NOZORDER);
	m_ctrlItemList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyToolbar, rectClient.Width(), rectClient.Height() -(cyCmb+cyToolbar), SWP_NOACTIVATE | SWP_NOZORDER);
}

void CIOCheckView::OnIOCheckSelChange()
{
	int index = m_wndIOCombo.GetCurSel(); 
	if (index == 0)
	{
		StartTimer();
		SetListInputCheck();
	}
	else
	{
		EndTimer();
		SetListOutputCheck();
	}

	m_ctrlItemList.SetFocus();
}

//void CIOCheckView::OnCmdSearch()
//{
//	CString strKeyword;
//	m_wndSeartchCtrl.GetWindowText(strKeyword);
//
//	int nCount = m_ctrlItemList.GetPropertyCount();
//	for (int i=0; i<nCount; i++)
//	{
//		CMFCPropertyGridProperty* pProps = m_ctrlItemList.GetProperty(i);
//		if (pProps != NULL)
//		{
//			CString strPropName = pProps->GetName();
//			if (strPropName.Find(strKeyword) >= 0)
//			{
//				m_ctrlItemList.SetCurSel(pProps);
//				m_ctrlItemList.EnsureVisible(pProps);
//				break;
//			}
//		}
//	}
//}

void CIOCheckView::InitData()
{
	
}

void CIOCheckView::StartTimer()
{
	if (m_wndIOCombo.GetCurSel() == 0 && !m_bTimerStart)
	{
		int nSec = GetConfigOptionINT(L"IOCHECK", L"IOFRESH")*1000;	// 获取刷新间隔秒数
		SetTimer(ID_TIMER_IOCHECK, nSec, 0);
		m_bTimerStart = TRUE;
	}
}

void CIOCheckView::EndTimer()
{
	if (m_bTimerStart)
	{
		KillTimer(ID_TIMER_IOCHECK);
		m_bTimerStart = FALSE;
	}
}

void CIOCheckView::ReceiveIOData(const vector<int>& arrBin)
{
	if (m_wndIOCombo.GetCurSel() == 1)
		return;

	vector<int> arrTemp = arrBin;
	for (int i=0; i<arrBin.size(); i++)
	{
		for (int j=0; j<m_arrInputCheck.size(); j++)
		{
			if (m_arrInputCheck[j].nMode != 1)
				continue;

			if (m_arrInputCheck[j].nBit==i+1)
			{
				m_arrInputCheck[j].nState = arrBin[i];
				break;
			}
		}
	}

	// 更新列表
	UpdateInputList();
}

// 输入检测列表
void CIOCheckView::UpdateInputList()
{
	CMFCPropertyGridProperty* pItem = NULL;
	for (int i=0; i<m_ctrlItemList.GetPropertyCount(); i++)
	{
		pItem = m_ctrlItemList.GetProperty(i);
		int nIndex = (int)pItem->GetData();
		COleVariant varCurrent;
		if (m_arrInputCheck[nIndex].nType == 2)
		{
			varCurrent = (_variant_t) m_arrInputCheck[nIndex].nState;
			varCurrent.ChangeType(VT_I4);
			pItem->SetValue(varCurrent);
		}
		else
		{
			varCurrent = (_variant_t) (m_arrInputCheck[nIndex].nState == 1);
			varCurrent.ChangeType(VT_BOOL);
			pItem->SetValue(varCurrent);
		}
		pItem = NULL;
	}
}

// 属性修改
LRESULT CIOCheckView::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	if (m_wndIOCombo.GetCurSel() == 1)
	{
		CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*) lParam;
		CString sname = pProp->GetName();  //被改变的参数名
		COleVariant val = pProp->GetValue(); //改变之后的值
		int nIndex = (int)pProp->GetData();
		if (m_arrOutputCheck[nIndex].nType == 2)
		{
			val.ChangeType(VT_I4);
			m_arrOutputCheck[nIndex].nStep = val.iVal - m_arrOutputCheck[nIndex].nState;
			m_arrOutputCheck[nIndex].nState = val.iVal;
		}
		else
		{
			val.ChangeType(VT_BOOL);
			if (val.bVal)
				m_arrOutputCheck[nIndex].nState = 1;
			else
				m_arrOutputCheck[nIndex].nState = 0;
		}
		this->GetParent()->SendMessage(ID_CMD_REQUEST_OUTPUTCHECK, 0, (LPARAM)&m_arrOutputCheck[nIndex]);
	}
	return 0;
}