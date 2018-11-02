//===================================================================================================
// Summary:
//		主框架类
// Date:
//		2018-07-07
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#include "stdafx.h"
#include "CNCSewingControl.h"

#include "MainFrm.h"
#include "DlgConnect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//===================================================================================================
//
//static const int nXTabMargin = 14;
//static const int nYTabMargin = 5;
//static const int nXMargin = 2;
//static const int nYMargin = 2;
//
//IMPLEMENT_DYNAMIC(CUDFMFCRibbonBar, CMFCRibbonBar)
//
//CUDFMFCRibbonBar::CUDFMFCRibbonBar(BOOL bReplaceFrameCaption /*= TRUE*/)
//: CMFCRibbonBar(bReplaceFrameCaption)
//{
//}
//
//CUDFMFCRibbonBar::~CUDFMFCRibbonBar()
//{
//}
//
//BEGIN_MESSAGE_MAP(CUDFMFCRibbonBar, CMFCRibbonBar)
//END_MESSAGE_MAP()
//
//CSize CUDFMFCRibbonBar::CalcFixedLayout( BOOL bStretch, BOOL bHorz )
//{
//	ASSERT_VALID(this);
//	CClientDC dc(this);
//	CFont* pOldFont = dc.SelectObject(GetFont());
//	ENSURE(pOldFont != NULL);
//
//	TEXTMETRIC tm;
//	dc.GetTextMetrics(&tm);
//
//	//“标题”的高度
//	m_nCaptionHeight = 0;
//	if (m_bReplaceFrameCaption)
//	{
//		m_nCaptionHeight = GetSystemMetrics(SM_CYCAPTION) + 1;
//		if (afxGlobalData.DwmIsCompositionEnabled())
//		{
//			m_nCaptionHeight += GetSystemMetrics(SM_CYSIZEFRAME);
//		}
//	}
//
//	int cy = 0;
//
//	CSize sizeMainButton = m_sizeMainButton;
//	double scale = afxGlobalData.GetRibbonImageScale();
//	if (scale > 1.)
//	{
//		sizeMainButton.cx = (int)(.5 + scale * sizeMainButton.cx);
//		sizeMainButton.cy = (int)(.5 + scale * sizeMainButton.cy);
//	}
//
//	if (m_dwHideFlags & AFX_RIBBONBAR_HIDE_ALL)
//	{
//		cy = m_nCaptionHeight;
//	}
//	else
//	{
//		if (m_bRecalcCategoryHeight)
//		{
//			m_nCategoryHeight = 0;
//		}
//		//“选项卡”的高度
//		m_nTabsHeight = tm.tmHeight + 2 * nYTabMargin;
//		if (m_bRecalcCategoryHeight)
//		{
//			for (int i = 0; i < m_arCategories.GetSize(); i++)
//			{
//				CMFCRibbonCategory* pCategory = m_arCategories [i];
//				ASSERT_VALID(pCategory);
//				m_nCategoryHeight = max(m_nCategoryHeight, pCategory->GetMaxHeight(&dc)+80);
//			}
//			m_bRecalcCategoryHeight = FALSE;
//		}
//
//		CUDFMFCRibbonQuickAccessToolBar *pQAToolbar = (CUDFMFCRibbonQuickAccessToolBar *)&m_QAToolbar;
//		const CSize sizeAQToolbar = pQAToolbar->GetRegularSize(&dc);
//		if (IsQuickAccessToolbarOnTop())
//		{
//			m_nCaptionHeight = max(m_nCaptionHeight, sizeAQToolbar.cy + (2 * nYMargin));
//		}
//
//		//“快速访问工具栏”的高度
//		const int nQuickAceesToolbarHeight = IsQuickAccessToolbarOnTop() ? 0 : sizeAQToolbar.cy;
//		//“分类”的高度
//		const int nCategoryHeight = (m_dwHideFlags & AFX_RIBBONBAR_HIDE_ELEMENTS) ? 0 : m_nCategoryHeight;
//		cy = nQuickAceesToolbarHeight + nCategoryHeight + max( m_nCaptionHeight + m_nTabsHeight, m_sizeMainButton.cy + nYMargin);
//	}
//
//	if (afxGlobalData.DwmIsCompositionEnabled())
//	{
//		if (GetParent()->IsZoomed() && m_bReplaceFrameCaption)
//		{
//			cy += ::GetSystemMetrics(SM_CYSIZEFRAME) - 2;
//		}
//	}
//
//	dc.SelectObject(pOldFont);
//
//	return CSize(32767, cy);
//}
//

// 解析花样文件
BOOL AnalysisSewFile(const ByteArray& arrSewByte, NeedleTrailData& dataPreview)
{
	// 文件头占用16个字节，文件结束标记占用12个字节，文件信息数据至少占用30个字节
	int nByteCount = (int)arrSewByte.size();
	if (nByteCount < 58)
		return FALSE;

	ByteArray arrByte = arrSewByte;
	arrByte.insert(arrByte.begin(), 0x00);	// 方便从1开始进行索引

	// 文件头前八个直接用于判断文件类型，第5位开始不同
	if (arrByte[5] == 0x23)	// 42 49 4C 00 23 13 00 C8 表示311G
	{
		// 直接从17个字节开始读取，按数据指令一段段读取，指令段后紧跟数据段
		int nCodeStart = 16;		// 当前指令段的起始位置(起始字节的前一个索引)
		int nDataStart = 28;		// 当前数据段的起始位置(起始字节的前一个索引)
		int nDataLength = 0;	// 当前数据段的长度
		// 从当前指令段开始获取12个字符
		while (nCodeStart+12 <= nByteCount)
		{
			// 获取数据段长度
			nDataLength = arrByte[nCodeStart+4]*65536+arrByte[nCodeStart+5]*256+arrByte[nCodeStart+6];
			nDataStart = nCodeStart+12;
			if ((nDataStart+nDataLength) > nByteCount)
			{
				break;	// 指令数据长度与指令不一致，跳出循环
			}

			// 检查指令段是否有效(指令段以CD 0C开头)
			if (arrByte[nCodeStart+1] == 0xCD && arrByte[nCodeStart+2] == 0x0C)
			{
				// 指令段的第8位表示指令类型
				if (arrByte[nCodeStart+8] == 0x01)	// 结束指令段
				{
					break;	// 结束跳出循环
				}
				else if (arrByte[nCodeStart+8] == 0x02)	// 暂不知道用途
				{

				}
				else if (arrByte[nCodeStart+8] == 0x03)	// 文件信息指令段
				{
					dataPreview.nStartPulseX = HexToCmt(arrByte[nDataStart+3], arrByte[nDataStart+4]);
					dataPreview.nStartPulseY = HexToCmt(arrByte[nDataStart+5], arrByte[nDataStart+6]);
					dataPreview.nPlusMaxX = HexToCmt(arrByte[nDataStart+11], arrByte[nDataStart+12]);
					dataPreview.nMinusMaxX = HexToCmt(arrByte[nDataStart+13], arrByte[nDataStart+14]);
					dataPreview.nPlusMaxY = HexToCmt(arrByte[nDataStart+15], arrByte[nDataStart+16]);
					dataPreview.nMinusMaxY = HexToCmt(arrByte[nDataStart+17], arrByte[nDataStart+18]);
				}
				else if (arrByte[nCodeStart+8] == 0x04)	// 针步数据指令段
				{
					if (nDataLength%2 != 0)
					{
						break;
					}
					for (int i=1; i+1<=nDataLength; i=i+2)
					{
						NeedleStepData dataStep;
						BYTE byteFisrt = arrByte[nDataStart+i];
						if ((byteFisrt&0xF0) == 0x60)	// 针步数据
						{
							long lPulseNum[2] = {0, 0};
							lPulseNum[0] = arrByte[nDataStart+i+1];
							if ((byteFisrt&0x04)==0x04)	// 负向
							{
								lPulseNum[0] = -lPulseNum[0];
							}

							// 同时包含X增量和Y增量
							if (i+3<=nDataLength && (arrByte[nDataStart+i+2]&0xF0) == 0x40)
							{
								lPulseNum[1] = arrByte[nDataStart+i+3];
								if ((arrByte[nDataStart+i+2]&0x04)==0x04)	// 负向
								{
									lPulseNum[1] = -lPulseNum[1];
								}
								i = i+2;
							}

							// 判断第一组数是否为Y轴增量
							if ((byteFisrt&0x08) == 0x08)	// Y轴
							{
								long lTemp = lPulseNum[0];
								lPulseNum[0] = lPulseNum[1];
								lPulseNum[1] = lTemp;
							}

							dataStep.lPulseOffsetX = lPulseNum[0];
							dataStep.lPulseOffsetY = lPulseNum[1];
							dataPreview.arrNode.push_back(dataStep);
						}
						else if ((byteFisrt&0xF0) == 0x20)	// 空移数据
						{
							long lPulseNum[2] = {0, 0};
							lPulseNum[0] = arrByte[nDataStart+i+1];

							// 判断有无高位（第2位表示有无高位，=1表示有高位）
							if (i+3<=nDataLength && (byteFisrt&0x02) == 0x02)
							{
								lPulseNum[0] = lPulseNum[0]*256+arrByte[nDataStart+i+3];
								i += 2;
							}
							if ((byteFisrt&0x04)==0x04)	// 负向（第3位表示方向，=1时表示负向）
							{
								lPulseNum[0] = -lPulseNum[0];
							}

							// 判断是否同时包含X增量和Y增量
							if (i+3<=nDataLength)
							{
								BYTE byteOther = arrByte[nDataStart+i+2];
								if ((byteOther&0xF0) != 0x60 && 
									(byteOther&0xF0) != 0xF0 &&
									(byteOther&0xF0) != 0x20)
								{
									lPulseNum[1] = arrByte[nDataStart+i+3];
									i= i+2;

									// 判断有无高位
									if (i+3<=nDataLength && (byteOther&0x02) == 0x02)
									{
										lPulseNum[1] = lPulseNum[1]*256+arrByte[nDataStart+i+3];
										i += 2;
									}
									if ((byteOther&0x04)==0x04)	// 负向
									{
										lPulseNum[1] = -lPulseNum[1];
									}
								}
							}

							// 判断第一组数是否为Y轴增量（第4位表示X轴还是Y轴，=1时表示只有Y轴）
							if ((byteFisrt&0x08) == 0x08)	// Y轴
							{
								long lTemp = lPulseNum[0];
								lPulseNum[0] = lPulseNum[1];
								lPulseNum[1] = lTemp;
							}
							dataStep.bEmptyMoveFlag = TRUE;
							dataStep.lPulseOffsetX = lPulseNum[0];
							dataStep.lPulseOffsetY = lPulseNum[1];
							dataPreview.arrNode.push_back(dataStep);
						}
						else if ((byteFisrt&0xF0) == 0xF0)	// 辅助功能数据
						{
							if (byteFisrt == 0xFF && arrByte[nDataStart+i+1]==0x00)
							{
								break;	// 结束读取
							}
							else
							{
								continue; // 跳过
							}
						}
					}
				}
				else if (arrByte[nCodeStart+8] == 0x0D)	// 时间信息指令段
				{

				}
				else if (arrByte[nCodeStart+8] == 0x05)	// 注释信息指令段
				{

				}
				nCodeStart += 12+nDataLength;	// 跳到指令数据的末尾
			}
			else
			{
				break;		// 不是指令段
			}
		}
	}
	else	// 42 49 4c 00 38 6b 00 64 表示311E
	{
		return FALSE;
	}
	return TRUE;
}

//===================================================================================================

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_SILVER);
	m_uPortID = GetConfigOptionINT(L"CONNECT", L"PORTID");
	m_port.m_pMainFrm = this;
	m_strDeviceName = L"";
	m_bConnected = FALSE;
	m_pPaneCurrent = NULL;
	m_pPaneSecond = NULL;
	m_nCurrentMode = MT_NULL;
	m_bTimerStartFlag = FALSE;
	m_statePreview = PS_CLOSE;
	m_bDebugMode = GetConfigOptionINT(L"INIT", L"DEBUGMODE");

	InitQueueData();
}

CMainFrame::~CMainFrame()
{
	EndWaitingTimer();
}

//===================================================================================================

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, OnUpdateApplicationLook)
	//ON_UPDATE_COMMAND_UI_RANGE(IDC_FIRSTBTN_SAVE_ID, IDC_FIRSTBTN_SAVE_ID+9998, OnUpdateSaveBtnState)
	ON_COMMAND(ID_SEWINGINFO_SET, OnSewinginfoView)
	ON_COMMAND(ID_SEWFILE_SET, OnCurrentSewfileView)
	ON_COMMAND(ID_SEW_EDIT, OnSewEditView)
	ON_COMMAND(ID_FILE_MANAGE, OnFileManageView)
	ON_COMMAND(ID_PARAM_CONFIG, OnParamConfigView)
	ON_COMMAND(ID_IO_CHECK, OnIoCheckView)
	ON_COMMAND(ID_SEW_CONNECT, OnSewConnect)
	ON_COMMAND(ID_SEW_BREAK, OnSewBreak)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SEW_CONNECT, ID_SEW_BREAK, OnUpdateConnectOrBreak)
	ON_MESSAGE(ID_CMD_RECEIVEDATA, OnReceiveData)
	ON_MESSAGE(ID_CMD_GET_FILELIST, OnGetFileList)
	ON_MESSAGE(ID_CMD_GET_PARAMLIST, OnGetParamList)
	ON_MESSAGE(ID_CMD_UPLOAD_SEWFILE, OnUploadSewFile)
	ON_MESSAGE(ID_CMD_RENAME_SEWFILE, OnRenameSewFile)
	ON_MESSAGE(ID_CMD_GET_SEWFILEDATA, OnDownloadSewfile)
	ON_MESSAGE(ID_CMD_SET_PARAM, OnSaveParam)
	ON_MESSAGE(ID_CMD_REQUEST_DELETEFILE, OnDeleteSewFile)
	ON_MESSAGE(ID_CMD_GET_SEWINGINFO, RequestGetSewingInfo)
	ON_MESSAGE(ID_CMD_SET_SEWINGINFO, RequestSetSewingInfo)
	ON_MESSAGE(ID_CMD_SET_CURRENTSEW, OnSetCurrSewFile)
	ON_MESSAGE(ID_CMD_REQUEST_INPUTCHECK, OnCheckInputPort)
	ON_MESSAGE(ID_CMD_REQUEST_OUTPUTCHECK, RequestOutputCheck)
	ON_MESSAGE(ID_CMD_PREVIEW_SEW, OnUpdatePreview)
	//ON_COMMAND(ID_CMD_RECEIVE_DELETEFILE, OnSewBreak)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//===================================================================================================

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	//BOOL bNameValid;
	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	m_wndRibbonBar.Create(this);
	InitializeRibbon();

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("未能创建状态栏\n");
	//	return -1;      // 未能创建
	//}

	//CString strTitlePane1;
	//CString strTitlePane2;
	//bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	//ASSERT(bNameValid);
	//bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	//ASSERT(bNameValid);
	//m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	//m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 加载菜单项图像(不在任何标准工具栏上):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// 创建停靠窗口
	if (!CreateDockingWindows())
	{
		TRACE0("未能创建停靠窗口\n");
		return -1;
	}

	//DockPane(&m_wndSewingInfo);
	//DockPane(&m_wndSewFile);

	/*SewFileData dataTemp;
	CString strFile = L"C:\\SUPERME\\ISMS0106.sew";
	ReadSewFile(strFile, dataTemp);
	AnalysisSewFile(dataTemp.arrByte, m_trailToPreview);*/
	

	// 自动连接
	if (m_uPortID > 0)
	{
		RequestConnect();
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~(LONG) FWS_ADDTOTITLE;

	// 修改初始对话框标题（窗口最大化时无法显示标题）
	cs.lpszName = L"CNCSewingControl - 双针缝纫机PC端控制软件（无设备连接）";

	if( !CFrameWndEx::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

// 初始化Ribbon栏
void CMainFrame::InitializeRibbon()
{
	BOOL bNameValid;

	CString strTemp;
	bNameValid = strTemp.LoadString(IDS_RIBBON_FILE);
	ASSERT(bNameValid);

	// 初始主按钮:
	/*m_MainButton.SetImage(IDB_MAIN);
	m_MainButton.SetText(_T("\nf"));
	m_MainButton.SetToolTipText(strTemp);*/

	// 加载面板图像:
	m_PanelImages.SetImageSize(CSize(16, 16));
	m_PanelImages.Load(IDB_BUTTONS);

	//m_wndRibbonBar.SetApplicationButton(&m_MainButton, CSize (45, 45));
	//CMFCRibbonMainPanel* pMainPanel = m_wndRibbonBar.AddMainCategory(strTemp, IDB_FILESMALL, IDB_FILELARGE);

	/*bNameValid = strTemp.LoadString(IDS_RIBBON_NEW);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_NEW, strTemp, 0, 0));
	bNameValid = strTemp.LoadString(IDS_RIBBON_OPEN);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_OPEN, strTemp, 1, 1));
	bNameValid = strTemp.LoadString(IDS_RIBBON_SAVE);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SAVE, strTemp, 2, 2));
	bNameValid = strTemp.LoadString(IDS_RIBBON_SAVEAS);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SAVE_AS, strTemp, 3, 3));*/

	//bNameValid = strTemp.LoadString(IDS_RIBBON_CLOSE);
	//ASSERT(bNameValid);
	//pMainPanel->Add(new CMFCRibbonButton(ID_FILE_CLOSE, strTemp, 9, 9));

	///*bNameValid = strTemp.LoadString(IDS_RIBBON_RECENT_DOCS);
	//ASSERT(bNameValid);*/
	//pMainPanel->AddRecentFilesList(L" ");

	//bNameValid = strTemp.LoadString(IDS_RIBBON_EXIT);
	//ASSERT(bNameValid);
	//pMainPanel->AddToBottom(new CMFCRibbonMainPanelButton(ID_APP_EXIT, strTemp, 15));

	// 添加“主”类别:
	bNameValid = strTemp.LoadString(IDS_RIBBON_HOME);
	ASSERT(bNameValid);
	CMFCRibbonCategory* pCategoryHome = m_wndRibbonBar.AddCategory(strTemp, IDB_WRITESMALL, IDB_SEWCONTROL_LARGE, CSize(16, 16), CSize(64, 64));

	// 创建“缝纫机通讯”面板
	CMFCRibbonPanel* pPanelSewConnect = pCategoryHome->AddPanel(L"缝纫机通讯", m_PanelImages.ExtractIcon(27));
	pPanelSewConnect->Add(new CMFCRibbonButton(ID_SEW_CONNECT, L"连接", -1, 6));
	pPanelSewConnect->Add(new CMFCRibbonButton(ID_SEW_BREAK, L"断开连接", -1, 7));

	// 创建“缝纫机控制”面板
	CMFCRibbonPanel* pPanelSewControl = pCategoryHome->AddPanel(L"缝纫机管理", m_PanelImages.ExtractIcon(27));
	pPanelSewControl->Add(new CMFCRibbonButton(ID_SEWINGINFO_SET, L"缝制信息", -1, 0));
	pPanelSewControl->Add(new CMFCRibbonButton(ID_SEWFILE_SET, L"花样选择", -1, 1));
	pPanelSewControl->Add(new CMFCRibbonButton(ID_SEW_EDIT, L"图形编辑", -1, 2));
	pPanelSewControl->Add(new CMFCRibbonButton(ID_FILE_MANAGE, L"文件管理", -1, 3));
	pPanelSewControl->Add(new CMFCRibbonButton(ID_PARAM_CONFIG, L"参数设置", -1, 4));
	pPanelSewControl->Add(new CMFCRibbonButton(ID_IO_CHECK, L"IO检测", -1, 5));

	// 创建和添加“视图”面板:
	/*bNameValid = strTemp.LoadString(IDS_RIBBON_VIEW);
	ASSERT(bNameValid);
	CMFCRibbonPanel* pPanelView = pCategoryHome->AddPanel(strTemp, m_PanelImages.ExtractIcon (7));

	bNameValid = strTemp.LoadString(IDS_RIBBON_STATUSBAR);
	ASSERT(bNameValid);
	CMFCRibbonButton* pBtnStatusBar = new CMFCRibbonCheckBox(ID_VIEW_STATUS_BAR, strTemp);
	pPanelView->Add(pBtnStatusBar);*/

	// 将元素添加到选项卡右侧:
	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE);
	ASSERT(bNameValid);
	CMFCRibbonButton* pVisualStyleButton = new CMFCRibbonButton(-1, strTemp, -1, -1);

	pVisualStyleButton->SetMenu(IDR_THEME_MENU, FALSE /* 无默认命令*/, TRUE /* 右对齐*/);

	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE_TIP);
	ASSERT(bNameValid);
	pVisualStyleButton->SetToolTipText(strTemp);
	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE_DESC);
	ASSERT(bNameValid);
	pVisualStyleButton->SetDescription(strTemp);
	m_wndRibbonBar.AddToTabs(pVisualStyleButton);

	// 添加快速访问工具栏命令:
	/*CList<UINT, UINT> lstQATCmds;
	lstQATCmds.AddTail(ID_FILE_NEW);
	lstQATCmds.AddTail(ID_FILE_OPEN);
	lstQATCmds.AddTail(ID_FILE_SAVE);
	m_wndRibbonBar.SetQuickAccessCommands(lstQATCmds);*/

	m_wndRibbonBar.AddToTabs(new CMFCRibbonButton(ID_APP_ABOUT, _T("\na"), m_PanelImages.ExtractIcon (0)));
}

// 创建停靠窗口
BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// 创建缝制信息窗口
	CString strSewingInfoView;
	bNameValid = strSewingInfoView.LoadString(IDS_SEWINGINFO_VIEW);
	ASSERT(bNameValid);
	if (!m_wndSewingInfo.Create(strSewingInfoView, this, CRect(0, 0, 0, 0), TRUE, ID_VIEW_SEWINGINFO, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS| WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“缝制信息”窗口\n");
		return FALSE;
	}
	m_wndSewingInfo.EnableDocking(CBRS_ALIGN_ANY);
	m_wndSewingInfo.ShowPane(FALSE, FALSE, FALSE);

	// 创建花样选择窗口
	CString strSewFileView;
	bNameValid = strSewFileView.LoadString(IDS_SEWFILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndSewFile.Create(strSewFileView, this, CRect(0, 0, 0, 0), TRUE, ID_VIEW_SEWFILE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“花样选择”窗口\n");
		return FALSE;
	}
	m_wndSewFile.EnableDocking(CBRS_ALIGN_ANY);
	m_wndSewFile.ShowPane(FALSE, FALSE, FALSE);

	// 创建IO检测窗口
	if (!m_wndIOCheck.Create(L"IO检测", this, CRect(0, 0, 0, 0), TRUE, ID_VIEW_IOCHECK, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“IO检测”窗口\n");
		return FALSE;
	}
	m_wndIOCheck.EnableDocking(CBRS_ALIGN_ANY);
	m_wndIOCheck.ShowPane(FALSE, FALSE, FALSE);

	// 创建参数设置窗口
	if (!m_wndParaConfig.Create(L"参数设置", this, CRect(0, 0, 0, 0), TRUE, ID_VIEW_PARACONFIG, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“参数设置”窗口\n");
		return FALSE;
	}
	m_wndParaConfig.EnableDocking(CBRS_ALIGN_ANY);
	m_wndParaConfig.ShowPane(FALSE, FALSE, FALSE);

	if (!m_wndFileManage.Create(L"文件管理-NC", this, CRect(0, 0, 0, 0), TRUE, ID_VIEW_FILEMANAGE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“文件管理”窗口\n");
		return FALSE;
	}
	m_wndFileManage.EnableDocking(CBRS_ALIGN_ANY);
	m_wndFileManage.ShowPane(FALSE, FALSE, FALSE);

	CString strTitle;
	strTitle.Format(L"文件管理-PC(%s)", GetConfigOption(L"INIT", L"LOCALFOLDER"));
	if (!m_wndLocalFileView.Create(strTitle, this, CRect(0, 0, 0, 0), TRUE, ID_VIEW_FILE_LOCAL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“本地文件”窗口\n");
		return FALSE;
	}
	m_wndLocalFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndLocalFileView.ShowPane(FALSE, FALSE, FALSE);

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	
	return TRUE;
}

// 关闭当前功能视图
void CMainFrame::CloseCurrentPane()
{
	if (m_pPaneCurrent != NULL)
	{
		if (::IsWindow(m_pPaneCurrent->m_hWnd))
		{
			m_pPaneCurrent->ShowPane(FALSE, FALSE, FALSE);
		}
	}
	if (m_pPaneSecond != NULL)
	{
		if (::IsWindow(m_pPaneSecond->m_hWnd))
		{
			m_pPaneSecond->ShowPane(FALSE, FALSE, FALSE);
		}
	}
	m_nCurrentMode = MT_NULL;
}

// 显示进度条
void CMainFrame::ShowProgressBar(CString strTips)
{
	if (IsWindow(m_dlgWaiting.m_hWnd))
	{
		m_dlgWaiting.SetInfo(strTips);
	}
	else
	{
		m_dlgWaiting.Create();
		m_dlgWaiting.SetInfo(strTips);
		this->EnableWindow(FALSE);
	}
}

// 关闭进度条
void CMainFrame::CloseProgressBar()
{
	if (IsWindow(m_dlgWaiting.m_hWnd))
	{
		m_dlgWaiting.DestroyWindow();
		this->EnableWindow(TRUE);
		this->SetFocus();
	}
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	/*HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);*/

	HICON hSewFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndSewFile.SetIcon(hSewFileViewIcon, FALSE);

	HICON hSewingInfoBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndSewingInfo.SetIcon(hSewingInfoBarIcon, FALSE);
	m_wndIOCheck.SetIcon(hSewingInfoBarIcon, FALSE);
	m_wndParaConfig.SetIcon(hSewingInfoBarIcon, FALSE);
	m_wndFileManage.SetIcon(hSewingInfoBarIcon, FALSE);
}

// 关闭停靠窗口
BOOL CMainFrame::OnCloseDockingPane(CDockablePane* pWnd)
{
	if (m_nCurrentMode == MT_IO_CHECK)
	{
		// 关闭计时器
		m_wndIOCheck.EndTimer();

		// 切换回通常模式
		BYTE data[] = {0x00, 0x00};
		CProtocol col(data, 2, 0x08, 0x00, 0xE5);
		m_port.WriteProtocol(&col);
	}
	else if ((pWnd == &m_wndLocalFileView && !m_wndFileManage.IsVisible()) ||
		(pWnd == &m_wndFileManage && !m_wndLocalFileView.IsVisible()))
	{
		m_statePreview = PS_CLOSE;
		m_nCurrentMode = MT_NULL;
		m_strFileToEdit = L"";
		return TRUE;
	}

	m_statePreview = PS_CLOSE;
	m_nCurrentMode = MT_NULL;
	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG

//===================================================================================================

// CMainFrame 消息处理程序

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

// 计时器
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case ID_TIMER_WAITANSWER:
		{
			// 关闭计时器
			EndWaitingTimer();

			// 关闭进度条
			CloseProgressBar();

			BYTE cmdCode = m_listProcessCol.front().m_commandCode;
			if (cmdCode == 0x01)	// 连接超时
			{
				m_port.CloseListenTread();
				m_port.SetIsListen(false);
				m_port.ClosePort();

				AfxMessageBox(L"连接超时！");
			}
			else
			{
				AfxMessageBox(L"NC端无响应！当前操作将终止");
			}
			
			// 将当前指令加入结束指令序列
			CodeRecode recode(0xE5, m_listProcessCol.front().m_commandCode, FALSE);
			m_vecFinishedCol.push_back(recode);
			
			// 初始化队列数据
			InitQueueData();
		}
		break;
	default:
		break;
	}

	CWnd::OnTimer(nIDEvent);
}

// 连接或断开连接时更新控件状态
void CMainFrame::OnUpdateConnectOrBreak(CCmdUI* pCmdUI)
{
	if (m_port.m_bIsListen && m_bConnected)
	{
		if (pCmdUI->m_nID == ID_SEW_CONNECT)
		{
			pCmdUI->Enable(FALSE);

			// 通讯连接时的窗口标题
			CString strTitle;
			strTitle.Format(L"CNCSewingControl - 双针缝纫机PC端控制软件（已连接设备：%s）", m_strDeviceName);
			SetWindowText(strTitle);
		}
		else
		{
			pCmdUI->Enable(TRUE);
		}
	}
	else
	{
		if (pCmdUI->m_nID == ID_SEW_CONNECT)
		{
			pCmdUI->Enable(TRUE);

			// 通讯断开时的窗口标题
			SetWindowText(L"CNCSewingControl - 双针缝纫机PC端控制软件（无设备连接）");
		}
		else
		{
			if (!m_bDebugMode)
			{
				pCmdUI->Enable(FALSE);
			}
		}
	}
}

//void CMainFrame::OnUpdateSaveBtnState(CCmdUI* pCmdUI)
//{
//	int nItem = pCmdUI->m_nID-IDC_FIRSTBTN_SAVE_ID;
//	if (nItem > 0 && nItem < m_wndSewingInfo.m_wndInfoList.GetPropertyCount())
//	{
//		CUserPropertyGridProperty* pProp = (CUserPropertyGridProperty*)m_wndSewingInfo.m_wndInfoList.GetProperty(nItem);
//		pCmdUI->Enable(!pProp->m_bNeedSave);
//	}
//}

// 显示缝纫信息窗口
void CMainFrame::OnSewinginfoView()
{
	if (!m_wndSewingInfo.IsVisible())
	{
		CloseCurrentPane();
		PreviewCurrentSew();

		CRect rectFrm;
		this->GetClientRect(rectFrm);
		m_wndSewingInfo.DockToFrameWindow(CBRS_ALIGN_LEFT, CRect(0, 0, rectFrm.Width()/3, 0));
		m_wndSewingInfo.ShowPane(TRUE, FALSE, TRUE);
		m_pPaneCurrent = &m_wndSewingInfo;
		m_pPaneSecond = NULL;

		m_nCurrentMode = MT_SEWINGINFO;
	}
}

// 显示当前花样文件设置窗口
void CMainFrame::OnCurrentSewfileView()
{
	if (!m_wndSewFile.IsVisible())
	{
		CloseCurrentPane();
		PreviewCurrentSew();

		CRect rectFrm;
		this->GetClientRect(rectFrm);
		m_wndSewFile.DockToFrameWindow(CBRS_ALIGN_LEFT, CRect(0, 0, rectFrm.Width()/3, 0));
		m_wndSewFile.ShowPane(TRUE, FALSE, TRUE);
		m_pPaneCurrent = &m_wndSewFile;
		m_pPaneSecond = NULL;

		m_nCurrentMode = MT_CURRENT_SEWFILE;
	}
}

// 显示花样文件编辑窗口
void CMainFrame::OnSewEditView()
{
	CString strProgramPath;
	strProgramPath = GetConfigOption(L"INIT", L"EDITPROGRAM");
	int nError = (int)ShellExecute(0, L"open", strProgramPath, m_strFileToEdit, NULL, SW_SHOW);
	if (nError < 32)
	{
		AfxMessageBox(L"花样编辑程序启动失败！");
	}
}

// 显示花样文件管理窗口
void CMainFrame::OnFileManageView()
{
	if (!m_wndFileManage.IsVisible())
	{
		CloseCurrentPane();

		m_statePreview = PS_PREPARE;

		CRect rectFrm;
		this->GetClientRect(rectFrm);
		m_wndFileManage.DockToFrameWindow(CBRS_ALIGN_LEFT, CRect(0, 0, rectFrm.Width()/3, 0));
		m_wndFileManage.ShowPane(TRUE, FALSE, TRUE);

		m_wndLocalFileView.DockToFrameWindow(CBRS_ALIGN_RIGHT, CRect(0, 0, rectFrm.Width()/3, 0));
		m_wndLocalFileView.ShowPane(TRUE, FALSE, TRUE);
		m_wndLocalFileView.UpdateFileList();

		m_pPaneCurrent = &m_wndFileManage;
		m_pPaneSecond = &m_wndLocalFileView;

		m_nCurrentMode = MT_SEWFILE_MANAGE;
	}
}

// 显示参数设置窗口
void CMainFrame::OnParamConfigView()
{
	if (!m_wndParaConfig.IsVisible())
	{
		CloseCurrentPane();
		PreviewCurrentSew();

		CRect rectFrm;
		this->GetClientRect(rectFrm);
		m_wndParaConfig.DockToFrameWindow(CBRS_ALIGN_LEFT, CRect(0, 0, rectFrm.Width()/3, 0));
		m_wndParaConfig.ShowPane(TRUE, FALSE, TRUE);
		m_pPaneCurrent = &m_wndParaConfig;
		m_pPaneSecond = NULL;

		m_nCurrentMode = MT_PARAM_SET;
	}
}

// 显示IO检测窗口
void CMainFrame::OnIoCheckView()
{
	if (!m_wndIOCheck.IsVisible())
	{
		CloseCurrentPane();
		PreviewCurrentSew();

		BYTE data[] = {0x00, 0x01};
		CProtocol col(data, 2, 0x08, 0x00, 0xE5);
		m_port.WriteProtocol(&col);

		m_wndIOCheck.StartTimer();

		CRect rectFrm;
		this->GetClientRect(rectFrm);
		m_wndIOCheck.DockToFrameWindow(CBRS_ALIGN_LEFT, CRect(0, 0, rectFrm.Width()/3, 0));
		m_wndIOCheck.ShowPane(TRUE, FALSE, TRUE);
		m_pPaneCurrent = &m_wndIOCheck;
		m_pPaneSecond = NULL;
	
		m_nCurrentMode = MT_IO_CHECK;
	}
}

// 通讯连接
void CMainFrame::OnSewConnect()
{
	if (!RequestConnect())
	{
		CDlgConnect dlgConnect;
		if (dlgConnect.DoModal() == IDOK)
		{
			m_uPortID = dlgConnect.m_nPort;
			if (RequestConnect())
			{
				// 记录端口号
				SetConfigOption(L"CONNECT", L"PORTID", m_uPortID);
			}
		}
	}
}

// 通讯断开
void CMainFrame::OnSewBreak()
{
	if (MessageBox(L"确认断开？", L"提示", MB_YESNO|MB_ICONINFORMATION) == IDYES)
	{
		m_port.CloseListenTread();
		m_port.SetIsListen(false);
		m_port.ClosePort();
	}
}

// 接收数据
LRESULT CMainFrame::OnReceiveData(WPARAM wParam, LPARAM lParam)
{
	BYTE byteReceiveCode = (int)wParam;
	BYTE* pData = (BYTE*)lParam;

	// 校验数据，如果队列为空或者与队列中的指令不匹配，则忽视
	if (m_listProcessCol.empty())
		return NULL;
	if (m_listProcessCol.front().m_receiveCode != byteReceiveCode)
		return NULL;

	// 关闭计时器
	EndWaitingTimer();

	switch (byteReceiveCode)
	{
	case 0x01:
		m_bConnected = TRUE;		// 已建立通讯连接
		// 无其他操作
		break;
	case 0x1D:
		UpdateDeviceInfo(pData);	// 更新设备信息
		break;
	case 0xA8:
		UpdateSewingInfo(pData);	// 更新缝制信息
		break;
	case 0x73:
		if (!UpdateFileList(pData))		// 更新文件列表
		{
			// 在当前指令后面插入一条指令
			BYTE dataFiles[] = {0x53, pData[0]+1};
			CProtocol colFiles(dataFiles, 0x02, 0x72, 0x01, 0xE5, 0x73);
			list<CProtocol>::iterator i = m_listProcessCol.begin();
			m_listProcessCol.insert(++i, colFiles);
		}
		break;
	case 0x27:
		if (!UpdateParamList(pData))	// 更新参数列表
		{
			BYTE byteHight = m_listProcessCol.front().m_data[1];
			BYTE byteLow = m_listProcessCol.front().m_data[2];
			if (byteLow < 0xFF)
			{
				byteLow += 1;
			}
			else
			{
				byteHight += 1;
				byteLow = 0x00;
			}
			BYTE dataParam[] = {0x01, byteHight, byteLow};
			CProtocol colParams(dataParam, 0x03, 0x27, 0x03, 0xE5);
			list<CProtocol>::iterator i = m_listProcessCol.begin();
			m_listProcessCol.insert(++i, colParams);
		}
		break;
	case 0x26:
		if (pData[0] == 0 && pData[1] == 0)	// 设置参数
		{
			m_wndParaConfig.SetSuccess();
		}
		break;
	case 0x77:
		if (!ReceiveSewfileData(pData))	// 接收花样文件数据
		{
			CProtocol colCopy = m_listProcessCol.front();
			colCopy.m_data[3] += 1;
			list<CProtocol>::iterator i = m_listProcessCol.begin();
			m_listProcessCol.insert(++i, colCopy);
		}
		break;
	case 0x7C:	// 文件删除反馈
		break;
	case 0x79:	// 文件创建及数据上传反馈
		break;
	case 0xA9:	// 查询或设置当前花样号
		m_nCmdType = CT_CURRENT_SEW;
		if (m_listProcessCol.front().m_data[0] == 0xFF && m_listProcessCol.front().m_data[1] == 0xFF)
		{
			if (pData[1]!=0 && pData[2]!=0 && pData[3]!=0)
			{
				// 查询状态下自动发起下载指令
				BYTE data[] = {0x53, pData[2], pData[3], 0x00};	// 数据块从0x00开始
				CProtocol col(data, 4, 0x76, 0x03, 0xE5, 0x77);
				list<CProtocol>::iterator i = m_listProcessCol.begin();
				m_listProcessCol.insert(++i, col);
			}
		}
		break;
	case 0x09:	// 输入端口查询及设置
		{
			ByteArray arrByte;
			arrByte.resize(4);
			arrByte[0] = pData[3];
			arrByte[1] = pData[2];
			arrByte[2] = pData[1];
			arrByte[3] = pData[0];
			vector<int> arrBin;
			HexToBin(arrByte, arrBin);
			m_wndIOCheck.ReceiveIOData(arrBin);
		}
		break;
	case 0x0A:	// 电机输出测试
		if (pData[0] == 0xFF)
		{
			AfxMessageBox(L"当前机器不支持该轴");
		}
		break;
	default:
		break;
	}

	// 记录当前指令
	CodeRecode recodeRequest(0xE5, m_listProcessCol.front().m_commandCode, TRUE);
	CodeRecode recodeAnswer(0xF6, byteReceiveCode, TRUE);
	m_vecFinishedCol.push_back(recodeRequest);
	m_vecFinishedCol.push_back(recodeAnswer);

	// 将当前指令移出队列
	m_listProcessCol.pop_front();

	// 执行下一条指令
	Process();

	return NULL;
}

// 获取参数列表
LRESULT CMainFrame::OnGetParamList(WPARAM wParam, LPARAM lParam)
{
	wParam;
	lParam;

	if (!IsProcessing())
	{
		ShowProgressBar(L"正在同步参数列表，请稍候...");

		// 获取参数列表(0x27)
		BYTE dataParams[] = {0x01, 0x00, 0x00};
		CProtocol colParams(dataParams, 0x03, 0x27, 0x03, 0xE5);
		m_listProcessCol.push_back(colParams);

		// 开始执行
		Process();
	}
	return NULL;
}

// 设置参数
LRESULT CMainFrame::OnSaveParam(WPARAM wParam, LPARAM lParam)
{
	wParam;
	ParaInfoArray& arrPara = *(ParaInfoArray*)lParam;

	if (!IsProcessing())
	{
		ShowProgressBar(L"正在设置参数，请稍候...");
		AddParamSaveCol(arrPara);

		// 开始执行
		Process();
	}
	
	return NULL;
}

// 上传SEW文件
LRESULT CMainFrame::OnUploadSewFile(WPARAM wParam, LPARAM lParam)
{
	m_nCmdType = CT_UPLOAD_SEWFILE;
	wParam;

	if (!IsProcessing())
	{
		ShowProgressBar(L"正在上传文件，请稍候...");
	
		SewFileData& data = *(SewFileData*)lParam;
		AddSewfileCreateCol(data.nID);
		AddUploadFileDataCol(data);

		SewFileDataArray arr;
		arr.push_back(data);
		m_wndFileManage.AddFileToList(arr);
		m_wndSewFile.AddFileToList(arr);

		// 开始执行
		Process();
	}

	return NULL;
}

// 读取文件列表
LRESULT CMainFrame::OnGetFileList(WPARAM wParam, LPARAM lParam)
{
	wParam;
	lParam;

	if (!IsProcessing())
	{
		ShowProgressBar(L"正在更新文件列表，请稍候...");

		AddFilesListGetCol();

		// 开始执行
		Process();
	}
	return NULL;
}

// 下载花样文件
LRESULT CMainFrame::OnDownloadSewfile(WPARAM wParam, LPARAM lParam)
{
	m_nCmdType = CT_DOWNLOAD_SEWFILE;
	
	wParam;
	vector<int>& arrID = *(vector<int>*)lParam;
	
	if (!IsProcessing())
	{
		ShowProgressBar(L"正在下载文件，请稍候...");
		
		for (int i=0; i<arrID.size(); i++)
		{
			AddSewfileDataGetCol(arrID[i]);
			InitFiledataBuff();
			m_buffFileTrans.nID = arrID[i];
		}

		// 开始执行
		Process();
	}
	
	return NULL;
}

// 重命名花样文件
LRESULT CMainFrame::OnRenameSewFile(WPARAM wParam, LPARAM lParam)
{
	m_nCmdType = CT_RENAME_SEWFILE;
	
	int nOldID = (int)wParam;
	int nNewID = (int)lParam;

	if (!IsProcessing())
	{
		ShowProgressBar(L"正在重命名文件，请稍候...");

		InitFiledataBuff();
		m_buffFileTrans.nID = nOldID;

		AddSewfileDataGetCol(nOldID);	// 先按旧文件名下载
		AddSewfileDeleteCol(nOldID);	// 删除旧文件
		AddSewfileCreateCol(nNewID);	// 创建新文件

		// 开始执行
		Process();
	}

	return NULL;
}

// 删除花样文件
LRESULT CMainFrame::OnDeleteSewFile(WPARAM wParam, LPARAM lParam)
{
	m_nCmdType = CT_DELETE_SEWFILE;
	wParam;
	vector<int>& arrID = *(vector<int>*)lParam;

	if (!IsProcessing())
	{
		ShowProgressBar(L"正在删除文件，请稍候...");

		for (int i=0; i<arrID.size(); i++)
		{
			AddSewfileDeleteCol(arrID[i]);
		}

		// 开始执行
		Process();
	}
	
	return NULL;
}

// 设置当前花样
LRESULT CMainFrame::OnSetCurrSewFile(WPARAM wParam, LPARAM lParam)
{
	lParam;
	int nID = (int)wParam;

	if (!IsProcessing())
	{
		ShowProgressBar(L"正在切换当前花样，请稍候...");

		AddCurrSewFileSetCol(nID);
		AddSewfileDataGetCol(nID);

		// 开始执行
		Process();
	}

	return NULL;
}

// 请求IO检测(输入)
LRESULT CMainFrame::OnCheckInputPort(WPARAM wParam, LPARAM lParam)
{
	wParam;
	lParam;

	RequestGetSpecialInputPort();
	return NULL;
}

// 请求IO检测(输出)
LRESULT CMainFrame::RequestOutputCheck(WPARAM wParam, LPARAM lParam)
{
	wParam;
	IOCheckData& iodata = *(IOCheckData*)lParam;

	// 发送指令
	if (iodata.nMode == 11)
	{
		RequestSetCommonOutputPort(iodata);
	}
	else if (iodata.nMode == 12)
	{
		RequestSetElectroOutputPort(iodata);
	}
	else if (iodata.nMode == 10)
	{
		RequestDrivenTest(iodata);
	}
	return NULL;
}

LRESULT CMainFrame::RequestSetSewingInfo(WPARAM wParam, LPARAM lParam)
{
	int nIndex = (int)wParam;
	int nVal = (int)lParam;

	switch (nIndex)
	{
	case 0:	// 主轴转速
		RequestSetSpindleSpeed(nVal);
		break;
	case 1:	// 底线计数器
		RequestSetBottomLineCounter(nVal);
		break;
	case 2:	// 工件计数器
		RequestSetWorkpiectCounter(nVal);
		break;
	case 3:	// 中压脚高度
		break;
	case 4:	// 线张力A
		RequestSetLineTension(nVal, 0x01);
		break;
	case 5:	// 线张力B
		RequestSetLineTension(nVal, 0x02);
		break;
	default:
		break;
	}
	return NULL;
}

// 更新花样预览数据
LRESULT CMainFrame::OnUpdatePreview(WPARAM wParam, LPARAM lParam)
{
	int nState = (int)wParam;
	if (nState == PS_RECEIVE)
	{
		if (lParam == NULL)
		{
			return NULL;
		}
		ByteArray &arrByte = *(ByteArray*)lParam;
		if (arrByte.empty())
		{
			return NULL;
		}
		NeedleTrailData trailTemp;
		if (!AnalysisSewFile(arrByte, trailTemp))
		{
			m_statePreview = PS_INVALID;
		}
		else
		{
			m_trailToPreview = trailTemp;
			m_statePreview = PS_VALID;
		}
	}
	else if (nState == PS_CLOSE)
	{
		m_statePreview = PS_CLOSE;
	}
	else if (nState == PS_PREPARE)
	{
		m_statePreview = PS_PREPARE;
	}
	this->GetActiveDocument()->UpdateAllViews(NULL);
	return NULL;
}

//===================================================================================================

// CMainFrame 操作

// 初始化队列数据
void CMainFrame::InitQueueData()
{
	m_listProcessCol.clear();
	m_listWaitCol.clear();
	EndWaitingTimer();
}

// 执行队列指令
void CMainFrame::Process()
{
	if (m_listProcessCol.empty())
	{
		if (!m_listWaitCol.empty())
		{
			// 将当前等待区的指令全部移到到执行列表中
			while (!m_listWaitCol.empty())
			{
				m_listProcessCol.push_back(m_listWaitCol.front());
				m_listWaitCol.pop_front();
			}
		}
		else
		{
			if (m_nCmdType == CT_RENAME_SEWFILE || m_nCmdType == CT_DELETE_SEWFILE || m_nCmdType == CT_UPLOAD_SEWFILE)
			{
				m_wndFileManage.SetFileList();
				m_wndSewFile.FillSewFileList();
			}
			CloseProgressBar();
			m_nCmdType = CT_NULL;
			return;
		}
	}

	// 发送指令
	m_port.WriteProtocol(&m_listProcessCol.front());

	// 同时启动计时器，如果超过指定时间无反应，则提示NC无响应
	StartWaitingTimer();
}

// 启动等待计时
void CMainFrame::StartWaitingTimer()
{
	if (m_bTimerStartFlag)
	{
		KillTimer(ID_TIMER_WAITANSWER);
		m_bTimerStartFlag = FALSE;
	}
	int nSec = GetConfigOptionINT(L"CONNECT", L"WAITANSWER")*1000;
	SetTimer(ID_TIMER_WAITANSWER, nSec, 0);
	m_bTimerStartFlag = TRUE;
}

// 关闭等待计时
void CMainFrame::EndWaitingTimer()
{
	if (m_bTimerStartFlag)
	{
		KillTimer(ID_TIMER_WAITANSWER);
		m_bTimerStartFlag = FALSE;
	}
}

// 初始化文件数据缓存
void CMainFrame::InitFiledataBuff()
{
	m_buffFileTrans.arrByte.clear();
	m_buffFileTrans.arrBlockSize.clear();
	m_buffFileTrans.nID = -1;
}

// 检查是否在执行中
BOOL CMainFrame::IsProcessing()
{
	return !m_listProcessCol.empty();
}

// 请求连接（0x01）
BOOL CMainFrame::RequestConnect()
{
	if (m_uPortID <= 0)
		return FALSE;

	CString strMsg;
	if (!m_port.m_bIsInit)
	{
		if (m_port.InitPort(m_uPortID))
		{
			if (m_port.OpenListenThread())
			{
				m_port.SetIsListen(true);

				ShowProgressBar(L"连接并获取设备数据中，请稍候...");

				// 数据初始化
				InitQueueData();

				// 准备完毕(0x01)
				CProtocol colReady(NULL, 0, 0x01, 0x00, 0xE5);
				m_listProcessCol.push_back(colReady);

				// 获取设备信息(0x1C)
				AddDeviceInfoGetCol();

				// 获取缝制信息(0xA8)
				CProtocol colSewing(NULL, 0, 0xA8, 0x00, 0xE5);
				m_listProcessCol.push_back(colSewing);

				// 获取文件列表(0x72)
				AddFilesListGetCol();
				
				// 获取参数列表(0x27)
				AddParamListGetCol();

				// 获取当前花样文件
				AddCurrSewFileGetCol();
	
				// 开始执行
				Process();
			}
			else
			{
				strMsg.Format(_T("打开监听%d失败"), m_uPortID);
				AfxMessageBox(strMsg);
				return FALSE;
			}
		}
		else
		{
			strMsg.Format(_T("初始端口%d失败"), m_uPortID);
			AfxMessageBox(strMsg);
			return FALSE;
		}
	}
	return TRUE;
}

// 请求获取设备信息（0x1C）
void CMainFrame::AddDeviceInfoGetCol()
{
	CProtocol colDevice(NULL, 0, 0x1C, 0x00, 0xE5, 0x1D);
	m_listProcessCol.push_back(colDevice);
}

// 接收设备信息（0x1D）
void CMainFrame::UpdateDeviceInfo(BYTE* pData)
{
	// 前几位表示机器型号（不知道具体有几位，暂定遇到0就结束）。每一位对应一个字母的ASCII码值
	int nPos = 0;
	char ch[128] = {0};
	char temp;
	while (1)
	{
		// 十六进制转ASCII
		temp= pData[nPos];
		if (temp == 0)
			break;

		ch[nPos] = temp;
		nPos++;
	}

	// 将设备名称更新到标题栏
	m_strDeviceName = ch;

	// 保存到配置文件中
	SetConfigOption(L"CONNECT", L"DEVICENAME", m_strDeviceName);
}

// 请求参数信息（0x27）
void CMainFrame::AddParamListGetCol()
{
	BYTE dataParams[] = {0x01, 0x00, 0x00};
	CProtocol colParams(dataParams, 0x03, 0x27, 0x03, 0xE5);
	m_listProcessCol.push_back(colParams);
}

// 接收参数信息（0x27）
BOOL CMainFrame::UpdateParamList(BYTE* pData)
{
	ParaInfo para;
	if (pData[1] == 0xff || pData[2] == 0xff)	// 结束
	{
		m_wndParaConfig.SetViewState(FALSE);
		m_wndParaConfig.UpdataParam(para, TRUE);
		return TRUE;
	}
	
	ByteArray arrByte;
	arrByte.resize(2);
	arrByte[0] = pData[2];
	arrByte[1] = pData[1];
	para.nType = Hex2Dec(arrByte);
	arrByte[0] = pData[4];
	arrByte[1] = pData[3];
	para.nID = Hex2Dec(arrByte);
	arrByte[0] = pData[6];
	arrByte[1] = pData[5];
	para.nCurrentValue = Hex2Dec(arrByte);
	arrByte[0] = pData[8];
	arrByte[1] = pData[7];
	para.nDefaultValue = Hex2Dec(arrByte);
	arrByte[0] = pData[10];
	arrByte[1] = pData[9];
	para.nMinValue = Hex2Dec(arrByte);
	arrByte[0] = pData[12];
	arrByte[1] = pData[11];
	para.nMaxValue = Hex2Dec(arrByte);
	int nNextIndex = m_wndParaConfig.UpdataParam(para, FALSE);
	return FALSE;
}

// 添加保存参数指令（0x26）
void CMainFrame::AddParamSaveCol(const ParaInfoArray& arrPara)
{
	int nParaCount = (int)arrPara.size();
	BYTE* bytedata = new BYTE[4*nParaCount+2];
	ByteArray arrByte;
	arrByte.resize(2);

	DecToHex(nParaCount*4, arrByte);
	if (arrByte.size() == 1)
	{
		arrByte.push_back(0x00);
	}
	bytedata[0] = arrByte[1];
	bytedata[1] = arrByte[0];

	for (int i=0; i<nParaCount; i++)
	{
		DecToHex(arrPara[i].nID, arrByte);
		if (arrByte.size() == 1)
		{
			arrByte.push_back(0x00);
		}
		bytedata[4*i+2] = arrByte[1];
		bytedata[4*i+3] = arrByte[0];

		DecToHex(arrPara[i].nCurrentValue, arrByte);
		if (arrByte.size() == 1)
		{
			arrByte.push_back(0x00);
		}
		bytedata[4*i+4] = arrByte[1];
		bytedata[4*i+5] = arrByte[0];
	}
	// 将参数值写入缓冲区
	CProtocol col1(bytedata, 4*nParaCount+2, 0x26, 0x00, 0xE5);
	m_listProcessCol.push_back(col1);

	BYTE data2[] = {0x00, 0x00};
	CProtocol col2(data2, 2, 0x26, 0x00, 0xE5);
	m_listProcessCol.push_back(col2);
}

// 添加文件创建指令（0x78）
void CMainFrame::AddSewfileCreateCol(int nFileID)
{
	//if (m_arrFileBuffer.size() == 0)
	//	return;

	//// 从队首取文件
	//SewFileData& sewData = m_arrFileBuffer[0];

	// 识别文件的ID
	ByteArray arrByte;
	DecToHex(nFileID, arrByte);
	if (arrByte.size() == 1)
	{
		arrByte.push_back(0x00);
	}
	m_nBlockIndex = 0;

	BYTE byteCreate[] = {0x53, arrByte[1], arrByte[0], 0x00};	// 第4个数表示模式
	CProtocol colCreate(byteCreate, 4, 0x78, 0x01, 0xE5, 0x79);
	m_listProcessCol.push_back(colCreate);

	InitFiledataBuff();
	m_buffFileTrans.nID = nFileID;
}

// 添加文上传指令(0x7A)
void CMainFrame::AddUploadFileDataCol(const SewFileData& sewData)
{
	int nByteCount = (int)sewData.arrByte.size();
	int nBlockSize = nByteCount/123;
	int nLastCount = nByteCount%123;
	if (nLastCount > 0)
		nBlockSize += 1;

	BYTE byteData[126];
	byteData[0] = 0;	// 块序号
	byteData[1] = 0x00;
	int nCount;
	for (int i=0, nIndex = 0; i<nBlockSize; i++)
	{
		if (i==(nBlockSize-1))
		{
			byteData[1] = 0x01;	// 中止
			nCount = nLastCount;
		}
		else
			nCount = 123;
		byteData[2] = nCount;	// 数量

		for (int j=0; j<nCount; j++)
		{
			byteData[3+j] = sewData.arrByte[nIndex+j];
		}
		nIndex += nCount;	// 下一个块从nIndex开始填充数据
		CProtocol colData(byteData, 126, 0x7A, 0x01, 0xE5, 0x79);
		m_listProcessCol.push_back(colData);

		byteData[0] += 1;
	}
}

// 文件列表获取指令（0x72）
void CMainFrame::AddFilesListGetCol()
{
	// 获取文件列表(0x72)
	BYTE dataFiles[] = {0x53, 0x00};
	CProtocol colFiles(dataFiles, 0x02, 0x72, 0x01, 0xE5, 0x73);
	m_listProcessCol.push_back(colFiles);
}

// 接收文件列表(0x73)
BOOL CMainFrame::UpdateFileList(BYTE* pData)
{
	SewFileDataArray arrFileList;

	// 第3个数表示花样数量
	ByteArray arrByte;
	arrByte.push_back(pData[2]);
	int nCount = Hex2Dec(arrByte);
	arrByte.resize(2);
	for (int i=0; i<nCount; i++)
	{
		SewFileData si;
		arrByte[0] = pData[4+i*2];
		arrByte[1] = pData[3+i*2];
		si.nID = Hex2Dec(arrByte);
		arrFileList.push_back(si);
	}

	// 更新到文件管理模块以及花样选择模块
	m_wndFileManage.UpdateFileList(arrFileList, (pData[1] == 0x01));
	m_wndSewFile.UpdateFileList(arrFileList, (pData[1] == 0x01));

	// 0x01 返回TRUE， 0x00 返回FALSE
	BOOL bFinished = (pData[1] == 0x01);
	return bFinished;
}

// 花样文件数据下载指令（0x76）
void CMainFrame::AddSewfileDataGetCol(int nFileID, BYTE byteSecIndex)
{
	ByteArray arrByte;
	DecToHex(nFileID, arrByte);
	if (arrByte.size() == 1)
	{
		arrByte.push_back(0x00);
	}
	
	BYTE data[] = {0x53, arrByte[1], arrByte[0], byteSecIndex};	// 数据块从0x00开始
	CProtocol col(data, 4, 0x76, 0x03, 0xE5, 0x77);
	m_listProcessCol.push_back(col);
}

// 接收花样文件数据（0x77）
BOOL CMainFrame::ReceiveSewfileData(BYTE* pData)
{
	BOOL bFinished = FALSE;
	// 数据接收完毕后执行下载或者等待后续操作

	// 第3个数表示花样数据个数
	int nCount = pData[2];
	for (int i=3; i<nCount+3; i++)
	{
		m_buffFileTrans.arrByte.push_back(pData[i]);
	}
	m_buffFileTrans.arrBlockSize.push_back(nCount);

	// 第2个数表示区域块是否结束
	if (pData[1] == 0x01)
	{
		m_buffFileTrans.nID = m_listProcessCol.front().m_data[1]*256+m_listProcessCol.front().m_data[2];
		if (CT_DOWNLOAD_SEWFILE == m_nCmdType)
		{
			// 文件传输完毕，直接下载到本地
			m_wndLocalFileView.DownLoadSewFile(m_buffFileTrans);
		}
		else if (CT_RENAME_SEWFILE == m_nCmdType)	// 重命名
		{
			// 向队列增加数据上传指令
			BYTE byteData[126];
			byteData[0] = 0;	// 块序号
			byteData[1] = 0x00;
			for (int i=0, nIndex = 0; i<m_buffFileTrans.arrBlockSize.size(); i++)
			{
				if (i==(m_buffFileTrans.arrBlockSize.size()-1))
				{
					byteData[1] = 0x01;	// 中止
				}
				nCount = m_buffFileTrans.arrBlockSize[i];
				byteData[2] = nCount;	// 数量

				for (int j=0; j<nCount; j++)
				{
					byteData[3+j] = m_buffFileTrans.arrByte[nIndex+j];
				}
				nIndex += nCount;	// 下一个块从nIndex开始填充数据
				CProtocol colData(byteData, 126, 0x7A, 0x01, 0xE5, 0x79);
				m_listProcessCol.push_back(colData);

				byteData[0] += 1;	// 块序号
			}
		}
		else if (CT_CURRENT_SEW == m_nCmdType)
		{
			// 放入缓存区
			m_currentFile = m_buffFileTrans;
			m_wndSewFile.UpdateCurrSewFile(m_currentFile.nID);
		}
		else if (CT_PREVIEW_SEW == m_nCmdType)
		{
			if (m_nCurrentMode == MT_SEWFILE_MANAGE)
			{
				m_wndFileManage.ReceiveSewfileData(m_buffFileTrans.arrByte);
			}
			else
			{
				m_wndSewFile.ReceiveSewfileData(m_buffFileTrans.arrByte);
			}
		}
		// 初始化缓存
		InitFiledataBuff();
		bFinished = TRUE;
	}
	return bFinished;
}

// 文件删除指令(0x7B)
void CMainFrame::AddSewfileDeleteCol(int nFileID)
{
	ByteArray arrByte;
	DecToHex(nFileID, arrByte);
	if (arrByte.size() == 1)
	{
		arrByte.push_back(0x00);
	}

	BYTE data[] = {0x53, arrByte[1], arrByte[0]};
	CProtocol col(data, 0x03, 0x7B, 0x01, 0xE5, 0x7C);
	m_listProcessCol.push_back(col);
}

// 请求获取专用输入端口状态(0x09)
void CMainFrame::RequestGetSpecialInputPort()
{
	BYTE byte[] = {0x00, 0x01, 0xFF, 0xFF};
	CProtocol col(byte, 4, 0x09, 0x00, 0xE5);
	m_port.WriteProtocol(&col);
}

// 请求获取通用输入端口状态(0x09)
void CMainFrame::RequestSetCommonInputPort()
{
	BYTE byte[] = {0x00, 0x02, 0xFF, 0xFF};
	CProtocol col(byte, 4, 0x09, 0x00, 0xE5);
	m_port.WriteProtocol(&col);
}

// 请求修改通用输出端口状态(0x09)
void CMainFrame::RequestSetCommonOutputPort(const IOCheckData& iodata)
{
	BYTE byte[] = {0x00, 0x11, (BYTE)(iodata.nBit), (BYTE)iodata.nState};
	CProtocol col(byte, 4, 0x09, 0x00, 0xE5);
	m_port.WriteProtocol(&col);
}

// 请求修改输出电磁铁状态(0x09)
void CMainFrame::RequestSetElectroOutputPort(const IOCheckData& iodata)
{
	BYTE byte[] = {0x00, 0x12, (BYTE)(iodata.nBit), (BYTE)iodata.nState};
	CProtocol col(byte, 4, 0x09, 0x00, 0xE5);
	m_port.WriteProtocol(&col);
}

// 请求电机输出测试(0x0A)
void CMainFrame::RequestDrivenTest(const IOCheckData& iodata)
{
	ByteArray arrByte;
	if (iodata.nStep > 0)
	{
		DecToHex(iodata.nStep, arrByte);
		if (arrByte.size() == 1)
		{
			arrByte.push_back(0x00);
		}
	}
	else if (iodata.nStep < 0)
	{
		DecToHex((-1)*iodata.nStep, arrByte);
		if (arrByte.size() == 1)
		{
			arrByte.push_back(0x00);
		}
		// 如果是负数，计算补码
		ByteArray arrTemp;
		HexToCmt(arrByte, arrTemp);
		arrByte = arrTemp;
		if (arrByte.size() == 1)
		{
			arrByte.push_back(0xFF);
		}
	}
	else
	{
		return;
	}
	

	BYTE byte[] = {0x00, (BYTE)iodata.nBit, arrByte[1], arrByte[0]};
	CProtocol col(byte, 4, 0x0A, 0x00, 0xE5);
	m_port.WriteProtocol(&col);
}

// 请求设置主轴转速(0xAA)
void CMainFrame::RequestSetSpindleSpeed(int nVal)
{
	ByteArray arrByte;
	DecToHex(nVal, arrByte);
	if (arrByte.size() == 1)
	{
		arrByte.push_back(0x00);
	}

	BYTE data[] = {arrByte[1], arrByte[0]};
	CProtocol col(data, 2, 0xAA, 0x00, 0xE5);
	m_port.WriteProtocol(&col);
}

// 请求设置底线计数器(0xAB)
void CMainFrame::RequestSetBottomLineCounter(int nVal)
{
	ByteArray arrByte;
	DecToHex(nVal, arrByte);
	if (arrByte.size() == 1)
	{
		arrByte.push_back(0x00);
	}

	BYTE data[] = {0xFF, 0xFF, arrByte[1], arrByte[0], 0xFF, 0xFF, 0xFF, 0xFF};
	CProtocol col(data, 8, 0xAB, 0x00, 0xE5);
	m_port.WriteProtocol(&col);
}

// 请求设置工件计数器(0xAB)
void CMainFrame::RequestSetWorkpiectCounter(int nVal)
{
	ByteArray arrByte;
	DecToHex(nVal, arrByte);
	if (arrByte.size() == 1)
	{
		arrByte.push_back(0x00);
		arrByte.push_back(0x00);
		arrByte.push_back(0x00);
	}
	else if (arrByte.size() == 2)
	{
		arrByte.push_back(0x00);
		arrByte.push_back(0x00);
	}
	else if (arrByte.size() == 3)
	{
		arrByte.push_back(0x00);
	}
	BYTE data[] = {0xFF, 0xFF, 0xFF, 0xFF, arrByte[3], arrByte[2], arrByte[1], arrByte[0]};
	CProtocol col(data, 8, 0xAB, 0x00, 0xE5);
	m_port.WriteProtocol(&col);
}

// 请求设置线张力(0x0B)
void CMainFrame::RequestSetLineTension(int nVal, BYTE index)
{
	ByteArray arrByte;
	DecToHex(nVal, arrByte);
	if (arrByte.size() == 1)
	{
		arrByte.push_back(0x00);
	}

	BYTE data[] = {0x00, index, arrByte[1], arrByte[0]};
	CProtocol col(data, 4, 0x0B, 0x00, 0xE5);
	m_port.WriteProtocol(&col);
}

// 请求获取缝制相关信息
LRESULT CMainFrame::RequestGetSewingInfo(WPARAM wParam, LPARAM lParam)
{
	lParam;
	wParam;

	CProtocol col(NULL, 0, 0xA8, 0x00, 0xE5);
	m_port.WriteProtocol(&col);
	return NULL;
}

// 接收缝制相关信息
void CMainFrame::UpdateSewingInfo(BYTE* pData)
{
	SewingInfo infoSewing;

	ByteArray arrByte;
	arrByte.resize(2);
	arrByte[0] = pData[1];
	arrByte[1] = pData[0];
	infoSewing.nBottomLineCounter = Hex2Dec(arrByte);
	arrByte.resize(4);
	arrByte[0] = pData[5];
	arrByte[1] = pData[4];
	arrByte[2] = pData[3];
	arrByte[3] = pData[2];
	infoSewing.nWorkpiectCounter = Hex2Dec(arrByte);
	infoSewing.nSewMode = pData[7];
	arrByte.resize(2);
	arrByte[0] = pData[11];
	arrByte[1] = pData[10];
	infoSewing.nCurrentSewFile = Hex2Dec(arrByte);
	infoSewing.nSpindleSpeed = HexToCmt(pData[12], pData[13]);
	infoSewing.nMediumLegHeight = HexToCmt(pData[18], pData[19]);
	infoSewing.nLineTensionA = HexToCmt(pData[20], pData[21]);
	infoSewing.nLineTensionB = HexToCmt(pData[22], pData[23]);

	m_wndSewingInfo.UpdateSewingInfo(infoSewing);
}

// 添加当前花样设置指令
void CMainFrame::AddCurrSewFileSetCol(int nID, BYTE byteMode)
{
	ByteArray arrByte;
	DecToHex(nID, arrByte);
	if (arrByte.size() == 1)
	{
		arrByte.push_back(0x00);
	}

	BYTE dataSet[] = {0x00, byteMode, arrByte[1], arrByte[0]};	// 第2位表示缝制模式
	CProtocol colSet(dataSet, 4, 0xA9, 0x00, 0xE5);
	m_listProcessCol.push_back(colSet);
}

// 添加当前花样获取指令
void CMainFrame::AddCurrSewFileGetCol()
{
	BYTE data[] = {0xFF, 0xFF, 0xFF, 0xFF};
	CProtocol col(data, 4, 0xA9, 0x00, 0xE5);
	m_listProcessCol.push_back(col);
}

// 获取预览数据
void CMainFrame::GetPreviewFileData(int nID)
{
	if (!IsProcessing())
	{
		m_nCmdType = CT_PREVIEW_SEW;
		AddSewfileDataGetCol(nID);
		InitFiledataBuff();
		m_buffFileTrans.nID = nID;
		Process();
	}
}

void CMainFrame::PreviewCurrentSew()
{
	m_statePreview = PS_CLOSE;
	if (!m_currentFile.arrByte.empty())
	{
		NeedleTrailData tempTrail;
		if (AnalysisSewFile(m_currentFile.arrByte, tempTrail))
		{
			m_trailToPreview = tempTrail;
			m_trailToPreview.strDesc.Format(L"NO.%d", m_currentFile.nID);
			m_statePreview = PS_VALID;
			this->GetActiveDocument()->UpdateAllViews(NULL);
		}
	}
	else
	{
		m_statePreview = PS_NOCURRENT;
	}
}