#include "stdafx.h"
#include "Lab2.h"
#include "ChildView.h"
#include "CMATRIX.H"
#include "LibChart2D.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_COMMAND(ID_DRAW, &CChildView::OnDraw)
END_MESSAGE_MAP()

// ����������� ��������� CChildView

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER | WS_OVERLAPPEDWINDOW;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return TRUE;
}

void CChildView::OnPaint()
{
	CPaintDC dc(this);
	if (Start)
	{
		SolarSystem.getRectWorld(RectWorld);				//��������� ������� ��������� ����
		RectWindow = SolarSystem.getRectWindow();			//��������� ������� ��������� ����
		setMode(dc, RectWindow, RectWorld);					//��������� ����� ����������� MM_ANISOTROPIC
		SolarSystem.draw(dc);								//��������� ��������� �������
		dc.SetMapMode(MM_TEXT);								//����������� � ����� ����������� MM_TEXT
	}
}

void CChildView::OnDraw()
{
	SolarSystem.setDt(0);						//������ ��������� �������� �������� ��������
	SolarSystem.setCoords();					//������ ����������
	SolarSystem.setDt(0.1);						//������ �������� ��������
	DtTimer = 15;								//����� �������� � ������������ ��� ������� SetTimer()
	Start = true;								//��������� ��������� � OnPaint()
	Invalidate();
}
 
void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	SolarSystem.setCoords();
	Invalidate();
}

void CChildView::OnLButtonDblClk(UINT flags, CPoint point)
{
	SetTimer(1, DtTimer, NULL);
}

void CChildView::OnRButtonDblClk(UINT flags, CPoint point)
{
	KillTimer(1);
}