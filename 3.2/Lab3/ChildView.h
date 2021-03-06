
// ChildView.h: интерфейс класса CChildView
//


#pragma once


// Окно CChildView

class CChildView : public CWnd
{
// Создание
public:
	CChildView();

// Атрибуты
public:
	int Index;
	int N_Bezier;
	CMatrix X, Y;
	CRect RW;	// Прямоугольник в окне
	CRectD RS;	// Прямоугольник области в МСК
	CPlot2D plot1;
	CPlot2D plot2;
	CPlot2D plot3;
	CPlot2D plot4;
public: CMyPen PenLine, // Перо для линий
		   PenAxis;	// Перо для осей
	  int graphType;

// Операции
public:
	double MyF1(double x);
	double MyF2(double x);
	double MyF3(double x);
	double MyF4(double x);
// Переопределение
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Реализация
public:
	virtual ~CChildView();

	// Созданные функции схемы сообщений
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTestsF1();
	afx_msg void OnTestsF2();
	afx_msg void OnTestsF3();
	afx_msg void OnTestsF4();
	afx_msg void OnTestsF1234();
};

