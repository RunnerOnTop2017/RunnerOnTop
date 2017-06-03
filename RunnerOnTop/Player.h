#pragma once

#include "Camera.h"
#include "GameObject.h"
#include "Shader.h"
#include "State.h"
#include "PathFinder.h"

// 최종 목적지 15,3
static D3DXVECTOR2 maxMap = { 3000.0f, 3800.0f };
static D3DXVECTOR2 minMap = { -3000.0f, -3800.0f };

class CPlayer : public CGameObject
{
protected:
	// 길찾기용
	int map[20][20];
	node_pos currentPos;
	std::vector<node_pos> route;
	//플레이어의 위치 벡터, x-축(Right), y-축(Up), z-축(Look) 벡터이다.
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXVECTOR3 m_d3dxvRight;
	D3DXVECTOR3 m_d3dxvUp;
	D3DXVECTOR3 m_d3dxvLook;

	//플레이어가 로컬 x-축(Right), y-축(Up), z-축(Look)으로 얼마만큼 회전했는가를 나타낸다.
	float m_fPitch;
	float m_fYaw;
	float m_fRoll;

	//플레이어의 이동 속도를 나타내는 벡터이다.
	D3DXVECTOR3 m_d3dxvVelocity;
	//플레이어에 작용하는 중력을 나타내는 벡터이다.
	D3DXVECTOR3 m_d3dxvGravity;
	//xz-평면에서 (한 프레임 동안) 플레이어의 이동 속력의 최대값을 나타낸다.
	float m_fMaxVelocityXZ;
	//y-축 방향으로 (한 프레임 동안) 플레이어의 이동 속력의 최대값을 나타낸다.
	float m_fMaxVelocityY;
	//플레이어에 작용하는 마찰력을 나타낸다.
	float m_fFriction;

	int NPCDirection;

	//플레이어의 위치가 바뀔 때마다 호출되는 OnPlayerUpdated() 함수에서 사용하는 데이터이다.
	LPVOID m_pPlayerUpdatedContext;
	//카메라의 위치가 바뀔 때마다 호출되는 OnCameraUpdated() 함수에서 사용하는 데이터이다.
	LPVOID m_pCameraUpdatedContext;

	//플레이어의 현재 카메라이다.
	CCamera *m_pCamera;
	
	//3인칭 카메라일 때 플레이어를 그리기 위해 사용하는 쉐이더이다.
	CAnimateShader *m_pShader;

	CCubeMesh *pCollision;
	int lastFloorIndex;
public:
	CPlayer();
	~CPlayer();

	D3DXVECTOR3 GetPosition() { return(m_d3dxvPosition); }
	D3DXVECTOR3 GetLookVector() { return(m_d3dxvLook); }
	D3DXVECTOR3 GetUpVector() { return(m_d3dxvUp); }
	D3DXVECTOR3 GetRightVector() { return(m_d3dxvRight); }

	void SetFriction(float fFriction) { m_fFriction = fFriction; }
	void SetGravity(const D3DXVECTOR3& d3dxvGravity) { m_d3dxvGravity = d3dxvGravity; }
	void SetMaxVelocityXZ(float fMaxVelocity) { m_fMaxVelocityXZ = fMaxVelocity; }
	void SetMaxVelocityY(float fMaxVelocity) { m_fMaxVelocityY = fMaxVelocity; }
	void SetVelocity(const D3DXVECTOR3& d3dxvVelocity) { m_d3dxvVelocity = d3dxvVelocity; }

	//아이템용
	bool bInteraction;
	bool EndAnimation;
	CGameObject *Interacted_OBJ;

	/*플레이어의 위치를 d3dxvPosition 위치로 설정한다. d3dxvPosition 벡터에서 현재 플레이어의 위치 벡터를 빼면 현재 플레이어의 위치에서 d3dxvPosition 방향으로의 방향 벡터가 된다. 현재 플레이어의 위치에서 이 방향 벡터 만큼 이동한다.*/
	void SetPosition(const D3DXVECTOR3& d3dxvPosition) { Move((d3dxvPosition - m_d3dxvPosition), false); }

	const D3DXVECTOR3& GetVelocity() const { return(m_d3dxvVelocity); }
	float GetYaw() const { return(m_fYaw); }
	float GetPitch() const { return(m_fPitch); }
	float GetRoll() const { return(m_fRoll); }

	CCamera *GetCamera() { return(m_pCamera); }
	void SetCamera(CCamera *pCamera) { m_pCamera = pCamera; }

	//플레이어를 이동하는 함수이다.
	void Move(UINT nDirection, float fDistance, bool bVelocity = false);
	void Move(const D3DXVECTOR3& d3dxvShift, bool bVelocity = false);
	void Move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);
	//플레이어를 회전하는 함수이다.
	void Rotate(float x, float y, float z);

	//플레이어의 위치와 회전 정보를 경과 시간에 따라 갱신하는 함수이다.
	void Update(float fTimeElapsed);

	//플레이어의 위치와 회전축으로부터 월드 변환 행렬을 생성하는 함수이다.
	void RegenerateWorldMatrix();

	//플레이어의 위치가 바뀔 때마다 호출되는 함수와 그 함수에서 사용하는 정보를 설정하는 함수이다.
	virtual bool OnPlayerUpdated(float fTimeElapsed);
	void SetPlayerUpdatedContext(LPVOID pContext) { m_pPlayerUpdatedContext = pContext; }

	//카메라의 위치가 바뀔 때마다 호출되는 함수와 그 함수에서 사용하는 정보를 설정하는 함수이다.
	virtual void OnCameraUpdated(float fTimeElapsed);
	void SetCameraUpdatedContext(LPVOID pContext) { m_pCameraUpdatedContext = pContext; }

	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext);

	//카메라를 변경할 때 호출되는 함수이다.
	CCamera *OnChangeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, DWORD nCurrentCameraMode);
	virtual void ChangeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed) { }

	//플레이어의 카메라가 3인칭 카메라일 때 플레이어 메쉬를 렌더링한다.
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};

class CAirplanePlayer : public CPlayer
{
public:
	CAirplanePlayer(ID3D11Device *pd3dDevice, CAnimateShader* pShader);
	~CAirplanePlayer();

	virtual void ChangeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	void SetState(CState* pState);
};

class CNPC : public CPlayer
{
public:
	CNPC(ID3D11Device *pd3dDevice, CAnimateShader* pShader);
	~CNPC();

	virtual void ChangeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	void SetState(CState* pState);
	virtual bool OnPlayerUpdated(float fTimeElapsed);

};
