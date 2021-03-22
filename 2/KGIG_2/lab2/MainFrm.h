#pragma once
#include "ChildView.h"

class CMainFrame : public CFrameWnd
{
protected: 
	DECLARE_DYNAMIC(CMainFrame)

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd); // ��� ������ ������ �������
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

#ifdef _DEBUG
	virtual void AssertValid() const;//��������� �������� �������� ������������� �������
	virtual void Dump(CDumpContext& dc) const;//��� ������ ������ �������� � ������
#endif

	CChildView    m_wndView;
};


