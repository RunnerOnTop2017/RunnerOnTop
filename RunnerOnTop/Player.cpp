#include "stdafx.h"
#include "Player.h"
#include "PathFinder.h"
#include "fmod.hpp"
#include "fmod_errors.h"

extern GAMESTATENUM gameState;
extern HWND mHwnd;
CPlayer::CPlayer()
{
	m_pCamera = NULL;

	m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_d3dxvRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_d3dxvUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_d3dxvLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	m_d3dxvVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_d3dxvGravity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fMaxVelocityXZ = 0.0f;
	m_fMaxVelocityY = 0.0f;
	m_fFriction = 0.0f;

	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = 0.0f;
	bInteraction = false;
	EndAnimation = false;
	Interacted_OBJ = NULL;
	lastFloorIndex = -1;
	m_pPlayerUpdatedContext = NULL;
	m_pCameraUpdatedContext = NULL;
	
	m_pShader = NULL;

	currentPos.x = -1;
	currentPos.y = -1;


	for(int i = 0; i<map_size_n; ++i)
	{ 
		for (int j = 0; j < map_size_m; ++j)
		{
			map[i][j] = 0;
		}
	}


}

CPlayer::~CPlayer()
{
	if (m_pCamera) delete m_pCamera;
}

/*플레이어의 위치와 회전축으로부터 월드 변환 행렬을 생성하는 함수이다.
플레이어의 Right 벡터가 월드 변환 행렬의 첫 번째 행 벡터,
Up 벡터가 두 번째 행 벡터,
Look 벡터가 세 번째 행 벡터, 플레이어의 위치 벡터가 네 번째 행 벡터가 된다.*/
void CPlayer::RegenerateWorldMatrix()
{
	m_d3dxmtxWorld._11 = m_d3dxvRight.x;
	m_d3dxmtxWorld._12 = m_d3dxvRight.y;
	m_d3dxmtxWorld._13 = m_d3dxvRight.z;
	m_d3dxmtxWorld._21 = m_d3dxvUp.x;
	m_d3dxmtxWorld._22 = m_d3dxvUp.y;
	m_d3dxmtxWorld._23 = m_d3dxvUp.z;
	m_d3dxmtxWorld._31 = m_d3dxvLook.x;
	m_d3dxmtxWorld._32 = m_d3dxvLook.y;
	m_d3dxmtxWorld._33 = m_d3dxvLook.z;
	m_d3dxmtxWorld._41 = m_d3dxvPosition.x;
	m_d3dxmtxWorld._42 = m_d3dxvPosition.y;
	m_d3dxmtxWorld._43 = m_d3dxvPosition.z;
}

/*플레이어의 위치를 변경하는 함수이다. 플레이어의 위치는 기본적으로 사용자가 플레이어를 이동하기 위한 키보드를 누를 때 변경된다. 플레이어의 이동 방향(dwDirection)에 따라 플레이어를 fDistance 만큼 이동한다.*/
void CPlayer::Move(UINT dwDirection, float fDistance, bool bUpdateVelocity)
{
	if (dwDirection)
	{

		D3DXVECTOR3 d3dxvShift = D3DXVECTOR3(0, 0, 0);
		if (m_pState->GetState() == STATE_SLIDE)
		{
			d3dxvShift += m_d3dxvLook * fDistance*1.2f;
		}
		else
		{
			//화살표 키 ‘↑’를 누르면 로컬 z-축 방향으로 이동(전진)한다. ‘↓’를 누르면 반대 방향으로 이동한다.
			if (/*dwDirection & DIR_FORWARD &&*/ (m_pState->GetState() == STATE_RUN || m_pState->GetState() == STATE_RUNJUMP))
			{
				d3dxvShift += m_d3dxvLook * fDistance;
				//MoveForward(fDistance);
			}
			if (/*dwDirection & DIR_BACKWARD &&*/ m_pState->GetState() == STATE_BACK) d3dxvShift -= m_d3dxvLook * fDistance;
			//화살표 키 ‘→’를 누르면 로컬 x-축 방향으로 이동한다. ‘←’를 누르면 반대 방향으로 이동한다.
			if (/*dwDirection & DIR_RIGHT && */(m_pState->GetState() == STATE_RIGHT || m_pState->GetSubState() == STATE_RIGHT))d3dxvShift += m_d3dxvRight * fDistance;
			if (/*dwDirection & DIR_LEFT &&*/ (m_pState->GetState() == STATE_LEFT || m_pState->GetSubState() == STATE_LEFT)) d3dxvShift -= m_d3dxvRight * fDistance;
			//‘Page Up’을 누르면 로컬 y-축 방향으로 이동한다. ‘Page Down’을 누르면 반대 방향으로 이동한다.
			//if ((dwDirection & DIR_UP) && (m_pState->GetState() != STATE_RUNJUMP)) d3dxvShift += m_d3dxvUp * fDistance * 4000.0f;
			//if (dwDirection & DIR_DOWN) d3dxvShift -= m_d3dxvUp * fDistance;
		}


		//플레이어를 현재 위치 벡터에서 d3dxvShift 벡터 만큼 이동한다.
		Move(d3dxvShift, bUpdateVelocity);
	}
}

void CPlayer::Move(const D3DXVECTOR3& d3dxvShift, bool bUpdateVelocity)
{
	//bUpdateVelocity가 참이면 플레이어를 이동하지 않고 속도 벡터를 변경한다.
	if (bUpdateVelocity)
	{
		m_d3dxvVelocity += d3dxvShift;
	}
	else
	{
		//플레이어를 현재 위치 벡터에서 d3dxvShift 벡터 만큼 이동한다.

		D3DXVECTOR3 d3dxvPosition = m_d3dxvPosition + d3dxvShift;
		m_d3dxvPosition = d3dxvPosition;

		RegenerateWorldMatrix();
		//D3DXMatrixTranslation(&m_d3dxmtxWorld, d3dxvPosition.x, d3dxvPosition.y, d3dxvPosition.z);

		//플레이어의 위치가 변경되었으므로 카메라의 위치도 d3dxvShift 벡터 만큼 이동한다.
		if(m_pCamera) m_pCamera->Move(d3dxvShift);
	}
}

void CPlayer::Rotate(float x, float y, float z)
{
	D3DXMATRIX mtxRotate;
	if (m_pCamera)
	{
		DWORD nCurrentCameraMode = m_pCamera->GetMode();

		//1인칭 카메라 또는 3인칭 카메라의 경우 플레이어의 회전은 약간의 제약이 따른다.
		if ((nCurrentCameraMode == FIRST_PERSON_CAMERA) || (nCurrentCameraMode == THIRD_PERSON_CAMERA))
		{
			/*로컬 x-축을 중심으로 회전하는 것은 고개를 앞뒤로 숙이는 동작에 해당한다. 그러므로 x-축을 중심으로 회전하는 각도는 -89.0~+89.0도 사이로 제한한다. x는 현재의 m_fPitch에서 실제 회전하는 각도이므로 x만큼 회전한 다음 Pitch가 +89도 보다 크거나 -89도 보다 작으면 m_fPitch가 +89도 또는 -89도가 되도록 회전각도(x)를 수정한다.*/
			if (x != 0.0f)
			{
				m_fPitch += x;
				if (m_fPitch > +89.0f) { x -= (m_fPitch - 89.0f); m_fPitch = +89.0f; }
				if (m_fPitch < -89.0f) { x -= (m_fPitch + 89.0f); m_fPitch = -89.0f; }
			}
			//로컬 y-축을 중심으로 회전하는 것은 몸통을 돌리는 것이므로 회전 각도의 제한이 없다.
			if (y != 0.0f)
			{
				m_fYaw += y;
				if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
				if (m_fYaw < 0.0f) m_fYaw += 360.0f;
			}
			/*로컬 z-축을 중심으로 회전하는 것은 몸통을 좌우로 기울이는 것이므로 회전 각도는 -20.0~+20.0도 사이로 제한된다. z는 현재의 m_fRoll에서 실제 회전하는 각도이므로 z만큼 회전한 다음 m_fRoll이 +20도 보다 크거나 -20도보다 작으면 m_fRoll이 +20도 또는 -20도가 되도록 회전각도(z)를 수정한다.*/
			if (z != 0.0f)
			{
				m_fRoll += z;
				if (m_fRoll > +20.0f) { z -= (m_fRoll - 20.0f); m_fRoll = +20.0f; }
				if (m_fRoll < -20.0f) { z -= (m_fRoll + 20.0f); m_fRoll = -20.0f; }
			}
			//카메라를 x, y, z 만큼 회전한다. 플레이어를 회전하면 카메라가 회전하게 된다.
			m_pCamera->Rotate(x, y, z);

			/*플레이어를 회전한다. 1인칭 카메라 또는 3인칭 카메라에서 플레이어의 회전은 로컬 y-축에서만 일어난다. 플레이어의 로컬 y-축(Up 벡터)을 기준으로 로컬 z-축(Look 벡터)와 로컬 x-축(Right 벡터)을 회전시킨다. 기본적으로 Up 벡터를 기준으로 회전하는 것은 플레이어가 똑바로 서있는 것을 가정한다는 의미이다.*/
			if (y != 0.0f)
			{
				D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvUp, (float)D3DXToRadian(y));
				D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
				D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
			}
		}
		else if (nCurrentCameraMode == SPACESHIP_CAMERA)
		{
			/*스페이스-쉽 카메라에서 플레이어의 회전은 회전 각도의 제한이 없다. 그리고 모든 축을 중심으로 회전을 할 수 있다.*/
			m_pCamera->Rotate(x, y, z);
			if (x != 0.0f)
			{
				D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvRight, (float)D3DXToRadian(x));
				D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
				D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
			}
			if (y != 0.0f)
			{
				D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvUp, (float)D3DXToRadian(y));
				D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
				D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
			}
			if (z != 0.0f)
			{
				D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvLook, (float)D3DXToRadian(z));
				D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
				D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
			}
		}
	}
	else
	{
		if (x != 0.0f)
		{
			m_fPitch += x;
			if (m_fPitch > +89.0f) { x -= (m_fPitch - 89.0f); m_fPitch = +89.0f; }
			if (m_fPitch < -89.0f) { x -= (m_fPitch + 89.0f); m_fPitch = -89.0f; }
		}
		//로컬 y-축을 중심으로 회전하는 것은 몸통을 돌리는 것이므로 회전 각도의 제한이 없다.
		if (y != 0.0f)
		{
			m_fYaw += y;
			if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
			if (m_fYaw < 0.0f) m_fYaw += 360.0f;
		}
		/*로컬 z-축을 중심으로 회전하는 것은 몸통을 좌우로 기울이는 것이므로 회전 각도는 -20.0~+20.0도 사이로 제한된다. z는 현재의 m_fRoll에서 실제 회전하는 각도이므로 z만큼 회전한 다음 m_fRoll이 +20도 보다 크거나 -20도보다 작으면 m_fRoll이 +20도 또는 -20도가 되도록 회전각도(z)를 수정한다.*/
		if (z != 0.0f)
		{
			m_fRoll += z;
			if (m_fRoll > +20.0f) { z -= (m_fRoll - 20.0f); m_fRoll = +20.0f; }
			if (m_fRoll < -20.0f) { z -= (m_fRoll + 20.0f); m_fRoll = -20.0f; }
		}
		if (y != 0.0f)
		{
			D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvUp, (float)D3DXToRadian(y));
			D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
			D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
		}

	}
	

	/*회전으로 인해 플레이어의 로컬 x-축, y-축, z-축이 서로 직교하지 않을 수 있으므로 z-축(LookAt 벡터)을 기준으로 하여 서로 직교하고 단위벡터가 되도록 한다.*/
	D3DXVec3Normalize(&m_d3dxvLook, &m_d3dxvLook);
	D3DXVec3Cross(&m_d3dxvRight, &m_d3dxvUp, &m_d3dxvLook);
	D3DXVec3Normalize(&m_d3dxvRight, &m_d3dxvRight);
	D3DXVec3Cross(&m_d3dxvUp, &m_d3dxvLook, &m_d3dxvRight);
	D3DXVec3Normalize(&m_d3dxvUp, &m_d3dxvUp);

	RegenerateWorldMatrix();
}

void CPlayer::Update(float fTimeElapsed)
{
	/*플레이어의 속도 벡터를 중력 벡터와 더한다. 중력 벡터에 fTimeElapsed를 곱하는 것은 중력을 시간에 비례하도록 적용한다는 의미이다.*/
	m_d3dxvVelocity += m_d3dxvGravity * fTimeElapsed;
	/*플레이어의 속도 벡터의 XZ-성분의 크기를 구한다. 이것이 XZ-평면의 최대 속력보다 크면 속도 벡터의 x와 z-방향 성분을 조정한다.*/
	float fLength = sqrtf(m_d3dxvVelocity.x * m_d3dxvVelocity.x + m_d3dxvVelocity.z * m_d3dxvVelocity.z);
	if (fLength > m_fMaxVelocityXZ)
	{
		m_d3dxvVelocity.x *= (m_fMaxVelocityXZ / fLength);
		m_d3dxvVelocity.z *= (m_fMaxVelocityXZ / fLength);
	}
	/*플레이어의 속도 벡터의 Y-성분의 크기를 구한다. 이것이 Y 축 방향의 최대 속력보다 크면 속도 벡터의 y-방향 성분을 조정한다.*/
	fLength = sqrtf(m_d3dxvVelocity.y * m_d3dxvVelocity.y);
	if (fLength > m_fMaxVelocityY) m_d3dxvVelocity.y *= (m_fMaxVelocityY / fLength);

	if (m_pCamera)
	{
		DWORD nCurrentCameraMode = m_pCamera->GetMode();



		if (nCurrentCameraMode != SPACESHIP_CAMERA)
			/*플레이어의 위치가 변경될 때 추가로 수행할 작업을 수행한다. 예를 들어, 플레이어의 위치가 변경되었지만 플레이어 객체에는 지형(Terrain)의 정보가 없다. 플레이어의 새로운 위치가 유효한 위치가 아닐 수도 있고 또는 플레이어의 충돌 검사 등을 수행할 필요가 있다. 이러한 상황에서 플레이어의 위치를 유효한 위치로 다시 변경할 수 있다.*/
			if (m_pPlayerUpdatedContext) OnPlayerUpdated(fTimeElapsed);

		/*플레이어를 속도 벡터 만큼 이동한다. 속도 벡터에 fTimeElapsed를 곱하는 것은 속도를 시간에 비례하도록 적용한다는 의미이다.*/
		Move(m_d3dxvVelocity * fTimeElapsed, false);



		//플레이어의 위치가 변경되었으므로 카메라의 상태를 갱신한다.
		if (nCurrentCameraMode == THIRD_PERSON_CAMERA) m_pCamera->Update(fTimeElapsed);
		//카메라의 위치가 변경될 때 추가로 수행할 작업을 수행한다. 


		if (m_pCameraUpdatedContext) OnCameraUpdated(fTimeElapsed);
		//카메라가 3인칭 카메라이면 카메라가 변경된 플레이어 위치를 바라보도록 한다.
		if (nCurrentCameraMode == THIRD_PERSON_CAMERA)
		{
			D3DXVECTOR3 look = m_d3dxvPosition;
			look.y += 90.0f;
			m_pCamera->SetLookAt(look);//m_d3dxvPosition);
		}
		//카메라의 카메라 변환 행렬을 다시 생성한다.
		if (m_pCamera) m_pCamera->RegenerateViewMatrix();
	}
	else
	{
		if (m_pPlayerUpdatedContext) OnPlayerUpdated(fTimeElapsed);

		
		Move(m_d3dxvVelocity * fTimeElapsed, false);

	}

	/*플레이어의 속도 벡터가 마찰력 때문에 감속이 되어야 한다면 감속 벡터를 생성한다. 속도 벡터의 반대 방향 벡터를 구하고 단위 벡터로 만든다. 마찰 계수를 시간에 비례하도록 하여 마찰력을 구한다. 단위 벡터에 마찰력을 곱하여 감속 벡터를 구한다. 속도 벡터에 감속 벡터를 더하여 속도 벡터를 줄인다. 마찰력이 속력보다 크면 속력은 0이 될 것이다.*/
	D3DXVECTOR3 d3dxvDeceleration = -m_d3dxvVelocity;
	D3DXVec3Normalize(&d3dxvDeceleration, &d3dxvDeceleration);
	fLength = D3DXVec3Length(&m_d3dxvVelocity);
	float fDeceleration = (m_fFriction * fTimeElapsed);
	if (fDeceleration > fLength) fDeceleration = fLength;
	m_d3dxvVelocity += d3dxvDeceleration * fDeceleration;
}

CCamera *CPlayer::OnChangeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, DWORD nCurrentCameraMode)
{
	CCamera *pNewCamera = NULL;
	//새로운 카메라의 모드에 따라 카메라를 새로 생성한다.
	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		pNewCamera = new CFirstPersonCamera(m_pCamera);
		break;
	case THIRD_PERSON_CAMERA:
		pNewCamera = new CThirdPersonCamera(m_pCamera);
		break;
	case SPACESHIP_CAMERA:
		pNewCamera = new CSpaceShipCamera(m_pCamera);
		break;
	}
	/*현재 카메라의 모드가 스페이스-쉽 모드의 카메라이고 새로운 카메라가 1인칭 또는 3인칭 카메라이면 플레이어의 Up 벡터를 월드좌표계의 y-축 방향 벡터(0, 1, 0)이 되도록 한다. 즉, 똑바로 서도록 한다. 그리고 스페이스-쉽 카메라의 경우 플레이어의 이동에는 제약이 없다. 특히, y-축 방향의 움직임이 자유롭다. 그러므로 플레이어의 위치는 공중(위치 벡터의 y-좌표가 0보다 크다)이 될 수 있다. 이때 새로운 카메라가 1인칭 또는 3인칭 카메라이면 플레이어의 위치는 지면이 되어야 한다. 그러므로 플레이어의 Right 벡터와 Look 벡터의 y 값을 0으로 만든다. 이제 플레이어의 Right 벡터와 Look 벡터는 단위벡터가 아니므로 정규화한다.*/
	if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		m_d3dxvUp = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		m_d3dxvRight.y = 0.0f;
		m_d3dxvLook.y = 0.0f;
		D3DXVec3Normalize(&m_d3dxvRight, &m_d3dxvRight);
		D3DXVec3Normalize(&m_d3dxvLook, &m_d3dxvLook);
		m_fPitch = 0.0f;
		m_fRoll = 0.0f;
		/*Look 벡터와 월드좌표계의 z-축(0, 0, 1)이 이루는 각도(내적=cos)를 계산하여 플레이어의 y-축의 회전 각도 m_fYaw로 설정한다.*/
		m_fYaw = (float)D3DXToDegree(acosf(D3DXVec3Dot(&D3DXVECTOR3(0.0f, 0.0f, 1.0f), &m_d3dxvLook)));
		if (m_d3dxvLook.x < 0.0f) m_fYaw = -m_fYaw;
	}
	else if ((nNewCameraMode == SPACESHIP_CAMERA) && m_pCamera)
	{
		/*새로운 카메라의 모드가 스페이스-쉽 모드의 카메라이고 현재 카메라 모드가 1인칭 또는 3인칭 카메라이면 플레이어의 로컬 축을 현재 카메라의 로컬 축과 같게 만든다.*/
		m_d3dxvRight = m_pCamera->GetRightVector();
		m_d3dxvUp = m_pCamera->GetUpVector();
		m_d3dxvLook = m_pCamera->GetLookVector();
	}

	if (pNewCamera)
	{
		//기존 카메라가 없으면 새로운 카메라를 위한 쉐이더 변수를 생성한다.
		if (!m_pCamera) pNewCamera->CreateShaderVariables(pd3dDevice);
		pNewCamera->SetMode(nNewCameraMode);
		//현재 카메라를 사용하는 플레이어 객체를 설정한다.
		pNewCamera->SetPlayer(this);
	}

	if (m_pCamera) delete m_pCamera;

	return(pNewCamera);
}


void CPlayer::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
}

void CPlayer::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (m_pCamera) m_pCamera->UpdateShaderVariables(pd3dDeviceContext);
}

void CPlayer::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (m_pShader)
	{
		m_pShader->m_ppObjects[0]->m_d3dxmtxWorld = m_d3dxmtxWorld;
		D3DXMATRIX matrix;
		D3DXMatrixTranslation(&matrix, 50.0f, 0.0f, 0.0f);
		m_pShader->m_ppObjects[1]->m_d3dxmtxWorld = matrix*m_d3dxmtxWorld;

		m_pShader->Render(pd3dDeviceContext);
	}
}

void CPlayer::ResetUpLookRight()
{
	m_d3dxvRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_d3dxvUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_d3dxvLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	RegenerateWorldMatrix();
}

#define _WITH_DEBUG

extern FMOD::System *pfmod;
extern FMOD::Channel *footCh;
extern FMOD::Sound *footstep;

bool CPlayer::OnPlayerUpdated(float fTimeElapsed)
{
	bool isPlaying;
	footCh->getPaused(&isPlaying);
	if (m_pState->GetState() == STATE_RUN)
	{
		if (isPlaying)
		{
			
			footCh->setPaused(false);
		}
	}
	else
	{
		if (!isPlaying)
		{
			footCh->setPaused(true);

		}
	}

	// 이동 거리
	D3DXVECTOR3 dxvShift = m_d3dxvVelocity *fTimeElapsed;

	//캐릭터 xyz min,max 구하기;
	CDiffuseNormalVertex * cVertex = pCollision->pVertices;
	D3DXVECTOR4 cPosition[8];
	D3DXVECTOR4 m_Pos;
	D3DXVECTOR3 d3dxv_cPosition = GetPosition();

	//D3DXVec3Transform(&m_Pos, &d3dxv_cPosition, &m_d3dxmtxWorld);
	//d3dxv_cPosition = { m_Pos.x, m_Pos.y, m_Pos.z };
	D3DXVECTOR3 min = { -15.0f, 0.0f, -15.0f };
	D3DXVECTOR3 max = { 15.0f, 75.0f, 15.0f };

	/*for (int i = 0; i < 8; ++i)
	{
		D3DXVec3Transform(&cPosition[i], &cVertex[i].m_d3dxvPosition, &m_d3dxmtxWorld);
	}
	

	float minX = cPosition[0].x, minY = cPosition[0].y, minZ = cPosition[0].z;
	float maxX = cPosition[0].x, maxY = cPosition[0].y, maxZ = cPosition[0].z;

	for (int i = 1; i < 8; ++i)
	{
		if (cPosition[i].x > maxX) maxX = cPosition[i].x;
		if (cPosition[i].x < minX) minX = cPosition[i].x;
		if (cPosition[i].y > maxY) maxY = cPosition[i].y;
		if (cPosition[i].y < minY) minY = cPosition[i].y;
		if (cPosition[i].z > maxZ) maxZ = cPosition[i].z;
		if (cPosition[i].z < minZ) minZ = cPosition[i].z;
	}*/
	
	D3DXVECTOR3 d3dxv_cMax = d3dxv_cPosition + max;//{ maxX, maxY, maxZ };
	D3DXVECTOR3 d3dxv_cMin = d3dxv_cPosition + min;//{ minX, minY, minZ };
	D3DXVECTOR3 d3dxv_center = (d3dxv_cMax + d3dxv_cMin) / 2.0f;

	CDiffusedShader *pShader = (CDiffusedShader*)m_pPlayerUpdatedContext;

	int nObjects = pShader->m_nObjects;
	
	int OBJECT_CNT = pShader->OBJECT_CNT;
	int FLOOR_CNT = pShader->FLOOR_CNT;
	int WALL_CNT = pShader->WALL_CNT;

	if (EndAnimation)
	{
		if (m_pState->GetPrevState() == STATE_FALLFRONT)
		{
			D3DXVECTOR3 vec = GetPosition();
			D3DXVECTOR3 look = GetLookAt();

			vec += look*100.0f;

			SetPosition(vec);
		}

		if (Interacted_OBJ)
		{
			Interacted_OBJ->ref->bInteracted = true;
			Interacted_OBJ = NULL;
		}
		EndAnimation = false;
	}


	// 바닥 충돌 체크
	for (int i = 0; i < FLOOR_CNT; ++i)
	{
		CDiffuseNormalVertex *mVertices = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->pVertices; //(8개)

		D3DXVECTOR3 d3dxvMax = { mVertices[1].m_d3dxvPosition.x , mVertices[4].m_d3dxvPosition.y, mVertices[2].m_d3dxvPosition.z };
		D3DXVECTOR3 d3dxvMin = { mVertices[0].m_d3dxvPosition.x , mVertices[0].m_d3dxvPosition.y,  mVertices[0].m_d3dxvPosition.z };



		bool x, y, z;
		CollisionCheck(d3dxv_cMax, d3dxv_cMin, d3dxvMax, d3dxvMin, dxvShift, x, y, z);
		OBJECTTAG tag = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->m_tag;
		
		if (tag == FALL)
		{
			if( (-1 != lastFloorIndex) && (x||y||z))
			{
				CDiffuseNormalVertex *vertices = ((CCubeMesh*)pShader->m_ppObjects[lastFloorIndex]->m_pMesh)->pVertices;

				D3DXVECTOR3 dMax = { vertices[1].m_d3dxvPosition.x , vertices[4].m_d3dxvPosition.y, vertices[2].m_d3dxvPosition.z };
				D3DXVECTOR3 dMin = { vertices[0].m_d3dxvPosition.x , vertices[0].m_d3dxvPosition.y,  vertices[0].m_d3dxvPosition.z };
				D3DXVECTOR3 vec = (dMax + dMin) / 2.0f;
				vec.y = dMax.y+0.1f;
				SetPosition(vec);
				m_d3dxvVelocity.x = 0.0f;
				m_d3dxvVelocity.y = 0.0f;
				m_d3dxvVelocity.z = 0.0f;

			}
		}
		else
		{
			if (m_pState->GetState() == STATE_RUNJUMP && (x || z))
			{
				if (lastFloorIndex != i)
				{

					float maxY = d3dxv_cMin.y;

					if (abs(maxY - d3dxvMax.y) < 20.0f)
					{
						m_d3dxvVelocity.y = 0.0f;
						D3DXVECTOR3 vec = GetPosition();
						vec.y = d3dxvMax.y + 0.1f;
						SetPosition(vec);
					}
					else
					{

						m_d3dxvVelocity.x = 0.0f;
						m_d3dxvVelocity.y = 0.0f;
						m_d3dxvVelocity.z = 0.0f;

					}
				}
				else
				{
					m_d3dxvVelocity.y = 0.0f;
					D3DXVECTOR3 vec = GetPosition();
					vec.y = d3dxvMax.y + 0.1f;
					SetPosition(vec);
				}
			}
			else if (x)
			{
				m_d3dxvVelocity.x = 0.0f;
			}
			else if (z)
			{
				m_d3dxvVelocity.z = 0.0f;
			}
			else if (y == true)
			{
				m_d3dxvVelocity.y = 0.0f;
				D3DXVECTOR3 vec = GetPosition();
				vec.y = d3dxvMax.y + 0.1f;
				SetPosition(vec);
				if (lastFloorIndex != i)
				{
					lastFloorIndex = i;
				}
			}
		}

		

		if (y) {
			//std::cout << "[" << lastFloorIndex << "]" << std::endl;
		}
	}

	if (m_pState->GetState() == STATE_RUNJUMP)
	{
		//m_d3dxvVelocity.y = 0.0f;
	}

#ifdef _DEBUG
	/*system("cls");
	std::cout << GetPosition() << std::endl;*/

#endif

	// 건물 충돌체크
	for (int i = FLOOR_CNT; i < nObjects; ++i)
	{
		CDiffuseNormalVertex *mVertices = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->pVertices; //(8개)
		D3DXVECTOR3 d3dxvMax = { mVertices[1].m_d3dxvPosition.x , mVertices[4].m_d3dxvPosition.y, mVertices[2].m_d3dxvPosition.z };
		D3DXVECTOR3 d3dxvMin = { mVertices[0].m_d3dxvPosition.x , mVertices[0].m_d3dxvPosition.y,  mVertices[0].m_d3dxvPosition.z };
		bool x, y, z;
		OBJECTTAG tag = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->m_tag;
		if (true == CollisionCheck(d3dxv_cMax, d3dxv_cMin, d3dxvMax, d3dxvMin, dxvShift, x, y, z) && tag!=BOX && tag!=REALBOX)
		{
			
			// 문?
			if (DOOR == tag)
			{
				if (bInteraction)
				{
					std::cout << "Interected!" << std::endl;
					if (pShader->m_ppObjects[i]->ref!= NULL && pShader->m_ppObjects[i]->ref->bInteracted == false && Interacted_OBJ == NULL)
					{
						m_pState->SetSubState(STATE_SMASH);
						Interacted_OBJ = pShader->m_ppObjects[i];
						//pShader->m_ppObjects[i]->ref->bInteracted = true;
					}
				}
			}
			else if (REALDOOR == tag)
			{
				if (NULL != pShader->m_ppObjects[i]->ref && false == pShader->m_ppObjects[i]->ref->bInteracted)
				{
					m_pState->SetState(STATE_FALLBACK);
					pShader->m_ppObjects[i]->ref->bInteracted = true;
					if (NULL!=Interacted_OBJ)
					{
						Interacted_OBJ->ref->bInteracted = true;
						Interacted_OBJ = NULL;
						EndAnimation = false;
					}
					
				}
			}
			else if (FENCE == tag)
			{
				if (m_pState->GetState() != STATE_SLIDE)
				{
					m_d3dxvVelocity = -50.0f*GetLookAt();
					/*if (x == true)
						m_d3dxvVelocity.x *= -1.0f;

					if (y == true)
						m_d3dxvVelocity.y *= -1.0f;

					if (z == true)
						m_d3dxvVelocity.z *= -1.0f;*/
					m_pState->SetState(STATE_FALLBACK);
				}
			}
			else if (FENCEHOLE == tag)
			{
				if (bInteraction)
				{
					if (pShader->m_ppObjects[i]->ref != NULL && Interacted_OBJ == NULL)
					{
						Interacted_OBJ = pShader->m_ppObjects[i];
						m_pState->SetState(STATE_SLIDE);
						bInteraction = false;
					}
				}
			}
			else if (PIPE == tag)
			{
				if (m_pState->GetState() != STATE_RUNJUMP &&Interacted_OBJ == NULL && m_pState->GetPrevState() != STATE_FALLFRONT)
				{
					m_pState->SetState(STATE_FALLFRONT);
					Interacted_OBJ = pShader->m_ppObjects[i];
				}
			}
			else if (CONDITIONER == tag)
			{
				m_d3dxvVelocity.x += 3.0f;
			}
			else if (JUMPAREA == tag)
			{

			}
			else if (GOAL == tag)
			{
				gameState = YOUWIN;
				InvalidateRect(mHwnd, NULL, false);
				ReleaseCapture();

			}
			else
			{
				//std::cout << "WALL : " << i << std::endl;
				if (x == true)
					m_d3dxvVelocity.x *= -1.0f;

				if (y == true)
					m_d3dxvVelocity.y *= -1.0f;

				if (z == true)
					m_d3dxvVelocity.z *= -1.0f;
			}

		}
	
		if (tag == REALBOX &&pShader->m_ppObjects[i]->ref->m_physics.isValid == false && m_pState->GetPrevState() != STATE_FALLFRONT)
		{
			//CDiffuseNormalVertex *mVertices = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->pVertices; //(8개)
			D3DXVECTOR3 d3dxvMax = { mVertices[1].m_d3dxvPosition.x , mVertices[4].m_d3dxvPosition.y, mVertices[2].m_d3dxvPosition.z };
			D3DXVECTOR3 d3dxvMin = { mVertices[0].m_d3dxvPosition.x , mVertices[0].m_d3dxvPosition.y,  mVertices[0].m_d3dxvPosition.z };
			D3DXMATRIX world = pShader->m_ppObjects[i]->ref->m_d3dxmtxWorld;
			D3DXVECTOR4 d3dv4Max, d3dv4Min;

			D3DXVec3Transform(&d3dv4Max, &d3dxvMax, &world);
			D3DXVec3Transform(&d3dv4Min, &d3dxvMin, &world);

			d3dxvMax = { d3dv4Max.x, d3dv4Max.y, d3dv4Max.z };
			d3dxvMin = { d3dv4Min.x, d3dv4Min.y, d3dv4Min.z };

			if (CollisionCheck(d3dxv_cMax, d3dxv_cMin, d3dxvMax, d3dxvMin, dxvShift, x, y, z))
			{
				if (m_pState->GetState() != STATE_RUNJUMP)
				{
					m_pState->SetState(STATE_FALLFRONT);
				}
				
			}
			

		}
		else if (tag == BOX)
		{
			//CDiffuseNormalVertex *mVertices = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->pVertices; //(8개)
			D3DXVECTOR3 d3dxvMax = { mVertices[1].m_d3dxvPosition.x , mVertices[4].m_d3dxvPosition.y, mVertices[2].m_d3dxvPosition.z };
			D3DXVECTOR3 d3dxvMin = { mVertices[0].m_d3dxvPosition.x , mVertices[0].m_d3dxvPosition.y,  mVertices[0].m_d3dxvPosition.z };
			D3DXMATRIX world = pShader->m_ppObjects[i]->ref->m_d3dxmtxWorld;
			D3DXVECTOR4 d3dv4Max, d3dv4Min;

			D3DXVec3Transform(&d3dv4Max, &d3dxvMax, &world);
			D3DXVec3Transform(&d3dv4Min, &d3dxvMin, &world);
			D3DXVECTOR3 pos = pShader->m_ppObjects[i]->ref->GetPosition();
			//d3dxvMax = { d3dv4Max.x, d3dv4Max.y, d3dv4Max.z };
			//d3dxvMin = { d3dv4Min.x, d3dv4Min.y, d3dv4Min.z };

			if (CollisionCheck(d3dxv_cMax, d3dxv_cMin, d3dxvMax+pos, d3dxvMin+pos, dxvShift, x, y, z))
			{
				if (true == bInteraction)
				{
					if (pShader->m_ppObjects[i]->ref)
					{
						if (pShader->m_ppObjects[i]->ref->m_physics.isValid == false)
						{
							m_pState->SetSubState(STATE_SMASH);
							pShader->m_ppObjects[i]->ref->m_physics.isValid = true;
							pShader->m_ppObjects[i]->ref->rotateValue = 0.0f;
							D3DXVECTOR3 look = GetLookAt();
							D3DXVECTOR3 right = GetRight();
							pShader->m_ppObjects[i]->ref->m_physics.velocity = { 0.0f, 0.0f ,0.0f };
							pShader->m_ppObjects[i]->ref->m_physics.velocity += -300.0f * look;
							pShader->m_ppObjects[i]->ref->m_physics.velocity += -300.0f * right;
							pShader->m_ppObjects[i]->ref->m_physics.velocity.y = 400.0f;

							std::cout << pShader->m_ppObjects[i]->ref->m_physics.velocity << std::endl;
						}
						
					}
				}
			}
		}


	}

	
	
	return true;
}

void CPlayer::OnCameraUpdated(float fTimeElapsed)
{
}


CAirplanePlayer::CAirplanePlayer(ID3D11Device *pd3dDevice, CAnimateShader *pShader)
{
	//비행기 메쉬를 생성한다.
	//CMesh *pAirplaneMesh = new CCharacterMesh(pd3dDevice, "Police01");
	CCubeMesh *Collision = new CCubeMesh(pd3dDevice, -15.0f, 15.0f, 0.0f, 75.0f, -15.0f, 15.0f);
	//SetMesh(pAirplaneMesh);
	pCollision = Collision;
	//플레이어(비행기) 메쉬를 렌더링할 때 사용할 쉐이더를 생성한다.
	m_pShader = pShader;// new CAnimateShader();
	//m_pShader->CreateShader(pd3dDevice);
	//m_pShader->CreateShaderVariables(pd3dDevice);
	//m_pShader->BuildObjects(pd3dDevice);

	//플레이어를 위한 쉐이더 변수를 생성한다.
	CreateShaderVariables(pd3dDevice);

	m_pShader->m_ppObjects[0]->m_pState = m_pState;
}

CAirplanePlayer::~CAirplanePlayer()
{
	if (m_pShader) delete m_pShader;
}

void CAirplanePlayer::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	DWORD nCurrentCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;
	if ((nCurrentCameraMode == THIRD_PERSON_CAMERA))
	{
		if (m_pShader)
		{
			m_pShader->m_ppObjects[0]->m_d3dxmtxWorld = m_d3dxmtxWorld;
			//D3DXMATRIX matrix;
			//D3DXMatrixTranslation(&matrix, 50.0f, 0.0f, 0.0f);
			//m_pShader->m_ppObjects[1]->m_d3dxmtxWorld = matrix*m_d3dxmtxWorld;

			m_pShader->Render(pd3dDeviceContext, NULL, 0);
		}
	}
}

void CAirplanePlayer::SetState(CState * pState)
{
	m_pState = pState;

	m_pShader->m_ppObjects[0]->m_pState = pState;
}

void CAirplanePlayer::ChangeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed)
{
	DWORD nCurrentCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;
	if (nCurrentCameraMode == nNewCameraMode) return;
	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		//플레이어의 특성을 1인칭 카메라 모드에 맞게 변경한다. 중력은 적용하지 않는다.
		SetFriction(200.0f);
		SetGravity(D3DXVECTOR3(0.0f, -10.0f, 0.0f));
		SetMaxVelocityXZ(125.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(pd3dDevice, FIRST_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(D3DXVECTOR3(0.0f, 20.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 500000.0f, ASPECT_RATIO, 60.0f);
		break;
	case SPACESHIP_CAMERA:
		//플레이어의 특성을 스페이스-쉽 카메라 모드에 맞게 변경한다. 중력은 적용하지 않는다.
		SetFriction(125.0f);
		SetGravity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetMaxVelocityXZ(400.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(pd3dDevice, SPACESHIP_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 500000.0f, ASPECT_RATIO, 60.0f);
		break;
	case THIRD_PERSON_CAMERA:
		//플레이어의 특성을 3인칭 카메라 모드에 맞게 변경한다. 지연 효과와 카메라 오프셋을 설정한다.
		SetFriction(400.0f);
		SetGravity(D3DXVECTOR3(0.0f, -400.0f, 0.0f));
		SetMaxVelocityXZ(125.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(pd3dDevice, THIRD_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.25f);
		m_pCamera->SetOffset(D3DXVECTOR3(0.0f, 150.0f, -300.0f));
		m_pCamera->GenerateProjectionMatrix(1.00f, 5000.0f, ASPECT_RATIO, 90.0f);
		break;
	default:
		break;
	}
	//카메라 정보를 시간에 따라 갱신한다.
	Update(fTimeElapsed);
}



CNPC::CNPC(ID3D11Device *pd3dDevice, CAnimateShader *pShader, int mapNumber)
{
	m_pCamera = NULL;
	//비행기 메쉬를 생성한다.
	//CMesh *pAirplaneMesh = new CCharacterMesh(pd3dDevice, "Police01");
	CCubeMesh *Collision = new CCubeMesh(pd3dDevice, -15.0f, 15.0f, 0.0f, 75.0f, -15.0f, 15.0f);
	//SetMesh(pAirplaneMesh);
	pCollision = new CCubeMesh(pd3dDevice, -15.0f, 15.0f, 0.0f, 75.0f, -15.0f, 15.0f);//Collision;
	//플레이어(비행기) 메쉬를 렌더링할 때 사용할 쉐이더를 생성한다.
	m_pShader = pShader;
	
	mapnumber = mapNumber;
	chasing = false;
	if (mapNumber == 1)
	{
		maxMap = { 3000.0f, 3800.0f };
		minMap = { -3000.0f, -3800.0f };
	}
	else
	{
		maxMap = { 345.0f, 3795.0f };
		minMap = { -2005.0f, -9450.0f };


		map2Route.push_back({ -733.247f, 3290.1f, 3382.35f });
		map2Route.push_back({ -1706.73f, 3290.1f, 3382.35f });
		map2Route.push_back({ -1706.95f, 3290.1f, 3237.79f });
		map2Route.push_back({ -1701.95f, 3290.1f, 1585.66f });
		map2Route.push_back({ -75.9984f, 3290.1f, 1634.86f });
		map2Route.push_back({ -27.3715f, 3290.1f, -109.575f });
		map2Route.push_back({ -1641.1f, 3290.1f, -134.71f });
		map2Route.push_back({ -1645.38f, 3290.1f, -2186.21f });
		map2Route.push_back({ -853.153f, 3290.1f, -2153.38f });
		map2Route.push_back({ -861.297f, 3290.1f, -3491.64f });
		map2Route.push_back({ -1662.36f, 3290.1f, -3489.93f });
		map2Route.push_back({ -1651.3f, 3290.1f, -5388.16f });
		map2Route.push_back({ -27.8956f, 3290.1f, -5393.5f });
		map2Route.push_back({ -27.8509f, 3290.1f, -7205.38f });
		map2Route.push_back({ -1674.4f, 3290.1f, -7238.39f });


		

	}
	RouteIndex = 0;
						//플레이어를 위한 쉐이더 변수를 생성한다.
	CreateShaderVariables(pd3dDevice);

	m_pShader->m_ppObjects[1]->m_pState = m_pState;
}

CNPC::~CNPC()
{
	if (m_pShader) delete m_pShader;
}

void CNPC::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	
	if (m_pShader)
	{
		m_pShader->m_ppObjects[1]->m_d3dxmtxWorld = m_d3dxmtxWorld;
		//std::cout << GetPosition().x << std::endl;
		//D3DXMATRIX matrix;
		//D3DXMatrixTranslation(&matrix, 50.0f, 0.0f, 0.0f);
		//m_pShader->m_ppObjects[1]->m_d3dxmtxWorld = matrix*m_d3dxmtxWorld;

		m_pShader->Render(pd3dDeviceContext, NULL, 1);
	}
}

void CNPC::SetState(CState * pState)
{
	m_pState = pState;

	m_pShader->m_ppObjects[1]->m_pState = pState;
}

void CNPC::ChangeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed)
{
	
}
bool CNPC::OnPlayerUpdated(float fTimeElapsed)
{
	//std::cout << "Called" << m_d3dxvVelocity.y <<std::endl;
	// 이동 거리
	D3DXVECTOR3 dxvShift = m_d3dxvVelocity *fTimeElapsed;

	//캐릭터 xyz min,max 구하기;
	CDiffuseNormalVertex * cVertex = pCollision->pVertices;
	D3DXVECTOR4 cPosition[8];
	D3DXVECTOR4 m_Pos;
	D3DXVECTOR3 d3dxv_cPosition = GetPosition();


	D3DXVECTOR3 min = { -15.0f, 0.0f, -15.0f };
	D3DXVECTOR3 max = { 15.0f, 75.0f, 15.0f };


	/*D3DXVec3Transform(&m_Pos, &d3dxv_cPosition, &m_d3dxmtxWorld);
	d3dxv_cPosition = { m_Pos.x, m_Pos.y, m_Pos.z };

	for (int i = 0; i < 8; ++i)
	{
		D3DXVec3Transform(&cPosition[i], &cVertex[i].m_d3dxvPosition, &m_d3dxmtxWorld);
	}


	float minX = cPosition[0].x, minY = cPosition[0].y, minZ = cPosition[0].z;
	float maxX = cPosition[0].x, maxY = cPosition[0].y, maxZ = cPosition[0].z;

	for (int i = 1; i < 8; ++i)
	{
		if (cPosition[i].x > maxX) maxX = cPosition[i].x;
		if (cPosition[i].x < minX) minX = cPosition[i].x;
		if (cPosition[i].y > maxY) maxY = cPosition[i].y;
		if (cPosition[i].y < minY) minY = cPosition[i].y;
		if (cPosition[i].z > maxZ) maxZ = cPosition[i].z;
		if (cPosition[i].z < minZ) minZ = cPosition[i].z;
	}*/

	D3DXVECTOR3 d3dxv_cMax = d3dxv_cPosition + max;
	D3DXVECTOR3 d3dxv_cMin = d3dxv_cPosition + min;
	D3DXVECTOR3 d3dxv_center = (d3dxv_cMax + d3dxv_cMin) / 2.0f;

	/*
	cVertex = enemy->pCollision->pVertices;
	for (int i = 0; i < 8; ++i)
	{
		D3DXVec3Transform(&cPosition[i], &cVertex[i].m_d3dxvPosition, &enemy->m_d3dxmtxWorld);
	}


	minX = cPosition[0].x, minY = cPosition[0].y, minZ = cPosition[0].z;
	maxX = cPosition[0].x, maxY = cPosition[0].y, maxZ = cPosition[0].z;

	for (int i = 1; i < 8; ++i)
	{
		if (cPosition[i].x > maxX) maxX = cPosition[i].x;
		if (cPosition[i].x < minX) minX = cPosition[i].x;
		if (cPosition[i].y > maxY) maxY = cPosition[i].y;
		if (cPosition[i].y < minY) minY = cPosition[i].y;
		if (cPosition[i].z > maxZ) maxZ = cPosition[i].z;
		if (cPosition[i].z < minZ) minZ = cPosition[i].z;
	}
*/
	
	D3DXVECTOR3 d3dxv_eMax = enemy->GetPosition() + max;
	D3DXVECTOR3 d3dxv_eMin = enemy->GetPosition() + min;
	D3DXVECTOR3 d3dxv_eCenter = (d3dxv_eMax + d3dxv_eMin) / 2.0f;

	CDiffusedShader *pShader = (CDiffusedShader*)m_pPlayerUpdatedContext;
	int OBJECT_CNT = pShader->OBJECT_CNT;
	int FLOOR_CNT = pShader->FLOOR_CNT;
	int WALL_CNT = pShader->WALL_CNT;
	float vtheta;
	float vs;
	float vt;
	float v1length;
	float v2length;
	D3DXVECTOR3 v1;
	D3DXVECTOR3 v2;
	float cosRad;

	if (EndAnimation)
	{
		if (m_pState->GetPrevState() == STATE_FALLFRONT)
		{
			D3DXVECTOR3 vec = GetPosition();
			D3DXVECTOR3 look = GetLookAt();

			vec += look*100.0f;

			SetPosition(vec);
		}

		if (Interacted_OBJ)
		{
			Interacted_OBJ->ref->bInteracted = true;
			Interacted_OBJ = NULL;
		}
		EndAnimation = false;
	}

	if (mapnumber == 1)
	{
		if (currentPos.x == -1 && currentPos.y == -1)
		{
			CreateNodeMap(map, minMap, maxMap, (CDiffusedShader*)m_pPlayerUpdatedContext, 2, FLOOR_CNT, true);
			for (int i = 0; i < map_size_m; ++i)
			{
				for (int j = 0; j < map_size_n; ++j)
				{
					if (map[j][i] == 1) map[j][i] = 0;
					else map[j][i] = 1;
				}
			}
			if (mapnumber == 1)
			{
				map[24][26] = 1;
				map[25][26] = 1;
				map[26][16] = 1;
				map[27][15] = 1;
				map[28][15] = 1;
				map[29][37] = 1;
				map[29][36] = 1;
				map[29][35] = 1;
				map[29][34] = 1;
				map[29][33] = 1;
				map[29][32] = 1;
				map[29][31] = 1;
				map[29][30] = 1;
				map[29][29] = 1;
				map[29][17] = 1;

			}
			NPCDirection = 3;
		}




		node_pos pos = PositionToNodeIndex(d3dxv_center.x, d3dxv_center.z, D_METER, minMap, maxMap);

		D3DXVECTOR4 minmax = NodeIndexToMinMax(pos, minMap, maxMap, 40);

		node_pos epos = PositionToNodeIndex(d3dxv_eCenter.x, d3dxv_eCenter.z, D_METER, minMap, maxMap);

		static node_pos_float tmp;
		static float degree;
		static std::string path;
		static std::string detailpath;
		static int ex, ey;
		static int detailPathIndex;
		static node_pos dPos;
		static bool detailcheck;
		static node_pos start;

		if ((pos.x == epos.x && pos.y == epos.y) && (pos.x != currentPos.x || pos.y != currentPos.y))
		{
			D3DXVECTOR3 look = GetLookVector();

			D3DXVECTOR3 v1 = { look.x,0.0f ,look.z };

			D3DXVECTOR3 v2 = { d3dxv_eCenter.x - d3dxv_center.x, 0.0f ,d3dxv_eCenter.z - d3dxv_center.z };

			float v1length = D3DXVec3Length(&v1);
			float v2length = D3DXVec3Length(&v2);

			float vs = D3DXVec3Dot(&v1, &v2);         // 거리
			float vt = acosf(vs / (v1length * v2length));
			float vtheta = (180 * vt) / D3DX_PI;             // 각도


			Rotate(0.0f, -1 * vtheta, 0.0f);
			currentPos = pos;

		}

		if (50.0f>GetDistance(d3dxv_center.x, d3dxv_center.z, d3dxv_eCenter.x, d3dxv_eCenter.z))
		{
			if (m_pState->GetState() == STATE_RUN) m_pState->SetState(STATE_IDLE);
			gameState = GAMEOVER;
			InvalidateRect(mHwnd, NULL, false);
			ReleaseCapture();
		}
		else
		{

			if (m_pState->GetState() == STATE_IDLE && detailpath.length() != 0) m_pState->SetState(STATE_RUN);
		}

		if (pos.x != currentPos.x || pos.y != currentPos.y)
		{
			float mdx = (maxMap.x - minMap.x) / map_size_n;
			float mdy = (maxMap.y - minMap.y) / map_size_m;
			float dMx = (pos.x * mdx) + minMap.x + mdx;
			float dmx = (pos.x * mdx) + minMap.x;

			float dMz = (pos.y * mdy) + minMap.y + mdy;
			float dmz = (pos.y * mdy) + minMap.y;

			for (int i = 0; i < detail_size_n; ++i) for (int j = 0; j < detail_size_m; ++j)
				detailmap[i][j] = 1;



			CreateNodeDetailMap(detailmap, { minmax.x,minmax.y }, { minmax.z, minmax.w }, (CDiffusedShader*)m_pPlayerUpdatedContext, WALL_CNT, OBJECT_CNT);


			path = pathFind(pos.x, pos.y, epos.x, epos.y, map);
			route.clear();
			route = PathStringToNodeIndex(path, pos);

			if (path.length() != 0) {
				char c = path.at(0);
				int d = c - '0';
				start = PositionToNodeIndex(d3dxv_center.x, d3dxv_center.z, 20, { minmax.x, minmax.y }, { minmax.z, minmax.w });

				FindEndPoint(ex, ey, d, detailmap, start.x, start.y);
				currentPos.x = pos.x;
				currentPos.y = pos.y;

				detailpath = detailpathFind(start.x, start.y, ex, ey, detailmap);

				if (detailpath.length() == 0)
				{
					start = PositionToNodeIndex(d3dxv_center.x, d3dxv_center.z, 20, { minmax.x, minmax.y }, { minmax.z, minmax.w });

					d = (d + 3) % 4;
					FindEndPoint(ex, ey, d, detailmap, start.x, start.y);
					detailpath = detailpathFind(start.x, start.y, ex, ey, detailmap);

				}

				detailPathIndex = 0;
				dPos = start;
				detailcheck = false;
			}
		}

		node_pos ndPos = PositionToNodeIndex(d3dxv_center.x, d3dxv_center.z, 20, { minmax.x, minmax.y }, { minmax.z, minmax.w });


		if (dPos != ndPos || !detailcheck)
		{
			detailcheck = true;
			if (detailpath.length() != 0 && detailPathIndex < detailpath.length())
			{
				if (detailpath.at(detailPathIndex) == '0')
				{
					if (NPCDirection != 0)
					{
						ResetUpLookRight();
						Rotate(0.0f, 90.0f, 0.0);
						NPCDirection = 0;

					}

				}
				else if (detailpath.at(detailPathIndex) == '3')
				{
					if (NPCDirection != 3)
					{
						ResetUpLookRight();
						Rotate(0.0f, 180.0f, 0.0);
						NPCDirection = 3;

					}

				}
				else if (detailpath.at(detailPathIndex) == '2')
				{
					if (NPCDirection != 2)
					{
						ResetUpLookRight();
						Rotate(0.0f, 270.0f, 0.0);
						NPCDirection = 2;

					}

				}
				else if (detailpath.at(detailPathIndex) == '1')
				{
					if (NPCDirection != 1)
					{
						ResetUpLookRight();
						Rotate(0.0f, 0.0f, 0.0);
						NPCDirection = 1;

					}

				}
			}
			else
			{
				m_pState->SetState(STATE_IDLE);
			}
			dPos = ndPos;
			detailPathIndex++;
		}




	}
	else if (mapnumber == 2)
	{

		//유저한테 가장 가까운 포인트 찾기
		D3DXVECTOR3 ePosition = enemy->GetPosition();
		int enemyIndex = 0;
		float eDistance = D3DXVec3Length(&D3DXVECTOR3(map2Route[0].x - ePosition.x , 0, map2Route[0].z - ePosition.z));

		for (int i = 1; i < map2Route.size(); ++i)
		{
			float dist = D3DXVec3Length(&D3DXVECTOR3(map2Route[i].x - ePosition.x, 0, map2Route[i].z - ePosition.z));
			if (dist < eDistance)
			{
				enemyIndex = i;
			}
		}


		//유저를 보는 방향벡터 생성
		D3DXVECTOR3 toEnemy = D3DXVECTOR3( ePosition.x - GetPosition().x, 0,  ePosition.z - GetPosition().z);
		//유저까지와의 거리
		float toEnemyDistance = D3DXVec3Length(&toEnemy);

		if (enemyIndex == RouteIndex && toEnemyDistance < 200.0f)
		{
			if (!chasing) chasing = true;
		}
		else
		{
			if (chasing)
			{
				RouteIndex = enemyIndex;
				chasing = false;
			}
		}

		D3DXVECTOR3 look = GetLookVector();

		v1 = { look.x, 0.0f ,look.z };

		if(chasing)
			v2 = toEnemy;
		else
			v2 = { map2Route[RouteIndex].x - GetPosition().x , 0.0f , map2Route[RouteIndex].z - GetPosition().z };
		//D3DXVECTOR3 v2 = { d3dxv_eCenter.x - d3dxv_center.x, 0.0f ,d3dxv_eCenter.z - d3dxv_center.z };
		//D3DXVec3Normalize(&v1, &v1);
		D3DXVECTOR3 v3;
		D3DXVec3Normalize(&v3, &v2);

		 v1length = D3DXVec3Length(&v1);
		 v2length = D3DXVec3Length(&v3);


		vs = D3DXVec3Dot(&v1, &v3);// 거리

		cosRad = vs / (v1length * v2length);
		
		if (!isnan(cosRad) && cosRad != 1.0f)
		{
			vt = acosf(cosRad);
			if (!isnan(vt))
			{
				vtheta = (180.0f * vt) / 3.1415926535f;
				if (vs != 1.0f && vs != -1.0f)
				{
					D3DXVECTOR3 cross;
					D3DXVec3Cross(&cross, &v1, &v3);
					if (cross.y < 0.0f) vtheta *= -1.0f;
					if (cross.y != 0.0f) Rotate(0.0f, vtheta, 0.0f);


				}
			}
			
		}

		if (D3DXVec3Length(&v2) < 10.0f)
		{
			RouteIndex++;
		}

		if (toEnemyDistance < 50.0f)
		{
			if (m_pState->GetState() == STATE_RUN) m_pState->SetState(STATE_IDLE);
			gameState = GAMEOVER;
			InvalidateRect(mHwnd, NULL, false);
			ReleaseCapture();
		}
		

	
	}


#ifdef _DEBUG
	//system("cls");
	//std::cout << GetPosition() << std::endl;
	//std::cout << NPCDirection << std::endl;
	//printf("NPOS : %d, %d\n", pos.x, pos.y);
	//printf("DPOS : %d, %d\n", dPos.x, dPos.y);
	//printf("STAT : %d, %d\n", start.x, start.y);
	//system("cls");
	/*std::cout << "v1 : " << v1 << std::endl;
	std::cout << "v2 : " << v2 << std::endl;

	std::cout << "v1length : " << v1length << std::endl;
	std::cout << "v2length : " << v2length << std::endl;

	std::cout << "vs : " << vs << std::endl;
	std::cout << "cosRad : " << cosRad << std::endl;

	std::cout << "vt : " << vt << std::endl;



	std::cout << "vtheta : " << vtheta << std::endl;

	std::cout << "Position : " << GetPosition() << std::endl;
	std::cout << "YawPitchRoll : " << GetYaw() << ", " << GetPitch() << ", " << GetRoll() << std::endl << std::endl;;*/
	//std::string str = "";
	//
	//bool b = false;

	//for (int y = 0; y < detail_size_m; ++y)
	//{
	//	for (int x = 0; x < detail_size_n; ++x)
	//	{
	//		b = false;
	//		for (int i = 0; i < route.size(); ++i)
	//		{
	//			if (route[i].x == x && route[i].y == y)
	//			{
	//				b = true;

	//				break;
	//			}
	//		}
	//		/*if (x == pos.x && y == pos.y) std::cout << "⊙";
	//		else if (b) 	std::cout << "◎";
	//		else if (map[x][y] == 0) std::cout << "○";
	//		else if (map[x][y] == 1) std::cout << "  ";*/
	//		//if (x == dPos.x && y == dPos.y) str.append("⊙");
	//		//else if (ex == x && ey == y) 	str.append("◎");
	//		//else if (detailmap[x][y] == 0) str.append("○");
	//		//else if (detailmap[x][y] == 1) str.append("●");



	//	}
	//	//std::cout << std::endl;
	//	////str.append("\n");
	//}
	//std::cout << str << std::endl;
	/*printf("Position : %f, %f, %f\n", d3dxv_center.x, d3dxv_center.y, d3dxv_center.z);
	printf("Node Pos : %d, %d\n", pos.x, pos.y);
	printf("Current Path[0] : %c\n", path[0]);
	printf("Node Center : %f, %f\n", tmp.x, tmp.y);
	printf("Degree : %f\n", degree);
	std::cout << path << std::endl;
	std::cout << epos.x<<", "<< epos.y << std::endl;*/
#endif
	//CDiffusedShader *pShader = (CDiffusedShader*)m_pPlayerUpdatedContext;

	int nObjects = pShader->m_nObjects;




	// 바닥 충돌 체크
	for (int i = 0; i < FLOOR_CNT; ++i)
	{
		CDiffuseNormalVertex *mVertices = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->pVertices; //(8개)

		D3DXVECTOR3 d3dxvMax = { mVertices[1].m_d3dxvPosition.x , mVertices[4].m_d3dxvPosition.y, mVertices[2].m_d3dxvPosition.z };
		D3DXVECTOR3 d3dxvMin = { mVertices[0].m_d3dxvPosition.x , mVertices[0].m_d3dxvPosition.y,  mVertices[0].m_d3dxvPosition.z };



		bool x, y, z;
		CollisionCheck(d3dxv_cMax, d3dxv_cMin, d3dxvMax, d3dxvMin, dxvShift, x, y, z);
		OBJECTTAG tag = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->m_tag;

		if (tag == FALL)
		{
			if ((-1 != lastFloorIndex) && (x || y || z))
			{
				CDiffuseNormalVertex *vertices = ((CCubeMesh*)pShader->m_ppObjects[lastFloorIndex]->m_pMesh)->pVertices;

				D3DXVECTOR3 dMax = { vertices[1].m_d3dxvPosition.x , vertices[4].m_d3dxvPosition.y, vertices[2].m_d3dxvPosition.z };
				D3DXVECTOR3 dMin = { vertices[0].m_d3dxvPosition.x , vertices[0].m_d3dxvPosition.y,  vertices[0].m_d3dxvPosition.z };
				D3DXVECTOR3 vec = (dMax + dMin) / 2.0f;
				vec.y = dMax.y + 0.1f;
				SetPosition(vec);
				m_d3dxvVelocity.x = 0.0f;
				m_d3dxvVelocity.y = 0.0f;
				m_d3dxvVelocity.z = 0.0f;

			}
		}
		else
		{
			if ((x || z) && m_pState->GetState() != STATE_RUNJUMP) m_pState->SetState(STATE_RUNJUMP);

			if (m_pState->GetState() == STATE_RUNJUMP && (x || z))
			{
				if (lastFloorIndex != i)
				{

					float maxY = d3dxv_cMin.y;

					if (abs(maxY - d3dxvMax.y) < 20.0f)
					{
						m_d3dxvVelocity.y = 0.0f;
						D3DXVECTOR3 vec = GetPosition();
						vec.y = d3dxvMax.y + 0.1f;
						SetPosition(vec);
					}
					else
					{

						m_d3dxvVelocity.x = 0.0f;
						//m_d3dxvVelocity.y = 0.0f;
						m_d3dxvVelocity.z = 0.0f;

					}
				}
				else
				{
					m_d3dxvVelocity.y = 0.0f;
					D3DXVECTOR3 vec = GetPosition();
					vec.y = d3dxvMax.y + 0.1f;
					SetPosition(vec);
				}
			}
			else if (x)
			{
				m_d3dxvVelocity.x = 0.0f;
			}
			else if (z)
			{
				m_d3dxvVelocity.z = 0.0f;
			}
			else if (y == true)
			{
				m_d3dxvVelocity.y = 0.0f;
				D3DXVECTOR3 vec = GetPosition();
				vec.y = d3dxvMax.y + 0.1f;
				SetPosition(vec);
				if (lastFloorIndex != i)
				{
					lastFloorIndex = i;
				}
			}
		}



		if (y) {
			//std::cout << "[" << lastFloorIndex << "]" << std::endl;
		}
	}

	if (m_pState->GetState() == STATE_RUNJUMP)
	{
		//m_d3dxvVelocity.y = 0.0f;
	}


	// 건물 충돌체크
	for (int i = FLOOR_CNT; i < nObjects; ++i)
	{
		CDiffuseNormalVertex *mVertices = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->pVertices; //(8개)
		D3DXVECTOR3 d3dxvMax = { mVertices[1].m_d3dxvPosition.x , mVertices[4].m_d3dxvPosition.y, mVertices[2].m_d3dxvPosition.z };
		D3DXVECTOR3 d3dxvMin = { mVertices[0].m_d3dxvPosition.x , mVertices[0].m_d3dxvPosition.y,  mVertices[0].m_d3dxvPosition.z };
		bool x, y, z;
		OBJECTTAG tag = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->m_tag;
		if (true == CollisionCheck(d3dxv_cMax, d3dxv_cMin, d3dxvMax, d3dxvMin, dxvShift, x, y, z) && tag != BOX && tag != REALBOX)
		{

			// 문?
			if (DOOR == tag)
			{
				bInteraction = true;
				if (bInteraction)
				{
					std::cout << "Interected!" << std::endl;
					if (pShader->m_ppObjects[i]->ref != NULL && pShader->m_ppObjects[i]->ref->bInteracted == false && Interacted_OBJ == NULL)
					{
						m_pState->SetSubState(STATE_SMASH);
						Interacted_OBJ = pShader->m_ppObjects[i];
						//pShader->m_ppObjects[i]->ref->bInteracted = true;
					}
				}
			}
			else if (REALDOOR == tag)
			{
				if (NULL != pShader->m_ppObjects[i]->ref && false == pShader->m_ppObjects[i]->ref->bInteracted)
				{
					m_pState->SetState(STATE_FALLBACK);
					pShader->m_ppObjects[i]->ref->bInteracted = true;
					if (NULL != Interacted_OBJ)
					{
						Interacted_OBJ->ref->bInteracted = true;
						Interacted_OBJ = NULL;
						EndAnimation = false;
					}

				}
			}
			else if (FENCE == tag)
			{
				if (m_pState->GetState() != STATE_SLIDE)
				{
					m_d3dxvVelocity = -50.0f*GetLookAt();
					/*if (x == true)
					m_d3dxvVelocity.x *= -1.0f;

					if (y == true)
					m_d3dxvVelocity.y *= -1.0f;

					if (z == true)
					m_d3dxvVelocity.z *= -1.0f;*/
					m_pState->SetState(STATE_FALLBACK);
				}
			}
			else if (FENCEHOLE == tag)
			{
				if (bInteraction)
				{
					if (pShader->m_ppObjects[i]->ref != NULL && Interacted_OBJ == NULL)
					{
						Interacted_OBJ = pShader->m_ppObjects[i];
						m_pState->SetState(STATE_SLIDE);
						bInteraction = false;
					}
				}
			}
			else if (PIPE == tag)
			{
				
				if (m_pState->GetState() != STATE_RUNJUMP &&Interacted_OBJ == NULL && m_pState->GetPrevState() != STATE_FALLFRONT)
				{
					//m_pState->SetState(STATE_FALLFRONT);
					m_pState->SetState(STATE_RUNJUMP);
					//Interacted_OBJ = pShader->m_ppObjects[i];
				}
			}
			else if (CONDITIONER == tag)
			{
				m_d3dxvVelocity.x += 3.0f;
			}
			else if (JUMPAREA == tag)
			{

			}
			else if (GOAL == tag)
			{
				gameState = YOUWIN;
				InvalidateRect(mHwnd, NULL, false);
				ReleaseCapture();

			}
			else
			{
				//std::cout << "WALL : " << i << std::endl;
				if (x == true)
					m_d3dxvVelocity.x *= -1.0f;

				if (y == true)
					m_d3dxvVelocity.y *= -1.0f;

				if (z == true)
					m_d3dxvVelocity.z *= -1.0f;
			}

		}

		if (tag == REALBOX &&pShader->m_ppObjects[i]->ref->m_physics.isValid == false && m_pState->GetPrevState() != STATE_FALLFRONT)
		{
			//CDiffuseNormalVertex *mVertices = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->pVertices; //(8개)
			D3DXVECTOR3 d3dxvMax = { mVertices[1].m_d3dxvPosition.x , mVertices[4].m_d3dxvPosition.y, mVertices[2].m_d3dxvPosition.z };
			D3DXVECTOR3 d3dxvMin = { mVertices[0].m_d3dxvPosition.x , mVertices[0].m_d3dxvPosition.y,  mVertices[0].m_d3dxvPosition.z };
			D3DXMATRIX world = pShader->m_ppObjects[i]->ref->m_d3dxmtxWorld;
			D3DXVECTOR4 d3dv4Max, d3dv4Min;

			D3DXVec3Transform(&d3dv4Max, &d3dxvMax, &world);
			D3DXVec3Transform(&d3dv4Min, &d3dxvMin, &world);

			d3dxvMax = { d3dv4Max.x, d3dv4Max.y, d3dv4Max.z };
			d3dxvMin = { d3dv4Min.x, d3dv4Min.y, d3dv4Min.z };

			if (CollisionCheck(d3dxv_cMax, d3dxv_cMin, d3dxvMax, d3dxvMin, dxvShift, x, y, z))
			{
				if (m_pState->GetState() != STATE_RUNJUMP)
				{
					m_pState->SetState(STATE_FALLFRONT);
				}

			}

			
		}
		else if (tag == BOX)
		{
			//CDiffuseNormalVertex *mVertices = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->pVertices; //(8개)
			D3DXVECTOR3 d3dxvMax = { mVertices[1].m_d3dxvPosition.x , mVertices[4].m_d3dxvPosition.y, mVertices[2].m_d3dxvPosition.z };
			D3DXVECTOR3 d3dxvMin = { mVertices[0].m_d3dxvPosition.x , mVertices[0].m_d3dxvPosition.y,  mVertices[0].m_d3dxvPosition.z };
			D3DXMATRIX world = pShader->m_ppObjects[i]->ref->m_d3dxmtxWorld;
			D3DXVECTOR4 d3dv4Max, d3dv4Min;

			D3DXVec3Transform(&d3dv4Max, &d3dxvMax, &world);
			D3DXVec3Transform(&d3dv4Min, &d3dxvMin, &world);
			D3DXVECTOR3 pos = pShader->m_ppObjects[i]->ref->GetPosition();
			//d3dxvMax = { d3dv4Max.x, d3dv4Max.y, d3dv4Max.z };
			//d3dxvMin = { d3dv4Min.x, d3dv4Min.y, d3dv4Min.z };

			if (CollisionCheck(d3dxv_cMax, d3dxv_cMin, d3dxvMax + pos, d3dxvMin + pos, dxvShift, x, y, z))
			{
				if (true == bInteraction)
				{
					if (pShader->m_ppObjects[i]->ref)
					{
						if (pShader->m_ppObjects[i]->ref->m_physics.isValid == false)
						{
							m_pState->SetSubState(STATE_SMASH);
							pShader->m_ppObjects[i]->ref->m_physics.isValid = true;
							pShader->m_ppObjects[i]->ref->rotateValue = 0.0f;
							D3DXVECTOR3 look = GetLookAt();
							D3DXVECTOR3 right = GetRight();
							pShader->m_ppObjects[i]->ref->m_physics.velocity = { 0.0f, 0.0f ,0.0f };
							pShader->m_ppObjects[i]->ref->m_physics.velocity += -300.0f * look;
							pShader->m_ppObjects[i]->ref->m_physics.velocity += -300.0f * right;
							pShader->m_ppObjects[i]->ref->m_physics.velocity.y = 400.0f;

							std::cout << pShader->m_ppObjects[i]->ref->m_physics.velocity << std::endl;
						}

					}
				}
			}
		}


	}

	//if (m_pState->GetState() == STATE_FALLBACK)
	//{
	//	FILE *fp = fopen("road.txt", "w");
	//	for (int i = 0; i < detail_size_m; ++i)
	//	{
	//		for (int j = 0; j < detail_size_n; ++j)
	//		{
	//			if(j == dPos.x && i == dPos.y) fprintf(fp, "2");

	//			else if (j == ex && i == ey) fprintf(fp, "3");

	//			else fprintf(fp, "%d", detailmap[j][i]);


	//		}
	//		fprintf(fp, "\n");
	//	}
	//	fclose(fp);
	//}

	return true;
}