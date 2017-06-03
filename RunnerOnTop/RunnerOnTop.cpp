// 3DTest.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "RunnerOnTop.h"
#include "GameFramework.h"


#define MAX_LOADSTRING 100
enum GAMESTATENUM {
	LOBBY, INGAME
};
// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.
CGameFramework gGameFramework;
HINSTANCE ghInstance;
GAMESTATENUM gameState;
// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
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

	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RUNNERONTOP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	gameState = LOBBY;
	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RUNNERONTOP));

	// �⺻ �޽��� �����Դϴ�.
	while (1)
	{
		if (gameState == LOBBY)
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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	ghInstance = hInstance;

	RECT rc = { 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT };
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION |WS_SYSMENU | WS_MINIMIZEBOX;
	AdjustWindowRect(&rc, dwStyle, FALSE);
	HWND hMainWnd = CreateWindow(szWindowClass, szTitle, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	if (!hMainWnd) return(FALSE);

	gGameFramework.OnCreate(hInstance, hMainWnd);

	::ShowWindow(hMainWnd, nCmdShow);
	::UpdateWindow(hMainWnd);

	return(TRUE);
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//

static HBITMAP bmp_background;
static HBITMAP bmp_menu;

static BOOL start_mouse;
static BOOL end_mouse;

static RECT rt;

int mx, my;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	
	HDC hMemDC, hOldMemDC;
	HBITMAP hBitmap, hOldBitmap;
	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �޴� ������ ���� �м��մϴ�.
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
		bmp_menu = (HBITMAP)LoadImage(NULL, L"Data\\UI\\menu.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
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
		if(gameState == LOBBY)
			UIProcessMessage(hWnd, message, wParam, lParam);
		else if(gameState == INGAME)
			gGameFramework.OnProcessingWindowMessage(hWnd, message, wParam, lParam);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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

		if (end_mouse)
			exit(0);
		if (start_mouse)
		{
			//SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0L);

			gameState = INGAME;

		}
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_MOUSEMOVE:
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

		InvalidateRect(hWnd, NULL, false);
		break;

	}

}
