//===================================================================================================
// Summary:
//		�����ļ���ͼ��
// Date:
//		2018-07-09
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#pragma once

// ͷ�ļ�

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
	CFont m_fntPropList;		// ����
	CFont m_fntToolBar;		// ����

	CLocalFileToolBar m_wndToolBar;	// ������
	CImageList m_imagesList;	// ͼ��
	CListCtrl m_listFile;		// ��ʾ�б�
	CEdit m_editSearch;			// �����ؼ�
	SewFileDataArray m_arrFiles;// �ļ����ݼ���
	CComboBox m_cmbType;		// �ļ�����

	CString m_strFileExt;

	void AdjustLayout();// ���ò���
	void SetViewCtrlFont();	// ��������

public:
	void InitData();	// ��ʼ��
	void UpdateFileList();	// �����ļ��б�
	BOOL DownLoadSewFile(SewFileData& sewData);	// �������ص��ļ�
	
	// ��д
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


