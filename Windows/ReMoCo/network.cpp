#include "stdafx.h"

void udp(){

}

void tcp(){
	
}

void thread1(void* param){
	HWND hWnd = (HWND)param;
	HANDLE g_wcThread = NULL;
	HDC hDC;

	MessageBox( hWnd, _T("thread�J�n"), _T("�m�F"), MB_OK );
	startButtonSetting(true);
	Sleep(5000);
	startButtonSetting(false);
	MessageBox( hWnd, _T("thread�J�n"), _T("�I��"), MB_OK );

	static LPCTSTR TEST_STR = _T("���{�^����������܂���");
	hDC = GetDC( hWnd );
	//SetTextColor( hDC, RGB(255,0,0) );  // �����̐F��ԐF�ɂ���
	TextOut( hDC, 50, 50, TEST_STR, (int)_tcslen(TEST_STR) );
	ReleaseDC( hWnd, hDC );
}