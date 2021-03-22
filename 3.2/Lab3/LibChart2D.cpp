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
	cz.cx = fabs(right - left);	// ������ ������������� �������
	cz.cy = fabs(top - bottom);	// ������ ������������� �������
	return cz;
}

//------------------------------------------------------------------------------

CMatrix getConverter(CRectD& rectWorld, CRect& rectWindow)//������������ ����������� ������� � �������
{
	CMatrix m(3, 3);
	double kx = (double)(((double)(rectWindow.right - rectWindow.left)) / ((double)(rectWorld.right - rectWorld.left)));//��������� ��������� �����  �������� ���� � �������
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
//dc - ������ �� ����� CDCMFC
//RS - ������� � ������� �����������
//RW - ������� � ������� ����������� 
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
	
	return buffer; // ������������� ����� ����������� ���������� ��������� ����������
}

/////////////////////// CLASS CPlot2D //////////////////////////////////

void CPlot2D::SetParams(CMatrix& XX, CMatrix& YY, CRect& RWX)
// XX - ������ ������ �� X 
// YY - ������ ������ �� Y 
// RWX - ������� � ���� 
{
	int nRowsX = XX.rows();
	int nRowsY = YY.rows();
	if (nRowsX != nRowsY)
	{
		TCHAR* error = _T("SetParams: ������������ ������� �������� ������");
		MessageBox(NULL, error, _T("������"), MB_ICONSTOP);

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
	RS.SetRectD(x_min, y_max, x_max, y_min);		// ������� � ������� ��
	RW.SetRect(RWX.left, RWX.top, RWX.right, RWX.bottom);	// ������� � ����
	K = getConverter(RS, RW);			// ������� ��������� ���������
}

//-------------------------------------------------------------------


void CPlot2D::SetWindowRect(CRect& RWX)
{
	RW.SetRect(RWX.left, RWX.top, RWX.right, RWX.bottom);	// ������� � ����
	K = getConverter(RS, RW);			// ������� ��������� ���������
}

//--------------------------------------------------------------------

void CPlot2D::SetPenLine(CMyPen& PLine)
// ��������� ���������� ���� ��� ����� �������
{
	PenLine.PenStyle = PLine.PenStyle;
	PenLine.PenWidth = PLine.PenWidth;
	PenLine.PenColor = PLine.PenColor;
}

//-------------------------------------------------------------------
void CPlot2D::SetPenAxis(CMyPen& PAxis)
// ��������� ���������� ���� ��� ����� ���� 
{
	PenAxis.PenStyle = PAxis.PenStyle;
	PenAxis.PenWidth = PAxis.PenWidth;
	PenAxis.PenColor = PAxis.PenColor;
}

//-----------------------------------------------------------------
void CPlot2D::GetRS(CRectD& RS)
// RS - ���������, ���� ������������ ��������� ������� �������
{
	RS.left = (this->RS).left;
	RS.top = (this->RS).top;
	RS.right = (this->RS).right;
	RS.bottom = (this->RS).bottom;
}

void CPlot2D::GetWindowCoords(double xs, double ys, int &xw, int &yw)
// ������������� ���������� ����� �� ��� � �������
// xs - x- ��������� ����� � ���
// ys - y- ��������� ����� � ���
// xw - x- ��������� ����� � ������� ��
// yw - y- ��������� ����� � ������� ��

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
// ������ ������ � ������ MM_TEXT - ����������� �������� ���������
// dc - ������ �� ����� CDC MFC
// Ind1=1/0 - ��������/�� �������� �����
// Ind2=1/0 - ��������/�� �������� ��� ���������
{
	double xs, ys;  // �������  ���������� �����
	int xw, yw;     // ������� ���������� �����
	dc.Rectangle(RW);								
		CPen MyPen1(PenAxis.PenStyle, PenAxis.PenWidth, PenAxis.PenColor);
		CPen* pOldPen1 = dc.SelectObject(&MyPen1);
		if (RS.left*RS.right <= 0)							// ����� ��� Y
		{
			xs = RS.left;  ys = RS.bottom;           // ����� (0,y_max) � ���
			GetWindowCoords(xs, ys, xw, yw); // (xw,yw) -����� (0,y_max) � ���		
			dc.MoveTo(xw, yw);		      // ���� � ����� (0,y_max)

			xs = RS.left;  ys = RS.top;        // ����� (0,y_min) � ���
			GetWindowCoords(xs, ys, xw, yw); // (xw,yw) -����� (0,y_min) � ���
			dc.LineTo(xw, yw);		      // ����� (0,y_max) - (0,y_min) - ��� Y
		}

		if (RS.top*RS.bottom <= 0)							// ����� ��� X
		{
			xs = RS.right ;  ys = RS.bottom;           // (xs,ys) - ����� (x_min,0) � ���
			GetWindowCoords(xs, ys, xw, yw); // (xw,yw) -����� (x_min,0) � ���  
			dc.MoveTo(xw, yw);		         // ���� � ����� (x_min,0)

			xs = RS.left;  ys = RS.bottom;          // (xs,ys) - ����� (x_max,0) � ���
			GetWindowCoords(xs, ys, xw, yw); // (xw,yw) -����� (x_max,0) � ���
			dc.LineTo(xw, yw);		         // ����� (x_min,0) - (x_max,0) - ��� X
		}
		dc.SelectObject(pOldPen1);
	xs = X(0); ys = Y(0);
	GetWindowCoords(xs, ys, xw, yw); // ���������� ��������� ����� ������� � ���
	CPen MyPen(PenLine.PenStyle, PenLine.PenWidth, PenLine.PenColor);
	CPen* pOldPen = dc.SelectObject(&MyPen);
	dc.MoveTo(xw, yw);        // ���� � ��������� ����� ��� ��������� �������
	for (int i = 1; i < X.rows(); i++)
	{
		xs = X(i); ys = Y(i);
		GetWindowCoords(xs, ys, xw, yw); // ���������� ��������� ����� ������� � ������� i � ���
		dc.LineTo(xw, yw);
	}
	dc.SelectObject(pOldPen);
}