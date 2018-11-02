//===================================================================================================
// Summary:
//		�������
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
//	//�����⡱�ĸ߶�
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
//		//��ѡ����ĸ߶�
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
//		//�����ٷ��ʹ��������ĸ߶�
//		const int nQuickAceesToolbarHeight = IsQuickAccessToolbarOnTop() ? 0 : sizeAQToolbar.cy;
//		//�����ࡱ�ĸ߶�
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

// ���������ļ�
BOOL AnalysisSewFile(const ByteArray& arrSewByte, NeedleTrailData& dataPreview)
{
	// �ļ�ͷռ��16���ֽڣ��ļ��������ռ��12���ֽڣ��ļ���Ϣ��������ռ��30���ֽ�
	int nByteCount = (int)arrSewByte.size();
	if (nByteCount < 58)
		return FALSE;

	ByteArray arrByte = arrSewByte;
	arrByte.insert(arrByte.begin(), 0x00);	// �����1��ʼ��������

	// �ļ�ͷǰ�˸�ֱ�������ж��ļ����ͣ���5λ��ʼ��ͬ
	if (arrByte[5] == 0x23)	// 42 49 4C 00 23 13 00 C8 ��ʾ311G
	{
		// ֱ�Ӵ�17���ֽڿ�ʼ��ȡ��������ָ��һ�ζζ�ȡ��ָ��κ�������ݶ�
		int nCodeStart = 16;		// ��ǰָ��ε���ʼλ��(��ʼ�ֽڵ�ǰһ������)
		int nDataStart = 28;		// ��ǰ���ݶε���ʼλ��(��ʼ�ֽڵ�ǰһ������)
		int nDataLength = 0;	// ��ǰ���ݶεĳ���
		// �ӵ�ǰָ��ο�ʼ��ȡ12���ַ�
		while (nCodeStart+12 <= nByteCount)
		{
			// ��ȡ���ݶγ���
			nDataLength = arrByte[nCodeStart+4]*65536+arrByte[nCodeStart+5]*256+arrByte[nCodeStart+6];
			nDataStart = nCodeStart+12;
			if ((nDataStart+nDataLength) > nByteCount)
			{
				break;	// ָ�����ݳ�����ָ�һ�£�����ѭ��
			}

			// ���ָ����Ƿ���Ч(ָ�����CD 0C��ͷ)
			if (arrByte[nCodeStart+1] == 0xCD && arrByte[nCodeStart+2] == 0x0C)
			{
				// ָ��εĵ�8λ��ʾָ������
				if (arrByte[nCodeStart+8] == 0x01)	// ����ָ���
				{
					break;	// ��������ѭ��
				}
				else if (arrByte[nCodeStart+8] == 0x02)	// �ݲ�֪����;
				{

				}
				else if (arrByte[nCodeStart+8] == 0x03)	// �ļ���Ϣָ���
				{
					dataPreview.nStartPulseX = HexToCmt(arrByte[nDataStart+3], arrByte[nDataStart+4]);
					dataPreview.nStartPulseY = HexToCmt(arrByte[nDataStart+5], arrByte[nDataStart+6]);
					dataPreview.nPlusMaxX = HexToCmt(arrByte[nDataStart+11], arrByte[nDataStart+12]);
					dataPreview.nMinusMaxX = HexToCmt(arrByte[nDataStart+13], arrByte[nDataStart+14]);
					dataPreview.nPlusMaxY = HexToCmt(arrByte[nDataStart+15], arrByte[nDataStart+16]);
					dataPreview.nMinusMaxY = HexToCmt(arrByte[nDataStart+17], arrByte[nDataStart+18]);
				}
				else if (arrByte[nCodeStart+8] == 0x04)	// �벽����ָ���
				{
					if (nDataLength%2 != 0)
					{
						break;
					}
					for (int i=1; i+1<=nDataLength; i=i+2)
					{
						NeedleStepData dataStep;
						BYTE byteFisrt = arrByte[nDataStart+i];
						if ((byteFisrt&0xF0) == 0x60)	// �벽����
						{
							long lPulseNum[2] = {0, 0};
							lPulseNum[0] = arrByte[nDataStart+i+1];
							if ((byteFisrt&0x04)==0x04)	// ����
							{
								lPulseNum[0] = -lPulseNum[0];
							}

							// ͬʱ����X������Y����
							if (i+3<=nDataLength && (arrByte[nDataStart+i+2]&0xF0) == 0x40)
							{
								lPulseNum[1] = arrByte[nDataStart+i+3];
								if ((arrByte[nDataStart+i+2]&0x04)==0x04)	// ����
								{
									lPulseNum[1] = -lPulseNum[1];
								}
								i = i+2;
							}

							// �жϵ�һ�����Ƿ�ΪY������
							if ((byteFisrt&0x08) == 0x08)	// Y��
							{
								long lTemp = lPulseNum[0];
								lPulseNum[0] = lPulseNum[1];
								lPulseNum[1] = lTemp;
							}

							dataStep.lPulseOffsetX = lPulseNum[0];
							dataStep.lPulseOffsetY = lPulseNum[1];
							dataPreview.arrNode.push_back(dataStep);
						}
						else if ((byteFisrt&0xF0) == 0x20)	// ��������
						{
							long lPulseNum[2] = {0, 0};
							lPulseNum[0] = arrByte[nDataStart+i+1];

							// �ж����޸�λ����2λ��ʾ���޸�λ��=1��ʾ�и�λ��
							if (i+3<=nDataLength && (byteFisrt&0x02) == 0x02)
							{
								lPulseNum[0] = lPulseNum[0]*256+arrByte[nDataStart+i+3];
								i += 2;
							}
							if ((byteFisrt&0x04)==0x04)	// ���򣨵�3λ��ʾ����=1ʱ��ʾ����
							{
								lPulseNum[0] = -lPulseNum[0];
							}

							// �ж��Ƿ�ͬʱ����X������Y����
							if (i+3<=nDataLength)
							{
								BYTE byteOther = arrByte[nDataStart+i+2];
								if ((byteOther&0xF0) != 0x60 && 
									(byteOther&0xF0) != 0xF0 &&
									(byteOther&0xF0) != 0x20)
								{
									lPulseNum[1] = arrByte[nDataStart+i+3];
									i= i+2;

									// �ж����޸�λ
									if (i+3<=nDataLength && (byteOther&0x02) == 0x02)
									{
										lPulseNum[1] = lPulseNum[1]*256+arrByte[nDataStart+i+3];
										i += 2;
									}
									if ((byteOther&0x04)==0x04)	// ����
									{
										lPulseNum[1] = -lPulseNum[1];
									}
								}
							}

							// �жϵ�һ�����Ƿ�ΪY����������4λ��ʾX�ỹ��Y�ᣬ=1ʱ��ʾֻ��Y�ᣩ
							if ((byteFisrt&0x08) == 0x08)	// Y��
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
						else if ((byteFisrt&0xF0) == 0xF0)	// ������������
						{
							if (byteFisrt == 0xFF && arrByte[nDataStart+i+1]==0x00)
							{
								break;	// ������ȡ
							}
							else
							{
								continue; // ����
							}
						}
					}
				}
				else if (arrByte[nCodeStart+8] == 0x0D)	// ʱ����Ϣָ���
				{

				}
				else if (arrByte[nCodeStart+8] == 0x05)	// ע����Ϣָ���
				{

				}
				nCodeStart += 12+nDataLength;	// ����ָ�����ݵ�ĩβ
			}
			else
			{
				break;		// ����ָ���
			}
		}
	}
	else	// 42 49 4c 00 38 6b 00 64 ��ʾ311E
	{
		return FALSE;
	}
	return TRUE;
}

//===================================================================================================

// CMainFrame ����/����

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
	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	m_wndRibbonBar.Create(this);
	InitializeRibbon();

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("δ�ܴ���״̬��\n");
	//	return -1;      // δ�ܴ���
	//}

	//CString strTitlePane1;
	//CString strTitlePane2;
	//bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	//ASSERT(bNameValid);
	//bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	//ASSERT(bNameValid);
	//m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	//m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ���ز˵���ͼ��(�����κα�׼��������):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// ����ͣ������
	if (!CreateDockingWindows())
	{
		TRACE0("δ�ܴ���ͣ������\n");
		return -1;
	}

	//DockPane(&m_wndSewingInfo);
	//DockPane(&m_wndSewFile);

	/*SewFileData dataTemp;
	CString strFile = L"C:\\SUPERME\\ISMS0106.sew";
	ReadSewFile(strFile, dataTemp);
	AnalysisSewFile(dataTemp.arrByte, m_trailToPreview);*/
	

	// �Զ�����
	if (m_uPortID > 0)
	{
		RequestConnect();
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~(LONG) FWS_ADDTOTITLE;

	// �޸ĳ�ʼ�Ի�����⣨�������ʱ�޷���ʾ���⣩
	cs.lpszName = L"CNCSewingControl - ˫����һ�PC�˿�����������豸���ӣ�";

	if( !CFrameWndEx::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

// ��ʼ��Ribbon��
void CMainFrame::InitializeRibbon()
{
	BOOL bNameValid;

	CString strTemp;
	bNameValid = strTemp.LoadString(IDS_RIBBON_FILE);
	ASSERT(bNameValid);

	// ��ʼ����ť:
	/*m_MainButton.SetImage(IDB_MAIN);
	m_MainButton.SetText(_T("\nf"));
	m_MainButton.SetToolTipText(strTemp);*/

	// �������ͼ��:
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

	// ��ӡ��������:
	bNameValid = strTemp.LoadString(IDS_RIBBON_HOME);
	ASSERT(bNameValid);
	CMFCRibbonCategory* pCategoryHome = m_wndRibbonBar.AddCategory(strTemp, IDB_WRITESMALL, IDB_SEWCONTROL_LARGE, CSize(16, 16), CSize(64, 64));

	// ���������һ�ͨѶ�����
	CMFCRibbonPanel* pPanelSewConnect = pCategoryHome->AddPanel(L"���һ�ͨѶ", m_PanelImages.ExtractIcon(27));
	pPanelSewConnect->Add(new CMFCRibbonButton(ID_SEW_CONNECT, L"����", -1, 6));
	pPanelSewConnect->Add(new CMFCRibbonButton(ID_SEW_BREAK, L"�Ͽ�����", -1, 7));

	// ���������һ����ơ����
	CMFCRibbonPanel* pPanelSewControl = pCategoryHome->AddPanel(L"���һ�����", m_PanelImages.ExtractIcon(27));
	pPanelSewControl->Add(new CMFCRibbonButton(ID_SEWINGINFO_SET, L"������Ϣ", -1, 0));
	pPanelSewControl->Add(new CMFCRibbonButton(ID_SEWFILE_SET, L"����ѡ��", -1, 1));
	pPanelSewControl->Add(new CMFCRibbonButton(ID_SEW_EDIT, L"ͼ�α༭", -1, 2));
	pPanelSewControl->Add(new CMFCRibbonButton(ID_FILE_MANAGE, L"�ļ�����", -1, 3));
	pPanelSewControl->Add(new CMFCRibbonButton(ID_PARAM_CONFIG, L"��������", -1, 4));
	pPanelSewControl->Add(new CMFCRibbonButton(ID_IO_CHECK, L"IO���", -1, 5));

	// ��������ӡ���ͼ�����:
	/*bNameValid = strTemp.LoadString(IDS_RIBBON_VIEW);
	ASSERT(bNameValid);
	CMFCRibbonPanel* pPanelView = pCategoryHome->AddPanel(strTemp, m_PanelImages.ExtractIcon (7));

	bNameValid = strTemp.LoadString(IDS_RIBBON_STATUSBAR);
	ASSERT(bNameValid);
	CMFCRibbonButton* pBtnStatusBar = new CMFCRibbonCheckBox(ID_VIEW_STATUS_BAR, strTemp);
	pPanelView->Add(pBtnStatusBar);*/

	// ��Ԫ����ӵ�ѡ��Ҳ�:
	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE);
	ASSERT(bNameValid);
	CMFCRibbonButton* pVisualStyleButton = new CMFCRibbonButton(-1, strTemp, -1, -1);

	pVisualStyleButton->SetMenu(IDR_THEME_MENU, FALSE /* ��Ĭ������*/, TRUE /* �Ҷ���*/);

	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE_TIP);
	ASSERT(bNameValid);
	pVisualStyleButton->SetToolTipText(strTemp);
	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE_DESC);
	ASSERT(bNameValid);
	pVisualStyleButton->SetDescription(strTemp);
	m_wndRibbonBar.AddToTabs(pVisualStyleButton);

	// ��ӿ��ٷ��ʹ���������:
	/*CList<UINT, UINT> lstQATCmds;
	lstQATCmds.AddTail(ID_FILE_NEW);
	lstQATCmds.AddTail(ID_FILE_OPEN);
	lstQATCmds.AddTail(ID_FILE_SAVE);
	m_wndRibbonBar.SetQuickAccessCommands(lstQATCmds);*/

	m_wndRibbonBar.AddToTabs(new CMFCRibbonButton(ID_APP_ABOUT, _T("\na"), m_PanelImages.ExtractIcon (0)));
}

// ����ͣ������
BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// ����������Ϣ����
	CString strSewingInfoView;
	bNameValid = strSewingInfoView.LoadString(IDS_SEWINGINFO_VIEW);
	ASSERT(bNameValid);
	if (!m_wndSewingInfo.Create(strSewingInfoView, this, CRect(0, 0, 0, 0), TRUE, ID_VIEW_SEWINGINFO, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS| WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ�����������Ϣ������\n");
		return FALSE;
	}
	m_wndSewingInfo.EnableDocking(CBRS_ALIGN_ANY);
	m_wndSewingInfo.ShowPane(FALSE, FALSE, FALSE);

	// ��������ѡ�񴰿�
	CString strSewFileView;
	bNameValid = strSewFileView.LoadString(IDS_SEWFILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndSewFile.Create(strSewFileView, this, CRect(0, 0, 0, 0), TRUE, ID_VIEW_SEWFILE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ���������ѡ�񡱴���\n");
		return FALSE;
	}
	m_wndSewFile.EnableDocking(CBRS_ALIGN_ANY);
	m_wndSewFile.ShowPane(FALSE, FALSE, FALSE);

	// ����IO��ⴰ��
	if (!m_wndIOCheck.Create(L"IO���", this, CRect(0, 0, 0, 0), TRUE, ID_VIEW_IOCHECK, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ�����IO��⡱����\n");
		return FALSE;
	}
	m_wndIOCheck.EnableDocking(CBRS_ALIGN_ANY);
	m_wndIOCheck.ShowPane(FALSE, FALSE, FALSE);

	// �����������ô���
	if (!m_wndParaConfig.Create(L"��������", this, CRect(0, 0, 0, 0), TRUE, ID_VIEW_PARACONFIG, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ������������á�����\n");
		return FALSE;
	}
	m_wndParaConfig.EnableDocking(CBRS_ALIGN_ANY);
	m_wndParaConfig.ShowPane(FALSE, FALSE, FALSE);

	if (!m_wndFileManage.Create(L"�ļ�����-NC", this, CRect(0, 0, 0, 0), TRUE, ID_VIEW_FILEMANAGE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ������ļ���������\n");
		return FALSE;
	}
	m_wndFileManage.EnableDocking(CBRS_ALIGN_ANY);
	m_wndFileManage.ShowPane(FALSE, FALSE, FALSE);

	CString strTitle;
	strTitle.Format(L"�ļ�����-PC(%s)", GetConfigOption(L"INIT", L"LOCALFOLDER"));
	if (!m_wndLocalFileView.Create(strTitle, this, CRect(0, 0, 0, 0), TRUE, ID_VIEW_FILE_LOCAL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ����������ļ�������\n");
		return FALSE;
	}
	m_wndLocalFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndLocalFileView.ShowPane(FALSE, FALSE, FALSE);

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	
	return TRUE;
}

// �رյ�ǰ������ͼ
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

// ��ʾ������
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

// �رս�����
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

// �ر�ͣ������
BOOL CMainFrame::OnCloseDockingPane(CDockablePane* pWnd)
{
	if (m_nCurrentMode == MT_IO_CHECK)
	{
		// �رռ�ʱ��
		m_wndIOCheck.EndTimer();

		// �л���ͨ��ģʽ
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

// CMainFrame ���

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

// CMainFrame ��Ϣ�������

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

// ��ʱ��
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case ID_TIMER_WAITANSWER:
		{
			// �رռ�ʱ��
			EndWaitingTimer();

			// �رս�����
			CloseProgressBar();

			BYTE cmdCode = m_listProcessCol.front().m_commandCode;
			if (cmdCode == 0x01)	// ���ӳ�ʱ
			{
				m_port.CloseListenTread();
				m_port.SetIsListen(false);
				m_port.ClosePort();

				AfxMessageBox(L"���ӳ�ʱ��");
			}
			else
			{
				AfxMessageBox(L"NC������Ӧ����ǰ��������ֹ");
			}
			
			// ����ǰָ��������ָ������
			CodeRecode recode(0xE5, m_listProcessCol.front().m_commandCode, FALSE);
			m_vecFinishedCol.push_back(recode);
			
			// ��ʼ����������
			InitQueueData();
		}
		break;
	default:
		break;
	}

	CWnd::OnTimer(nIDEvent);
}

// ���ӻ�Ͽ�����ʱ���¿ؼ�״̬
void CMainFrame::OnUpdateConnectOrBreak(CCmdUI* pCmdUI)
{
	if (m_port.m_bIsListen && m_bConnected)
	{
		if (pCmdUI->m_nID == ID_SEW_CONNECT)
		{
			pCmdUI->Enable(FALSE);

			// ͨѶ����ʱ�Ĵ��ڱ���
			CString strTitle;
			strTitle.Format(L"CNCSewingControl - ˫����һ�PC�˿���������������豸��%s��", m_strDeviceName);
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

			// ͨѶ�Ͽ�ʱ�Ĵ��ڱ���
			SetWindowText(L"CNCSewingControl - ˫����һ�PC�˿�����������豸���ӣ�");
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

// ��ʾ������Ϣ����
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

// ��ʾ��ǰ�����ļ����ô���
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

// ��ʾ�����ļ��༭����
void CMainFrame::OnSewEditView()
{
	CString strProgramPath;
	strProgramPath = GetConfigOption(L"INIT", L"EDITPROGRAM");
	int nError = (int)ShellExecute(0, L"open", strProgramPath, m_strFileToEdit, NULL, SW_SHOW);
	if (nError < 32)
	{
		AfxMessageBox(L"�����༭��������ʧ�ܣ�");
	}
}

// ��ʾ�����ļ�������
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

// ��ʾ�������ô���
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

// ��ʾIO��ⴰ��
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

// ͨѶ����
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
				// ��¼�˿ں�
				SetConfigOption(L"CONNECT", L"PORTID", m_uPortID);
			}
		}
	}
}

// ͨѶ�Ͽ�
void CMainFrame::OnSewBreak()
{
	if (MessageBox(L"ȷ�϶Ͽ���", L"��ʾ", MB_YESNO|MB_ICONINFORMATION) == IDYES)
	{
		m_port.CloseListenTread();
		m_port.SetIsListen(false);
		m_port.ClosePort();
	}
}

// ��������
LRESULT CMainFrame::OnReceiveData(WPARAM wParam, LPARAM lParam)
{
	BYTE byteReceiveCode = (int)wParam;
	BYTE* pData = (BYTE*)lParam;

	// У�����ݣ��������Ϊ�ջ���������е�ָ�ƥ�䣬�����
	if (m_listProcessCol.empty())
		return NULL;
	if (m_listProcessCol.front().m_receiveCode != byteReceiveCode)
		return NULL;

	// �رռ�ʱ��
	EndWaitingTimer();

	switch (byteReceiveCode)
	{
	case 0x01:
		m_bConnected = TRUE;		// �ѽ���ͨѶ����
		// ����������
		break;
	case 0x1D:
		UpdateDeviceInfo(pData);	// �����豸��Ϣ
		break;
	case 0xA8:
		UpdateSewingInfo(pData);	// ���·�����Ϣ
		break;
	case 0x73:
		if (!UpdateFileList(pData))		// �����ļ��б�
		{
			// �ڵ�ǰָ��������һ��ָ��
			BYTE dataFiles[] = {0x53, pData[0]+1};
			CProtocol colFiles(dataFiles, 0x02, 0x72, 0x01, 0xE5, 0x73);
			list<CProtocol>::iterator i = m_listProcessCol.begin();
			m_listProcessCol.insert(++i, colFiles);
		}
		break;
	case 0x27:
		if (!UpdateParamList(pData))	// ���²����б�
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
		if (pData[0] == 0 && pData[1] == 0)	// ���ò���
		{
			m_wndParaConfig.SetSuccess();
		}
		break;
	case 0x77:
		if (!ReceiveSewfileData(pData))	// ���ջ����ļ�����
		{
			CProtocol colCopy = m_listProcessCol.front();
			colCopy.m_data[3] += 1;
			list<CProtocol>::iterator i = m_listProcessCol.begin();
			m_listProcessCol.insert(++i, colCopy);
		}
		break;
	case 0x7C:	// �ļ�ɾ������
		break;
	case 0x79:	// �ļ������������ϴ�����
		break;
	case 0xA9:	// ��ѯ�����õ�ǰ������
		m_nCmdType = CT_CURRENT_SEW;
		if (m_listProcessCol.front().m_data[0] == 0xFF && m_listProcessCol.front().m_data[1] == 0xFF)
		{
			if (pData[1]!=0 && pData[2]!=0 && pData[3]!=0)
			{
				// ��ѯ״̬���Զ���������ָ��
				BYTE data[] = {0x53, pData[2], pData[3], 0x00};	// ���ݿ��0x00��ʼ
				CProtocol col(data, 4, 0x76, 0x03, 0xE5, 0x77);
				list<CProtocol>::iterator i = m_listProcessCol.begin();
				m_listProcessCol.insert(++i, col);
			}
		}
		break;
	case 0x09:	// ����˿ڲ�ѯ������
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
	case 0x0A:	// ����������
		if (pData[0] == 0xFF)
		{
			AfxMessageBox(L"��ǰ������֧�ָ���");
		}
		break;
	default:
		break;
	}

	// ��¼��ǰָ��
	CodeRecode recodeRequest(0xE5, m_listProcessCol.front().m_commandCode, TRUE);
	CodeRecode recodeAnswer(0xF6, byteReceiveCode, TRUE);
	m_vecFinishedCol.push_back(recodeRequest);
	m_vecFinishedCol.push_back(recodeAnswer);

	// ����ǰָ���Ƴ�����
	m_listProcessCol.pop_front();

	// ִ����һ��ָ��
	Process();

	return NULL;
}

// ��ȡ�����б�
LRESULT CMainFrame::OnGetParamList(WPARAM wParam, LPARAM lParam)
{
	wParam;
	lParam;

	if (!IsProcessing())
	{
		ShowProgressBar(L"����ͬ�������б����Ժ�...");

		// ��ȡ�����б�(0x27)
		BYTE dataParams[] = {0x01, 0x00, 0x00};
		CProtocol colParams(dataParams, 0x03, 0x27, 0x03, 0xE5);
		m_listProcessCol.push_back(colParams);

		// ��ʼִ��
		Process();
	}
	return NULL;
}

// ���ò���
LRESULT CMainFrame::OnSaveParam(WPARAM wParam, LPARAM lParam)
{
	wParam;
	ParaInfoArray& arrPara = *(ParaInfoArray*)lParam;

	if (!IsProcessing())
	{
		ShowProgressBar(L"�������ò��������Ժ�...");
		AddParamSaveCol(arrPara);

		// ��ʼִ��
		Process();
	}
	
	return NULL;
}

// �ϴ�SEW�ļ�
LRESULT CMainFrame::OnUploadSewFile(WPARAM wParam, LPARAM lParam)
{
	m_nCmdType = CT_UPLOAD_SEWFILE;
	wParam;

	if (!IsProcessing())
	{
		ShowProgressBar(L"�����ϴ��ļ������Ժ�...");
	
		SewFileData& data = *(SewFileData*)lParam;
		AddSewfileCreateCol(data.nID);
		AddUploadFileDataCol(data);

		SewFileDataArray arr;
		arr.push_back(data);
		m_wndFileManage.AddFileToList(arr);
		m_wndSewFile.AddFileToList(arr);

		// ��ʼִ��
		Process();
	}

	return NULL;
}

// ��ȡ�ļ��б�
LRESULT CMainFrame::OnGetFileList(WPARAM wParam, LPARAM lParam)
{
	wParam;
	lParam;

	if (!IsProcessing())
	{
		ShowProgressBar(L"���ڸ����ļ��б����Ժ�...");

		AddFilesListGetCol();

		// ��ʼִ��
		Process();
	}
	return NULL;
}

// ���ػ����ļ�
LRESULT CMainFrame::OnDownloadSewfile(WPARAM wParam, LPARAM lParam)
{
	m_nCmdType = CT_DOWNLOAD_SEWFILE;
	
	wParam;
	vector<int>& arrID = *(vector<int>*)lParam;
	
	if (!IsProcessing())
	{
		ShowProgressBar(L"���������ļ������Ժ�...");
		
		for (int i=0; i<arrID.size(); i++)
		{
			AddSewfileDataGetCol(arrID[i]);
			InitFiledataBuff();
			m_buffFileTrans.nID = arrID[i];
		}

		// ��ʼִ��
		Process();
	}
	
	return NULL;
}

// �����������ļ�
LRESULT CMainFrame::OnRenameSewFile(WPARAM wParam, LPARAM lParam)
{
	m_nCmdType = CT_RENAME_SEWFILE;
	
	int nOldID = (int)wParam;
	int nNewID = (int)lParam;

	if (!IsProcessing())
	{
		ShowProgressBar(L"�����������ļ������Ժ�...");

		InitFiledataBuff();
		m_buffFileTrans.nID = nOldID;

		AddSewfileDataGetCol(nOldID);	// �Ȱ����ļ�������
		AddSewfileDeleteCol(nOldID);	// ɾ�����ļ�
		AddSewfileCreateCol(nNewID);	// �������ļ�

		// ��ʼִ��
		Process();
	}

	return NULL;
}

// ɾ�������ļ�
LRESULT CMainFrame::OnDeleteSewFile(WPARAM wParam, LPARAM lParam)
{
	m_nCmdType = CT_DELETE_SEWFILE;
	wParam;
	vector<int>& arrID = *(vector<int>*)lParam;

	if (!IsProcessing())
	{
		ShowProgressBar(L"����ɾ���ļ������Ժ�...");

		for (int i=0; i<arrID.size(); i++)
		{
			AddSewfileDeleteCol(arrID[i]);
		}

		// ��ʼִ��
		Process();
	}
	
	return NULL;
}

// ���õ�ǰ����
LRESULT CMainFrame::OnSetCurrSewFile(WPARAM wParam, LPARAM lParam)
{
	lParam;
	int nID = (int)wParam;

	if (!IsProcessing())
	{
		ShowProgressBar(L"�����л���ǰ���������Ժ�...");

		AddCurrSewFileSetCol(nID);
		AddSewfileDataGetCol(nID);

		// ��ʼִ��
		Process();
	}

	return NULL;
}

// ����IO���(����)
LRESULT CMainFrame::OnCheckInputPort(WPARAM wParam, LPARAM lParam)
{
	wParam;
	lParam;

	RequestGetSpecialInputPort();
	return NULL;
}

// ����IO���(���)
LRESULT CMainFrame::RequestOutputCheck(WPARAM wParam, LPARAM lParam)
{
	wParam;
	IOCheckData& iodata = *(IOCheckData*)lParam;

	// ����ָ��
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
	case 0:	// ����ת��
		RequestSetSpindleSpeed(nVal);
		break;
	case 1:	// ���߼�����
		RequestSetBottomLineCounter(nVal);
		break;
	case 2:	// ����������
		RequestSetWorkpiectCounter(nVal);
		break;
	case 3:	// ��ѹ�Ÿ߶�
		break;
	case 4:	// ������A
		RequestSetLineTension(nVal, 0x01);
		break;
	case 5:	// ������B
		RequestSetLineTension(nVal, 0x02);
		break;
	default:
		break;
	}
	return NULL;
}

// ���»���Ԥ������
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

// CMainFrame ����

// ��ʼ����������
void CMainFrame::InitQueueData()
{
	m_listProcessCol.clear();
	m_listWaitCol.clear();
	EndWaitingTimer();
}

// ִ�ж���ָ��
void CMainFrame::Process()
{
	if (m_listProcessCol.empty())
	{
		if (!m_listWaitCol.empty())
		{
			// ����ǰ�ȴ�����ָ��ȫ���Ƶ���ִ���б���
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

	// ����ָ��
	m_port.WriteProtocol(&m_listProcessCol.front());

	// ͬʱ������ʱ�����������ָ��ʱ���޷�Ӧ������ʾNC����Ӧ
	StartWaitingTimer();
}

// �����ȴ���ʱ
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

// �رյȴ���ʱ
void CMainFrame::EndWaitingTimer()
{
	if (m_bTimerStartFlag)
	{
		KillTimer(ID_TIMER_WAITANSWER);
		m_bTimerStartFlag = FALSE;
	}
}

// ��ʼ���ļ����ݻ���
void CMainFrame::InitFiledataBuff()
{
	m_buffFileTrans.arrByte.clear();
	m_buffFileTrans.arrBlockSize.clear();
	m_buffFileTrans.nID = -1;
}

// ����Ƿ���ִ����
BOOL CMainFrame::IsProcessing()
{
	return !m_listProcessCol.empty();
}

// �������ӣ�0x01��
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

				ShowProgressBar(L"���Ӳ���ȡ�豸�����У����Ժ�...");

				// ���ݳ�ʼ��
				InitQueueData();

				// ׼�����(0x01)
				CProtocol colReady(NULL, 0, 0x01, 0x00, 0xE5);
				m_listProcessCol.push_back(colReady);

				// ��ȡ�豸��Ϣ(0x1C)
				AddDeviceInfoGetCol();

				// ��ȡ������Ϣ(0xA8)
				CProtocol colSewing(NULL, 0, 0xA8, 0x00, 0xE5);
				m_listProcessCol.push_back(colSewing);

				// ��ȡ�ļ��б�(0x72)
				AddFilesListGetCol();
				
				// ��ȡ�����б�(0x27)
				AddParamListGetCol();

				// ��ȡ��ǰ�����ļ�
				AddCurrSewFileGetCol();
	
				// ��ʼִ��
				Process();
			}
			else
			{
				strMsg.Format(_T("�򿪼���%dʧ��"), m_uPortID);
				AfxMessageBox(strMsg);
				return FALSE;
			}
		}
		else
		{
			strMsg.Format(_T("��ʼ�˿�%dʧ��"), m_uPortID);
			AfxMessageBox(strMsg);
			return FALSE;
		}
	}
	return TRUE;
}

// �����ȡ�豸��Ϣ��0x1C��
void CMainFrame::AddDeviceInfoGetCol()
{
	CProtocol colDevice(NULL, 0, 0x1C, 0x00, 0xE5, 0x1D);
	m_listProcessCol.push_back(colDevice);
}

// �����豸��Ϣ��0x1D��
void CMainFrame::UpdateDeviceInfo(BYTE* pData)
{
	// ǰ��λ��ʾ�����ͺţ���֪�������м�λ���ݶ�����0�ͽ�������ÿһλ��Ӧһ����ĸ��ASCII��ֵ
	int nPos = 0;
	char ch[128] = {0};
	char temp;
	while (1)
	{
		// ʮ������תASCII
		temp= pData[nPos];
		if (temp == 0)
			break;

		ch[nPos] = temp;
		nPos++;
	}

	// ���豸���Ƹ��µ�������
	m_strDeviceName = ch;

	// ���浽�����ļ���
	SetConfigOption(L"CONNECT", L"DEVICENAME", m_strDeviceName);
}

// ���������Ϣ��0x27��
void CMainFrame::AddParamListGetCol()
{
	BYTE dataParams[] = {0x01, 0x00, 0x00};
	CProtocol colParams(dataParams, 0x03, 0x27, 0x03, 0xE5);
	m_listProcessCol.push_back(colParams);
}

// ���ղ�����Ϣ��0x27��
BOOL CMainFrame::UpdateParamList(BYTE* pData)
{
	ParaInfo para;
	if (pData[1] == 0xff || pData[2] == 0xff)	// ����
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

// ��ӱ������ָ�0x26��
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
	// ������ֵд�뻺����
	CProtocol col1(bytedata, 4*nParaCount+2, 0x26, 0x00, 0xE5);
	m_listProcessCol.push_back(col1);

	BYTE data2[] = {0x00, 0x00};
	CProtocol col2(data2, 2, 0x26, 0x00, 0xE5);
	m_listProcessCol.push_back(col2);
}

// ����ļ�����ָ�0x78��
void CMainFrame::AddSewfileCreateCol(int nFileID)
{
	//if (m_arrFileBuffer.size() == 0)
	//	return;

	//// �Ӷ���ȡ�ļ�
	//SewFileData& sewData = m_arrFileBuffer[0];

	// ʶ���ļ���ID
	ByteArray arrByte;
	DecToHex(nFileID, arrByte);
	if (arrByte.size() == 1)
	{
		arrByte.push_back(0x00);
	}
	m_nBlockIndex = 0;

	BYTE byteCreate[] = {0x53, arrByte[1], arrByte[0], 0x00};	// ��4������ʾģʽ
	CProtocol colCreate(byteCreate, 4, 0x78, 0x01, 0xE5, 0x79);
	m_listProcessCol.push_back(colCreate);

	InitFiledataBuff();
	m_buffFileTrans.nID = nFileID;
}

// ������ϴ�ָ��(0x7A)
void CMainFrame::AddUploadFileDataCol(const SewFileData& sewData)
{
	int nByteCount = (int)sewData.arrByte.size();
	int nBlockSize = nByteCount/123;
	int nLastCount = nByteCount%123;
	if (nLastCount > 0)
		nBlockSize += 1;

	BYTE byteData[126];
	byteData[0] = 0;	// �����
	byteData[1] = 0x00;
	int nCount;
	for (int i=0, nIndex = 0; i<nBlockSize; i++)
	{
		if (i==(nBlockSize-1))
		{
			byteData[1] = 0x01;	// ��ֹ
			nCount = nLastCount;
		}
		else
			nCount = 123;
		byteData[2] = nCount;	// ����

		for (int j=0; j<nCount; j++)
		{
			byteData[3+j] = sewData.arrByte[nIndex+j];
		}
		nIndex += nCount;	// ��һ�����nIndex��ʼ�������
		CProtocol colData(byteData, 126, 0x7A, 0x01, 0xE5, 0x79);
		m_listProcessCol.push_back(colData);

		byteData[0] += 1;
	}
}

// �ļ��б��ȡָ�0x72��
void CMainFrame::AddFilesListGetCol()
{
	// ��ȡ�ļ��б�(0x72)
	BYTE dataFiles[] = {0x53, 0x00};
	CProtocol colFiles(dataFiles, 0x02, 0x72, 0x01, 0xE5, 0x73);
	m_listProcessCol.push_back(colFiles);
}

// �����ļ��б�(0x73)
BOOL CMainFrame::UpdateFileList(BYTE* pData)
{
	SewFileDataArray arrFileList;

	// ��3������ʾ��������
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

	// ���µ��ļ�����ģ���Լ�����ѡ��ģ��
	m_wndFileManage.UpdateFileList(arrFileList, (pData[1] == 0x01));
	m_wndSewFile.UpdateFileList(arrFileList, (pData[1] == 0x01));

	// 0x01 ����TRUE�� 0x00 ����FALSE
	BOOL bFinished = (pData[1] == 0x01);
	return bFinished;
}

// �����ļ���������ָ�0x76��
void CMainFrame::AddSewfileDataGetCol(int nFileID, BYTE byteSecIndex)
{
	ByteArray arrByte;
	DecToHex(nFileID, arrByte);
	if (arrByte.size() == 1)
	{
		arrByte.push_back(0x00);
	}
	
	BYTE data[] = {0x53, arrByte[1], arrByte[0], byteSecIndex};	// ���ݿ��0x00��ʼ
	CProtocol col(data, 4, 0x76, 0x03, 0xE5, 0x77);
	m_listProcessCol.push_back(col);
}

// ���ջ����ļ����ݣ�0x77��
BOOL CMainFrame::ReceiveSewfileData(BYTE* pData)
{
	BOOL bFinished = FALSE;
	// ���ݽ�����Ϻ�ִ�����ػ��ߵȴ���������

	// ��3������ʾ�������ݸ���
	int nCount = pData[2];
	for (int i=3; i<nCount+3; i++)
	{
		m_buffFileTrans.arrByte.push_back(pData[i]);
	}
	m_buffFileTrans.arrBlockSize.push_back(nCount);

	// ��2������ʾ������Ƿ����
	if (pData[1] == 0x01)
	{
		m_buffFileTrans.nID = m_listProcessCol.front().m_data[1]*256+m_listProcessCol.front().m_data[2];
		if (CT_DOWNLOAD_SEWFILE == m_nCmdType)
		{
			// �ļ�������ϣ�ֱ�����ص�����
			m_wndLocalFileView.DownLoadSewFile(m_buffFileTrans);
		}
		else if (CT_RENAME_SEWFILE == m_nCmdType)	// ������
		{
			// ��������������ϴ�ָ��
			BYTE byteData[126];
			byteData[0] = 0;	// �����
			byteData[1] = 0x00;
			for (int i=0, nIndex = 0; i<m_buffFileTrans.arrBlockSize.size(); i++)
			{
				if (i==(m_buffFileTrans.arrBlockSize.size()-1))
				{
					byteData[1] = 0x01;	// ��ֹ
				}
				nCount = m_buffFileTrans.arrBlockSize[i];
				byteData[2] = nCount;	// ����

				for (int j=0; j<nCount; j++)
				{
					byteData[3+j] = m_buffFileTrans.arrByte[nIndex+j];
				}
				nIndex += nCount;	// ��һ�����nIndex��ʼ�������
				CProtocol colData(byteData, 126, 0x7A, 0x01, 0xE5, 0x79);
				m_listProcessCol.push_back(colData);

				byteData[0] += 1;	// �����
			}
		}
		else if (CT_CURRENT_SEW == m_nCmdType)
		{
			// ���뻺����
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
		// ��ʼ������
		InitFiledataBuff();
		bFinished = TRUE;
	}
	return bFinished;
}

// �ļ�ɾ��ָ��(0x7B)
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

// �����ȡר������˿�״̬(0x09)
void CMainFrame::RequestGetSpecialInputPort()
{
	BYTE byte[] = {0x00, 0x01, 0xFF, 0xFF};
	CProtocol col(byte, 4, 0x09, 0x00, 0xE5);
	m_port.WriteProtocol(&col);
}

// �����ȡͨ������˿�״̬(0x09)
void CMainFrame::RequestSetCommonInputPort()
{
	BYTE byte[] = {0x00, 0x02, 0xFF, 0xFF};
	CProtocol col(byte, 4, 0x09, 0x00, 0xE5);
	m_port.WriteProtocol(&col);
}

// �����޸�ͨ������˿�״̬(0x09)
void CMainFrame::RequestSetCommonOutputPort(const IOCheckData& iodata)
{
	BYTE byte[] = {0x00, 0x11, (BYTE)(iodata.nBit), (BYTE)iodata.nState};
	CProtocol col(byte, 4, 0x09, 0x00, 0xE5);
	m_port.WriteProtocol(&col);
}

// �����޸���������״̬(0x09)
void CMainFrame::RequestSetElectroOutputPort(const IOCheckData& iodata)
{
	BYTE byte[] = {0x00, 0x12, (BYTE)(iodata.nBit), (BYTE)iodata.nState};
	CProtocol col(byte, 4, 0x09, 0x00, 0xE5);
	m_port.WriteProtocol(&col);
}

// �������������(0x0A)
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
		// ����Ǹ��������㲹��
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

// ������������ת��(0xAA)
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

// �������õ��߼�����(0xAB)
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

// �������ù���������(0xAB)
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

// ��������������(0x0B)
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

// �����ȡ���������Ϣ
LRESULT CMainFrame::RequestGetSewingInfo(WPARAM wParam, LPARAM lParam)
{
	lParam;
	wParam;

	CProtocol col(NULL, 0, 0xA8, 0x00, 0xE5);
	m_port.WriteProtocol(&col);
	return NULL;
}

// ���շ��������Ϣ
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

// ��ӵ�ǰ��������ָ��
void CMainFrame::AddCurrSewFileSetCol(int nID, BYTE byteMode)
{
	ByteArray arrByte;
	DecToHex(nID, arrByte);
	if (arrByte.size() == 1)
	{
		arrByte.push_back(0x00);
	}

	BYTE dataSet[] = {0x00, byteMode, arrByte[1], arrByte[0]};	// ��2λ��ʾ����ģʽ
	CProtocol colSet(dataSet, 4, 0xA9, 0x00, 0xE5);
	m_listProcessCol.push_back(colSet);
}

// ��ӵ�ǰ������ȡָ��
void CMainFrame::AddCurrSewFileGetCol()
{
	BYTE data[] = {0xFF, 0xFF, 0xFF, 0xFF};
	CProtocol col(data, 4, 0xA9, 0x00, 0xE5);
	m_listProcessCol.push_back(col);
}

// ��ȡԤ������
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