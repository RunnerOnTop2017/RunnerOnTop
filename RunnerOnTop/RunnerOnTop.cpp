// 3DTest.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "RunnerOnTop.h"
#include "GameFramework.h"


#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;								// 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.
CGameFramework gGameFramework;
HINSTANCE ghInstance;

GAMESTATENUM gameState;
HWND mHwnd;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void UIProcessMessage(HWND hWnd,UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	HACCEL hAccelTable;

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RUNNERONTOP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	gameState = LOBBY;
	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RUNNERONTOP));

	// 기본 메시지 루프입니다.
	while (1)
	{
		if (gameState != INGAME)
		{
			if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT) break;
				if (!::TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
			}
		}
		else if(gameState == INGAME)
		{ 
			if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT) break;
				if (!::TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
			}
			else
			{
				gGameFramework.FrameAdvance();
				//InvalidateRect(msg.hwnd, NULL, false);
			}
		}
	
	}
	gGameFramework.OnDestroy();

	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_RUNNERONTOP));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;// MAKEINTRESOURCE(IDC_RUNNERONTOP);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	ghInstance = hInstance;

	RECT rc = { 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT };
	DWORD dwStyle = WS_POPUP;//WS_OVERLAPPED | WS_CAPTION |WS_SYSMENU | WS_MINIMIZEBOX;
	AdjustWindowRect(&rc, dwStyle, FALSE);
	HWND hMainWnd = CreateWindow(szWindowClass, szTitle, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	if (!hMainWnd) return(FALSE);

	gGameFramework.OnCreate(hInstance, hMainWnd);

	::ShowWindow(hMainWnd, nCmdShow);
	::UpdateWindow(hMainWnd);

	return(TRUE);
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
//

static HBITMAP bmp_background;
static HBITMAP bmp_menu;
static HBITMAP bmp_Map1;
static HBITMAP bmp_over;
static HBITMAP bmp_win;


static BOOL start_mouse;
static BOOL end_mouse;

static RECT rt;

int mx, my;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	mHwnd = hWnd;
	
	HDC hMemDC, hOldMemDC;
	HBITMAP hBitmap, hOldBitmap;
	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 메뉴 선택을 구문 분석합니다.
		switch (wmId)
		{
		case IDM_FULLSCREEN:
			SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0L);
			InvalidateRect(hWnd, NULL, false);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
		//SetWindowLongPtr()
		//SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0L);

		
		bmp_background = (HBITMAP)LoadImage(NULL, L"Data\\UI\\background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);// (HBITMAP)LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
		bmp_menu = (HBITMAP)LoadImage(NULL, L"Data\\UI\\menu2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		bmp_Map1 = (HBITMAP)LoadImage(NULL, L"Data\\UI\\map1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		bmp_over = (HBITMAP)LoadImage(NULL, L"Data\\UI\\gameover.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		bmp_win = (HBITMAP)LoadImage(NULL, L"Data\\UI\\youwin.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		//InvalidateRect(hWnd, NULL, false);

		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		if (gameState == LOBBY)
		{
			GetClientRect(hWnd, &rt);

			hMemDC = CreateCompatibleDC(hdc);
			hBitmap = CreateCompatibleBitmap(hdc, 1280, 720);
			hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

			hOldMemDC = CreateCompatibleDC(hdc);

			SelectObject(hOldMemDC, bmp_background);
			StretchBlt(hMemDC, 0, 0, 1280, 720, hOldMemDC, 0, 0, 1024, 1024, SRCCOPY);

			SelectObject(hOldMemDC, bmp_menu);

			if (start_mouse)
				StretchBlt(hMemDC, 900, 540, 130, 60, hOldMemDC, 230, 40, 170, 50, SRCCOPY);
			else
				StretchBlt(hMemDC, 900, 540, 130, 60, hOldMemDC, 30, 40, 170, 50, SRCCOPY);

			if (end_mouse)
				StretchBlt(hMemDC, 900, 600, 130, 60, hOldMemDC, 230, 285, 170, 50, SRCCOPY);
			else
				StretchBlt(hMemDC, 900, 600, 130, 60, hOldMemDC, 30, 285, 170, 50, SRCCOPY);

			DeleteDC(hOldMemDC);

			//BitBlt(hdc, 0, 0, rt.right, rt.bottom, hMemDC, 0, 0, SRCCOPY);
			StretchBlt(hdc, 0, 0, rt.right, rt.bottom, hMemDC, 0, 0, 1280, 720, SRCCOPY);

			SelectObject(hMemDC, hOldBitmap);
			DeleteObject(hBitmap);

			DeleteDC(hMemDC);
		}
		else if (gameState == MAPMENU)
		{
			GetClientRect(hWnd, &rt);

			hMemDC = CreateCompatibleDC(hdc);
			hBitmap = CreateCompatibleBitmap(hdc, 1280, 720);
			hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

			hOldMemDC = CreateCompatibleDC(hdc);
			
			FillRect(hMemDC, &rt, WHITE_BRUSH);
			
			// map1 그림 출력
			SelectObject(hOldMemDC, bmp_Map1);
			StretchBlt(hMemDC, 100, 100, 400, 400, hOldMemDC, 0, 0, 1024,1024, SRCCOPY);

			// map2 그림 출력 (현재는 없음)
			RECT bm2 = { 750, 100,  1150,500 };
			FillRect(hMemDC, &bm2, CreateSolidBrush(BLACK_BRUSH));

			SelectObject(hOldMemDC, bmp_menu);

			// map1 글씨 출력
			StretchBlt(hMemDC, 220, 540, 130, 60, hOldMemDC, 30, 90, 170, 50, SRCCOPY);

			// map2 글씨 출력
			StretchBlt(hMemDC, 900, 540, 130, 60, hOldMemDC, 30, 140, 170, 50, SRCCOPY);

			//back 글씨 출력
			StretchBlt(hMemDC, 600, 640, 130, 60, hOldMemDC, 230, 90, 170, 50, SRCCOPY);



			DeleteDC(hOldMemDC);

			
			StretchBlt(hdc, 0, 0, rt.right, rt.bottom, hMemDC, 0, 0, 1280, 720, SRCCOPY);

			SelectObject(hMemDC, hOldBitmap);
			DeleteObject(hBitmap);

			DeleteDC(hMemDC);
		}

		else if (gameState == GAMEOVER)
		{
			GetClientRect(hWnd, &rt);

			hMemDC = CreateCompatibleDC(hdc);
			hBitmap = CreateCompatibleBitmap(hdc, 1280, 720);
			hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

			hOldMemDC = CreateCompatibleDC(hdc);

			FillRect(hMemDC, &rt, WHITE_BRUSH);

			// map1 그림 출력
			SelectObject(hOldMemDC, bmp_over);
			StretchBlt(hMemDC, 0, 0, 1280, 720, hOldMemDC, 0, 0, 1280, 720, SRCCOPY);

		
			DeleteDC(hOldMemDC);


			StretchBlt(hdc, 0, 0, rt.right, rt.bottom, hMemDC, 0, 0, 1280, 720, SRCCOPY);

			SelectObject(hMemDC, hOldBitmap);
			DeleteObject(hBitmap);

			DeleteDC(hMemDC);
		}
		else if (gameState == YOUWIN)
		{
			GetClientRect(hWnd, &rt);

			hMemDC = CreateCompatibleDC(hdc);
			hBitmap = CreateCompatibleBitmap(hdc, 1280, 720);
			hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

			hOldMemDC = CreateCompatibleDC(hdc);

			FillRect(hMemDC, &rt, WHITE_BRUSH);

			// map1 그림 출력
			SelectObject(hOldMemDC, bmp_win);
			StretchBlt(hMemDC, 0, 0, 1280, 720, hOldMemDC, 0, 0, 1280, 720, SRCCOPY);


			DeleteDC(hOldMemDC);


			StretchBlt(hdc, 0, 0, rt.right, rt.bottom, hMemDC, 0, 0, 1280, 720, SRCCOPY);

			SelectObject(hMemDC, hOldBitmap);
			DeleteObject(hBitmap);

			DeleteDC(hMemDC);
		}
		EndPaint(hWnd, &ps);
	}
		
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_KEYDOWN:
	case WM_KEYUP:
		if(gameState != INGAME)
			UIProcessMessage(hWnd, message, wParam, lParam);
		else
			gGameFramework.OnProcessingWindowMessage(hWnd, message, wParam, lParam);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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

void UIProcessMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (gameState == LOBBY)
		{
			if (end_mouse)
				exit(0);
			if (start_mouse)
			{
				//SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0L);

				gameState = MAPMENU;

			}

		}
		else if (gameState == MAPMENU)
		{
			//map1
			if ((rt.right * (220.0f / 1280.0f) < mx && mx < rt.right * (350.0f / 1280.0f)) && (rt.bottom * (540.0f / 720.0f) < my && my < rt.bottom * (600.0f / 720.0f)))
			{
				gameState = INGAME;
			}
			//map2
			else if ((rt.right * (900.0f / 1280.0f) < mx && mx < rt.right * (1030.0f / 1280.0f)) && (rt.bottom * (540.0f / 720.0f) < my && my < rt.bottom * (600.0f / 720.0f)))
			{

			}
			// back
			else if ((rt.right * (600.0f / 1280.0f) < mx && mx < rt.right * (730.0f / 1280.0f)) && (rt.bottom * (640.0f / 720.0f) < my && my < rt.bottom * (700.0f / 720.0f)))
			{
				gameState = LOBBY;
			}
		}
		else if (gameState == GAMEOVER || gameState == YOUWIN)
		{
			gGameFramework.OnCreate(hInst, hWnd);
			gameState = LOBBY;
		}
	
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_MOUSEMOVE:
		if (gameState == LOBBY)
		{
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			rt.right * (1030 / 1280);
			rt.bottom * (540 / 720);
			if ((rt.right * (900.0f / 1280.0f) < mx && mx < rt.right * (1030.0f / 1280.0f)) && (rt.bottom * (540.0f / 720.0f) < my && my < rt.bottom * (600.0f / 720.0f)))
				start_mouse = true;
			else
				start_mouse = false;

			if ((rt.right * (900.0f / 1280.0f) < mx && mx < rt.right * (1030.0f / 1280.0f)) && (rt.bottom * (600.0f / 720.0f) < my&&my < rt.bottom * (660.0f / 720.0f)))
				end_mouse = true;
			else
				end_mouse = false;

		}
	
		InvalidateRect(hWnd, NULL, false);
		break;

	}

}
