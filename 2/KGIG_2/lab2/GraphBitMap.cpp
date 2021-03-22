#pragma once
#include "stdafx.h"
#include "GraphBitMap.h"
#include <fstream>

int counter = 0;



//ОТОБРАЖЕНИЕ (ЧТ. С ДИСКА) РИСУНКА BMP В ЗАД.ОБЛАСТИ ОКНА

int ShowBitMap(HWND hWnd,	///дискриптор окна, куда выв. изобр.
	HANDLE hBit,			///дискриптор рисунка
	int x, int y)			///коорд. угла изобр. в окне вывода
{

	BITMAP BitMap; 
	// извлекает информацию для заданного графического объекта.
	GetObjectW(hBit, sizeof(BITMAP), &BitMap);
	int Height = BitMap.bmHeight;
	int Width = BitMap.bmWidth;
	//звлекает дескриптор дисплейного контекста устройства (DC) для рабочей области заданного окна или для всего экрана
	HDC hdc = GetDC(hWnd);
	//создает контекст устройства в памяти  (DC), совместимый с заданным устройством.
	HDC hdcMem = CreateCompatibleDC(hdc);

	//отображает объект в заданный контекст устройства(DC).Новый объект заменяет предыдущий объект того же самого типа
	HBITMAP OldBitmap = (HBITMAP)SelectObject(hdcMem, hBit);
	/*выполняет передачу битовых блоков данных о цвете, соответствующих прямоугольнику пикселей 
	из заданного исходного контекста устройства в целевой контекст устройства*/
	BitBlt(hdc, x, y, Width, Height, hdcMem, 0, 0, SRCCOPY);
	return 0;
}



//ЗАПИСЬ ИЗОБР NAME.BMP НА ДИСК

int ClientToBmp(HWND hWnd,	///дискриптор окна, раб.область кот-го сохр.
	char *Name)				///имя файла для сохр
{
	RECT r;///область в  окне, которая сохраняется в файле
	GetClientRect (hWnd, &r); ///размер рабочей области
	return ClientRectToBmp (hWnd, Name, r);
}




int ClientRectToBmp(HWND hWnd, char* name, RECT r) 
	{
	HANDLE fh = CreateFile((LPCWSTR)name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	BITMAPINFOHEADER bi;
	ZeroMemory (&bi, sizeof (BITMAPINFOHEADER));

	bi.biSize = sizeof (BITMAPINFOHEADER);  ///Размер файла
	bi.biWidth = r.right - r.left;;
	bi.biHeight = r.bottom - r.top;
	bi.biPlanes = 0; 
	bi.biBitCount = 32; ///количество бит на пиксель
	bi.biSizeImage = (bi.biWidth * bi.biBitCount + 31)/32*4*bi.biHeight; ///вычисляем размер картинки

	BITMAPFILEHEADER bmfHdr;
	ZeroMemory (&bmfHdr, sizeof (BITMAPFILEHEADER));
	bmfHdr.bfType = 0x4D42; 
	bmfHdr.bfSize = bi.biSizeImage + sizeof (BITMAPFILEHEADER)+bi.biSize;
	bmfHdr.bfReserved1 = bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)bi.biSize; 


	HDC hDC = GetDC (hWnd);
	//создает контекст устройства в памяти  (DC), совместимый с заданным устройством.
	HDC hDCMem = CreateCompatibleDC (hDC);

	//создает точечный рисунок, совместимый с устройством, которое связано с заданным контекстом устройств
	HBITMAP hBitmap = CreateCompatibleBitmap (hDC, bi.biWidth, bi.biHeight); 

	//отображает объект в заданный контекст устройства(DC).Новый объект заменяет предыдущий объект того же самого типа
	HBITMAP oldBitmap = (HBITMAP)SelectObject (hDCMem, hBitmap);

	/*выполняет передачу битовых блоков данных о цвете, соответствующих прямоугольнику пикселей из 
	заданного исходного контекста устройства в целевой контекст устройства*/
	BitBlt (hDCMem, 0, 0, bi.biWidth, bi.biHeight, hDC, r.left, r.top, SRCCOPY);
	
	//отображает объект в заданный контекст устройства(DC).Новый объект заменяет предыдущий объект того же самого типа
	hBitmap = (HBITMAP)SelectObject (hDCMem, oldBitmap);

	HANDLE hDIB = GlobalAlloc (GHND , bi.biSizeImage);
	char* lp = (char*)GlobalLock (hDIB);

	//извлекает биты заданного совместимого точечного рисунка и копирует их в буфер как DIB, используя заданный формат.
	GetDIBits (hDC,			// дескриптор DC
		hBitmap,			//дескриптор рисунка
		0,					// первая устанавливаемая строка развертки 
		bi.biHeight,		 // число копируемых строк развертки
		lp,					// массив для битов рисунка 
		(LPBITMAPINFO)&bi,	// буфер данных рисунка
		DIB_RGB_COLORS		// индексы RGB или палитры
	);
	DWORD dwWritten = sizeof (BITMAPFILEHEADER);
	
	//Запись заголовка в файл
		WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	dwWritten = sizeof (BITMAPINFOHEADER);
	//Запись в файл загружаемого заголовка
		WriteFile (fh, (LPSTR)&bi, sizeof (BITMAPINFOHEADER), &dwWritten, NULL);
	dwWritten = bi.biSizeImage;
	//Запись изображения на диск
	    WriteFile (fh, lp, bi.biSizeImage, &dwWritten, NULL);

	GlobalUnlock(hDIB);   
	GlobalFree(hDIB);

	DeleteObject(hBitmap);
	lp = NULL;
	CloseHandle (fh);
	ReleaseDC (hWnd, hDCMem);
	ReleaseDC (hWnd, hDC);
	DeleteDC (hDCMem);
	DeleteDC (hDC);
	if (dwWritten == 2) return 2;
	counter++;
	return 0;
	}

  