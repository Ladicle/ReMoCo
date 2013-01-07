#include "stdafx.h"

void udp(){

}

void tcp(){
	
}

void thread1(void* param){
	HWND hWnd = (HWND)param;
	HANDLE g_wcThread = NULL;
	HDC hDC;

	MessageBox( hWnd, _T("thread開始"), _T("確認"), MB_OK );
	startButtonSetting(true);
	Sleep(5000);
	startButtonSetting(false);
	MessageBox( hWnd, _T("thread開始"), _T("終了"), MB_OK );

	static LPCTSTR TEST_STR = _T("左ボタンが押されました");
	hDC = GetDC( hWnd );
	//SetTextColor( hDC, RGB(255,0,0) );  // 文字の色を赤色にする
	TextOut( hDC, 50, 50, TEST_STR, (int)_tcslen(TEST_STR) );
	ReleaseDC( hWnd, hDC );
}