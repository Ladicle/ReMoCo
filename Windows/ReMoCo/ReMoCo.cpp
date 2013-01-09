// ReMoCo.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "ReMoCo.h"

#define MAX_LOADSTRING	100
#define	WINDOW_WIDTH	480
#define	WINDOW_HEIGHT	340
#define ID_BUTTON1		(0)
#define ID_BUTTON2		(ID_BUTTON1+1)
#define ID_BUTTON3		(ID_BUTTON1+2)
#define WS_ORIGINSTYLE	WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE
#define WINDOW_X		(( GetSystemMetrics( SM_CXSCREEN ) - WINDOW_WIDTH  ) / 2)
#define WINDOW_Y		(( GetSystemMetrics( SM_CYSCREEN ) - WINDOW_HEIGHT ) / 2)

// �O���[�o���ϐ�:
HINSTANCE hInst;								// ���݂̃C���^�[�t�F�C�X
TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��

// �{�^��
static HWND hWifiButton, hUsbButton, hStopButton;

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
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

 	// TODO: �����ɃR�[�h��}�����Ă��������B
	MSG msg;
	HACCEL hAccelTable;

	// �O���[�o������������������Ă��܂��B
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_REMOCO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// �A�v���P�[�V�����̏����������s���܂�:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_REMOCO));

	// ���C�� ���b�Z�[�W ���[�v:
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
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
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
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

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


// �v�b�V���{�^�����쐬����
HWND CreatePushButton(HWND hWnd, int x, int y, int w, int h, LPCTSTR caption, int id)
{
	return CreateWindow(
		_T("BUTTON"),                            // �E�B���h�E�N���X��
		caption,                                 // �L���v�V����
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,   // �X�^�C���w��
		x, y,                                    // ���W
		w, h,                                    // �T�C�Y
		hWnd,                                    // �e�E�B���h�E�̃n���h��
		(HMENU)(INT_PTR)id,                      // ���j���[�n���h���i������ID�w��ɗ��p�ł���j
		hInst,                                 // �C���X�^���X�n���h��
		NULL                                     // ���̑��̍쐬�f�[�^
	);
}

//�{�^���̗L�������ݒ�
void startButtonSetting(bool start){
	EnableWindow(hWifiButton, !start);
	EnableWindow(hUsbButton,  !start);
	EnableWindow(hStopButton, start);
}


//
//  �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI:  ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND	- �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT	- ���C�� �E�B���h�E�̕`��
//  WM_DESTROY	- ���~���b�Z�[�W��\�����Ė߂�
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
		hWifiButton = CreatePushButton( hWnd, 20,  20, 80, 40, _T("Wi-Fi"), ID_BUTTON1 );
		hUsbButton  = CreatePushButton( hWnd, 20,  70, 80, 40, _T("USB"), ID_BUTTON2 );
		hStopButton = CreatePushButton( hWnd, 20,  120, 80, 40, _T("Stop"), ID_BUTTON3 );
		startButtonSetting(false);
		return 0;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �I�����ꂽ���j���[�̉��:
		switch (wmId)
		{
		case ID_BUTTON1:
			if(g_wcThread != NULL && WaitForSingleObject(g_wcThread, 0) == WAIT_TIMEOUT){
				MessageBox( hWnd, _T("���ݎ��s���ł��B"), _T("WordCount"), MB_OK | MB_ICONEXCLAMATION );
				break;
			}
			CloseHandle(g_wcThread);

			g_wcThread = (HANDLE)_beginthread(thread1, 0, hWnd);
			
			break;
		case ID_BUTTON2:
			MessageBox( hWnd, _T("�v�b�V���{�^���Q��������܂����I"), _T("�m�F"), MB_OK );
			startButtonSetting(true);
			break;
		case ID_BUTTON3:
			MessageBox( hWnd, _T("�v�b�V���{�^��3��������܂����I"), _T("�m�F"), MB_OK );
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
		// TODO: �`��R�[�h�������ɒǉ����Ă�������...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���[�ł��B
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
