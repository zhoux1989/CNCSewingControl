//===================================================================================================
// Summary:
//		本地文件视图类
// Date:
//		2018-07-09
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#pragma once

// 头文件

class CLocalFileToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};
//
//class CMyShelistCtrl : public CMFCShellListCtrl
//{
//	//virtual void OnSetColumns();
//	virtual HRESULT EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent);
//};

class CLocalFileView : public CDockablePane
{
public:
	CLocalFileView();
	virtual ~CLocalFileView();

protected:
	CFont m_fntPropList;		// 字体
	CFont m_fntToolBar;		// 字体

	CLocalFileToolBar m_wndToolBar;	// 工具栏
	CImageList m_imagesList;	// 图标
	CListCtrl m_listFile;		// 显示列表
	CEdit m_editSearch;			// 搜索控件
	SewFileDataArray m_arrFiles;// 文件数据集合
	CComboBox m_cmbType;		// 文件类型

	CString m_strFileExt;

	void AdjustLayout();// 设置布局
	void SetViewCtrlFont();	// 设置字体

public:
	void InitData();	// 初始化
	void UpdateFileList();	// 更新文件列表
	BOOL DownLoadSewFile(SewFileData& sewData);	// 保存下载的文件
	
	// 重写
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRefresh();
	afx_msg void OnSearch();
	afx_msg void OnUploadFile();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnToolTipsNotify(UINT id,NMHDR * pNMHDR,LRESULT* pResult);
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCbnSelchangeType();
	afx_msg void OnNMDblclkList (NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};


