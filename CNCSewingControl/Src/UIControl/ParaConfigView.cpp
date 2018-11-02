//===================================================================================================
// Summary:
//		参数设置视图类
// Date:
//		2018-07-09
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#include "stdafx.h"

#include "ParaConfigView.h"
#include "Resource.h"
#include "MainFrm.h"
#include "CNCSewingControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//===================================================================================================

CUDFPropertyGridProperty::CUDFPropertyGridProperty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr):CMFCPropertyGridProperty(strName, varValue, lpszDescr)
{
}

void CUDFPropertyGridProperty::OnDrawValue(CDC* pDC, CRect rect)
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

CParaConfigView::CParaConfigView()
{
	InitData();
}

CParaConfigView::~CParaConfigView()
{
}

BEGIN_MESSAGE_MAP(CParaConfigView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_CMD_SAVE, OnCmdSave)
	ON_COMMAND(ID_CMD_SEARCH, OnCmdSearch)
	ON_COMMAND(ID_CMD_RETURN, OnCmdReturn)
	ON_COMMAND(ID_CMD_REFRESH, OnRefresh)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_UPDATE_COMMAND_UI_RANGE(ID_CMD_SAVE, ID_CMD_SAVE, OnUpdateValid)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipsNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

int CParaConfigView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 初始化工具栏
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PARACONFIG);
	m_wndToolBar.LoadToolBar(IDR_PARACONFIG, 0, 0, TRUE);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);
	m_wndToolBar.EnableToolTips(TRUE);

	// 初始化搜索输入框
	m_wndToolBar.SetButtonInfo(3, ID_PLACE_HOLDER, TBBS_SEPARATOR, 160);
	CRect rectBar;
	m_wndToolBar.GetItemRect(3, &rectBar);
	rectBar.bottom += 50;
	m_editSearch.Create(WS_VISIBLE|WS_TABSTOP|CBS_AUTOHSCROLL,
		rectBar, &m_wndToolBar, IDC_EDIT_SEARCH);

	afxGlobalData.m_nMaxToolTipWidth = 5;

	// 创建列表
	if (!m_ctrlParaList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("未能创建属性网格\n");
		return -1;
	}

	// 隐藏表头
	m_ctrlParaList.EnableHeaderCtrl(FALSE);
	// 显示提示区
	m_ctrlParaList.EnableDescriptionArea();
	m_ctrlParaList.SetVSDotNetLook();
	m_ctrlParaList.MarkModifiedProperties();
	m_ctrlParaList.SetBoolLabels(L"ON", L"OFF");

	// 设置视图控件的字体
	SetViewCtrlFont();

	// 读取参数XML
	CParaXMLParse xml;
	if (xml.ReadParamList(m_mapParaInfo))
	{
		// 显示参数列表
		//SetParaList();
	}

	// 调整窗口布局
	AdjustLayout();
	return 0;
}

void CParaConfigView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CParaConfigView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectClient;
	this->GetClientRect(rectClient);

	CBrush brush;
	brush.CreateSolidBrush(COLORREF(RGB(255, 255, 255)));
	dc.FillRect(&rectClient,&brush);
	brush.DeleteObject();

	CDockablePane::OnPaint();

	CFont newFont;
	newFont.CreatePointFont(400, L"华文彩云", &dc);
	CFont* poldFont = dc.SelectObject(&newFont);
	dc.SetTextColor(RGB(125, 125, 125));
	dc.TextOut(rectClient.Width()/2-160, rectClient.Height()/2-46, L"参数读取中...");
	dc.SelectObject(poldFont);
	newFont.DeleteObject();
}

void CParaConfigView::OnUpdateValid(CCmdUI* pCmdUI)
{
	if (pCmdUI->m_nID == ID_SEW_CONNECT)
	{
		if (m_paraToUpdate.size() > 0)
		{
			pCmdUI->Enable(FALSE);
		}
		else
		{
			pCmdUI->Enable(TRUE);
		}
	}
}

BOOL CParaConfigView::OnToolTipsNotify(UINT id, NMHDR* pNMHDR,LRESULT* pResult)
{
	TOOLTIPTEXT* pT =(TOOLTIPTEXT*)pNMHDR;
	UINT nID = (int)pNMHDR->idFrom;
	if (nID)
	{
		//UINT nIndex = m_wndToolBar.CommandToIndex(nID);
		if(nID == 1)
		{
			pT->lpszText = L"保存修改参数项";
		}
		else if(nID == 2)
		{
			pT->lpszText = L"将选择项还原为默认值";
		}
		else if(nID == 4)
		{
			pT->lpszText = L"更新参数列表";
		}
		else if(nID == 6)
		{
			pT->lpszText = L"按编号或名称搜索";
		}
	}
	return FALSE;
}

// 参数列表
void CParaConfigView::SetParaList()
{
	m_ctrlParaList.RemoveAll();

	CUDFPropertyGridProperty* pItem = NULL;
	CString strText, strTips;
	for (int i=0; i<(int)m_paraList.size(); i++)
	{
		m_paraList[i].strParaName = m_mapParaInfo[m_paraList[i].nID].strParaName;
		m_paraList[i].strDescription = m_mapParaInfo[m_paraList[i].nID].strDescription;
		//m_paraList[i].nDefaultValue = m_mapParaInfo[m_paraList[i].nID].nDefaultValue;

		strText.Format(L"%03d   %s", m_paraList[i].nID, m_paraList[i].strParaName);
		strTips = m_paraList[i].strDescription;
		COleVariant varOrigin, varCurrent;
		if (m_paraList[i].nType == 2)
		{
			varOrigin = (_variant_t) m_paraList[i].nDefaultValue;
			varOrigin.ChangeType(VT_I4);
			varCurrent = (_variant_t) m_paraList[i].nCurrentValue;
			varCurrent.ChangeType(VT_I4);
			pItem = new CUDFPropertyGridProperty(strText, varCurrent, strTips);
			pItem->SetOriginalValue(varOrigin);	// 设置默认值
			pItem->EnableSpinControl(TRUE, m_paraList[i].nMinValue, m_paraList[i].nMaxValue);
			pItem->SetData(i);
		}
		else
		{
			varOrigin = (_variant_t) (m_paraList[i].nCurrentValue == 1);
			varOrigin.ChangeType(VT_BOOL);
			varCurrent = (_variant_t) (m_paraList[i].nDefaultValue == 1);
			varCurrent.ChangeType(VT_BOOL);
			pItem = new CUDFPropertyGridProperty(strText, (_variant_t) varOrigin, strTips);
			pItem->SetOriginalValue((_variant_t) varCurrent);	// 设置默认值
			pItem->SetData(i);
		}
		m_ctrlParaList.AddProperty(pItem);
	}
}

void CParaConfigView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_ctrlParaList.SetFocus();
}

void CParaConfigView::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetViewCtrlFont();
}

// 设置字体
void CParaConfigView::SetViewCtrlFont()
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

	m_ctrlParaList.SetFont(&m_fntPropList);
	m_editSearch.SetFont(&m_fntPropList);
}

// 调整布局
void CParaConfigView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);
	int cyCmb = 0;

	int cyToolbar = 0;
	cyToolbar = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	//CRect rectSearch;
	//m_wndToolBar.GetItemRect(3, &rectSearch);
	//m_editSearch.SetWindowPos(NULL, rectSearch.left, rectSearch.top+2, 160, cyToolbar-6, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyToolbar, SWP_NOACTIVATE | SWP_NOZORDER);
	m_ctrlParaList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyToolbar, rectClient.Width(), rectClient.Height() -(cyCmb+cyToolbar), SWP_NOACTIVATE | SWP_NOZORDER);

	HDITEM item;
	item.cxy=310;
	item.mask=HDI_WIDTH;
	m_ctrlParaList.GetHeaderCtrl().SetItem(0, new HDITEM(item));
}

void CParaConfigView::OnCmdSearch()
{
	CString strKeyword;
	m_editSearch.GetWindowText(strKeyword);

	int nCount = m_ctrlParaList.GetPropertyCount();
	for (int i=0; i<nCount; i++)
	{
		CMFCPropertyGridProperty* pProps = m_ctrlParaList.GetProperty(i);
		if (pProps != NULL)
		{
			CString strPropName = pProps->GetName();
			if (strPropName.Find(strKeyword) >= 0)
			{
				m_ctrlParaList.SetCurSel(pProps);
				m_ctrlParaList.EnsureVisible(pProps);
				break;
			}
		}
	}
}

void CParaConfigView::OnCmdSave()
{
	if (m_paraToUpdate.size() > 0)
	{
		this->GetParent()->SendMessage(ID_CMD_SET_PARAM, 0, (LPARAM)&m_paraToUpdate);
	}
}

void CParaConfigView::OnCmdReturn()
{
	CMFCPropertyGridProperty* pProps = m_ctrlParaList.GetCurSel();
	if (pProps != NULL)
	{
		pProps->ResetOriginalValue();
	}
}

// 属性修改
LRESULT CParaConfigView::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*) lParam;
	CString sname = pProp->GetName();  //被改变的参数名
	COleVariant val = pProp->GetValue(); //改变之后的值
	
	int nIndex = (int)pProp->GetData();

	if (m_paraList[nIndex].nType == 2)
	{
		val.ChangeType(VT_I4);
		m_paraList[nIndex].nCurrentValue = val.iVal;
	}
	else
	{
		val.ChangeType(VT_BOOL);
		m_paraList[nIndex].nCurrentValue = val.bVal;
	}

	int nPos = -1;
	for (int i=0; i<m_paraToUpdate.size(); i++)
	{
		if (m_paraToUpdate[i].nID == m_paraList[nIndex].nID)
		{
			nPos = i;
			break;
		}
	}
	if (nPos == -1)
	{
		m_paraToUpdate.push_back(m_paraList[nIndex]);
	}
	else
	{
		m_paraToUpdate[nPos].nCurrentValue = m_paraList[nIndex].nCurrentValue;
	}
	
	return 0;
}

// 刷新
void CParaConfigView::OnRefresh()
{
	InitData();
	this->GetParent()->SendMessage(ID_CMD_GET_PARAMLIST, 0, 0);
}

// 修改视图状态
void CParaConfigView::SetViewState(BOOL bStartUpdate)
{
	if (bStartUpdate)
	{
		m_ctrlParaList.ShowWindow(SW_HIDE);
		//EnableWindow(FALSE);
	}
	else
	{
		m_ctrlParaList.ShowWindow(SW_SHOW);
		//EnableWindow(TRUE);
	}
}

// 更新参数
int CParaConfigView::UpdataParam(ParaInfo para, BOOL bEndFlag)
{
	if (bEndFlag)
	{
		m_nParamInUpdate = -1;

		// 更新参数列表控件
		SetParaList();
	}
	else
	{
		if (m_nParamInUpdate == -1)
		{
			m_paraList.clear();
		}
		m_nParamInUpdate += 1;
		
		if (para.nType != 0)	// 过滤无效参数
			m_paraList.push_back(para);
	}
	return m_nParamInUpdate+1;
}

// 初始化
void CParaConfigView::InitData()
{
	m_nParamInUpdate = -1;
	m_paraList.clear();
	m_paraToUpdate.clear();
}

void CParaConfigView::SetSuccess()
{
	m_paraToUpdate.clear();
}