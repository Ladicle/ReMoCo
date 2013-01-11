// ReMoCo.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ReMoCo.h"

#define MAX_LOADSTRING	100
#define BTN_ID_WIFI		(0)
#define BTN_ID_USB		(BTN_ID_WIFI+1)
#define BTN_ID_STOP		(BTN_ID_WIFI+2)
#define WS_ORIGINSTYLE	WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX
#define WINDOW_X		(( GetSystemMetrics( SM_CXSCREEN ) - WINDOW_WIDTH  ) / 2)
#define WINDOW_Y		(( GetSystemMetrics( SM_CYSCREEN ) - WINDOW_HEIGHT ) / 2)

// グローバル変数:
HINSTANCE hInst;								// 現在のインターフェイス
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

// ボタン
static HWND hWifiButton, hUsbButton, hStopButton;

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ここにコードを挿入してください。
	MSG msg;
	HACCEL hAccelTable;

	// グローバル文字列を初期化しています。
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_REMOCO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーションの初期化を実行します:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_REMOCO));

	// メイン メッセージ ループ:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_REMOCO));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_REMOCO);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

   hWnd = CreateWindow(szWindowClass, szTitle, WS_ORIGINSTYLE,
	   WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


// プッシュボタンを作成する
HWND CreatePushButton(HWND hWnd, int x, int y, int w, int h, LPCTSTR caption, int id)
{
	return CreateWindow(
		_T("BUTTON"),                            // ウィンドウクラス名
		caption,                                 // キャプション
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,   // スタイル指定
		x, y,                                    // 座標
		w, h,                                    // サイズ
		hWnd,                                    // 親ウィンドウのハンドル
		(HMENU)(INT_PTR)id,                      // メニューハンドル（ここをID指定に流用できる）
		hInst,									 // インスタンスハンドル
		NULL                                     // その他の作成データ
	);
}

//ボタンの有効無効設定
void startButtonSetting(bool start){
	EnableWindow(hWifiButton, !start);
	EnableWindow(hUsbButton,  !start);
	EnableWindow(hStopButton, start);
}


//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:  メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	HANDLE g_wcThread = NULL;

	switch (message)
	{
	case WM_CREATE:
		hWifiButton = CreatePushButton( hWnd, 20,  20, 140, 60,  _T("Wi-Fi"), BTN_ID_WIFI );
		hUsbButton  = CreatePushButton( hWnd, 20,  90, 140, 60,  _T("USB"),	 BTN_ID_USB );
		hStopButton = CreatePushButton( hWnd, 20,  160, 140, 60, _T("Stop"),  BTN_ID_STOP );
		startButtonSetting(false);
		return 0;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId)
		{
		case BTN_ID_WIFI:
			if(g_wcThread != NULL && WaitForSingleObject(g_wcThread, 0) == WAIT_TIMEOUT){
				MessageBox( hWnd, _T("実行中です。"), _T("Error"), MB_ERROR );
				break;
			}
			CloseHandle(g_wcThread);
			g_wcThread = (HANDLE)_beginthread(wifi, 0, hWnd);
			break;

		case BTN_ID_USB:
			if(g_wcThread != NULL && WaitForSingleObject(g_wcThread, 0) == WAIT_TIMEOUT){
				MessageBox( hWnd, _T("実行中です。"), _T("Error"), MB_ERROR );
				break;
			}
			CloseHandle(g_wcThread);
			g_wcThread = (HANDLE)_beginthread(usb, 0, hWnd);
			break;

		case BTN_ID_STOP:
			MessageBox( hWnd, _T("停止します。"), _T("Message"), MB_OK );
			startButtonSetting(false);
			break;

		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		CloseHandle(g_wcThread);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
