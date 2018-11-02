//===================================================================================================
// Summary:
//		花样文件视图类
// Date:
//		2018-07-07
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#pragma once

// 头文件

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
	void InitData();	// 初始化
	void FillSewFileList();	// 填充数据
	void UpdateFileList(SewFileDataArray& arr, BOOL bFinished);
	void UpdateCurrSewFile(int nSewID);	// 更新当前花样文件
	void SetCurrSewFile(int nItem);	// 设置当前花样
	void ReceiveSewfileData(ByteArray& arrByte);	// 接收读取的数据
	void AddFileToList(const SewFileDataArray& arr);

protected:
	void AdjustLayout();
	void SetFont(); // 设置字体

// 重写
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

