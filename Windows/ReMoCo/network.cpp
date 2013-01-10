#include "stdafx.h"

//Network設定
#define BUFFER_SIZE		512
#define UDP_PORT		60001
#define TCP_PORT		60002
#define USB_IPADDR		"127.0.0.1"

//Process ID
#define	ID_KEYBOARD		0
#define	ID_KEY_ARROW	3
#define	ID_KEY_SPECIAL	4
#define ID_MOUSE		1
#define ID_MOUSE_MOVE	2
#define	ID_STOP			-2

//Mouse event
#define MOUSE_RIGHT_UP		"ru"
#define MOUSE_RIGHT_DOWN	"rd"
#define MOUSE_LEFT_UP		"lu"
#define	MOUSE_LEFT_DOWN		"ld"
#define MOUSE_CENTER_UP		"cu"
#define MOUSE_CENTER_DOWN	"cd"
#define MOUSE_CENTER_MOVE	"cm"


//Network
int udp(HWND hWnd);
int tcp(HWND hWnd);
void sendMessage(void* socket);

//Controll
void Keyboard(char *t);
void Keyboard_arrow(char *t);
void Keyboard_special(char *t);
void Mouse(char *u);
void Mouse_move(int x1,int y1);

//Common
char	ipaddr[16];
TCHAR	message_buf[1024];



//
//  関数: wifi(void* param)
//
//  目的: Wi-Fi接続時のAndroidとの通信処理を行う。
//
//  引数: ウィンドウのハンドラー
//
//
void wifi(void* param)
{
	HWND	hWnd = (HWND)param;
	WORD	wVersionRequested = MAKEWORD(2,2);
    WSADATA wsaData[1];

	startButtonSetting(true);

    if(WSAStartup(wVersionRequested,wsaData)){
		wsprintf(message_buf, _T("Failed WSAStartup\nError: %d"), WSAGetLastError());
        MessageBox( hWnd, message_buf, _T("Error"), MB_ERROR );
		startButtonSetting(false);
		return;
    }

	if(udp(hWnd) == -1){
        MessageBox( hWnd, _T("Failed udp process"), _T("Error"), MB_ERROR );
		startButtonSetting(false);
		WSACleanup();
		return;
	}

	if(tcp(hWnd) == -1){
        MessageBox( hWnd, _T("Failed tcp process"), _T("Error"), MB_ERROR );
		startButtonSetting(false);
		WSACleanup();
		return;
	}

	startButtonSetting(false);
	WSACleanup();
	return;
}


//
//  関数: usb(void* param)
//
//  目的: usb接続時のAndroidとの通信処理を行う。
//
//  引数: ウィンドウのハンドラー
//
//
void usb(void* param)
{
	HWND	hWnd = (HWND)param;
	WORD	wVersionRequested = MAKEWORD(2,2);
    WSADATA wsaData[1];
	char	command[BUFFER_SIZE];

	startButtonSetting(true);
	
	MessageBox( hWnd, _T("端末をUSB接続し\nUSBボタンを押して下さい。"), _T("Message"), MB_OK );
	sprintf_s(command, sizeof(command), "adb forward tcp:%d tcp:%d", TCP_PORT, TCP_PORT);
	system(command);

    if(WSAStartup(wVersionRequested,wsaData)){
		wsprintf(message_buf, _T("Failed WSAStartup\nError: %d"), WSAGetLastError());
        MessageBox( hWnd, message_buf, _T("Error"), MB_ERROR );
		startButtonSetting(false);
		return;
    }

	strcpy_s(ipaddr, sizeof(ipaddr), USB_IPADDR);
	if(tcp(hWnd) == -1){
        MessageBox( hWnd, _T("Failed tcp process"), _T("Error"), MB_ERROR );
		startButtonSetting(false);
		WSACleanup();
		return;
	}

	startButtonSetting(false);
	WSACleanup();
	return;
}


//
//  関数: udp(HWND hWnd)
//
//  目的: UDP受信を行いAndroid端末のIPアドレスを取得する。
//
//  コメント: Android側でブロードキャストアドレス宛に送信したメッセージ
//			　を受信し、IPアドレスを取得する。
//
//
int udp(HWND hWnd)
{
	SOCKET sock;
	struct sockaddr_in addr;
	struct sockaddr_in from;
	int	   recv_len = 0;
	int	   sockaddr_in_size = sizeof(struct sockaddr_in);


	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock == INVALID_SOCKET){
		wsprintf(message_buf, _T("Failed socket\nError: %d"), WSAGetLastError());
        MessageBox( hWnd, message_buf, _T("Error"), MB_ERROR );
		return -1;
	}
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(UDP_PORT);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	

	if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR){
		wsprintf(message_buf, _T("Failed bind\nError: %d"), WSAGetLastError());
        MessageBox( hWnd, message_buf, _T("Error"), MB_ERROR );
		shutdown(sock , SD_BOTH);
		closesocket(sock);
		return -1;
	}

	MessageBox( hWnd, _T("Wi-FIボタンを押してください。"), _T("Message"), MB_OK );

	memset(ipaddr, 0, sizeof(ipaddr));
	recv_len = recvfrom(sock, ipaddr, sizeof(ipaddr)-1, 0, (struct sockaddr *)&from, &sockaddr_in_size);
	ipaddr[recv_len] = '\0';

	if(recv_len == SOCKET_ERROR){
		wsprintf(message_buf, _T("Failed recv\nError: %d"), WSAGetLastError());
        MessageBox( hWnd, message_buf, _T("Error"), MB_ERROR );
		shutdown(sock , SD_BOTH);
		closesocket(sock);
		return -1;
	}

	if(recv_len == 0){
        MessageBox( hWnd, _T("Close socket"), _T("Error"), MB_ERROR );
		shutdown(sock , SD_BOTH);
		closesocket(sock);
		return -1;
	}

	wsprintf(message_buf, _T("IPADDR: %s"), ipaddr);
	MessageBox( hWnd, message_buf, _T("Message"), MB_OK );

	shutdown(sock , SD_BOTH);
	closesocket(sock);
	return 0;
}


//
//  関数: tcp(HWND hWnd)
//
//  目的: TCP通信を行いAndroid端末から操作内容を取得する。
//
//  ID_KEYBOARD		- キーボードイベントの処理
//  ID_MOUSE		- マウスボタンの処理
//  ID_MOUSE_MOVE	- マウスカーソルの処理
//  ID_STOP			- 通信の終了
//
//
int tcp(HWND hWnd)
{
	SOCKET	sock;
	HANDLE	thread;
	struct  sockaddr_in addr;
	char	recv_buff[BUFFER_SIZE], *process_buff;
	char	*ctx, *delim=",";
	int		id, x, y;
	int		recv_len = 0;
	int		sockaddr_in_size = sizeof(struct sockaddr_in);


	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET){
		wsprintf(message_buf, _T("Failed socket\nError: %d"), WSAGetLastError());
        MessageBox( hWnd, message_buf, _T("Error"), MB_ERROR );
		return -1;
	}
	
	addr.sin_port = htons(TCP_PORT);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ipaddr);


	if(connect(sock, (struct sockaddr *) &addr, sockaddr_in_size)){
		wsprintf(message_buf, _T("Failed connect\nError: %d"), WSAGetLastError());
        MessageBox( hWnd, message_buf, _T("Error"), MB_ERROR );
		shutdown(sock , SD_BOTH);
		closesocket(sock);
		return -1;
	}

	//ダミーメッセージの送信
	thread = (HANDLE)_beginthread(sendMessage, 0, (void *)sock);
	CloseHandle(thread);

	while(1)
	{
		memset(recv_buff, 0, sizeof(recv_buff));
		recv_len = recv(sock, recv_buff, sizeof(recv_buff)-1, 0);

		if(recv_len == SOCKET_ERROR){
			wsprintf(message_buf, _T("Failed recv\nError: %d"), WSAGetLastError());
			MessageBox( hWnd, message_buf, _T("Error"), MB_ERROR );
			shutdown(sock , SD_BOTH);
			closesocket(sock);
			return -1;
		}

		if(recv_len == 0){
			MessageBox( hWnd, _T("Close socket"), _T("Error"), MB_ERROR );
			shutdown(sock , SD_BOTH);
			closesocket(sock);
			return -1;
		}

		//受信内容
		wsprintf(message_buf, _T("recv: %s"), recv_buff);
		MessageBox( hWnd, message_buf, _T("Messager"), MB_OK );


		//処理抽出		
		process_buff = strtok_s(recv_buff, delim, &ctx);
		id = atoi(process_buff);


		switch(id){
		case ID_KEYBOARD:
			process_buff = strtok_s(NULL, delim, &ctx);
			Keyboard(process_buff);
			break;

		case ID_KEY_ARROW:
			process_buff = strtok_s(NULL, delim, &ctx);
			Keyboard_arrow(process_buff);
			break;

		case ID_KEY_SPECIAL:
			process_buff = strtok_s(NULL, delim, &ctx);
			Keyboard_special(process_buff);
			break;

		case ID_MOUSE:
			process_buff = strtok_s(NULL, delim, &ctx);
			Mouse(process_buff);
			break;

		case ID_MOUSE_MOVE:
			process_buff = strtok_s(NULL, delim, &ctx);
			x = atoi(process_buff);
			process_buff = strtok_s(NULL, delim, &ctx);
			y = atoi(process_buff);
			Mouse_move(x,y);
			break;

		case ID_STOP:
			shutdown(sock , SD_BOTH);
			closesocket(sock);
			return 0;
		}
	}

	shutdown(sock , SD_BOTH);
	closesocket(sock);
	return 0;
}

//
//  関数: sendMessage(void* socket)
//
//  目的: 1秒おきにダミーメッセージを送信する。
//
//  コメント: メッセージの送受信が一定時間行われないとソケットが
//			　タイムアウトエラーを吐くため、ダミーメッセージを送信することで
//			　エラーを抑制する。
//
//
void sendMessage(void* param)
{
	const char* message = "-1";
	int   sock	 = (int)param;
	int	  length = sizeof(message);

	while(1){
		if(send(sock, message, length, 0) == SOCKET_ERROR){
			shutdown(sock , SD_BOTH);
			closesocket(sock);
			return;
		}
		Sleep(60000);
	}

	shutdown(sock , SD_BOTH);
	closesocket(sock);
	return;
}


//
//  関数: void Keyboard(char *t)
//
//  目的: キーボードイベントを制御する。
//
//
void Keyboard(char *t)
{
	WPARAM wp = *t;

	keybd_event( wp, 0, 0, 0 );
	keybd_event( wp, 0, KEYEVENTF_KEYUP, 0);
}


//
//  関数: void Keyboard_arrow(char *t)
//
//  目的: キーボードの矢印イベントを制御する。
//
//
void Keyboard_arrow(char *t)
{
	WPARAM wp = *t;

	if(strcmp(t, "r")==0){
		keybd_event( VK_DELETE, 0, 0, 0 );
		keybd_event( VK_DELETE, 0, KEYEVENTF_KEYUP, 0);
		return;
	}
	if(strcmp(t, "l")==0){
		keybd_event( VK_DELETE, 0, 0, 0 );
		keybd_event( VK_DELETE, 0, KEYEVENTF_KEYUP, 0);
		return;
	}
	if(strcmp(t, "d")==0){
		keybd_event( VK_BACK, 0, 0, 0 );
		keybd_event( VK_BACK, 0, KEYEVENTF_KEYUP, 0);
		return;
	}
	if(strcmp(t, "u")==0){
		keybd_event( VK_TAB, 0, 0, 0 );
		keybd_event( VK_TAB, 0, KEYEVENTF_KEYUP, 0);
		return;
	}
}


//
//  関数: void Keyboard_special(char *t)
//
//  目的: キーボードの矢印イベントを制御する。
//
//
void Keyboard_special(char *t)
{
	WPARAM wp = *t;

	//SHIFT
	if(strcmp(t, "shift")==0){
		keybd_event( VK_SHIFT, 0, 0, 0);
		return;
	}
	if(strcmp(t, "unshift")==0){
		keybd_event( VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
		return;
	}

	//CONTROL
	if(strcmp(t, "control")==0){
		keybd_event( VK_CONTROL, 0, 0, 0);
		return;
	}
	if(strcmp(t, "uncontrol")==0){
		keybd_event( VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
		return;
	}

	//ALT
	if(strcmp(t, "alt")==0){
		keybd_event( VK_MENU, 0, 0, 0);
		return;
	}
	if(strcmp(t, "unalt")==0){
		keybd_event( VK_MENU, 0, KEYEVENTF_KEYUP, 0);
		return;
	}

	//ETC
	if(strcmp(t, "div")==0){
		keybd_event( VK_DIVIDE, 0, 0, 0 );
		keybd_event( VK_DIVIDE, 0, KEYEVENTF_KEYUP, 0);
		return;
	}
	if(strcmp(t, "del")==0){
		keybd_event( VK_DELETE, 0, 0, 0 );
		keybd_event( VK_DELETE, 0, KEYEVENTF_KEYUP, 0);
		return;
	}
	if(strcmp(t, "bs")==0){
		keybd_event( VK_BACK, 0, 0, 0 );
		keybd_event( VK_BACK, 0, KEYEVENTF_KEYUP, 0);
		return;
	}
	if(strcmp(t, "sp")==0){
		keybd_event( VK_SPACE, 0, 0, 0 );
		keybd_event( VK_SPACE, 0, KEYEVENTF_KEYUP, 0);
		return;
	}
	if(strcmp(t, "ret")==0){
		keybd_event( VK_RETURN, 0, 0, 0 );
		keybd_event( VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
		return;
	}
	if(strcmp(t, "sub")==0){
		keybd_event( VK_SUBTRACT, 0, 0, 0 );
		keybd_event( VK_SUBTRACT, 0, KEYEVENTF_KEYUP, 0);
		return;
	}
	if(strcmp(t, "dec")==0){
		keybd_event( VK_DECIMAL, 0, 0, 0 );
		keybd_event( VK_DECIMAL, 0, KEYEVENTF_KEYUP, 0);
		return;
	}
	if(strcmp(t, "mul")==0){
		keybd_event( VK_MULTIPLY, 0, 0, 0 );
		keybd_event( VK_MULTIPLY, 0, KEYEVENTF_KEYUP, 0);
		return;
	}
	if(strcmp(t, "add")==0){
		keybd_event( VK_ADD, 0, 0, 0 );
		keybd_event( VK_ADD, 0, KEYEVENTF_KEYUP, 0);
		return;
	}
}


//
//  関数: Mouse(char *u)
//
//  目的: マウスのボタン操作を制御する。
//
//
void Mouse(char *u)
{	
	char *ctx, *delim=",";
	int	z = 0;
	static int flag = 0;

	//LEFT
	if(strcmp(u, MOUSE_LEFT_DOWN)==0){
		if(flag==0){
			mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
			flag = 1;
		}
		return;
	}
	if(strcmp(u, MOUSE_LEFT_UP)==0){
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
		flag = 0;
		return;
	}
	
	//RIGHT
	if(strcmp(u, MOUSE_RIGHT_DOWN)==0){
		if(flag==0){
			mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
			flag=1;
		}
		return;
	}
	if(strcmp(u, MOUSE_RIGHT_UP)==0){
		mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
		flag=0;
		return;
	}

	//CENTER
	if(strcmp(u, MOUSE_CENTER_DOWN)==0){
		if(flag==0){
			mouse_event(MOUSEEVENTF_MIDDLEDOWN,0,0,0,0);
		}
		return;
	}
	if(strcmp(u, MOUSE_CENTER_UP)==0){
		mouse_event(MOUSEEVENTF_MIDDLEUP,0,0,0,0);
		flag=0;
		return;
	}

	//WHEEL
	if(strcmp(u, MOUSE_CENTER_MOVE)==0){
		u = strtok_s(NULL, delim, &ctx);
		z = atoi(u);
		mouse_event(MOUSEEVENTF_WHEEL,0,0,z,0);
	}
}


//
//  関数: Mouse_move(int x1,int y1)
//
//  目的: マウスのカーソル位置を制御する。
//
//
void Mouse_move(int x1,int y1)
{
	int dx, dy;
	static int x2 = 0;
	static int y2 = 0;

	dx = x2-x1;
	dy = y2-y1;
	mouse_event(MOUSEEVENTF_MOVE, dx, dy, 0, 0);

	x2 = x1;
	y2 = y1;
}