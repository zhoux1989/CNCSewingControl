//===================================================================================================
// Summary:
//		�����ļ���ͼ��
// Date:
//		2018-07-07
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#pragma once

// ͷ�ļ�

class CSewFileToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CSewFileView : public CDockablePane
{
public:
	CSewFileView();
	virtual ~CSewFileView();

protected:
	CFont m_fntPropList;
	CSewFileToolBar m_wndToolBar;
	CImageList m_listImages;
	UINT m_nCurrSort;
	CListCtrl m_ctrlSewFileList;
	SewFileDataArray m_arrFileList;
	int m_nIndexFlag;
	int m_nMarkedItem;
	int m_nCurrFileID;
	int m_nSelIndex;

public:
	void InitData();	// ��ʼ��
	void FillSewFileList();	// �������
	void UpdateFileList(SewFileDataArray& arr, BOOL bFinished);
	void UpdateCurrSewFile(int nSewID);	// ���µ�ǰ�����ļ�
	void SetCurrSewFile(int nItem);	// ���õ�ǰ����
	void ReceiveSewfileData(ByteArray& arrByte);	// ���ն�ȡ������
	void AddFileToList(const SewFileDataArray& arr);

protected:
	void AdjustLayout();
	void SetFont(); // ��������

// ��д
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnNewFolder();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
	afx_msg void OnItemdblclickSewList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

