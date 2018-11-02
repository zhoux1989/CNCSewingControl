//===================================================================================================
// Summary:
//		������Ϣ��ͼ��
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
			// ��ȡ���Ե�����
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
// CSewingInfoView ��Ϣ�������

int CSewingInfoView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// �������:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndInfoList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("δ�ܴ�����������\n");
		return -1;      // δ�ܴ���
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

// ��ʼ��������Ϣ�б�
void CSewingInfoView::InitSewingInfoList()
{
	SetSewingInfoListFont();
	// ���ر�ͷ
	m_wndInfoList.EnableHeaderCtrl(FALSE);
	// ��ʾ��ʾ��
	m_wndInfoList.EnableDescriptionArea();
	m_wndInfoList.SetVSDotNetLook();
	m_wndInfoList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("���һ���Ϣ"));
	CUserPropertyGridProperty* pItem = NULL;

	COleVariant var;
	var = (_variant_t)0;
	var.ChangeType(VT_I4);
	pItem = new CUserPropertyGridProperty(_T("�����ٶ�(ת/��)"), var, _T("˫�����������ٶ�"));
	pItem->m_nStep = 100;
	pItem->SetData(0);
	pItem->AllowEdit(FALSE);
	m_wndInfoList.AddProperty(pItem);

	pItem = new CUserPropertyGridProperty(_T("���߼�����"), var, _T("˫�����õ��߼�����"));
	pItem->SetData(1);
	pItem->AllowEdit(FALSE);
	m_wndInfoList.AddProperty(pItem);

	pItem = new CUserPropertyGridProperty(_T("����������"), var, _T("˫�����ù���������"));
	pItem->SetData(2);
	pItem->AllowEdit(FALSE);
	m_wndInfoList.AddProperty(pItem);

	pItem = new CUserPropertyGridProperty(_T("��ѹ�Ÿ߶�(��λ0.1mm)"), var, _T("˫��������ѹ�Ÿ߶�"));
	pItem->SetData(3);
	pItem->AllowEdit(FALSE);
	m_wndInfoList.AddProperty(pItem);

	pItem = new CUserPropertyGridProperty(_T("������A"), var, _T("˫������������A"));
	pItem->SetData(4);
	pItem->AllowEdit(FALSE);
	m_wndInfoList.AddProperty(pItem);

	pItem = new CUserPropertyGridProperty(_T("������B"), var, _T("˫������������B"));
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

// ������Ϣ�б�����
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

// ������Ϣ
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

// ��ʼ��
void CSewingInfoView::InitData()
{
}

LRESULT CSewingInfoView::OnSaveValue(WPARAM wParam, LPARAM lParam)
{
	this->GetParent()->SendMessage(ID_CMD_SET_SEWINGINFO, wParam, lParam);
	return NULL;
}
