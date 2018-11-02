//===================================================================================================
// Summary:
//		主框架类
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


// 解析花样文件
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
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:
	NeedleTrailData m_trailToPreview;	// 用于预览的轨迹数据缓存
	PreviewState m_statePreview;		// 预览状态
	CString m_strFileToEdit;			// 待编辑的花样文件

protected:
	CSerialPort m_port;		// 通讯接口
	UINT m_uPortID;			// 通讯端口
	BOOL m_bConnected;		// 通讯已连接
	int m_nCurrentMode;		// 当前功能模块
	CString m_strDeviceName;// 设备名称
	int m_nBlockIndex;		// 数据块的序号
	int m_nCmdType;			// 当前指令
	int m_nCurrentFile;		// 当前的花样文件

	list<CProtocol> m_listWaitCol;	// 等待指令序列
	list<CProtocol> m_listProcessCol;// 执行指令序列
	vector<CodeRecode> m_vecFinishedCol;// 结束指令序列
	BOOL m_bTimerStartFlag;	// 计时器启动标志
	SewFileData m_buffFileTrans;	// 文件信息传输缓存
	SewFileData m_currentFile;		// 记录当前花样数据

	BOOL m_bDebugMode;		// 进入调试模式

// 操作
public:
	void InitQueueData();// 初始化队列数据
	void Process();		// 执行队列指令
	BOOL IsProcessing();// 检查是否在执行中
	void StartWaitingTimer();	// 启动等待计时
	void EndWaitingTimer();		// 关闭等待计时
	void InitFiledataBuff();	// 初始化文件数据缓存
	BOOL RequestConnect();		// 请求连接
	void AddDeviceInfoGetCol();	// 获取设备信息
	void UpdateDeviceInfo(BYTE* pData);	// 接收设备信息
	void AddParamListGetCol();	// 请求参数信息（0x27）
	BOOL UpdateParamList(BYTE* pData);	// 接收参数信息（0x27）
	void AddParamSaveCol(const ParaInfoArray& arrPara);// 请求保存参数到缓存区（0x26）
	void AddSewfileCreateCol(int nFileID);	// 请求创建文件（0x78）
	void AddUploadFileDataCol(const SewFileData& sewData);	// 请求上传数据（0x7A）
	void AddFilesListGetCol();		// 请求获取文件列表（0x72）
	BOOL UpdateFileList(BYTE* pData);	// 接收文件列表(0x73)
	void AddSewfileDataGetCol(int nFileID, BYTE byteSecIndex = 0x00);	// 请求下载花样文件数据（0x76）
	BOOL ReceiveSewfileData(BYTE* pData);	// 接收花样文件数据（0x77）
	void AddSewfileDeleteCol(int nFileID);	// 请求删除文件(0x7B)
	void RequestGetSpecialInputPort();	// 请求获取专用输入端口状态(0x09)
	void RequestSetCommonInputPort();	// 请求获取通用输入端口状态(0x09)
	void RequestSetCommonOutputPort(const IOCheckData& iodata);	// 请求修改通用输出端口状态(0x09)
	void RequestSetElectroOutputPort(const IOCheckData& iodata);	// 请求修改输出电磁铁状态(0x09)
	void RequestDrivenTest(const IOCheckData& iodata);	// 请求电机输出测试(0x0A)
	void RequestSetSpindleSpeed(int nVal);	// 请求设置主轴转速(0xAA)
	void RequestSetBottomLineCounter(int nVal);// 请求设置底线计数器(0xAB)
	void RequestSetWorkpiectCounter(int nVal);	// 请求设置工件计数器(0xAB)
	void RequestSetLineTension(int nVal, BYTE index);	// 请求设置线张力(0x0B)
	void UpdateSewingInfo(BYTE* pData);	// 接收缝制相关信息
	void AddCurrSewFileSetCol(int nID, BYTE byteMode = 0x00);	// 请求设置当前花样
	void AddCurrSewFileGetCol();	// 请求获取当前花样
	void GetPreviewFileData(int nID);	// 获取预览数据
	void PreviewCurrentSew();

// 界面
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
	CDockablePane* m_pPaneCurrent;	// 当前窗口
	CDockablePane* m_pPaneSecond;	// 当前附加窗口
	CDlgWaiting m_dlgWaiting;		// 等待进度条

	void InitializeRibbon();
	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
	void CloseCurrentPane();
	void ShowProgressBar(CString strTips);	// 显示进度条
	void CloseProgressBar();	// 关闭进度条

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCloseDockingPane(CDockablePane* pWnd);

public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 消息映射函数
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


