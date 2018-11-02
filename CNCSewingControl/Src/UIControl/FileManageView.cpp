//===================================================================================================
// Summary:
//		�ļ�������ͼ��
// Date:
//		2018-07-09
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#include "stdafx.h"
#include "MainFrm.h"
#include "FileManageView.h"
#include "Resource.h"
#include "CNCSewingControl.h"
#include "DlgFileRename.h"

//////////////////////////////////////////////////////////////////////
// ����/����
//////////////////////////////////////////////////////////////////////

CFileManageView::CFileManageView()
{
	InitData();
}

CFileManageView::~CFileManageView()
{
}

BEGIN_MESSAGE_MAP(CFileManageView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//ON_COMMAND(ID_CMD_COPY, OnFileCopy)
	ON_COMMAND(ID_CMD_DELETE, OnFileDelete)
	ON_COMMAND(ID_CMD_RENAME, OnFileRename)
	ON_COMMAND(ID_CMD_SEARCH, OnFileSearch)
	//ON_COMMAND(ID_CMD_FILECUT, OnFileCut)
	ON_COMMAND(ID_CMD_DOWNLOADSEW, OnFileDownload)
	ON_COMMAND(ID_CMD_REFRESH, OnRefresh)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipsNotify)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FILE, OnItemchangedList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileManageView ��Ϣ�������

// ����
int CFileManageView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ��ʼ��������
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_FILEMANAGE);
	m_wndToolBar.LoadToolBar(IDR_FILEMANAGE, 0, 0, TRUE);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��:
	m_wndToolBar.EnableToolTips(TRUE);

	// ��ʼ���ļ��б�
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | LVS_ICON | LVS_SHOWSELALWAYS | LVS_ALIGNTOP | LVS_AUTOARRANGE | WS_TABSTOP;
	if (!m_listSewFile.Create(dwViewStyle, CRect(0, 0, 0, 0), this, IDC_LIST_FILE))
	{
		TRACE0("δ�ܴ�����ͼ\n");
		return -1;
	}

	// ��ʼ���б�ͼ��
	if (m_imagesList.GetSafeHandle() == NULL )
	{
		m_imagesList.Create(48, 48, ILC_MASK|ILC_COLOR32, 0, 0);
		m_imagesList.Add(AfxGetApp()->LoadIcon(IDI_FILE_MANAGE));
		m_listSewFile.SetImageList(&m_imagesList, LVSIL_NORMAL);
	}

	// ��ʼ�����������
	m_wndToolBar.SetButtonInfo(4, ID_PLACE_HOLDER, TBBS_SEPARATOR, 120);
	CRect rectBar;
	m_wndToolBar.GetItemRect(4, &rectBar);
	rectBar.bottom += 50;
	m_editSearch.Create(WS_VISIBLE|WS_TABSTOP|CBS_AUTOHSCROLL,
		rectBar, &m_wndToolBar, IDC_EDIT_SEARCH);
	m_editSearch.SetFont(CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT)));

	// ������ͼ�ؼ�������
	SetViewCtrlFont();

	// �����ļ��б�
	SetFileList();

	return 0;
}

// ���óߴ�
void CFileManageView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

// ����б�����
void CFileManageView::SetFileList()
{
	m_listSewFile.DeleteAllItems();
	CString strFile;
	for (int i=0; i<m_arrFileList.size(); i++)
	{
		strFile.Format(L"NO.%04d", m_arrFileList[i].nID);
		m_listSewFile.InsertItem(i, strFile);
		m_listSewFile.SetItemData(i, m_arrFileList[i].nID);
	}
}

void CFileManageView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyToolbar = 0;
	cyToolbar = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	//CRect rectSearch;
	//m_wndToolBar.GetItemRect(4, &rectSearch);
	//m_editSearch.SetWindowPos(NULL, rectSearch.left, rectSearch.top+2, 160, cyToolbar-6, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyToolbar, SWP_NOACTIVATE | SWP_NOZORDER);
	m_listSewFile.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyToolbar + 1, rectClient.Width() - 2, rectClient.Height() - cyToolbar-2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CFileManageView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

BOOL CFileManageView::OnToolTipsNotify(UINT id, NMHDR* pNMHDR,LRESULT* pResult)
{
	TOOLTIPTEXT* pT =(TOOLTIPTEXT*)pNMHDR;
	UINT nID = (int)pNMHDR->idFrom;
	if (nID)
	{
		//UINT nIndex = m_wndToolBar.CommandToIndex(nID);
		if(nID == 2)
		{
			pT->lpszText = L"ˢ��";
		}
		else if(nID == 3)
		{
			pT->lpszText = L"ɾ��";
		}
		else if(nID == 5)
		{
			pT->lpszText = L"������";
		}
		else if(nID == 6)
		{
			pT->lpszText = L"���ص�����";
		}
		else if(nID == 8)
		{
			pT->lpszText = L"����";
		}
	}
	return FALSE;
}

// ����
//void CFileManageView::OnFileCopy()
//{
//	//if (m_bUpdateWorking || m_arrIndexToDownload.size() > 0)
//	//	return;
//
//	//int nItem = m_listSewFile.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
//	//if (nItem == -1)
//	//	return;
//
//	//CDlgFileRename dlgCopy;
//	//int nOldID = (int)m_listSewFile.GetItemData(nItem);
//	//dlgCopy.m_bCopyDlg = TRUE;
//	//dlgCopy.m_nFileID = nOldID;
//	//if (dlgCopy.DoModal() == IDOK)
//	//{
//	//	int nNewItem = m_listSewFile.InsertItem(m_listSewFile.GetItemCount(), dlgCopy.m_strFileName);
//	//	m_listSewFile.SetItemData(nNewItem, dlgCopy.m_nFileID);
//	//	int nNewID = dlgCopy.m_nFileID;
//	//	m_nIndexFlag = nNewItem; // ��¼��ǰҪ������ļ�λ��
//	//	
//	//	SewFileData newSew;
//	//	newSew.nID = nNewID;
//	//	m_arrFileList.push_back(newSew);
//
//	//	m_nCmdType = FCMD_COPY;
//	//	this->GetParent()->SendMessage(ID_CMD_REQUEST_GETSEWFILEDATA, nOldID, nNewID);
//	//}
//	//m_listSewFile.SetFocus();
//}

// ɾ��
void CFileManageView::OnFileDelete()
{
	if (m_bUpdateWorking)
		return;

	if (m_listSewFile.GetSelectedCount() > 0)
	{
		if (MessageBox(L"�Ƿ�ɾ����ѡ�����ļ���", L"����", MB_OKCANCEL) == IDOK)
		{
			vector<int> arrID;
			vector<int> arrIndex;
			int nItem = -1;
			while (1)
			{
				nItem = m_listSewFile.GetNextItem(nItem, LVNI_ALL | LVNI_SELECTED);
				if (nItem == -1)
					break;
				arrID.push_back((int)m_listSewFile.GetItemData(nItem));
				arrIndex.push_back(nItem);
			}
			
			for (int i=(int)arrIndex.size()-1; i>=0; i--)
			{
				m_arrFileList.erase(m_arrFileList.begin()+i);
			}

			if (arrID.size() > 0)
			{
				this->GetParent()->SendMessage(ID_CMD_REQUEST_DELETEFILE, 0, (LPARAM)&arrID);
			}
		}
	}
}

// ������
void CFileManageView::OnFileRename()
{
	if (m_bUpdateWorking)
		return;

	int nItem = m_listSewFile.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1)
		return;

	CDlgFileRename dlgRename;
	int nOldID = (int)m_listSewFile.GetItemData(nItem);
	dlgRename.m_nFileID = nOldID;
	m_nSewIDToDelete = nOldID;
	if (dlgRename.DoModal() == IDOK)
	{
		// �����ļ�����ָ��
		int nNewID = dlgRename.m_nFileID;
		m_arrFileList[nItem].nID = nNewID;
		this->GetParent()->SendMessage(ID_CMD_RENAME_SEWFILE, nOldID, nNewID);
	}
	m_listSewFile.SetFocus();
}

// ����
void CFileManageView::OnFileSearch()
{
	CString strKeyword;
	m_editSearch.GetWindowText(strKeyword);

	CString strFileName;
	for (int i=0; i<m_listSewFile.GetItemCount(); i++)
	{
		strFileName = m_listSewFile.GetItemText(i, 0);
		if (strFileName.Find(strKeyword) >= 0)
		{
			m_listSewFile.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			m_listSewFile.EnsureVisible(i, FALSE);
		}
	}
}

// ����
//void CFileManageView::OnFileCut()
//{
//	AfxMessageBox(_T("����..."));
//}

// ���浽����
void CFileManageView::OnFileDownload()
{
	// if (m_bUpdateWorking || m_arrIndexToDownload.size() > 0)
	//	return;

	vector<int> arrID;
	int nID;
	int nItem = -1;
	while (1)
	{
		nItem = m_listSewFile.GetNextItem(nItem, LVNI_ALL | LVNI_SELECTED);
		if (nItem == -1)
			break;

		nID = (int)m_listSewFile.GetItemData(nItem);
		arrID.push_back(nID);
	}

	if (arrID.size() > 0)
	{
		this->GetParent()->SendMessage(ID_CMD_GET_SEWFILEDATA, 0, (LPARAM)&arrID);
	}
}

// ѡ���ļ�ʱԤ���ļ�
void CFileManageView::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->uChanged == LVIF_STATE)
	{
		if(pNMListView->uNewState & LVIS_SELECTED)
		{
			//��������
			int nItem = pNMListView->iItem;
			int nID = (int)m_listSewFile.GetItemData(nItem);
			m_nSelIndex = nItem;

			CMainFrame* pFrm = (CMainFrame*)this->GetParent();
			pFrm->m_trailToPreview.strDesc = m_listSewFile.GetItemText(nItem, 0);
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
			this->GetParent()->SendMessage(ID_CMD_PREVIEW_SEW, PS_PREPARE);
		}
	}
	*pResult = 0;
}

// �ػ�
void CFileManageView::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	CRect rectTree;
	m_listSewFile.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

// ���ý���
void CFileManageView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_listSewFile.SetFocus();
}

// ��������
void CFileManageView::SetViewCtrlFont()
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
	m_listSewFile.SetFont(&m_fntPropList);

	m_editSearch.SetFont(&m_fntPropList);
}

// ����ļ����б���
void CFileManageView::UpdateFileList(SewFileDataArray& arr, BOOL bFinished)
{
	if (!m_bUpdateWorking)
	{
		// ��ʼ����
		m_arrFileList.clear();
		m_bUpdateWorking = TRUE;
	}

	for (int i=0; i<arr.size(); i++)
	{
		m_arrFileList.push_back(arr[i]);
	}

	if (bFinished)
	{
		// ��������
		m_bUpdateWorking = FALSE;

		// ���µ��б�ؼ���
		SetFileList();
	}
}

// ��ʼ��
void CFileManageView::InitData()
{
	m_bUpdateWorking = FALSE;
	m_nSewIDToDelete = -1;
	m_nSelIndex = -1;
	m_arrFileList.clear();
}

// ���ļ���ӵ��б�
void CFileManageView::AddFileToList(const SewFileDataArray& arr)
{
	for (int i=0; i<arr.size(); i++)
	{
		m_arrFileList.push_back(arr[i]);
	}
	SetFileList();
}

// ˢ��
void CFileManageView::OnRefresh()
{
	InitData();
	this->GetParent()->SendMessage(ID_CMD_GET_FILELIST, 0, 0);
}

// ���ն�ȡ������
void CFileManageView::ReceiveSewfileData(ByteArray& arrByte)
{
	if (m_nSelIndex >= 0)
	{
		m_arrFileList[m_nSelIndex].arrByte = arrByte;
		CMainFrame* pFrm = (CMainFrame*)this->GetParent();
		pFrm->SendMessage(ID_CMD_PREVIEW_SEW, PS_RECEIVE, (LPARAM)&arrByte);
	}
}

// ��ȡʧ��
void CFileManageView::ReceiveSewfileDataFailed()
{
	if (m_nSelIndex >= 0)
	{
		CString strTips;
		strTips.Format(L"�ļ�%s��ȡʧ��:", m_listSewFile.GetItemText(m_nSelIndex, 0));
		AfxMessageBox(strTips);
	}
}
