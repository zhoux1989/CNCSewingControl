//===================================================================================================
// Summary:
//		����ͼ��
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

// CCNCSewingControlView ����/����

CCNCSewingControlView::CCNCSewingControlView()
{
	// TODO: �ڴ˴���ӹ������

}

CCNCSewingControlView::~CCNCSewingControlView()
{
}

BOOL CCNCSewingControlView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// ���óߴ�
void CCNCSewingControlView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
}

// CCNCSewingControlView ����

void CCNCSewingControlView::OnDraw(CDC* pDC)
{
	CCNCSewingControlDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���

	// ���ñ�����ʾ����
	CMainFrame* pMainFrame =(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	if (pMainFrame != NULL)
	{
		CFont newFont;
		newFont.CreatePointFont(400, L"���Ĳ���", pDC);
		CFont* poldFont = pDC->SelectObject(&newFont);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(125, 125, 125));
		CRect rectClient;
		this->GetClientRect(rectClient);

		switch (pMainFrame->m_statePreview)
		{
		default:
		case PS_CLOSE:
			pDC->TextOut(rectClient.Width()/2-160, rectClient.Height()/2-40, L"����˫����һ�");
			break;
		case PS_PREPARE:
			pDC->TextOut(rectClient.Width()/2-160, rectClient.Height()/2-40, L"�˴�Ԥ������");
			break;
		case PS_VALID:
			PreviewSewfile(pMainFrame->m_trailToPreview);
			break;
		case PS_INVALID:
			pDC->TextOut(rectClient.Width()/2-160, rectClient.Height()/2-40, L"�޷���ʾԤ��");
			break;
		case PS_NOCURRENT:
			pDC->TextOut(rectClient.Width()/2-160, rectClient.Height()/2-40, L"δ���õ�ǰ����");
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


// CCNCSewingControlView ���

#ifdef _DEBUG
void CCNCSewingControlView::AssertValid() const
{
	CView::AssertValid();
}

void CCNCSewingControlView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCNCSewingControlDoc* CCNCSewingControlView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCNCSewingControlDoc)));
	return (CCNCSewingControlDoc*)m_pDocument;
}
#endif //_DEBUG


// CCNCSewingControlView ��Ϣ�������

// Ԥ�������ļ�
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

	// ���غ�ɫ����
	CPen penTrail, penEmptyMove;
	penTrail.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	penEmptyMove.CreatePen(PS_DOT, 1, RGB(0,0,255));
	CPen *pOldPen = pDC->SelectObject(&penTrail);

	// ���Ͻ���ʾ�����ļ�������Ϣ
	CFont newFont;
	newFont.CreatePointFont(120, L"����", pDC);
	CFont* poldFont = pDC->SelectObject(&newFont);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0, 0, 0));

	CMainFrame* pMainFrame =(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	CString strTips;
	strTips.Format(L"�������/�ļ����ƣ�%s", trailToPreview.strDesc);
	pDC->TextOut(10, 10, strTips);
	//pDC->TextOut(10, 40, L"����ʱ�䣺");
	strTips.Format(L"������%d", trailToPreview.arrNode.size()+1);
	pDC->TextOut(10, 40, strTips);

	// ���ݻ����ļ���Χ�������ű���
	double dZoomRatio = 1.0;
	double dZoomRatioX = (trailToPreview.nPlusMaxX-trailToPreview.nMinusMaxX)*PULSE_UNIT;
	if (dZoomRatioX > 0)
		dZoomRatioX = rectPreview.Width()/dZoomRatioX;
	double dZoomRatioY = (trailToPreview.nPlusMaxY-trailToPreview.nMinusMaxY)*PULSE_UNIT;
	if (dZoomRatioY > 0)
		dZoomRatioY = rectPreview.Height()/dZoomRatioY;
	dZoomRatio = dZoomRatioX>dZoomRatioY?dZoomRatioY:dZoomRatioX;

	// ��ȡ�����ļ������� 
	double dTrailCenterX = (trailToPreview.nPlusMaxX+trailToPreview.nMinusMaxX)*PULSE_UNIT/2;
	double dTrailCenterY = (trailToPreview.nPlusMaxY+trailToPreview.nMinusMaxY)*PULSE_UNIT/2;
	CPoint pntCenter;
	pntCenter.x = m_previewCtrl.ptOrigin.x+(int)(dTrailCenterX*dZoomRatio);
	pntCenter.y = m_previewCtrl.ptOrigin.y-(int)(dTrailCenterY*dZoomRatio);
	CPoint pntOffset;
	pntOffset.x = m_previewCtrl.ptOrigin.x-pntCenter.x;
	pntOffset.y = m_previewCtrl.ptOrigin.y-pntCenter.y;

	// �ƶ������
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

// ��������ʶ
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