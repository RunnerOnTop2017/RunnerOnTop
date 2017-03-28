#pragma once

#include"GameObject.h"
#include"Shader.h"

#define MAX_LIGHTS		1
#define POINT_LIGHT		1.0f
#define SPOT_LIGHT		2.0f
#define DIRECTIONAL_LIGHT	3.0f
#define CHARACTER_PATH "Data\\Character\\"
#define ANIMATION_PATH "Data\\Animation\\"

struct LIGHT
{
	D3DXCOLOR m_d3dxcAmbient;
	D3DXCOLOR m_d3dxcDiffuse;
	D3DXCOLOR m_d3dxcSpecular;
	D3DXVECTOR3 m_d3dxvPosition;
	float m_fRange;
	D3DXVECTOR3 m_d3dxvDirection;
	float m_nType;
	D3DXVECTOR3 m_d3dxvAttenuation;
	float m_fFalloff;
	float m_fTheta; //cos(m_fTheta)
	float m_fPhi; //cos(m_fPhi)
	float m_bEnable;
	float padding;
};

//상수 버퍼는 크기가 16의 배수가 되어야 한다. 
struct LIGHTS
{
	LIGHT m_pLights[MAX_LIGHTS];
	D3DXCOLOR m_d3dxcGlobalAmbient;
	D3DXVECTOR4 m_d3dxvCameraPosition;
};


class CScene
{
private:
	

	//CGameObject **m_ppObjects;
	//int m_nObjects;
public:
	CScene();
	~CScene();
	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void BuildObjects(ID3D11Device *pd3dDevice);
	void ReleaseObjects();

	bool ProcessInput();
	void AnimateObjects(ID3D11Device *pd3dDevice,float fTimeElapsed);
	void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);

	CShader **m_ppShaders;
	int m_nShaders;
	CSkyBoxShader *m_pSkybox;

	LIGHTS *m_pLights;
	ID3D11Buffer *m_pd3dcbLights;

	void BuildLights(ID3D11Device *pd3dDevice);
	void UpdateLights(ID3D11DeviceContext *pd3dDeviceContext);
	void ReleaseLights();
};

