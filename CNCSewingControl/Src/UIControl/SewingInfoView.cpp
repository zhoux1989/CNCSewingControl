//===================================================================================================
// Summary:
//		缝纫信息视图类
// Date:
//		2018-07-07
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#include "stdafx.h"

#include "SewingInfoView.h"
#include "Resource.h"
#include "MainFrm.h"
#include "CNCSewingControl.h"
#include "DlgValueSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDoubleClickedPropertyCtrl

BEGIN_MESSAGE_MAP(CDoubleClickedPropertyCtrl, CMFCPropertyGridCtrl)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

void CDoubleClickedPropertyCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CMFCPropertyGridProperty::ClickArea clickArea;
	CMFCPropertyGridProperty* pHit = HitTest(point, &clickArea);
	switch (clickArea) 
	{ 
	case CMFCPropertyGridProperty::ClickExpandBox:
		break;
	case CMFCPropertyGridProperty::ClickName:
		{
			// 获取属性的名称
			CString strName = pHit->GetName();
			COleVariant var = pHit->GetValue();
			CRect rect;
			this->GetParent()->GetWindowRect(rect);
			//ClientToScreen(rect);
			
			CDlgValueSet dlgValueSet;
			dlgValueSet.InitData(strName, var, rect.right+5, rect.top, 1.0);
			if (dlgValueSet.DoModal() == IDOK)
			{
				pHit->SetValue(dlgValueSet.m_var);
				dlgValueSet.m_var.ChangeType(VT_I4);
				this->GetParent()->GetParent()->SendMessage(ID_CMD_SET_SEWINGINFO, pHit->GetData(), dlgValueSet.m_var.iVal);
			}
		}
		break;
	case CMFCPropertyGridProperty::ClickValue:
		break;
	default:
		break;
	}
	CMFCPropertyGridCtrl::OnLButtonDblClk(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// CSewingInfoView

CSewingInfoView::CSewingInfoView()
{
	InitData();
}

CSewingInfoView::~CSewingInfoView()
{
}

BEGIN_MESSAGE_MAP(CSewingInfoView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_MESSAGE(ID_CMD_SAVE_VALUE, OnSaveValue)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSewingInfoView 消息处理程序

int CSewingInfoView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建组合:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndInfoList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("未能创建属性网格\n");
		return -1;      // 未能创建
	}

	InitSewingInfoList();

	AdjustLayout();

	return 0;
}

void CSewingInfoView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

// 初始化缝纫信息列表
void CSewingInfoView::InitSewingInfoList()
{
	SetSewingInfoListFont();
	// 隐藏表头
	m_wndInfoList.EnableHeaderCtrl(FALSE);
	// 显示提示区
	m_wndInfoList.EnableDescriptionArea();
	m_wndInfoList.SetVSDotNetLook();
	m_wndInfoList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("缝纫机信息"));
	CUserPropertyGridProperty* pItem = NULL;

	COleVariant var;
	var = (_variant_t)0;
	var.ChangeType(VT_I4);
	pItem = new CUserPropertyGridProperty(_T("主轴速度(转/分)"), var, _T("双击设置主轴速度"));
	pItem->m_nStep = 100;
	pItem->SetData(0);
	pItem->AllowEdit(FALSE);
	m_wndInfoList.AddProperty(pItem);

	pItem = new CUserPropertyGridProperty(_T("底线计数器"), var, _T("双击设置底线计数器"));
	pItem->SetData(1);
	pItem->AllowEdit(FALSE);
	m_wndInfoList.AddProperty(pItem);

	pItem = new CUserPropertyGridProperty(_T("工件计数器"), var, _T("双击设置工件计数器"));
	pItem->SetData(2);
	pItem->AllowEdit(FALSE);
	m_wndInfoList.AddProperty(pItem);

	pItem = new CUserPropertyGridProperty(_T("中压脚高度(单位0.1mm)"), var, _T("双击设置中压脚高度"));
	pItem->SetData(3);
	pItem->AllowEdit(FALSE);
	m_wndInfoList.AddProperty(pItem);

	pItem = new CUserPropertyGridProperty(_T("线张力A"), var, _T("双击设置线张力A"));
	pItem->SetData(4);
	pItem->AllowEdit(FALSE);
	m_wndInfoList.AddProperty(pItem);

	pItem = new CUserPropertyGridProperty(_T("线张力B"), var, _T("双击设置线张力B"));
	pItem->SetData(5);
	pItem->AllowEdit(FALSE);
	m_wndInfoList.AddProperty(pItem);

	m_wndInfoList.AddButtonGroup(0, FALSE, TRUE);
	m_wndInfoList.AddButtonGroup(1, FALSE, TRUE);
	m_wndInfoList.AddButtonGroup(2, FALSE, TRUE);
	m_wndInfoList.AddButtonGroup(3, FALSE, TRUE);
	m_wndInfoList.AddButtonGroup(4, FALSE, TRUE);
	m_wndInfoList.AddButtonGroup(5, FALSE, TRUE);
}

void CSewingInfoView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndInfoList.SetFocus();
}

void CSewingInfoView::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetSewingInfoListFont();
}

// 设置信息列表字体
void CSewingInfoView::SetSewingInfoListFont()
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

	m_wndInfoList.SetFont(&m_fntPropList);
}

void CSewingInfoView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	int cyTlb = 0;
	//cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
	//m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndInfoList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() -cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);

	m_wndInfoList.UpdateSubCtrlPos();
}

// 更新信息
void CSewingInfoView::UpdateSewingInfo(SewingInfo infoSewing)
{
	m_infoSewing = infoSewing;
	COleVariant var;

	var = (_variant_t)m_infoSewing.nSpindleSpeed;
	var.ChangeType(VT_I4);
	m_wndInfoList.GetProperty(0)->SetValue(var);

	var = (_variant_t)m_infoSewing.nBottomLineCounter;
	var.ChangeType(VT_I4);
	m_wndInfoList.GetProperty(1)->SetValue(var);

	var = (_variant_t)m_infoSewing.nWorkpiectCounter;
	var.ChangeType(VT_I4);
	m_wndInfoList.GetProperty(2)->SetValue(var);

	var = (_variant_t)m_infoSewing.nMediumLegHeight;
	var.ChangeType(VT_I4);
	m_wndInfoList.GetProperty(3)->SetValue(var);

	var = (_variant_t)m_infoSewing.nLineTensionA;
	var.ChangeType(VT_I4);
	m_wndInfoList.GetProperty(4)->SetValue(var);

	var = (_variant_t)m_infoSewing.nLineTensionB;
	var.ChangeType(VT_I4);
	m_wndInfoList.GetProperty(5)->SetValue(var);
}

// 初始化
void CSewingInfoView::InitData()
{
}

LRESULT CSewingInfoView::OnSaveValue(WPARAM wParam, LPARAM lParam)
{
	this->GetParent()->SendMessage(ID_CMD_SET_SEWINGINFO, wParam, lParam);
	return NULL;
}
