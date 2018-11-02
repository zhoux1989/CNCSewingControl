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
#include "LocalFileView.h"
#include "Resource.h"
#include "CNCSewingControl.h"
#include "DlgFileRename.h"

//===================================================================================================

//
//void CMyShelistCtrl::OnSetColumns()
//{
//	LVCOLUMN lvColumn;
//	lvColumn.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH;
//	LPWSTR sName[4] = {_T("����"),_T("��С"),_T("����"),_T("�޸�")};
//	for(int i=0; i<4; i++)
//	{
//		lvColumn.pszText = sName[i];
//		lvColumn.fmt = LVCFMT_CENTER;
//		SetColumn(i, &lvColumn);
//	}
//}

//HRESULT CMyShelistCtrl::EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent)
//{
//	LPENUMIDLIST pEnum = NULL;
//	HRESULT hRes = pParentFolder->EnumObjects(NULL, m_nTypes, &pEnum);
//	if (SUCCEEDED(hRes) && pEnum != NULL)
//	{
//		LPITEMIDLIST pidlTemp;
//		DWORD dwFetched = 1;
//		LPAFX_SHELLITEMINFO pItem;
//		while (pEnum->Next(1, &pidlTemp, &dwFetched) == S_OK && dwFetched)
//		{
//			LVITEM lvItem;
//			ZeroMemory(&lvItem, sizeof(lvItem));
//
//			lvItem.mask = LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
//
//			pParentFolder->AddRef();
//
//			pItem = (LPAFX_SHELLITEMINFO)GlobalAlloc(GPTR, sizeof(AFX_SHELLITEMINFO));
//
//			pItem->pidlRel = pidlTemp;
//			pItem->pidlFQ = theApp.GetShellManager()->ConcatenateItem(pidlParent, pidlTemp);
//
//			pItem->pParentFolder = pParentFolder;
//			lvItem.lParam = (LPARAM)pItem;
//
//			lvItem.pszText = _T("");
//			lvItem.iImage = OnGetItemIcon(GetItemCount(), pItem);
//
//			DWORD dwAttr = SFGAO_FOLDER | SFGAO_DISPLAYATTRMASK;
//
//			pParentFolder->GetAttributesOf(1, (LPCITEMIDLIST*)&pidlTemp, &dwAttr);
//
//			if (dwAttr & SFGAO_SHARE)
//			{
//				lvItem.mask |= LVIF_STATE;
//				lvItem.stateMask |= LVIS_OVERLAYMASK;
//				lvItem.state |= INDEXTOOVERLAYMASK(1);
//			}
//			if (dwAttr & SFGAO_GHOSTED)
//			{
//				lvItem.mask |= LVIF_STATE;
//				lvItem.stateMask |= LVIS_CUT;
//				lvItem.state |= LVIS_CUT;
//			}
//
//			if (dwAttr & SFGAO_FOLDER)
//			{
//			}
//			else
//			{
//				TCHAR szPath [MAX_PATH];
//				if (SHGetPathFromIDList(pItem->pidlFQ, szPath))
//				{
//					CFileStatus fs;	
//					if (CFile::GetStatus(szPath, fs))
//					{
//						CString s = szPath;
//						int n = s.ReverseFind('.');	
//						if(n!=-1)
//						{
//							s = s.Mid(n+1);
//							s.MakeLower();
//							if(s != _T("sew"))
//								continue;
//						}
//					}
//				}
//			}
//
//			int iItem = InsertItem(&lvItem);
//			if (iItem >= 0)
//			{
//				const int nColumns = m_wndHeader.GetItemCount();
//				for (int iColumn = 0; iColumn < nColumns; iColumn++)
//				{
//					SetItemText(iItem, iColumn, OnGetItemText(iItem, iColumn, pItem));
//				}
//			} 
//			dwFetched = 0;
//		}
//		pEnum->Release();
//	} 
//	return hRes;
//}


//===================================================================================================


//////////////////////////////////////////////////////////////////////
// ����/����
//////////////////////////////////////////////////////////////////////

CLocalFileView::CLocalFileView()
{
	InitData();
}

CLocalFileView::~CLocalFileView()
{
}

BEGIN_MESSAGE_MAP(CLocalFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_CMD_UPLOADSEW, OnUploadFile)
	ON_COMMAND(ID_CMD_REFRESH, OnRefresh)
	ON_COMMAND(ID_CMD_SEARCH, OnSearch)
	ON_WM_PAINT()
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipsNotify)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FILE, OnItemchangedList)
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocalFileView ��Ϣ�������

// ����
int CLocalFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ��ʼ��������
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_LOCALFILEMANAGE);
	m_wndToolBar.LoadToolBar(IDR_LOCALFILEMANAGE, 0, 0, TRUE);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��:
	m_wndToolBar.EnableToolTips(TRUE);

	// ��ʼ���ļ��б�
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | LVS_ICON | LVS_SHOWSELALWAYS | LVS_ALIGNTOP | LVS_AUTOARRANGE | WS_TABSTOP;
	if (!m_listFile.Create(dwViewStyle, CRect(0, 0, 0, 0), this, IDC_LIST_FILE))
	{
		TRACE0("δ�ܴ�����ͼ\n");
		return -1;
	}
	// ��ʼ���б�ͼ��
	if (m_imagesList.GetSafeHandle() == NULL )
	{
		m_imagesList.Create(48, 48, ILC_MASK|ILC_COLOR32, 0, 0);
		m_imagesList.Add(AfxGetApp()->LoadIcon(IDI_FILE_MANAGE));
		m_listFile.SetImageList(&m_imagesList, LVSIL_NORMAL);
	}

	// ��ʼ�����������
	m_wndToolBar.SetButtonInfo(2, ID_PLACE_HOLDER, TBBS_SEPARATOR, 120);
	CRect rectBar;
	m_wndToolBar.GetItemRect(2, &rectBar);
	rectBar.bottom += 50;
	m_editSearch.Create(WS_VISIBLE|WS_TABSTOP|CBS_AUTOHSCROLL,
		rectBar, &m_wndToolBar, IDC_EDIT_SEARCH);
	m_editSearch.SetFont(CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT)));

	// ������ͼ�ؼ�������
	SetViewCtrlFont();

	return 0;
}

// ���óߴ�
void CLocalFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

BOOL CLocalFileView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

// �ϴ����ػ����ļ�
void CLocalFileView::OnUploadFile()
{
	int nItem = m_listFile.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1)
		return;

	int nID = -1;
	CString strFileName =m_listFile.GetItemText(nItem, 0);
	int nLast = strFileName.ReverseFind(L'.');
	if (nLast > 0)
	{
		int nFirst = 0;
		for (int i=nLast-1; i>0; i--)
		{
			if (!IsNumber(strFileName.GetAt(i)))
			{
				nFirst = i+1;
				break;
			}
		}
		if (nFirst < nLast)
		{
			nID = _wtoi(strFileName.Mid(nFirst, nLast-nFirst));
		}
	}
	
	CDlgFileRename dlg;
	dlg.m_bCopyDlg = TRUE;
	dlg.m_nFileID = nID;
	if (dlg.DoModal() == IDOK)
	{
		CString strFile;
		int nPos = (int)m_listFile.GetItemData(nItem);
		SewFileData sewData;
		sewData.nID = dlg.m_nFileID;
		if (m_arrFiles[nPos].arrByte.empty())
		{
			if (!ReadSewFile(m_arrFiles[nPos].strPath, sewData.arrByte))
			{
				// ��ȡʧ��
				CString strTips;
				strTips.Format(L"�ļ�%s��ȡʧ��:", m_listFile.GetItemText(nItem, 0));
				AfxMessageBox(strTips);
				return;
			}
		}
		else
		{
			// ֱ�Ӵӻ����ȡ
			sewData.arrByte = m_arrFiles[nPos].arrByte;
		}
		this->GetParent()->SendMessage(ID_CMD_UPLOAD_SEWFILE, 0, (LPARAM)&sewData);
	}
}

// ˢ��
void CLocalFileView::OnRefresh()
{
	UpdateFileList();
}

// ����
void CLocalFileView::OnSearch()
{
	CString strKeyword;
	m_editSearch.GetWindowText(strKeyword);

	CString strFileName;
	for (int i=0; i<m_listFile.GetItemCount(); i++)
	{
		strFileName = m_listFile.GetItemText(i, 0);
		if (strFileName.Find(strKeyword) >= 0)
		{
			m_listFile.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			m_listFile.EnsureVisible(i, FALSE);
		}
	}
}

// �ػ�
void CLocalFileView::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	CRect rectTree;
	m_listFile.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

// ���ý���
void CLocalFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_listFile.SetFocus();
}

BOOL CLocalFileView::OnToolTipsNotify(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
	TOOLTIPTEXT* pT =(TOOLTIPTEXT*)pNMHDR;
	UINT nID = (int)pNMHDR->idFrom;
	if (nID)
	{
		//UINT nIndex = m_wndToolBar.CommandToIndex(nID);
		if(nID == 1)
		{
			pT->lpszText = L"�ϴ�";
		}
		else if(nID == 2)
		{
			pT->lpszText = L"ˢ��";
		}
		else if(nID == 4)
		{
			pT->lpszText = L"����";
		}
	}
	return FALSE;
}

// ѡ���ļ�ʱԤ���ļ�
void CLocalFileView::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->uChanged == LVIF_STATE)
	{
		CMainFrame* pFrm = (CMainFrame*)this->GetParent();
		if(pNMListView->uNewState & LVIS_SELECTED)
		{
			//��������
			int nItem = pNMListView->iItem;
			int nIndex = (int)m_listFile.GetItemData(nItem);
			
			if (m_arrFiles[nIndex].arrByte.empty())
			{
				pFrm->SendMessage(ID_CMD_PREVIEW_SEW, PS_PREPARE);

				if (!ReadSewFile(m_arrFiles[nIndex].strPath, m_arrFiles[nIndex].arrByte))
				{
					// ��ȡʧ��
					CString strTips;
					strTips.Format(L"�ļ�%s��ȡʧ��:", m_listFile.GetItemText(nItem, 0));
					AfxMessageBox(strTips);
					*pResult = 0;
					return;
				}
			}
			pFrm->m_trailToPreview.strDesc = m_listFile.GetItemText(nItem, 0);
			pFrm->m_strFileToEdit = m_arrFiles[nIndex].strPath;
			pFrm->SendMessage(ID_CMD_PREVIEW_SEW, PS_RECEIVE, (LPARAM)&m_arrFiles[nIndex].arrByte);
		}
		else
		{
			pFrm->SendMessage(ID_CMD_PREVIEW_SEW, PS_PREPARE);
			pFrm->m_strFileToEdit = L"";
		}
	}
	*pResult = 0;
}

//===================================================================================================

// ��������
void CLocalFileView::SetViewCtrlFont()
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
	m_listFile.SetFont(&m_fntPropList);

	m_editSearch.SetFont(&m_fntPropList);
}

// ��������
void CLocalFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyToolbar = 0;
	cyToolbar = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyToolbar, SWP_NOACTIVATE | SWP_NOZORDER);
	m_listFile.SetWindowPos(NULL, rectClient.left, rectClient.top + cyToolbar + 1, rectClient.Width(), rectClient.Height() - cyToolbar-2, SWP_NOACTIVATE | SWP_NOZORDER);
}

// ��ʼ��
void CLocalFileView::InitData()
{
	m_arrFiles.clear();
}

// �����ļ��б�
void CLocalFileView::UpdateFileList()
{
	CString strFolder = GetConfigOption(L"INIT", L"LOCALFOLDER");
	if (!PathIsDirectory(strFolder))
	{
		CreateDirectory(strFolder, 0);
	}

	// �������ݻ���
	m_arrFiles.clear();
	m_listFile.DeleteAllItems();

	// �����ļ����µ�ȫ��sew�ļ�
	CString strFind = strFolder + L"\\*.sew";
	CFileFind find;
	BOOL bFind = find.FindFile(strFind);
	
	CStringArray arrFileName;
	int nIndex = 0;
	while (bFind)
	{
		bFind = find.FindNextFile();
		if (!find.IsDots())
		{
			SewFileData file;
			file.strPath = find.GetFilePath();
			m_arrFiles.push_back(file);
			m_listFile.InsertItem(nIndex, find.GetFileName());
			m_listFile.SetItemData(nIndex, nIndex);
			nIndex++;
		}
	}
}

// �������ص��ļ�
BOOL CLocalFileView::DownLoadSewFile(SewFileData& sewData)
{
	// ���ļ����ص�����
	CString strPath = GetConfigOption(L"INIT", L"LOCALFOLDER");
	CString strFileName;
	strFileName.Format(L"ISMS%04d.sew", sewData.nID);
	strPath += L"\\" + strFileName;

	if (SaveSewFile(strPath, sewData.arrByte))
	{
		sewData.strPath = strPath;
		m_arrFiles.push_back(sewData);
		int nCount = (int)m_listFile.GetItemCount();
		int nItem = m_listFile.InsertItem(nCount, strFileName);
		m_listFile.SetItemData(nItem, m_arrFiles.size()-1);
		return TRUE;
	}

	return FALSE;
}
