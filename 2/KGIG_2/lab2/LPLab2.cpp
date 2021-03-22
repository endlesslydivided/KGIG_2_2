#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "LPLab2.h"
#include "MainFrm.h"
#include "GraphBitMap.h"

BEGIN_MESSAGE_MAP(Clab2App, CWinApp)
	ON_COMMAND(ID_FILE_LOADIMAGE, &Clab2App::LoadImageBMP)
	ON_COMMAND(ID_FILE_SAVEAREA, &Clab2App::SaveAreaBMP)
	ON_COMMAND(ID_FILE_SAVESCREEN, &Clab2App::SaveScreen)
END_MESSAGE_MAP()


//�������� �����������
void Clab2App::LoadImageBMP()
{
	CFileDialog fileDialog(true,NULL,L"");						///������� ������
	fileDialog.m_ofn.lpstrFilter = L"images (.bmp)\0*.bmp\0\0"; 
	int result = fileDialog.DoModal();							///����� ���� ���� � ��������� ���
	if (result==IDOK)	///���� ���� ������
	{
		CWnd *cwnd = AfxGetMainWnd();
		hwnd = cwnd->GetSafeHwnd();		///������ �� �����
		hBit = (HBITMAP)LoadImage(NULL
			, fileDialog.GetPathName().GetBuffer(), 
			IMAGE_BITMAP,
			0,
			0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);	
		ShowBitMap(hwnd, hBit, Choose.x, Choose.y);		///����� �����
	}
}


void Clab2App::SaveArea() 
{
	CFileDialog fileDialog(FALSE, NULL, L"names.bmp");		///������� ������	
	fileDialog.m_ofn.lpstrFilter = L"images (.bmp)\0*.bmp\0\0";
	int result = fileDialog.DoModal();					///����� ���� ���� � ��������� ���
	if (result==IDOK)	///���� ���� ������
	{
		CWnd *cwnd = AfxGetMainWnd();		///��������� ��-�� �� ����� ����
		HWND hwnd = cwnd->GetSafeHwnd();	///������ �� �����,hWnd - ���������� ����, ���� ��� �����
		RECT r;				///��������� ��� ����� ����-��, r � ������� � ����, ���� ��������� �����
		r.left   = From.x;
		r.top = From.y;
		r.right  = To.x;
		r.bottom = To.y;
		if (ClientRectToBmp(hwnd, LPSTR(fileDialog.GetPathName().GetBuffer()), r) != 0)
			AfxMessageBox(L"Error");
	}
}
void Clab2App::SaveScreen() 
{
	CFileDialog fileDialog(FALSE, NULL, L"screen.bmp");	
	fileDialog.m_ofn.lpstrFilter = L"images(.bmp)\0*.bmp\0\0";
	int result = fileDialog.DoModal();	
	if (result==IDOK)	
	{
		CWnd *cwnd = AfxGetMainWnd();
		HWND hwnd = cwnd->GetSafeHwnd();

		if (!ClientToBmp(hwnd, (LPSTR)fileDialog.GetPathName().GetBuffer()) == 0)
			AfxMessageBox(L"Error");
	}
}
void Clab2App::SaveAreaBMP()
{
	SaveAr = true;
}
Clab2App::Clab2App()
{
	From.x = From.y = 0;
	To.x = To.y = 0;
}
Clab2App theApp;

BOOL Clab2App::InitInstance()
{
	CWinApp::InitInstance();

	EnableTaskbarInteraction(FALSE);
	SetRegistryKey(L"Local AppWizard-Generated Applications");
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

int Clab2App::ExitInstance()
{
	return CWinApp::ExitInstance();
}

class CAboutDlg : public CDialogEx
{
	protected: 	virtual void DoDataExchange(CDataExchange* pDX);  
	protected:	DECLARE_MESSAGE_MAP()
};

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}