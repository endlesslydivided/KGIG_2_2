#include "stdafx.h"
#include "ChildView.h"

CRectD::CRectD(double l, double t, double r, double b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}
//------------------------------------------------------------------------------
void CRectD::SetRectD(double l, double t, double r, double b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}

//------------------------------------------------------------------------------
CSizeD CRectD::SizeD()
{
	CSizeD cz;
	cz.cx = fabs(right - left);	// Ширина прямоугольной области
	cz.cy = fabs(top - bottom);	// Высота прямоугольной области
	return cz;
}

//------------------------------------------------------------------------------

CMatrix getConverter(CRectD& rectWorld, CRect& rectWindow)//конвертируем отображение мировых и оконных
{
	CMatrix m(3, 3);
	double kx = (double)(((double)(rectWindow.right - rectWindow.left)) / ((double)(rectWorld.right - rectWorld.left)));//вычисляем отношение между  размером окна и мировых
	double ky = (double)(((double)(rectWindow.bottom - rectWindow.top)) / ((double)(rectWorld.bottom - rectWorld.top)));
	m(0, 1) = m(1, 0) = m(2, 0) = m(2, 1) = 0;
	m(2, 2) = 1;
	m(0, 0) = kx;
	m(0, 2) = rectWindow.left - kx * rectWorld.left;
	m(1, 1) = -ky;
	m(1, 2) = rectWindow.bottom + ky * rectWorld.top;
	return m;
}

//------------------------------------------------------------------------------

int SetMyMode(CDC& dc, CRectD& RS, CRect& RW) //MFC
//dc - ссылка на класс CDCMFC
//RS - область в мировых координатах
//RW - область в оконных координатах 
{
	int dsx = RS.right - RS.left;
	int dsy = RS.top - RS.bottom;
	int xsL = RS.left;
	int ysL = RS.bottom;
	int dwx = RW.right - RW.left;
	int dwy = RW.bottom - RW.top;
	int xwL = RW.left;
	int ywH = RW.bottom;
	dc.SetWindowExt(dsx, dsy);
	dc.SetViewportExt(dwx, -dwy);
	dc.SetWindowOrg(xsL, ysL);
	dc.SetViewportOrg(xwL, ywH);
	int buffer =  dc.SetMapMode(MM_ANISOTROPIC);
	
	return buffer; // устанавливает режим отображения указанного контекста устройства
}

/////////////////////// CLASS CPlot2D //////////////////////////////////

void CPlot2D::SetParams(CMatrix& XX, CMatrix& YY, CRect& RWX)
// XX - вектор данных по X 
// YY - вектор данных по Y 
// RWX - область в окне 
{
	int nRowsX = XX.rows();
	int nRowsY = YY.rows();
	if (nRowsX != nRowsY)
	{
		TCHAR* error = _T("SetParams: неправильные размеры массивов данных");
		MessageBox(NULL, error, _T("Ошибка"), MB_ICONSTOP);

		exit(1);
	}
	X.RedimMatrix(nRowsX);
	Y.RedimMatrix(nRowsY);
	X = XX;
	Y = YY;
	double x_max = X.MaxElement();
	double x_min = X.MinElement();
	double y_max = Y.MaxElement();
	double y_min = Y.MinElement();
	RS.SetRectD(x_min, y_max, x_max, y_min);		// Область в мировой СК
	RW.SetRect(RWX.left, RWX.top, RWX.right, RWX.bottom);	// Область в окне
	K = getConverter(RS, RW);			// Матрица пересчета координат
}

//-------------------------------------------------------------------


void CPlot2D::SetWindowRect(CRect& RWX)
{
	RW.SetRect(RWX.left, RWX.top, RWX.right, RWX.bottom);	// Область в окне
	K = getConverter(RS, RW);			// Матрица пересчета координат
}

//--------------------------------------------------------------------

void CPlot2D::SetPenLine(CMyPen& PLine)
// Установка параметров пера для линии графика
{
	PenLine.PenStyle = PLine.PenStyle;
	PenLine.PenWidth = PLine.PenWidth;
	PenLine.PenColor = PLine.PenColor;
}

//-------------------------------------------------------------------
void CPlot2D::SetPenAxis(CMyPen& PAxis)
// Установка параметров пера для линий осей 
{
	PenAxis.PenStyle = PAxis.PenStyle;
	PenAxis.PenWidth = PAxis.PenWidth;
	PenAxis.PenColor = PAxis.PenColor;
}

//-----------------------------------------------------------------
void CPlot2D::GetRS(CRectD& RS)
// RS - структура, куда записываются параметры области графика
{
	RS.left = (this->RS).left;
	RS.top = (this->RS).top;
	RS.right = (this->RS).right;
	RS.bottom = (this->RS).bottom;
}

void CPlot2D::GetWindowCoords(double xs, double ys, int &xw, int &yw)
// Пересчитывает координаты точки из МСК в оконную
// xs - x- кордината точки в МСК
// ys - y- кордината точки в МСК
// xw - x- кордината точки в оконной СК
// yw - y- кордината точки в оконной СК

{
	CMatrix V(3), W(3);
	V(2) = 1;
	V(0) = xs;
	V(1) = ys;
	W = K * V;
	xw = (int)W(0);
	yw = (int)W(1);
}

//-----------------------------------------------------------------
void CPlot2D::plot(CDC& dc)
// Рисует график в режиме MM_TEXT - собственный пересчет координат
// dc - ссылка на класс CDC MFC
// Ind1=1/0 - рисовать/не рисовать рамку
// Ind2=1/0 - рисовать/не рисовать оси координат
{
	double xs, ys;  // мировые  координаты точки
	int xw, yw;     // оконные координаты точки
	dc.Rectangle(RW);								
		CPen MyPen1(PenAxis.PenStyle, PenAxis.PenWidth, PenAxis.PenColor);
		CPen* pOldPen1 = dc.SelectObject(&MyPen1);
		if (RS.left*RS.right <= 0)							// Нужна Ось Y
		{
			xs = RS.left;  ys = RS.bottom;           // Точка (0,y_max) в МСК
			GetWindowCoords(xs, ys, xw, yw); // (xw,yw) -точка (0,y_max) в ОСК		
			dc.MoveTo(xw, yw);		      // Перо в точку (0,y_max)

			xs = RS.left;  ys = RS.top;        // Точка (0,y_min) в МСК
			GetWindowCoords(xs, ys, xw, yw); // (xw,yw) -точка (0,y_min) в ОСК
			dc.LineTo(xw, yw);		      // Линия (0,y_max) - (0,y_min) - Ось Y
		}

		if (RS.top*RS.bottom <= 0)							// Нужна Ось X
		{
			xs = RS.right ;  ys = RS.bottom;           // (xs,ys) - точка (x_min,0) в МСК
			GetWindowCoords(xs, ys, xw, yw); // (xw,yw) -точка (x_min,0) в ОСК  
			dc.MoveTo(xw, yw);		         // Перо в точку (x_min,0)

			xs = RS.left;  ys = RS.bottom;          // (xs,ys) - точка (x_max,0) в МСК
			GetWindowCoords(xs, ys, xw, yw); // (xw,yw) -точка (x_max,0) в ОСК
			dc.LineTo(xw, yw);		         // Линия (x_min,0) - (x_max,0) - Ось X
		}
		dc.SelectObject(pOldPen1);
	xs = X(0); ys = Y(0);
	GetWindowCoords(xs, ys, xw, yw); // координаты начальной точки графика в ОСК
	CPen MyPen(PenLine.PenStyle, PenLine.PenWidth, PenLine.PenColor);
	CPen* pOldPen = dc.SelectObject(&MyPen);
	dc.MoveTo(xw, yw);        // Перо в начальную точка для рисования графика
	for (int i = 1; i < X.rows(); i++)
	{
		xs = X(i); ys = Y(i);
		GetWindowCoords(xs, ys, xw, yw); // координаты начальной точки графика с номером i в ОСК
		dc.LineTo(xw, yw);
	}
	dc.SelectObject(pOldPen);
}