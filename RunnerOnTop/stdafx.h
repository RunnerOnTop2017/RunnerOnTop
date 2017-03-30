#pragma once

#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#endif

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

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

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
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
