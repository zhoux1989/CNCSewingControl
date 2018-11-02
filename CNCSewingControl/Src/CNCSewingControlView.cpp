//===================================================================================================
// Summary:
//		主视图类
// Date:
//		2018-07-07
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#include "stdafx.h"
#include "CNCSewingControl.h"
#include "MainFrm.h"
#include "CNCSewingControlDoc.h"
#include "CNCSewingControlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCNCSewingControlView

IMPLEMENT_DYNCREATE(CCNCSewingControlView, CView)

BEGIN_MESSAGE_MAP(CCNCSewingControlView, CView)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CCNCSewingControlView 构造/析构

CCNCSewingControlView::CCNCSewingControlView()
{
	// TODO: 在此处添加构造代码

}

CCNCSewingControlView::~CCNCSewingControlView()
{
}

BOOL CCNCSewingControlView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// 设置尺寸
void CCNCSewingControlView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
}

// CCNCSewingControlView 绘制

void CCNCSewingControlView::OnDraw(CDC* pDC)
{
	CCNCSewingControlDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码

	// 设置背景显示文字
	CMainFrame* pMainFrame =(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	if (pMainFrame != NULL)
	{
		CFont newFont;
		newFont.CreatePointFont(400, L"华文彩云", pDC);
		CFont* poldFont = pDC->SelectObject(&newFont);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(125, 125, 125));
		CRect rectClient;
		this->GetClientRect(rectClient);

		switch (pMainFrame->m_statePreview)
		{
		default:
		case PS_CLOSE:
			pDC->TextOut(rectClient.Width()/2-160, rectClient.Height()/2-40, L"舒普双针缝纫机");
			break;
		case PS_PREPARE:
			pDC->TextOut(rectClient.Width()/2-160, rectClient.Height()/2-40, L"此处预览花样");
			break;
		case PS_VALID:
			PreviewSewfile(pMainFrame->m_trailToPreview);
			break;
		case PS_INVALID:
			pDC->TextOut(rectClient.Width()/2-160, rectClient.Height()/2-40, L"无法显示预览");
			break;
		case PS_NOCURRENT:
			pDC->TextOut(rectClient.Width()/2-160, rectClient.Height()/2-40, L"未设置当前花样");
			break;
		}
		pDC->SelectObject(poldFont);
		newFont.DeleteObject();
	}
}

void CCNCSewingControlView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCNCSewingControlView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CCNCSewingControlView 诊断

#ifdef _DEBUG
void CCNCSewingControlView::AssertValid() const
{
	CView::AssertValid();
}

void CCNCSewingControlView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCNCSewingControlDoc* CCNCSewingControlView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCNCSewingControlDoc)));
	return (CCNCSewingControlDoc*)m_pDocument;
}
#endif //_DEBUG


// CCNCSewingControlView 消息处理程序

// 预览花样文件
void CCNCSewingControlView::PreviewSewfile(const NeedleTrailData& trailToPreview)
{
	if (trailToPreview.arrNode.size() == 0)
	{
		return;
	}

	CRect rectPreview;
	GetClientRect(rectPreview);
	m_previewCtrl.ptOrigin.x = rectPreview.Width()/2;
	m_previewCtrl.ptOrigin.y = rectPreview.Height()/2;
	rectPreview.DeflateRect(10, 70, 10, 10);
	m_previewCtrl.rectPreview = rectPreview;

	CDC *pDC = GetDC();

	// 加载黑色画笔
	CPen penTrail, penEmptyMove;
	penTrail.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	penEmptyMove.CreatePen(PS_DOT, 1, RGB(0,0,255));
	CPen *pOldPen = pDC->SelectObject(&penTrail);

	// 左上角显示花样文件基本信息
	CFont newFont;
	newFont.CreatePointFont(120, L"宋体", pDC);
	CFont* poldFont = pDC->SelectObject(&newFont);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0, 0, 0));

	CMainFrame* pMainFrame =(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	CString strTips;
	strTips.Format(L"花样编号/文件名称：%s", trailToPreview.strDesc);
	pDC->TextOut(10, 10, strTips);
	//pDC->TextOut(10, 40, L"创建时间：");
	strTips.Format(L"针数：%d", trailToPreview.arrNode.size()+1);
	pDC->TextOut(10, 40, strTips);

	// 根据花样文件范围计算缩放比例
	double dZoomRatio = 1.0;
	double dZoomRatioX = (trailToPreview.nPlusMaxX-trailToPreview.nMinusMaxX)*PULSE_UNIT;
	if (dZoomRatioX > 0)
		dZoomRatioX = rectPreview.Width()/dZoomRatioX;
	double dZoomRatioY = (trailToPreview.nPlusMaxY-trailToPreview.nMinusMaxY)*PULSE_UNIT;
	if (dZoomRatioY > 0)
		dZoomRatioY = rectPreview.Height()/dZoomRatioY;
	dZoomRatio = dZoomRatioX>dZoomRatioY?dZoomRatioY:dZoomRatioX;

	// 获取花样文件的中心 
	double dTrailCenterX = (trailToPreview.nPlusMaxX+trailToPreview.nMinusMaxX)*PULSE_UNIT/2;
	double dTrailCenterY = (trailToPreview.nPlusMaxY+trailToPreview.nMinusMaxY)*PULSE_UNIT/2;
	CPoint pntCenter;
	pntCenter.x = m_previewCtrl.ptOrigin.x+(int)(dTrailCenterX*dZoomRatio);
	pntCenter.y = m_previewCtrl.ptOrigin.y-(int)(dTrailCenterY*dZoomRatio);
	CPoint pntOffset;
	pntOffset.x = m_previewCtrl.ptOrigin.x-pntCenter.x;
	pntOffset.y = m_previewCtrl.ptOrigin.y-pntCenter.y;

	// 移动到起点
	double dNeedlePosX = trailToPreview.nStartPulseX*PULSE_UNIT;
	double dNeedlePosY = trailToPreview.nStartPulseY*PULSE_UNIT;
	CPoint pnt;
	pnt.x = m_previewCtrl.ptOrigin.x+(int)(dNeedlePosX*dZoomRatio)+pntOffset.x;
	pnt.y = m_previewCtrl.ptOrigin.y-(int)(dNeedlePosY*dZoomRatio)+pntOffset.y;
	pDC->MoveTo(pnt);
	DrawStartEndFlag(pDC, pnt, TRUE);

	for (int i=0; i<trailToPreview.arrNode.size(); i++)
	{
		dNeedlePosX += trailToPreview.arrNode[i].lPulseOffsetX*PULSE_UNIT;
		dNeedlePosY += trailToPreview.arrNode[i].lPulseOffsetY*PULSE_UNIT;
		pnt.x = m_previewCtrl.ptOrigin.x+(int)(dNeedlePosX*dZoomRatio)+pntOffset.x;
		pnt.y = m_previewCtrl.ptOrigin.y-(int)(dNeedlePosY*dZoomRatio)+pntOffset.y;
		if (trailToPreview.arrNode[i].bEmptyMoveFlag)
		{
			if (m_previewCtrl.bShowEmptyMoveTrail)
			{
				pDC->SelectObject(&penEmptyMove);
				pDC->LineTo(pnt);
				pDC->SelectObject(&penTrail);
			}
			else
			{
				pDC->MoveTo(pnt);
			}
		}
		else
		{
			pDC->LineTo(pnt);
		}
	}
	DrawStartEndFlag(pDC, pnt, FALSE);

	pDC->SelectObject(pOldPen);
	penTrail.DeleteObject();
	penEmptyMove.DeleteObject();
	ReleaseDC(pDC);
}

// 绘制起点标识
void CCNCSewingControlView::DrawStartEndFlag(CDC* pDC, CPoint pnt, BOOL bStart)
{
	CBrush brush;
	if (bStart)
	{
		brush.CreateSolidBrush(RGB(0, 0, 255));
	}
	else
	{
		brush.CreateSolidBrush(RGB(255, 0, 0));
	}
	int nSize = 5;
	CRect rectFlg(pnt.x-nSize, pnt.y-nSize, pnt.x+nSize, pnt.y+nSize);
	pDC->FillRect(rectFlg, &brush);
	brush.DeleteObject();
}