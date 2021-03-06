#pragma once

#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#endif
#pragma comment(lib, "fmodex_vc")
#pragma comment(lib, "ws2_32")
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>
#include <WinSock2.h>
#include <CommCtrl.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <iomanip>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <D3DX10math.h>
#include <math.h>
#include <mmsystem.h>
#include <d3d9types.h>
#include "fmod.hpp"
#include "fmod_errors.h"

#include <xnamath.h>
#include"RunnerOnTop.h"


#define	WM_SOCKET	WM_USER + 1

#define FRAME_BUFFER_WIDTH	GetSystemMetrics(SM_CXSCREEN) // 1600
#define FRAME_BUFFER_HEIGHT	GetSystemMetrics(SM_CYSCREEN) //900
#define VS_SLOT_VIEWPROJECTION_MATRIX	0x00
#define VS_SLOT_WORLD_MATRIX			0x01
#define PS_SLOT_COLOR	0x00
#define PS_SLOT_LIGHT		0x00
#define PS_SLOT_MATERIAL	0x01
#define PS_SLOT_TEXTURE		0x00
#define PS_SLOT_BUMP		0x09
#define PS_SLOT_SAMPLER_STATE		0x00
#define VK_KEYW 0x57
#define VK_KEYS 0x53
#define VK_KEYA 0x41
#define VK_KEYD 0x44
#define VK_KEYE 0x45

#define OBJECT_DOOR 0
#define OBJECT_LADDER 1
#define OBJECT_FENCE 2
#define OBJECT_PIPE 3
#define OBJECT_CONDITIONER 4
#define OBJECT_FENCEHOLE 6
#define OBJECT_MAP1 10
#define OBJECT_MAP2 11

#define WM_INTERACT WM_USER+1
#define CHANGE_POS_SLIDE 0

//서버용--------------
//--------------------
#define SERVERPORT 9000
#define BUF_SIZE    1024
#define MAX_BUFF_SIZE   4000
#define MAX_PACKET_SIZE  255
#define MAX_Client 999

//---------------------

// 길찾기용 알고리즘
// 20x20 형태
const int map_size_n = 40;
const int map_size_m = 40;

const int detail_size_n = 20;
const int detail_size_m = 20;


#define D_METER 40
enum GAMESTATENUM {
	LOBBY, MAPMENU, INGAME, INGAME2, GAMEOVER, YOUWIN, MATCHING, LOADING
};

// 첫번째 박스의 max. min, 두번째 박스의 max, min을 인자로 받아 충돌하면 true 아니면 false. 인자로 받은 x,y,z,에 x축과 y축 z축 에 대한 충돌세부사항이 저장된다.
bool CollisionCheck(D3DXVECTOR3& max1, D3DXVECTOR3& min1, D3DXVECTOR3& max2, D3DXVECTOR3& min2, D3DXVECTOR3& shift, bool& x, bool& y, bool& z);

std::ostream& operator<<(std::ostream& os, D3DXVECTOR3 d3v);
