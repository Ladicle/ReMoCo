#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
// Windows �w�b�_�[ �t�@�C��:
#include <windows.h>

// C �����^�C�� �w�b�_�[ �t�@�C��
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//�ǉ���C�����^�C���w�b�_
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>
#include <string.h>

//Window�ݒ�
#define	WINDOW_WIDTH	480
#define	WINDOW_HEIGHT	340
#define MB_ERROR		MB_OK | MB_ICONERROR

//Network�ݒ�
#define UDP_PORT		60001
#define TCP_PORT		60002
#define USB_IPADDR		"127.0.0.1"

void usb(void* param);
void wifi(void* param);
void startButtonSetting(bool start);
