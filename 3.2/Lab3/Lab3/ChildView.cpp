
// ChildView.cpp: реализация класса CChildView
//

#include "stdafx.h"
#include "Lab3.h"
#include "ChildView.h"

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
	ON_COMMAND(ID_TESTS_F1, &CChildView::OnTestsF1)
	ON_COMMAND(ID_TESTS_F2, &CChildView::OnTestsF2)
	ON_COMMAND(ID_TESTS_F3, &CChildView::OnTestsF3)
	ON_COMMAND(ID_TESTS_F4, &CChildView::OnTestsF4)
	ON_COMMAND(ID_TESTS_F1234, &CChildView::OnTestsF1234)
END_MESSAGE_MAP()



// Обработчики сообщений CChildView

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint()
{
	CPaintDC dc(this); // контекст устройства для рисования
	if (Index == 1)
	{
		Graph.Draw(dc, 1, 1);
	}

	if (Index == 2)
	{
		Graph.GetRS(RS);
		SetMyMode(dc, RS, RW);	      // Устанавливает режим отображения MM_ANISOTROPIC 
		Graph.Draw1(dc, 1, 1);
		dc.SetMapMode(MM_TEXT);			 // Устанавливает режим отображения MM_TEXT
	}
}

double CChildView::MyF1(double x)
{
	double y = sin(x) / x;
	return y;
}

double CChildView::MyF2(double x)
{
	double y = pow(x, 3);
	return y;
}

double CChildView::MyF3(double x)
{
	double y = sqrt(x) * sin(x);
	return y;
}

double CChildView::MyF4(double x)
{
	double y = x * x;
	return y;
}

void CChildView::OnTestsF1()	// MM_TEXT
{
	double xL = -3 * pi;
	double xH = -xL;
	double dx = pi / 36;
	int N = (xH - xL) / dx;
	X.RedimMatrix(N + 1);	// Изменяет размер матрицы с уничтожением данных
	Y.RedimMatrix(N + 1);
	for (int i = 0; i <= N; i++)
	{
		X(i) = xL + i * dx;
		Y(i) = MyF1(X(i));
	}
	PenLine.Set(PS_SOLID, 1, RGB(255, 0, 0));
	PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 255));
	RW.SetRect(270, 35, 670, 435);
	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);
	Index = 1;
	this->Invalidate();
}

void CChildView::OnTestsF2()
{
	double xL = -5;
	double xH = -xL;
	double dx = 0.25;
	int N = (xH - xL) / dx;
	X.RedimMatrix(N + 1);
	Y.RedimMatrix(N + 1);
	for (int i = 0; i <= N; i++)
	{
		X(i) = xL + i * dx;
		Y(i) = MyF2(X(i));
	}
	PenLine.Set(PS_SOLID, 0, RGB(0, 255, 0));
	PenAxis.Set(PS_SOLID, 0, RGB(0, 0, 255));
	RW.SetRect(270, 35, 670, 435);
	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);
	Index = 2;
	this->Invalidate();
}


void CChildView::OnTestsF3()
{
	double xL = 0;
	double xH = 6 * pi;
	double dx = pi / 36;
	int N = (xH - xL) / dx;
	X.RedimMatrix(N + 1);	// Изменяет размер матрицы с уничтожением данных
	Y.RedimMatrix(N + 1);
	for (int i = 0; i <= N; i++)
	{
		X(i) = xL + i * dx;
		Y(i) = MyF3(X(i));
	}
	PenLine.Set(PS_DASHDOT, 3, RGB(255, 0, 0));
	PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 0));
	RW.SetRect(270, 35, 670, 435);
	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);
	Index = 1;
	this->Invalidate();
}


void CChildView::OnTestsF4()
{
	double xL = -10;
	double xH = -xL;
	double dx = 0.25;
	int N = (xH - xL) / dx;
	X.RedimMatrix(N + 1);
	Y.RedimMatrix(N + 1);
	for (int i = 0; i <= N; i++)
	{
		X(i) = xL + i * dx;
		Y(i) = MyF4(X(i));
	}
	PenLine.Set(PS_SOLID, 0, RGB(255, 0, 0));
	PenAxis.Set(PS_SOLID, 0, RGB(0, 0, 255));
	RW.SetRect(270, 35, 670, 435);
	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);
	Index = 2;
	this->Invalidate();
}


void CChildView::OnTestsF1234()
{
	double xL = -10;
	double xH = -xL;
	double dx = 0.25;
	int N = (xH - xL) / dx;
	X.RedimMatrix(N + 1);
	Y.RedimMatrix(N + 1);
	for (int i = 0; i <= N; i++)
	{
		X(i) = xL + i * dx;
		Y(i) = MyF4(X(i));
	}
	PenLine.Set(PS_SOLID, 2, RGB(255, 0, 0));
	PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 255));
	RW.SetRect(270, 35, 670, 435);
	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);
	Index = 1;
	this->Invalidate();
}
