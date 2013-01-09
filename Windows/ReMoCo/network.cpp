#include "stdafx.h"

int udp(HWND hWnd);
int tcp(HWND hWnd);

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
	HWND hWnd = (HWND)param;

	system("adb forward tcp:60002 tcp:60002");
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
	MessageBox( hWnd, message_buf, _T("Error"), MB_OK );

	shutdown(sock , SD_BOTH);
	closesocket(sock);
	return 0;
}


//
//  �֐�: tcp(HWND hWnd)
//
//  �ړI: TCP�ʐM���s��Android�[�����瑀����e���擾����B
//
//  �R�����g: 
//
//
int tcp(HWND hWnd)
{

	return 0;
}
