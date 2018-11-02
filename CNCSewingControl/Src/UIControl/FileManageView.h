//===================================================================================================
// Summary:
//		�ļ�������ͼ��
// Date:
//		2018-07-09
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#pragma once

// ͷ�ļ�

// ��������
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
	CFont m_fntPropList;	// ����
	CFileManageToolBar m_wndToolBar;	// ������
	CImageList m_imagesList;	// ͼ��
	CListCtrl m_listSewFile;	// �ļ��б�
	CEdit m_editSearch;			// �����ؼ�
	SewFileDataArray m_arrFileList;	// �����ļ����ݼ�
	BOOL m_bUpdateWorking;	// �Ƿ����������
	int m_nSewIDToDelete;	// ��¼������ǰ���ļ����
	vector<int>m_arrIndexToDownload;// �����ص��ļ�����
	int m_nSelIndex;

protected:
	// ��������
	void SetViewCtrlFont();
	void AdjustLayout();

public:
	void InitData();	// ��ʼ��
	void SetFileList();	// �������
	void UpdateFileList(SewFileDataArray& arr, BOOL bFinished);
	void AddFileToList(const SewFileDataArray& arr);
	void ReceiveSewfileData(ByteArray& arrByte);	// ���ն�ȡ������
	void ReceiveSewfileDataFailed();	// ��ȡʧ��

// ��д
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

