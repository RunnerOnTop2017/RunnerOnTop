#pragma once

#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#endif

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
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


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <D3DX10math.h>
#include <math.h>
#include <mmsystem.h>
#include <d3d9types.h>
#include <xnamath.h>

#define FRAME_BUFFER_WIDTH	1280
#define FRAME_BUFFER_HEIGHT	720
#define VS_SLOT_VIEWPROJECTION_MATRIX	0x00
#define VS_SLOT_WORLD_MATRIX			0x01
#define PS_SLOT_COLOR	0x00
#define PS_SLOT_LIGHT		0x00
#define PS_SLOT_MATERIAL	0x01
#define PS_SLOT_TEXTURE		0x00
#define PS_SLOT_BUMP		0x09
#define PS_SLOT_SAMPLER_STATE		0x02
