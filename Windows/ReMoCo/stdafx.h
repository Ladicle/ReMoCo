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
#define	WINDOW_WIDTH	200
#define	WINDOW_HEIGHT	300
#define MB_ERROR		MB_OK | MB_ICONERROR

void usb(void* param);
void wifi(void* param);
void startButtonSetting(bool start);
