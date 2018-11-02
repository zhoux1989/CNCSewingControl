//===================================================================================================
// Summary:
//		�����ļ���ͼ��
// Date:
//		2018-07-07
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#include "stdafx.h"
#include "MainFrm.h"
#include "SewFileView.h"
#include "Resource.h"
#include "CNCSewingControl.h"

class CClassViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CSewFileView;

	DECLARE_SERIAL(CClassViewMenuButton)

public:
	CClassViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// ����/����
//////////////////////////////////////////////////////////////////////

CSewFileView::CSewFileView()
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;

	InitData();
}

CSewFileView::~CSewFileView()
{
}

BEGIN_MESSAGE_MAP(CSewFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILE, OnItemdblclickSewList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FILE, OnItemchangedList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView ��Ϣ�������

int CSewFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ��ʼ���ļ��б�
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | LVS_ICON | LVS_SINGLESEL | LVS_ALIGNTOP | LVS_AUTOARRANGE | WS_TABSTOP;
	if (!m_ctrlSewFileList.Create(dwViewStyle, CRect(0, 0, 0, 0), this, IDC_LIST_FILE))
	{
		TRACE0("δ�ܴ�����ͼ\n");
		return -1;
	}
	SetFont();

	// ��ʼ���б�ͼ��
	if (m_listImages.GetSafeHandle() == NULL )
	{
		m_listImages.Create(48, 48, ILC_MASK|ILC_COLOR32, 0, 0);
		m_listImages.Add(AfxGetApp()->LoadIcon(IDI_FILE_MANAGE));
		m_listImages.Add(AfxGetApp()->LoadIcon(IDI_FILE_SELECTED));
		m_ctrlSewFileList.SetImageList(&m_listImages, LVSIL_NORMAL);
	}

	// ����ͼ��:
	//m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	//m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* ������*/);

	//OnChangeVisualStyle();

	//m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	//m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	//m_wndToolBar.SetOwner(this);

	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��:
	//m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	/*CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, CClassViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}*/

	// ����һЩ��̬����ͼ����(�˴�ֻ������������룬�����Ǹ��ӵ�����)
	//FillClassView();

	return 0;
}

void CSewFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CSewFileView::OnItemdblclickSewList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	POSITION pos = m_ctrlSewFileList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int nItem = m_ctrlSewFileList.GetNextSelectedItem(pos);
		if (nItem != -1)
		{
			SetCurrSewFile(nItem);

			// �������õ�ǰ������ָ��
			int nSewID = (int)m_ctrlSewFileList.GetItemData(nItem);
			this->GetParent()->SendMessage(ID_CMD_SET_CURRENTSEW, nSewID);
		}
	}

	*pResult = 0;
}

// ����б�����
void CSewFileView::FillSewFileList()
{
	CString strFile;
	m_ctrlSewFileList.DeleteAllItems();
	int nIndex = -1;
	for (int i=0; i<m_arrFileList.size(); i++)
	{
		strFile.Format(L"No.%d", m_arrFileList[i].nID);

		m_ctrlSewFileList.InsertItem(i, strFile);
		m_ctrlSewFileList.SetItemData(i, m_arrFileList[i].nID);
		if (m_arrFileList[i].nID == m_nCurrFileID)
		{
			nIndex = i;
		}
	}

	if (nIndex!=m_nMarkedItem && nIndex>=0)
	{
		SetCurrSewFile(nIndex);
	}
}

void CSewFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	//CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
	//ASSERT_VALID(pWndTree);

	//if (pWnd != pWndTree)
	//{
	//	CDockablePane::OnContextMenu(pWnd, point);
	//	return;
	//}

	//if (point != CPoint(-1, -1))
	//{
	//	// ѡ���ѵ�������:
	//	CPoint ptTree = point;
	//	pWndTree->ScreenToClient(&ptTree);

	//	UINT flags = 0;
	//	HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
	//	if (hTreeItem != NULL)
	//	{
	//		pWndTree->SelectItem(hTreeItem);
	//	}
	//}

	//pWndTree->SetFocus();
	//CMenu menu;
	//menu.LoadMenu(IDR_POPUP_SORT);

	//CMenu* pSumMenu = menu.GetSubMenu(0);

	//if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	//{
	//	CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

	//	if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
	//		return;

	//	((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
	//	UpdateDialogControls(this, FALSE);
	//}
}

void CSewFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = 0;
	//cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	//m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_ctrlSewFileList.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CSewFileView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CSewFileView::OnNewFolder()
{
	AfxMessageBox(_T("�½��ļ���..."));
}

void CSewFileView::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	CRect rectTree;
	m_ctrlSewFileList.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CSewFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_ctrlSewFileList.SetFocus();
}

// ���õ�ǰ����
void CSewFileView::SetCurrSewFile(int nItem)
{
	if (nItem == m_nMarkedItem)
		return;

	if (m_nMarkedItem >= 0)
	{
		LVITEM itemOld;
		itemOld.iItem = m_nMarkedItem;
		itemOld.mask = LVIF_IMAGE;
		itemOld.iImage = 0;
		m_ctrlSewFileList.SetItem(&itemOld);
	}

	LVITEM itemNew;
	itemNew.iItem = nItem;
	itemNew.mask = LVIF_IMAGE;
	itemNew.iImage = 1;
	m_ctrlSewFileList.SetItem(&itemNew);

	m_nMarkedItem = nItem;
}

// ����ļ����б���
void CSewFileView::UpdateFileList(SewFileDataArray& arr, BOOL bFinished)
{
	if (m_nIndexFlag == -1)
	{
		m_arrFileList.clear();
	}
	m_nIndexFlag += 1;

	for (int i=0; i<arr.size(); i++)
	{
		m_arrFileList.push_back(arr[i]);
	}

	if (bFinished)
	{
		m_nIndexFlag = -1;

		// ���µ��б�ؼ���
		FillSewFileList();
	}
}

// ���µ�ǰ�����ļ�
void CSewFileView::UpdateCurrSewFile(int nSewID)
{
	int nData;
	for (int i=0; i<m_ctrlSewFileList.GetItemCount(); i++)
	{
		nData = (int)m_ctrlSewFileList.GetItemData(i);

		if (nData == nSewID)
		{
			SetCurrSewFile(i);
		}
	}
}

// ��ʼ��
void CSewFileView::InitData()
{
	m_nMarkedItem = -1;
	m_arrFileList.clear();
	m_nIndexFlag = -1;
	m_nCurrFileID = -1;
	m_nSelIndex = -1;
}

// ������Ϣ�б�����
void CSewFileView::SetFont()
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

	m_ctrlSewFileList.SetFont(&m_fntPropList);
}

// ѡ���ļ�ʱԤ���ļ�
void CSewFileView::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->uChanged == LVIF_STATE)
	{
		CMainFrame* pFrm = (CMainFrame*)this->GetParent();
		if(pNMListView->uNewState & LVIS_SELECTED)
		{
			//��������
			int nItem = pNMListView->iItem;
			int nID = (int)m_ctrlSewFileList.GetItemData(nItem);
			m_nSelIndex = nItem;

			pFrm->m_trailToPreview.strDesc = m_ctrlSewFileList.GetItemText(nItem, 0);
			if (m_arrFileList[nItem].arrByte.empty())
			{
				this->GetParent()->SendMessage(ID_CMD_PREVIEW_SEW, PS_PREPARE);
				pFrm->GetPreviewFileData(nID);
				// �ȴ���������
			}
			else
			{
				pFrm->SendMessage(ID_CMD_PREVIEW_SEW, PS_RECEIVE, (LPARAM)&m_arrFileList[nItem].arrByte);
			}
		}
		else
		{
			m_nSelIndex = -1;
			pFrm->PreviewCurrentSew();
		}
	}
	*pResult = 0;
}

// ���ն�ȡ������
void CSewFileView::ReceiveSewfileData(ByteArray& arrByte)
{
	if (m_nSelIndex >= 0)
	{
		m_arrFileList[m_nSelIndex].arrByte = arrByte;
		CMainFrame* pFrm = (CMainFrame*)this->GetParent();
		pFrm->SendMessage(ID_CMD_PREVIEW_SEW, PS_RECEIVE, (LPARAM)&arrByte);
	}
}

// ���ļ���ӵ��б�
void CSewFileView::AddFileToList(const SewFileDataArray& arr)
{
	for (int i=0; i<arr.size(); i++)
	{
		m_arrFileList.push_back(arr[i]);
	}
	FillSewFileList();
}