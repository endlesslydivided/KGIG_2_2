#pragma once

#include "CMATRIX.H"
#include "LibChart2D.h"

// окно CChildView

class CChildView : public CWnd
{
	// Создание
public:
	CChildView();

	// Атрибуты
public:
	CSolarSystem SolarSystem;
	int DtTimer;
	CRect RectWindow;
	CRectD RectWorld;
	bool Start = false;

	// Операции
public:

	// Переопределение
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// Реализация
public:
	virtual ~CChildView();

	// Созданные функции схемы сообщений
protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT, CPoint);
	afx_msg void OnRButtonDblClk(UINT, CPoint);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDraw();
};