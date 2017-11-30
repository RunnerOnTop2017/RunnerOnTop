#include "stdafx.h"
#include "GameFramework.h"
#include "State.h"
#include "UIClass.h"
#include "ServerModule.h"
float tickTime = 0;
extern ServerModule network;
CGameFramework::CGameFramework()
{
	m_pd3dDevice = NULL;
	m_pDXGISwapChain = NULL;
	m_pd3dRenderTargetView = NULL;
	m_pd3dDeviceContext = NULL;

	m_nWndClientWidth = FRAME_BUFFER_WIDTH;
	m_nWndClientHeight = FRAME_BUFFER_HEIGHT;

	m_pScene = NULL;
	_tcscpy_s(m_pszBuffer, _T("RunnerOnTop : "));

	m_nPlayers = 0;
	m_ppPlayers = NULL;
	m_pNPC = NULL;
	m_pd3dDepthStencilBuffer = NULL;
	m_pd3dDepthStencilView = NULL;

	loaded = false;
}

CGameFramework::~CGameFramework()
{
}

//���� �Լ��� ���� ���α׷��� ����Ǹ� ȣ��ȴٴ� �Ϳ� �����϶�. 
bool CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd, int MapNumber)
{
	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	//Direct3D ����̽�, ����̽� ���ؽ�Ʈ, ���� ü�� ���� �����ϴ� �Լ��� ȣ���Ѵ�. 
	if (!CreateDirect3DDisplay()) return(false);
	
	//�������� ��ü(���� ���� ��ü)�� �����Ѵ�. 
	//BuildObjects(MapNumber);

	return(true);
}

bool CGameFramework::CreateRenderTargetDepthStencilView()
{
	HRESULT hResult = S_OK;

	ID3D11Texture2D *pd3dBackBuffer;
	if (FAILED(hResult = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return(false);
	if (FAILED(hResult = m_pd3dDevice->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return(false);
	if (pd3dBackBuffer) pd3dBackBuffer->Release();

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC d3dDepthStencilBufferDesc;
	ZeroMemory(&d3dDepthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dDepthStencilBufferDesc.Width = m_nWndClientWidth;
	d3dDepthStencilBufferDesc.Height = m_nWndClientHeight;
	d3dDepthStencilBufferDesc.MipLevels = 1;
	d3dDepthStencilBufferDesc.ArraySize = 1;
	d3dDepthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dDepthStencilBufferDesc.SampleDesc.Count = 1;
	d3dDepthStencilBufferDesc.SampleDesc.Quality = 0;
	d3dDepthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dDepthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	d3dDepthStencilBufferDesc.CPUAccessFlags = 0;
	d3dDepthStencilBufferDesc.MiscFlags = 0;
	if (FAILED(hResult = m_pd3dDevice->CreateTexture2D(&d3dDepthStencilBufferDesc, NULL, &m_pd3dDepthStencilBuffer))) return(false);


	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	
	if (FAILED(hResult = m_pd3dDevice->CreateDepthStencilState(&depthStencilDesc, &m_pd3dDepthStencilState))) return (false);
	
	// Set the depth stencil state.
	m_pd3dDeviceContext->OMSetDepthStencilState(m_pd3dDepthStencilState, 1);


	D3D11_DEPTH_STENCIL_DESC depthDisableStencilDesc;
	ZeroMemory(&depthDisableStencilDesc, sizeof(depthDisableStencilDesc));

	// Set up the description of the stencil state.
	depthDisableStencilDesc.DepthEnable = false;
	depthDisableStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisableStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthDisableStencilDesc.StencilEnable = true;
	depthDisableStencilDesc.StencilReadMask = 0xFF;
	depthDisableStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthDisableStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisableStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisableStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisableStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthDisableStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisableStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisableStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisableStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.

	if (FAILED(hResult = m_pd3dDevice->CreateDepthStencilState(&depthDisableStencilDesc, &m_pd3dDepthDisableStencilState))) return (false);



	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	d3dDepthStencilViewDesc.Format = d3dDepthStencilBufferDesc.Format;
	d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;
	if (FAILED(hResult = m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer, &d3dDepthStencilViewDesc, &m_pd3dDepthStencilView))) return(false);

	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);

	return(true);
}

bool CGameFramework::CreateDirect3DDisplay()
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	m_nWndClientWidth = rcClient.right - rcClient.left;
	m_nWndClientHeight = rcClient.bottom - rcClient.top;

	UINT dwCreateDeviceFlags = 0;
#ifdef _DEBUG
	dwCreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//����̽��� �����ϱ� ���Ͽ� �õ��� ����̹� ������ ������ ��Ÿ����.
	D3D_DRIVER_TYPE d3dDriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT nDriverTypes = sizeof(d3dDriverTypes) / sizeof(D3D10_DRIVER_TYPE);

	//����̽��� �����ϱ� ���Ͽ� �õ��� Ư�� ������ ������ ��Ÿ����.
	D3D_FEATURE_LEVEL d3dFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT nFeatureLevels = sizeof(d3dFeatureLevels) / sizeof(D3D_FEATURE_LEVEL);

	//������ ���� ü���� �����ϴ� ����ü�̴�.
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = 1;
	dxgiSwapChainDesc.BufferDesc.Width = m_nWndClientWidth;
	dxgiSwapChainDesc.BufferDesc.Height = m_nWndClientHeight;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.OutputWindow = m_hWnd;
	dxgiSwapChainDesc.SampleDesc.Count = 1;
	dxgiSwapChainDesc.SampleDesc.Quality = 0;
	dxgiSwapChainDesc.Windowed = TRUE;

	D3D_DRIVER_TYPE nd3dDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL nd3dFeatureLevel = D3D_FEATURE_LEVEL_11_0;

	HRESULT hResult = S_OK;
	//����̽��� ����̹� ������ Ư�� ������ �����ϴ� ����̽��� ���� ü���� �����Ѵ�. 
	//������� ����̽� ������ �õ��ϰ� �����ϸ� ���� ������ ����̽��� �����Ѵ�.
	for (UINT i = 0; i < nDriverTypes; i++)
	{
		nd3dDriverType = d3dDriverTypes[i];
		if (SUCCEEDED(hResult = D3D11CreateDeviceAndSwapChain(NULL, nd3dDriverType, NULL,
			dwCreateDeviceFlags, d3dFeatureLevels, nFeatureLevels, D3D11_SDK_VERSION, &dxgiSwapChainDesc,
			&m_pDXGISwapChain, &m_pd3dDevice, &nd3dFeatureLevel, &m_pd3dDeviceContext))) break;
	}
	if (!m_pDXGISwapChain || !m_pd3dDevice || !m_pd3dDeviceContext) return(false);

	//���� Ÿ�� �並 �����ϴ� �Լ��� ȣ���Ѵ�.

	if (!CreateRenderTargetDepthStencilView()) return(false);

	//SetViewport();

	return(true);
}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		//���콺 ĸ�ĸ� �ϰ� ���� ���콺 ��ġ�� �����´�.
		SetCapture(hWnd);
		GetCursorPos(&m_ptOldCursorPos);
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		//���콺 ĸ�ĸ� �����Ѵ�.
		//ReleaseCapture();
		break;
	}
}

void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
			/*��F1�� Ű�� ������ 1��Ī ī�޶�, ��F2�� Ű�� ������ �����̽�-�� ī�޶�� �����Ѵ�, ��F3�� Ű�� ������ 3��Ī ī�޶�� �����Ѵ�.*/
		case VK_F1:
		case VK_F2:
		case VK_F3:
			m_ppPlayers[0]->ChangeCamera(m_pd3dDevice, (wParam - VK_F1 + 1), m_GameTimer.GetTimeElapsed());
			break;

		default:
			break;
		}
		break;
	default:
		break;
	}
}

LRESULT CALLBACK CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
		/*�������� ũ�Ⱑ ����� ��(����� ��Alt+Enter�� ��ü ȭ�� ���� ������ ���� ��ȯ�� ��)
		���� ü���� �ĸ���� ũ�⸦ �����ϰ� �ĸ���ۿ� ���� ���� Ÿ�� �並 �ٽ� �����Ѵ�. */
	case WM_SIZE:
	{
					m_nWndClientWidth = LOWORD(lParam);
					m_nWndClientHeight = HIWORD(lParam);

					m_pd3dDeviceContext->OMSetRenderTargets(0, NULL, NULL);

					if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
					if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
					if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();

					m_pDXGISwapChain->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

					CreateRenderTargetDepthStencilView();

					if (m_ppPlayers)
					{
						CCamera *pCamera = m_ppPlayers[0]->GetCamera();
						pCamera->SetViewport(m_pd3dDeviceContext, 0, 0, m_nWndClientWidth, m_nWndClientHeight, 0.0f, 1.0f);
					}
					break;
	}
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		m_ppPlayers[0]->m_pState->ProcessInput(nMessageID, wParam, lParam);
		//m_pNPC->m_pState->ProcessInput(nMessageID, wParam, lParam);
		break;
	}
	return(0);
}

//���� �Լ��� ���� ���α׷��� ����� �� ȣ��ȴٴ� �Ϳ� �����϶�. 
void CGameFramework::OnDestroy()
{
	ReleaseObjects();

	if (m_pd3dDeviceContext) m_pd3dDeviceContext->ClearState();
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
	if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();
	if (m_pDXGISwapChain) m_pDXGISwapChain->Release();
	if (m_pd3dDeviceContext) m_pd3dDeviceContext->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();
}

void CGameFramework::BuildObjects(int mapNum, int roll)
{
	m_pScene = new CScene(mapNum);
	if (m_pScene)
	{
		m_pScene->BuildObjects(m_pd3dDevice);
		m_pScene->BuildLights(m_pd3dDevice);
	}
	m_nPlayers = 1;
	m_ppPlayers = new CPlayer*[m_nPlayers];

	CAirplanePlayer *pAirplanePlyer = new CAirplanePlayer(m_pd3dDevice, m_pScene->m_pCharacters,roll);
	//�÷��̾��� ī�޶� �����̽�-�� ī�޶�� �����Ѵ�.
	pAirplanePlyer->ChangeCamera(m_pd3dDevice, THIRD_PERSON_CAMERA, m_GameTimer.GetTimeElapsed());

	CCamera *pCamera = pAirplanePlyer->GetCamera();
	pCamera->SetViewport(m_pd3dDeviceContext, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
	pCamera->GenerateProjectionMatrix(1.01f, 500000.0f, ASPECT_RATIO, 45.0f);
	pCamera->GenerateOrthoMatrixc(FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.1f, 1000.0f);

	pCamera->GenerateViewMatrix();

	//-1000.0f, 3280.0f, -8150.0f
	if (roll == 0)
	{
		if(mapNum == 1)
			pAirplanePlyer->SetPosition(D3DXVECTOR3(200.0f, 3310.0f, 3500.0f));
		else
			pAirplanePlyer->SetPosition(D3DXVECTOR3(-50.0f, 3310.0f, 3500.0f));

	}
	else
	{
		if(mapNum == 1)
			pAirplanePlyer->SetPosition(D3DXVECTOR3(1628.0f, 3300.0f, 3240.0f));
		else
			pAirplanePlyer->SetPosition(D3DXVECTOR3(200.0f, 3310.0f, 3500.0f));
	}


	//m_pNPC->SetPosition(D3DXVECTOR3(200.0f, 3310.0f, 3500.0f));
	//pAirplanePlyer->SetPosition(D3DXVECTOR3(-300.0f, 3310.0f, -8000.0f));
	pAirplanePlyer->Rotate(0.0f, 180.0f, 0.0f);
	CState *pState = new CState();
	CAnimationClip *pAnimationClip = new CAnimationClip();

	pAnimationClip->LoadAnimation("idle");
	if(roll == 0)
	pAnimationClip->LoadAnimation("run");
	else
		pAnimationClip->LoadAnimation("run2");

	pAnimationClip->LoadAnimation("left");
	pAnimationClip->LoadAnimation("right");
	pAnimationClip->LoadAnimation("backward");
	pAnimationClip->LoadAnimation("jump", 40);
	pAnimationClip->LoadAnimation("jumping");
	pAnimationClip->LoadAnimation("slide");
	pAnimationClip->LoadAnimation("smash");
	pAnimationClip->LoadAnimation("fallback");
	pAnimationClip->LoadAnimation("standup");
	pAnimationClip->LoadAnimation("fallfront");


	pState->SetPlayer(pAirplanePlyer);


	pState->SetAnimationClip(pAnimationClip);
	pState->SetTimer(&m_GameTimer);
	pAirplanePlyer->SetState(pState);
	m_ppPlayers[0] = pAirplanePlyer;
	

	m_nNpc = 4;
	m_pNPC = new CNPC*[m_nNpc];

	for (int i = 0; i < m_nNpc; ++i)
	{
		if (i == roll) m_pNPC[i] = NULL;
		else
		{
			m_pNPC[i] = new CNPC(m_pd3dDevice, m_pScene->m_pCharacters, i);
			m_pNPC[i]->SetFriction(400.0f);
			m_pNPC[i]->SetGravity(D3DXVECTOR3(0.0f, -400.0f, 0.0f));
			m_pNPC[i]->SetMaxVelocityXZ(125.0f);
			m_pNPC[i]->SetMaxVelocityY(400.0f);

			m_pNPC[i]->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			m_pNPC[i]->Rotate(0.0f, 180.0f, 0.0f);
			pState = new CState();
			pAnimationClip = new CAnimationClip();

			pAnimationClip->LoadAnimation("idle");
			if(i == 0) pAnimationClip->LoadAnimation("run");
			else pAnimationClip->LoadAnimation("run2");
			pAnimationClip->LoadAnimation("left");
			pAnimationClip->LoadAnimation("right");
			pAnimationClip->LoadAnimation("backward");
			pAnimationClip->LoadAnimation("jump", 40);
			pAnimationClip->LoadAnimation("jumping");
			pAnimationClip->LoadAnimation("slide");
			pAnimationClip->LoadAnimation("smash");
			pAnimationClip->LoadAnimation("fallback");
			pAnimationClip->LoadAnimation("standup");
			pAnimationClip->LoadAnimation("fallfront");


			pState->SetPlayer(m_pNPC[i]);


			pState->SetAnimationClip(pAnimationClip);
			pState->SetTimer(&m_GameTimer);
			m_pNPC[i]->SetState(pState);
			
			m_pNPC[i]->m_pState->SetState(STATE_IDLE);
		}
	}
	if (roll != 0)
	{
		m_ppPlayers[0]->enemy = (void*)m_pNPC[0];
	}
	//m_ppPlayers[0]->npc = m_pNPC;

	/*if (mapNum == 1||true)
	{
		m_pNPC[0] = new CNPC(m_pd3dDevice, m_pScene->m_pCharacters, mapNum);
		m_pNPC[0]->SetFriction(400.0f);
		m_pNPC[0]->SetGravity(D3DXVECTOR3(0.0f, -400.0f, 0.0f));
		m_pNPC[0]->SetMaxVelocityXZ(125.0f);
		m_pNPC[0]->SetMaxVelocityY(400.0f);
		
		m_pNPC[0]->SetPosition(D3DXVECTOR3(1628.0f, 3300.0f, 3240.0f));
		
		m_pNPC[0]->Rotate(0.0f, 180.0f, 0.0f);
		pState = new CState();
		pAnimationClip = new CAnimationClip();

		pAnimationClip->LoadAnimation("idle");
		pAnimationClip->LoadAnimation("run2");
		pAnimationClip->LoadAnimation("left");
		pAnimationClip->LoadAnimation("right");
		pAnimationClip->LoadAnimation("backward");
		pAnimationClip->LoadAnimation("jump", 40);
		pAnimationClip->LoadAnimation("jumping");
		pAnimationClip->LoadAnimation("slide");
		pAnimationClip->LoadAnimation("smash");
		pAnimationClip->LoadAnimation("fallback");
		pAnimationClip->LoadAnimation("standup");
		pAnimationClip->LoadAnimation("fallfront");


		pState->SetPlayer(m_pNPC[0]);


		pState->SetAnimationClip(pAnimationClip);
		pState->SetTimer(&m_GameTimer);
		m_pNPC[0]->SetState(pState);
		m_pNPC[0]->enemy = m_ppPlayers[0];
		m_pNPC[0]->m_pState->SetState(STATE_RUN);
	}
	else
	{
		if (m_pNPC)m_pNPC = NULL;
	}*/
	tickTime = 0.0f;
}

void CGameFramework::ReleaseObjects()
{


	if (m_pScene) m_pScene->ReleaseObjects();
	if (m_pScene) delete m_pScene;

	if (m_ppPlayers)
	{
		for (int j = 0; j < m_nPlayers; j++) delete m_ppPlayers[j];
		delete[] m_ppPlayers;
	}
}

void CGameFramework::ProcessInput()
{
	bool bProcessedByScene = false;
	if (m_pScene) bProcessedByScene = m_pScene->ProcessInput();
	if (!bProcessedByScene)
	{
		static UCHAR pKeyBuffer[256];
		DWORD dwDirection = 0;
		if(m_ppPlayers[0])m_ppPlayers[0]->bInteraction = false;
		/*Ű������ ���� ������ ��ȯ�Ѵ�. ȭ��ǥ Ű(���桯, ���硯, ���衯, ���顯)�� ������ �÷��̾ ������/����(���� x-��), ��/��(���� z-��)�� �̵��Ѵ�. ��Page Up���� ��Page Down�� Ű�� ������ �÷��̾ ��/�Ʒ�(���� y-��)�� �̵��Ѵ�.*/
		if (GetKeyboardState(pKeyBuffer) && m_ppPlayers[0] )
		{
			if (/*pKeyBuffer[VK_UP] & 0xF0 || pKeyBuffer[VkKeyScan('w')] & 0xF0 ||*/ m_ppPlayers[0]->m_pState->GetState() == STATE_RUN|| m_ppPlayers[0]->m_pState->GetState() == STATE_RUNJUMP ||m_ppPlayers[0]->m_pState->GetState() == STATE_SLIDE) dwDirection |= DIR_FORWARD;
			if (pKeyBuffer[VK_DOWN] & 0xF0 || pKeyBuffer[VkKeyScan('s')] & 0xF0) dwDirection |= DIR_BACKWARD;
			if (pKeyBuffer[VK_LEFT] & 0xF0 || pKeyBuffer[VkKeyScan('a')] & 0xF0) dwDirection |= DIR_LEFT;
			if (pKeyBuffer[VK_RIGHT] & 0xF0 || pKeyBuffer[VkKeyScan('d')] & 0xF0) dwDirection |= DIR_RIGHT;
			if (pKeyBuffer[VK_SPACE] & 0xF0 || pKeyBuffer[VkKeyScan('r')] & 0xF0) dwDirection |= DIR_UP;
			if (pKeyBuffer[VK_NEXT] & 0xF0 || pKeyBuffer[VkKeyScan('f')] & 0xF0) dwDirection |= DIR_DOWN;
			if (pKeyBuffer[VkKeyScan('e')] & 0xF0) m_ppPlayers[0]->bInteraction = true;		
		}
		float cxDelta = 0.0f, cyDelta = 0.0f;
		POINT ptCursorPos;
		/*���콺�� ĸ�������� ���콺�� �󸶸�ŭ �̵��Ͽ��� ���� ����Ѵ�. ���콺 ���� �Ǵ� ������ ��ư�� ������ ���� �޽���(WM_LBUTTONDOWN, WM_RBUTTONDOWN)�� ó���� �� ���콺�� ĸ���Ͽ���. �׷��Ƿ� ���콺�� ĸ�ĵ� ���� ���콺 ��ư�� ������ ���¸� �ǹ��Ѵ�. ���콺�� �¿� �Ǵ� ���Ϸ� �����̸� �÷��̾ x-�� �Ǵ� y-������ ȸ���Ѵ�.*/
		if (GetCapture() == m_hWnd)
		{
			//���콺 Ŀ���� ȭ�鿡�� ���ش�(������ �ʰ� �Ѵ�).
			SetCursor(NULL);
			//���� ���콺 Ŀ���� ��ġ�� �����´�.
			GetCursorPos(&ptCursorPos);
			//���콺 ��ư�� ���� ä�� ���� ��ġ���� ���� ���콺 Ŀ���� ��ġ���� ������ ���� ���Ѵ�.
			cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
			cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
			SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
		}
		//�÷��̾ �̵��ϰų�(dwDirection) ȸ���Ѵ�(cxDelta �Ǵ� cyDelta).
		if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
		{
			if( (cxDelta || cyDelta ) && m_ppPlayers[0]->m_pState->GetState() != STATE_SLIDE &&m_ppPlayers[0]->m_pState->GetState() != STATE_FALLBACK&&m_ppPlayers[0]->m_pState->GetState() != STATE_FALLFRONT)
			{
				/*cxDelta�� y-���� ȸ���� ��Ÿ���� cyDelta�� x-���� ȸ���� ��Ÿ����. ������ ���콺 ��ư�� ������ ��� cxDelta�� z-���� ȸ���� ��Ÿ����.*/
				if (pKeyBuffer[VK_RBUTTON] & 0xF0)
					m_ppPlayers[0]->Rotate(cyDelta, 0.0f, -cxDelta);
				else
					m_ppPlayers[0]->Rotate(cyDelta, cxDelta, 0.0f);
			}
			/*�÷��̾ dwDirection �������� �̵��Ѵ�(�����δ� �ӵ� ���͸� �����Ѵ�). �̵� �Ÿ��� �ð��� ����ϵ��� �Ѵ�. �÷��̾��� �̵� �ӷ��� 500/�ʷ� �����Ѵ�. ���� �÷��̾��� �̵� �ӷ��� �ִٸ� �� ���� ����Ѵ�.*/
			if (dwDirection) m_ppPlayers[0]->Move(dwDirection, 500.0f * m_GameTimer.GetTimeElapsed(), true);
		}

		dwDirection = 0;
		/*if (m_pNPC[0]) m_pNPC[0]->bInteraction = true;

		if (m_pNPC[0])
		{
			if (m_pNPC[0]->m_pState->GetState() == STATE_RUN || m_pNPC[0]->m_pState->GetState() == STATE_RUNJUMP || m_pNPC[0]->m_pState->GetState() == STATE_SLIDE) dwDirection |= DIR_FORWARD;
		}
		if (dwDirection && m_pNPC) m_pNPC[0]->Move(dwDirection, 500.0f * m_GameTimer.GetTimeElapsed(), true);*/
	}
	////�÷��̾ ������ �̵��ϰ� ī�޶� �����Ѵ�. �߷°� �������� ������ �ӵ� ���Ϳ� �����Ѵ�.
	float timeElapsed = m_GameTimer.GetTimeElapsed();
	if(m_ppPlayers[0])m_ppPlayers[0]->Update(timeElapsed);
	
	/*if(m_pNPC[0] != NULL &&m_pNPC[0]->GetPlayerUpdatedContext() != NULL)
		m_pNPC[0]->Update(timeElapsed);*/
	

}

void CGameFramework::AnimateObjects()
{
	if (m_pScene) m_pScene->AnimateObjects(m_pd3dDevice,m_GameTimer.GetTimeElapsed());
}

extern FMOD::System *pfmod;

void CGameFramework::FrameAdvance()
{
	m_GameTimer.Tick(60.0f);
	pfmod->update();
	ProcessInput();
	
	AnimateObjects();
	if (tickTime < 5.0f && m_pNPC == NULL)
	{
		tickTime += m_GameTimer.GetTimeElapsed();
	}
	else if (m_pNPC == NULL)
	{
		/*m_pNPC = new CNPC(m_pd3dDevice, m_pScene->m_pCharacters, 2);
		m_pNPC->SetFriction(400.0f);
		m_pNPC->SetGravity(D3DXVECTOR3(0.0f, -400.0f, 0.0f));
		m_pNPC->SetMaxVelocityXZ(125.0f);
		m_pNPC->SetMaxVelocityY(400.0f);
		m_pNPC->SetPosition(D3DXVECTOR3(200.0f, 3310.0f, 3500.0f));
		m_pNPC->Rotate(0.0f, 180.0f, 0.0f);
		CState* pState = new CState();
		CAnimationClip* pAnimationClip = new CAnimationClip();

		pAnimationClip->LoadAnimation("idle");
		pAnimationClip->LoadAnimation("run2");
		pAnimationClip->LoadAnimation("left");
		pAnimationClip->LoadAnimation("right");
		pAnimationClip->LoadAnimation("backward");
		pAnimationClip->LoadAnimation("jump", 40);
		pAnimationClip->LoadAnimation("jumping");
		pAnimationClip->LoadAnimation("slide");
		pAnimationClip->LoadAnimation("smash");
		pAnimationClip->LoadAnimation("fallback");
		pAnimationClip->LoadAnimation("standup");
		pAnimationClip->LoadAnimation("fallfront");


		pState->SetPlayer(m_pNPC);


		pState->SetAnimationClip(pAnimationClip);
		pState->SetTimer(&m_GameTimer);
		m_pNPC->SetState(pState);
		m_pNPC->enemy = m_ppPlayers[0];
		m_pNPC->m_pState->SetState(STATE_RUN);*/
	}

	//m_pd3dDeviceContext->

	float fClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	if (m_pd3dRenderTargetView) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);
	if (m_pd3dDepthStencilView) m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	CCamera *pCamera = NULL;
	for (int i = 0; i < m_nPlayers; i++)
	{
		if (m_ppPlayers[i])
		{
			
			m_ppPlayers[i]->UpdateShaderVariables(m_pd3dDeviceContext);
			pCamera = m_ppPlayers[i]->GetCamera();
			m_ppPlayers[i]->SetPlayerUpdatedContext(m_pScene->m_ppShaders[3]);
		}
		
		//3��Ī ī�޶��� �� �÷��̾ �������Ѵ�.
		for (int j = 0; j < m_nPlayers; j++) if (m_ppPlayers[j]) m_ppPlayers[j]->Render(m_pd3dDeviceContext);

		
	}

	for (int i = 0; i < m_nNpc; ++i)
	{
		if (m_pNPC[i])
		{
			m_pNPC[i]->UpdateShaderVariables(m_pd3dDeviceContext);

			m_pNPC[i]->SetPlayerUpdatedContext(m_pScene->m_ppShaders[3]);
			m_pNPC[i]->Render(m_pd3dDeviceContext);
		}
	}
	
	if (m_pScene) m_pScene->Render(m_pd3dDeviceContext, pCamera);



	m_pd3dDeviceContext->OMSetDepthStencilState(m_pd3dDepthDisableStencilState, 1);
	if (m_pScene)m_pScene->DrawUI(m_pd3dDeviceContext, pCamera);
	m_pd3dDeviceContext->OMSetDepthStencilState(m_pd3dDepthStencilState, 1);


	m_pDXGISwapChain->Present(0, 0);

	m_GameTimer.GetFrameRate((LPSTR)m_pszBuffer + 28, 30);
	::SetWindowText(m_hWnd, m_pszBuffer);
}

