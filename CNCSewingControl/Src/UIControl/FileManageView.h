//===================================================================================================
// Summary:
//		文件管理视图类
// Date:
//		2018-07-09
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#pragma once

// 头文件

// 工具栏类
class CFileManageToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileManageView : public CDockablePane
{
public:
	CFileManageView();
	virtual ~CFileManageView();

protected:
	CFont m_fntPropList;	// 字体
	CFileManageToolBar m_wndToolBar;	// 工具栏
	CImageList m_imagesList;	// 图标
	CListCtrl m_listSewFile;	// 文件列表
	CEdit m_editSearch;			// 搜索控件
	SewFileDataArray m_arrFileList;	// 花样文件数据集
	BOOL m_bUpdateWorking;	// 是否接收数据中
	int m_nSewIDToDelete;	// 记录重命名前的文件编号
	vector<int>m_arrIndexToDownload;// 待下载的文件索引
	int m_nSelIndex;

protected:
	// 设置字体
	void SetViewCtrlFont();
	void AdjustLayout();

public:
	void InitData();	// 初始化
	void SetFileList();	// 填充数据
	void UpdateFileList(SewFileDataArray& arr, BOOL bFinished);
	void AddFileToList(const SewFileDataArray& arr);
	void ReceiveSewfileData(ByteArray& arrByte);	// 接收读取的数据
	void ReceiveSewfileDataFailed();	// 读取失败

// 重写
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//afx_msg void OnFileCopy();
	afx_msg void OnFileDelete();
	afx_msg void OnFileRename();
	afx_msg void OnFileSearch();
	//afx_msg void OnFileCut();
	afx_msg void OnFileDownload();
	afx_msg void OnRefresh();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnToolTipsNotify(UINT id,NMHDR * pNMHDR,LRESULT* pResult);
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

