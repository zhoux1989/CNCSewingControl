//===================================================================================================
// Summary:
//		�������
// Date:
//		2018-07-07
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#pragma once

#include "LocalFileView.h"
#include "SewFileView.h"
#include "SewingInfoView.h"
#include "ParaConfigView.h"
#include "IOCheckView.h"
#include "FileManageView.h"
#include "SerialPort.h"
#include "DlgWaiting.h"


// ���������ļ�
BOOL AnalysisSewFile(const ByteArray& arrSewByte, NeedleTrailData& dataPreview);

//class CUDFMFCRibbonQuickAccessToolBar : public CMFCRibbonQuickAccessToolBar
//{
//	friend class CUDFMFCRibbonBar;
//};
//
//class CUDFMFCRibbonBar : public CMFCRibbonBar
//{
//	DECLARE_DYNAMIC(CUDFMFCRibbonBar)
//
//public:
//	CUDFMFCRibbonBar(BOOL bReplaceFrameCaption = TRUE);
//	virtual ~CUDFMFCRibbonBar();
//
//protected:
//	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
//	DECLARE_MESSAGE_MAP()
//};

class CMainFrame : public CFrameWndEx
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:
	NeedleTrailData m_trailToPreview;	// ����Ԥ���Ĺ켣���ݻ���
	PreviewState m_statePreview;		// Ԥ��״̬
	CString m_strFileToEdit;			// ���༭�Ļ����ļ�

protected:
	CSerialPort m_port;		// ͨѶ�ӿ�
	UINT m_uPortID;			// ͨѶ�˿�
	BOOL m_bConnected;		// ͨѶ������
	int m_nCurrentMode;		// ��ǰ����ģ��
	CString m_strDeviceName;// �豸����
	int m_nBlockIndex;		// ���ݿ�����
	int m_nCmdType;			// ��ǰָ��
	int m_nCurrentFile;		// ��ǰ�Ļ����ļ�

	list<CProtocol> m_listWaitCol;	// �ȴ�ָ������
	list<CProtocol> m_listProcessCol;// ִ��ָ������
	vector<CodeRecode> m_vecFinishedCol;// ����ָ������
	BOOL m_bTimerStartFlag;	// ��ʱ��������־
	SewFileData m_buffFileTrans;	// �ļ���Ϣ���仺��
	SewFileData m_currentFile;		// ��¼��ǰ��������

	BOOL m_bDebugMode;		// �������ģʽ

// ����
public:
	void InitQueueData();// ��ʼ����������
	void Process();		// ִ�ж���ָ��
	BOOL IsProcessing();// ����Ƿ���ִ����
	void StartWaitingTimer();	// �����ȴ���ʱ
	void EndWaitingTimer();		// �رյȴ���ʱ
	void InitFiledataBuff();	// ��ʼ���ļ����ݻ���
	BOOL RequestConnect();		// ��������
	void AddDeviceInfoGetCol();	// ��ȡ�豸��Ϣ
	void UpdateDeviceInfo(BYTE* pData);	// �����豸��Ϣ
	void AddParamListGetCol();	// ���������Ϣ��0x27��
	BOOL UpdateParamList(BYTE* pData);	// ���ղ�����Ϣ��0x27��
	void AddParamSaveCol(const ParaInfoArray& arrPara);// ���󱣴��������������0x26��
	void AddSewfileCreateCol(int nFileID);	// ���󴴽��ļ���0x78��
	void AddUploadFileDataCol(const SewFileData& sewData);	// �����ϴ����ݣ�0x7A��
	void AddFilesListGetCol();		// �����ȡ�ļ��б�0x72��
	BOOL UpdateFileList(BYTE* pData);	// �����ļ��б�(0x73)
	void AddSewfileDataGetCol(int nFileID, BYTE byteSecIndex = 0x00);	// �������ػ����ļ����ݣ�0x76��
	BOOL ReceiveSewfileData(BYTE* pData);	// ���ջ����ļ����ݣ�0x77��
	void AddSewfileDeleteCol(int nFileID);	// ����ɾ���ļ�(0x7B)
	void RequestGetSpecialInputPort();	// �����ȡר������˿�״̬(0x09)
	void RequestSetCommonInputPort();	// �����ȡͨ������˿�״̬(0x09)
	void RequestSetCommonOutputPort(const IOCheckData& iodata);	// �����޸�ͨ������˿�״̬(0x09)
	void RequestSetElectroOutputPort(const IOCheckData& iodata);	// �����޸���������״̬(0x09)
	void RequestDrivenTest(const IOCheckData& iodata);	// �������������(0x0A)
	void RequestSetSpindleSpeed(int nVal);	// ������������ת��(0xAA)
	void RequestSetBottomLineCounter(int nVal);// �������õ��߼�����(0xAB)
	void RequestSetWorkpiectCounter(int nVal);	// �������ù���������(0xAB)
	void RequestSetLineTension(int nVal, BYTE index);	// ��������������(0x0B)
	void UpdateSewingInfo(BYTE* pData);	// ���շ��������Ϣ
	void AddCurrSewFileSetCol(int nID, BYTE byteMode = 0x00);	// �������õ�ǰ����
	void AddCurrSewFileGetCol();	// �����ȡ��ǰ����
	void GetPreviewFileData(int nID);	// ��ȡԤ������
	void PreviewCurrentSew();

// ����
protected:
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	//CMFCRibbonStatusBar  m_wndStatusBar;
	CLocalFileView      m_wndLocalFileView;
	CSewFileView        m_wndSewFile;
	CSewingInfoView   m_wndSewingInfo;
	CIOCheckView m_wndIOCheck;
	CParaConfigView m_wndParaConfig;
	CFileManageView m_wndFileManage;
	CDockablePane* m_pPaneCurrent;	// ��ǰ����
	CDockablePane* m_pPaneSecond;	// ��ǰ���Ӵ���
	CDlgWaiting m_dlgWaiting;		// �ȴ�������

	void InitializeRibbon();
	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
	void CloseCurrentPane();
	void ShowProgressBar(CString strTips);	// ��ʾ������
	void CloseProgressBar();	// �رս�����

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCloseDockingPane(CDockablePane* pWnd);

public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// ��Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnUpdateConnectOrBreak(CCmdUI* pCmdUI);
	//afx_msg void OnUpdateSaveBtnState(CCmdUI* pCmdUI);
	afx_msg void OnViewrecovery();
	afx_msg void OnSewinginfoView();
	afx_msg void OnCurrentSewfileView();
	afx_msg void OnSewEditView();
	afx_msg void OnFileManageView();
	afx_msg void OnParamConfigView();
	afx_msg void OnIoCheckView();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSewConnect();
	afx_msg void OnSewBreak();
	afx_msg LRESULT OnReceiveData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetParamList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetFileList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteSewFile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT RequestGetSewingInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT RequestSetSewingInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetCurrSewFile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDownloadSewfile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRenameSewFile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUploadSewFile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveParam(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCheckInputPort(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT RequestOutputCheck(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdatePreview(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};


