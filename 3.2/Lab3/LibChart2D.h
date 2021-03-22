#ifndef LIBGRAPH
#define LIBGRAPH 1
const double pi = 3.14159;
//typedef double (*pfunc)(double);	// ��������� �� �������

struct CSizeD
{
	double cx;
	double cy;
};
//-------------------------------------------------------------------------------
struct CRectD
{
	double left;
	double top;
	double right;
	double bottom;
	CRectD() { left = top = right = bottom = 0; };
	CRectD(double l, double t, double r, double b);
	void SetRectD(double l, double t, double r, double b);
	CSizeD SizeD();	// ���������� �������(������, ������) �������������� 
};

//-------------------------------------------------------------------------------
struct CMyPen
{
	int PenStyle;				// ����� ����
	public: int PenWidth;				// ������� ���� 
	COLORREF PenColor;	// ���� ���� 
	CMyPen() { PenStyle = PS_SOLID; PenWidth = 1; PenColor = RGB(0, 0, 0); };
	void Set(int PS, int PW, COLORREF PC) { PenStyle = PS; PenWidth = PW; PenColor = PC; };
};
//-------------------------------------------------------------------------------

CMatrix getConverter(CRectD& rectWorld, CRect& rectWindow);
// ���������� ������� ��������� ��������� �� ������� � �������
// rs - ������� � ������� ����������� - double
// rw - ������� � ������� ����������� - int

//------------------------------------------------------------------------------
int SetMyMode(CDC& dc, CRectD& RS, CRect& RW);  //MFC
												 // ������������� ����� ����������� MM_ANISOTROPIC � ��� ���������
												

class CPlot2D
{
public:
	CMatrix X;		// ��������
	CMatrix Y;		// �������
	CMatrix K;		// ������� ��������� ��������
	CRect RW;		// ������������� � ����
	CRectD RS;		// ������������� ������� � ���
	CMyPen PenLine;		// ���� ��� �����
public :  CMyPen PenAxis;		// ���� ��� ����
public:
	CPlot2D() { K.RedimMatrix(3, 3); };		// �������� ������ ������� � ����������� ������, ������� ����� ���������
	void SetParams(CMatrix& XX, CMatrix& YY, CRect& RWX);	// ��������� ���������� �������
	void SetWindowRect(CRect& RWX);		//��������� ������� � ���� ��� ����������� �������
	void GetWindowCoords(double xs, double ys, int &xw, int &yw);	//�������� ���������� ����� �� ��� � ������� ��
	void SetPenLine(CMyPen& PLine);		// ���� ��� ��������� �������
	void SetPenAxis(CMyPen& PAxis);		// ���� ��� ���� ���������
	void plot(CDC& dc);
	void GetRS(CRectD& RS);		// ���������� ������� ������� � ������� ��
};


#endif