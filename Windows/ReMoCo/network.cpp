#include "stdafx.h"

//Network�ݒ�
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
//  �֐�: wifi(void* param)
//
//  �ړI: Wi-Fi�ڑ�����Android�Ƃ̒ʐM�������s���B
//
//  ����: �E�B���h�E�̃n���h���[
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
//  �֐�: usb(void* param)
//
//  �ړI: usb�ڑ�����Android�Ƃ̒ʐM�������s���B
//
//  ����: �E�B���h�E�̃n���h���[
//
//
void usb(void* param)
{
	HWND	hWnd = (HWND)param;
	WORD	wVersionRequested = MAKEWORD(2,2);
    WSADATA wsaData[1];
	char	command[BUFFER_SIZE];

	startButtonSetting(true);
	
	MessageBox( hWnd, _T("�[����USB�ڑ���\nUSB�{�^���������ĉ������B"), _T("Message"), MB_OK );
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
//  �֐�: udp(HWND hWnd)
//
//  �ړI: UDP��M���s��Android�[����IP�A�h���X���擾����B
//
//  �R�����g: Android���Ńu���[�h�L���X�g�A�h���X���ɑ��M�������b�Z�[�W
//			�@����M���AIP�A�h���X���擾����B
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

	MessageBox( hWnd, _T("Wi-FI�{�^���������Ă��������B"), _T("Message"), MB_OK );

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
//  �֐�: tcp(HWND hWnd)
//
//  �ړI: TCP�ʐM���s��Android�[�����瑀����e���擾����B
//
//  ID_KEYBOARD		- �L�[�{�[�h�C�x���g�̏���
//  ID_MOUSE		- �}�E�X�{�^���̏���
//  ID_MOUSE_MOVE	- �}�E�X�J�[�\���̏���
//  ID_STOP			- �ʐM�̏I��
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

	//�_�~�[���b�Z�[�W�̑��M
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

		//��M���e
		wsprintf(message_buf, _T("recv: %s"), recv_buff);
		MessageBox( hWnd, message_buf, _T("Messager"), MB_OK );


		//�������o		
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
//  �֐�: sendMessage(void* socket)
//
//  �ړI: 1�b�����Ƀ_�~�[���b�Z�[�W�𑗐M����B
//
//  �R�����g: ���b�Z�[�W�̑���M����莞�ԍs���Ȃ��ƃ\�P�b�g��
//			�@�^�C���A�E�g�G���[��f�����߁A�_�~�[���b�Z�[�W�𑗐M���邱�Ƃ�
//			�@�G���[��}������B
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
//  �֐�: void Keyboard(char *t)
//
//  �ړI: �L�[�{�[�h�C�x���g�𐧌䂷��B
//
//
void Keyboard(char *t)
{
	WPARAM wp = *t;

	keybd_event( wp, 0, 0, 0 );
	keybd_event( wp, 0, KEYEVENTF_KEYUP, 0);
}


//
//  �֐�: void Keyboard_arrow(char *t)
//
//  �ړI: �L�[�{�[�h�̖��C�x���g�𐧌䂷��B
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
//  �֐�: void Keyboard_special(char *t)
//
//  �ړI: �L�[�{�[�h�̖��C�x���g�𐧌䂷��B
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
//  �֐�: Mouse(char *u)
//
//  �ړI: �}�E�X�̃{�^������𐧌䂷��B
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
//  �֐�: Mouse_move(int x1,int y1)
//
//  �ړI: �}�E�X�̃J�[�\���ʒu�𐧌䂷��B
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