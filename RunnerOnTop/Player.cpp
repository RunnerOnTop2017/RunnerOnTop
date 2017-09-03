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

/*�÷��̾��� ��ġ�� ȸ�������κ��� ���� ��ȯ ����� �����ϴ� �Լ��̴�.
�÷��̾��� Right ���Ͱ� ���� ��ȯ ����� ù ��° �� ����,
Up ���Ͱ� �� ��° �� ����,
Look ���Ͱ� �� ��° �� ����, �÷��̾��� ��ġ ���Ͱ� �� ��° �� ���Ͱ� �ȴ�.*/
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

/*�÷��̾��� ��ġ�� �����ϴ� �Լ��̴�. �÷��̾��� ��ġ�� �⺻������ ����ڰ� �÷��̾ �̵��ϱ� ���� Ű���带 ���� �� ����ȴ�. �÷��̾��� �̵� ����(dwDirection)�� ���� �÷��̾ fDistance ��ŭ �̵��Ѵ�.*/
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
			//ȭ��ǥ Ű ���衯�� ������ ���� z-�� �������� �̵�(����)�Ѵ�. ���顯�� ������ �ݴ� �������� �̵��Ѵ�.
			if (/*dwDirection & DIR_FORWARD &&*/ (m_pState->GetState() == STATE_RUN || m_pState->GetState() == STATE_RUNJUMP))
			{
				d3dxvShift += m_d3dxvLook * fDistance;
				//MoveForward(fDistance);
			}
			if (/*dwDirection & DIR_BACKWARD &&*/ m_pState->GetState() == STATE_BACK) d3dxvShift -= m_d3dxvLook * fDistance;
			//ȭ��ǥ Ű ���桯�� ������ ���� x-�� �������� �̵��Ѵ�. ���硯�� ������ �ݴ� �������� �̵��Ѵ�.
			if (/*dwDirection & DIR_RIGHT && */(m_pState->GetState() == STATE_RIGHT || m_pState->GetSubState() == STATE_RIGHT))d3dxvShift += m_d3dxvRight * fDistance;
			if (/*dwDirection & DIR_LEFT &&*/ (m_pState->GetState() == STATE_LEFT || m_pState->GetSubState() == STATE_LEFT)) d3dxvShift -= m_d3dxvRight * fDistance;
			//��Page Up���� ������ ���� y-�� �������� �̵��Ѵ�. ��Page Down���� ������ �ݴ� �������� �̵��Ѵ�.
			//if ((dwDirection & DIR_UP) && (m_pState->GetState() != STATE_RUNJUMP)) d3dxvShift += m_d3dxvUp * fDistance * 4000.0f;
			//if (dwDirection & DIR_DOWN) d3dxvShift -= m_d3dxvUp * fDistance;
		}


		//�÷��̾ ���� ��ġ ���Ϳ��� d3dxvShift ���� ��ŭ �̵��Ѵ�.
		Move(d3dxvShift, bUpdateVelocity);
	}
}

void CPlayer::Move(const D3DXVECTOR3& d3dxvShift, bool bUpdateVelocity)
{
	//bUpdateVelocity�� ���̸� �÷��̾ �̵����� �ʰ� �ӵ� ���͸� �����Ѵ�.
	if (bUpdateVelocity)
	{
		m_d3dxvVelocity += d3dxvShift;
	}
	else
	{
		//�÷��̾ ���� ��ġ ���Ϳ��� d3dxvShift ���� ��ŭ �̵��Ѵ�.

		D3DXVECTOR3 d3dxvPosition = m_d3dxvPosition + d3dxvShift;
		m_d3dxvPosition = d3dxvPosition;

		RegenerateWorldMatrix();
		//D3DXMatrixTranslation(&m_d3dxmtxWorld, d3dxvPosition.x, d3dxvPosition.y, d3dxvPosition.z);

		//�÷��̾��� ��ġ�� ����Ǿ����Ƿ� ī�޶��� ��ġ�� d3dxvShift ���� ��ŭ �̵��Ѵ�.
		if(m_pCamera) m_pCamera->Move(d3dxvShift);
	}
}

void CPlayer::Rotate(float x, float y, float z)
{
	D3DXMATRIX mtxRotate;
	if (m_pCamera)
	{
		DWORD nCurrentCameraMode = m_pCamera->GetMode();

		//1��Ī ī�޶� �Ǵ� 3��Ī ī�޶��� ��� �÷��̾��� ȸ���� �ణ�� ������ ������.
		if ((nCurrentCameraMode == FIRST_PERSON_CAMERA) || (nCurrentCameraMode == THIRD_PERSON_CAMERA))
		{
			/*���� x-���� �߽����� ȸ���ϴ� ���� ���� �յڷ� ���̴� ���ۿ� �ش��Ѵ�. �׷��Ƿ� x-���� �߽����� ȸ���ϴ� ������ -89.0~+89.0�� ���̷� �����Ѵ�. x�� ������ m_fPitch���� ���� ȸ���ϴ� �����̹Ƿ� x��ŭ ȸ���� ���� Pitch�� +89�� ���� ũ�ų� -89�� ���� ������ m_fPitch�� +89�� �Ǵ� -89���� �ǵ��� ȸ������(x)�� �����Ѵ�.*/
			if (x != 0.0f)
			{
				m_fPitch += x;
				if (m_fPitch > +89.0f) { x -= (m_fPitch - 89.0f); m_fPitch = +89.0f; }
				if (m_fPitch < -89.0f) { x -= (m_fPitch + 89.0f); m_fPitch = -89.0f; }
			}
			//���� y-���� �߽����� ȸ���ϴ� ���� ������ ������ ���̹Ƿ� ȸ�� ������ ������ ����.
			if (y != 0.0f)
			{
				m_fYaw += y;
				if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
				if (m_fYaw < 0.0f) m_fYaw += 360.0f;
			}
			/*���� z-���� �߽����� ȸ���ϴ� ���� ������ �¿�� ����̴� ���̹Ƿ� ȸ�� ������ -20.0~+20.0�� ���̷� ���ѵȴ�. z�� ������ m_fRoll���� ���� ȸ���ϴ� �����̹Ƿ� z��ŭ ȸ���� ���� m_fRoll�� +20�� ���� ũ�ų� -20������ ������ m_fRoll�� +20�� �Ǵ� -20���� �ǵ��� ȸ������(z)�� �����Ѵ�.*/
			if (z != 0.0f)
			{
				m_fRoll += z;
				if (m_fRoll > +20.0f) { z -= (m_fRoll - 20.0f); m_fRoll = +20.0f; }
				if (m_fRoll < -20.0f) { z -= (m_fRoll + 20.0f); m_fRoll = -20.0f; }
			}
			//ī�޶� x, y, z ��ŭ ȸ���Ѵ�. �÷��̾ ȸ���ϸ� ī�޶� ȸ���ϰ� �ȴ�.
			m_pCamera->Rotate(x, y, z);

			/*�÷��̾ ȸ���Ѵ�. 1��Ī ī�޶� �Ǵ� 3��Ī ī�޶󿡼� �÷��̾��� ȸ���� ���� y-�࿡���� �Ͼ��. �÷��̾��� ���� y-��(Up ����)�� �������� ���� z-��(Look ����)�� ���� x-��(Right ����)�� ȸ����Ų��. �⺻������ Up ���͸� �������� ȸ���ϴ� ���� �÷��̾ �ȹٷ� ���ִ� ���� �����Ѵٴ� �ǹ��̴�.*/
			if (y != 0.0f)
			{
				D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvUp, (float)D3DXToRadian(y));
				D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
				D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
			}
		}
		else if (nCurrentCameraMode == SPACESHIP_CAMERA)
		{
			/*�����̽�-�� ī�޶󿡼� �÷��̾��� ȸ���� ȸ�� ������ ������ ����. �׸��� ��� ���� �߽����� ȸ���� �� �� �ִ�.*/
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
		//���� y-���� �߽����� ȸ���ϴ� ���� ������ ������ ���̹Ƿ� ȸ�� ������ ������ ����.
		if (y != 0.0f)
		{
			m_fYaw += y;
			if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
			if (m_fYaw < 0.0f) m_fYaw += 360.0f;
		}
		/*���� z-���� �߽����� ȸ���ϴ� ���� ������ �¿�� ����̴� ���̹Ƿ� ȸ�� ������ -20.0~+20.0�� ���̷� ���ѵȴ�. z�� ������ m_fRoll���� ���� ȸ���ϴ� �����̹Ƿ� z��ŭ ȸ���� ���� m_fRoll�� +20�� ���� ũ�ų� -20������ ������ m_fRoll�� +20�� �Ǵ� -20���� �ǵ��� ȸ������(z)�� �����Ѵ�.*/
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
	

	/*ȸ������ ���� �÷��̾��� ���� x-��, y-��, z-���� ���� �������� ���� �� �����Ƿ� z-��(LookAt ����)�� �������� �Ͽ� ���� �����ϰ� �������Ͱ� �ǵ��� �Ѵ�.*/
	D3DXVec3Normalize(&m_d3dxvLook, &m_d3dxvLook);
	D3DXVec3Cross(&m_d3dxvRight, &m_d3dxvUp, &m_d3dxvLook);
	D3DXVec3Normalize(&m_d3dxvRight, &m_d3dxvRight);
	D3DXVec3Cross(&m_d3dxvUp, &m_d3dxvLook, &m_d3dxvRight);
	D3DXVec3Normalize(&m_d3dxvUp, &m_d3dxvUp);

	RegenerateWorldMatrix();
}

void CPlayer::Update(float fTimeElapsed)
{
	/*�÷��̾��� �ӵ� ���͸� �߷� ���Ϳ� ���Ѵ�. �߷� ���Ϳ� fTimeElapsed�� ���ϴ� ���� �߷��� �ð��� ����ϵ��� �����Ѵٴ� �ǹ��̴�.*/
	m_d3dxvVelocity += m_d3dxvGravity * fTimeElapsed;
	/*�÷��̾��� �ӵ� ������ XZ-������ ũ�⸦ ���Ѵ�. �̰��� XZ-����� �ִ� �ӷº��� ũ�� �ӵ� ������ x�� z-���� ������ �����Ѵ�.*/
	float fLength = sqrtf(m_d3dxvVelocity.x * m_d3dxvVelocity.x + m_d3dxvVelocity.z * m_d3dxvVelocity.z);
	if (fLength > m_fMaxVelocityXZ)
	{
		m_d3dxvVelocity.x *= (m_fMaxVelocityXZ / fLength);
		m_d3dxvVelocity.z *= (m_fMaxVelocityXZ / fLength);
	}
	/*�÷��̾��� �ӵ� ������ Y-������ ũ�⸦ ���Ѵ�. �̰��� Y �� ������ �ִ� �ӷº��� ũ�� �ӵ� ������ y-���� ������ �����Ѵ�.*/
	fLength = sqrtf(m_d3dxvVelocity.y * m_d3dxvVelocity.y);
	if (fLength > m_fMaxVelocityY) m_d3dxvVelocity.y *= (m_fMaxVelocityY / fLength);

	if (m_pCamera)
	{
		DWORD nCurrentCameraMode = m_pCamera->GetMode();



		if (nCurrentCameraMode != SPACESHIP_CAMERA)
			/*�÷��̾��� ��ġ�� ����� �� �߰��� ������ �۾��� �����Ѵ�. ���� ���, �÷��̾��� ��ġ�� ����Ǿ����� �÷��̾� ��ü���� ����(Terrain)�� ������ ����. �÷��̾��� ���ο� ��ġ�� ��ȿ�� ��ġ�� �ƴ� ���� �ְ� �Ǵ� �÷��̾��� �浹 �˻� ���� ������ �ʿ䰡 �ִ�. �̷��� ��Ȳ���� �÷��̾��� ��ġ�� ��ȿ�� ��ġ�� �ٽ� ������ �� �ִ�.*/
			if (m_pPlayerUpdatedContext) OnPlayerUpdated(fTimeElapsed);

		/*�÷��̾ �ӵ� ���� ��ŭ �̵��Ѵ�. �ӵ� ���Ϳ� fTimeElapsed�� ���ϴ� ���� �ӵ��� �ð��� ����ϵ��� �����Ѵٴ� �ǹ��̴�.*/
		Move(m_d3dxvVelocity * fTimeElapsed, false);



		//�÷��̾��� ��ġ�� ����Ǿ����Ƿ� ī�޶��� ���¸� �����Ѵ�.
		if (nCurrentCameraMode == THIRD_PERSON_CAMERA) m_pCamera->Update(fTimeElapsed);
		//ī�޶��� ��ġ�� ����� �� �߰��� ������ �۾��� �����Ѵ�. 


		if (m_pCameraUpdatedContext) OnCameraUpdated(fTimeElapsed);
		//ī�޶� 3��Ī ī�޶��̸� ī�޶� ����� �÷��̾� ��ġ�� �ٶ󺸵��� �Ѵ�.
		if (nCurrentCameraMode == THIRD_PERSON_CAMERA)
		{
			D3DXVECTOR3 look = m_d3dxvPosition;
			look.y += 90.0f;
			m_pCamera->SetLookAt(look);//m_d3dxvPosition);
		}
		//ī�޶��� ī�޶� ��ȯ ����� �ٽ� �����Ѵ�.
		if (m_pCamera) m_pCamera->RegenerateViewMatrix();
	}
	else
	{
		if (m_pPlayerUpdatedContext) OnPlayerUpdated(fTimeElapsed);

		
		Move(m_d3dxvVelocity * fTimeElapsed, false);

	}

	/*�÷��̾��� �ӵ� ���Ͱ� ������ ������ ������ �Ǿ�� �Ѵٸ� ���� ���͸� �����Ѵ�. �ӵ� ������ �ݴ� ���� ���͸� ���ϰ� ���� ���ͷ� �����. ���� ����� �ð��� ����ϵ��� �Ͽ� �������� ���Ѵ�. ���� ���Ϳ� �������� ���Ͽ� ���� ���͸� ���Ѵ�. �ӵ� ���Ϳ� ���� ���͸� ���Ͽ� �ӵ� ���͸� ���δ�. �������� �ӷº��� ũ�� �ӷ��� 0�� �� ���̴�.*/
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
	//���ο� ī�޶��� ��忡 ���� ī�޶� ���� �����Ѵ�.
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
	/*���� ī�޶��� ��尡 �����̽�-�� ����� ī�޶��̰� ���ο� ī�޶� 1��Ī �Ǵ� 3��Ī ī�޶��̸� �÷��̾��� Up ���͸� ������ǥ���� y-�� ���� ����(0, 1, 0)�� �ǵ��� �Ѵ�. ��, �ȹٷ� ������ �Ѵ�. �׸��� �����̽�-�� ī�޶��� ��� �÷��̾��� �̵����� ������ ����. Ư��, y-�� ������ �������� �����Ӵ�. �׷��Ƿ� �÷��̾��� ��ġ�� ����(��ġ ������ y-��ǥ�� 0���� ũ��)�� �� �� �ִ�. �̶� ���ο� ī�޶� 1��Ī �Ǵ� 3��Ī ī�޶��̸� �÷��̾��� ��ġ�� ������ �Ǿ�� �Ѵ�. �׷��Ƿ� �÷��̾��� Right ���Ϳ� Look ������ y ���� 0���� �����. ���� �÷��̾��� Right ���Ϳ� Look ���ʹ� �������Ͱ� �ƴϹǷ� ����ȭ�Ѵ�.*/
	if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		m_d3dxvUp = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		m_d3dxvRight.y = 0.0f;
		m_d3dxvLook.y = 0.0f;
		D3DXVec3Normalize(&m_d3dxvRight, &m_d3dxvRight);
		D3DXVec3Normalize(&m_d3dxvLook, &m_d3dxvLook);
		m_fPitch = 0.0f;
		m_fRoll = 0.0f;
		/*Look ���Ϳ� ������ǥ���� z-��(0, 0, 1)�� �̷�� ����(����=cos)�� ����Ͽ� �÷��̾��� y-���� ȸ�� ���� m_fYaw�� �����Ѵ�.*/
		m_fYaw = (float)D3DXToDegree(acosf(D3DXVec3Dot(&D3DXVECTOR3(0.0f, 0.0f, 1.0f), &m_d3dxvLook)));
		if (m_d3dxvLook.x < 0.0f) m_fYaw = -m_fYaw;
	}
	else if ((nNewCameraMode == SPACESHIP_CAMERA) && m_pCamera)
	{
		/*���ο� ī�޶��� ��尡 �����̽�-�� ����� ī�޶��̰� ���� ī�޶� ��尡 1��Ī �Ǵ� 3��Ī ī�޶��̸� �÷��̾��� ���� ���� ���� ī�޶��� ���� ��� ���� �����.*/
		m_d3dxvRight = m_pCamera->GetRightVector();
		m_d3dxvUp = m_pCamera->GetUpVector();
		m_d3dxvLook = m_pCamera->GetLookVector();
	}

	if (pNewCamera)
	{
		//���� ī�޶� ������ ���ο� ī�޶� ���� ���̴� ������ �����Ѵ�.
		if (!m_pCamera) pNewCamera->CreateShaderVariables(pd3dDevice);
		pNewCamera->SetMode(nNewCameraMode);
		//���� ī�޶� ����ϴ� �÷��̾� ��ü�� �����Ѵ�.
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

	// �̵� �Ÿ�
	D3DXVECTOR3 dxvShift = m_d3dxvVelocity *fTimeElapsed;

	//ĳ���� xyz min,max ���ϱ�;
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


	// �ٴ� �浹 üũ
	for (int i = 0; i < FLOOR_CNT; ++i)
	{
		CDiffuseNormalVertex *mVertices = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->pVertices; //(8��)

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

	// �ǹ� �浹üũ
	for (int i = FLOOR_CNT; i < nObjects; ++i)
	{
		CDiffuseNormalVertex *mVertices = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->pVertices; //(8��)
		D3DXVECTOR3 d3dxvMax = { mVertices[1].m_d3dxvPosition.x , mVertices[4].m_d3dxvPosition.y, mVertices[2].m_d3dxvPosition.z };
		D3DXVECTOR3 d3dxvMin = { mVertices[0].m_d3dxvPosition.x , mVertices[0].m_d3dxvPosition.y,  mVertices[0].m_d3dxvPosition.z };
		bool x, y, z;
		OBJECTTAG tag = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->m_tag;
		if (true == CollisionCheck(d3dxv_cMax, d3dxv_cMin, d3dxvMax, d3dxvMin, dxvShift, x, y, z) && tag!=BOX && tag!=REALBOX)
		{
			
			// ��?
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
			//CDiffuseNormalVertex *mVertices = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->pVertices; //(8��)
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
			//CDiffuseNormalVertex *mVertices = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->pVertices; //(8��)
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
	//����� �޽��� �����Ѵ�.
	//CMesh *pAirplaneMesh = new CCharacterMesh(pd3dDevice, "Police01");
	CCubeMesh *Collision = new CCubeMesh(pd3dDevice, -15.0f, 15.0f, 0.0f, 75.0f, -15.0f, 15.0f);
	//SetMesh(pAirplaneMesh);
	pCollision = Collision;
	//�÷��̾�(�����) �޽��� �������� �� ����� ���̴��� �����Ѵ�.
	m_pShader = pShader;// new CAnimateShader();
	//m_pShader->CreateShader(pd3dDevice);
	//m_pShader->CreateShaderVariables(pd3dDevice);
	//m_pShader->BuildObjects(pd3dDevice);

	//�÷��̾ ���� ���̴� ������ �����Ѵ�.
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
		//�÷��̾��� Ư���� 1��Ī ī�޶� ��忡 �°� �����Ѵ�. �߷��� �������� �ʴ´�.
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
		//�÷��̾��� Ư���� �����̽�-�� ī�޶� ��忡 �°� �����Ѵ�. �߷��� �������� �ʴ´�.
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
		//�÷��̾��� Ư���� 3��Ī ī�޶� ��忡 �°� �����Ѵ�. ���� ȿ���� ī�޶� �������� �����Ѵ�.
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
	//ī�޶� ������ �ð��� ���� �����Ѵ�.
	Update(fTimeElapsed);
}



CNPC::CNPC(ID3D11Device *pd3dDevice, CAnimateShader *pShader, int mapNumber)
{
	m_pCamera = NULL;
	//����� �޽��� �����Ѵ�.
	//CMesh *pAirplaneMesh = new CCharacterMesh(pd3dDevice, "Police01");
	CCubeMesh *Collision = new CCubeMesh(pd3dDevice, -15.0f, 15.0f, 0.0f, 75.0f, -15.0f, 15.0f);
	//SetMesh(pAirplaneMesh);
	pCollision = new CCubeMesh(pd3dDevice, -15.0f, 15.0f, 0.0f, 75.0f, -15.0f, 15.0f);//Collision;
	//�÷��̾�(�����) �޽��� �������� �� ����� ���̴��� �����Ѵ�.
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
						//�÷��̾ ���� ���̴� ������ �����Ѵ�.
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
	// �̵� �Ÿ�
	D3DXVECTOR3 dxvShift = m_d3dxvVelocity *fTimeElapsed;

	//ĳ���� xyz min,max ���ϱ�;
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

			float vs = D3DXVec3Dot(&v1, &v2);         // �Ÿ�
			float vt = acosf(vs / (v1length * v2length));
			float vtheta = (180 * vt) / D3DX_PI;             // ����


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

		//�������� ���� ����� ����Ʈ ã��
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


		//������ ���� ���⺤�� ����
		D3DXVECTOR3 toEnemy = D3DXVECTOR3( ePosition.x - GetPosition().x, 0,  ePosition.z - GetPosition().z);
		//������������ �Ÿ�
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


		vs = D3DXVec3Dot(&v1, &v3);// �Ÿ�

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
	//		/*if (x == pos.x && y == pos.y) std::cout << "��";
	//		else if (b) 	std::cout << "��";
	//		else if (map[x][y] == 0) std::cout << "��";
	//		else if (map[x][y] == 1) std::cout << "  ";*/
	//		//if (x == dPos.x && y == dPos.y) str.append("��");
	//		//else if (ex == x && ey == y) 	str.append("��");
	//		//else if (detailmap[x][y] == 0) str.append("��");
	//		//else if (detailmap[x][y] == 1) str.append("��");



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




	// �ٴ� �浹 üũ
	for (int i = 0; i < FLOOR_CNT; ++i)
	{
		CDiffuseNormalVertex *mVertices = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->pVertices; //(8��)

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


	// �ǹ� �浹üũ
	for (int i = FLOOR_CNT; i < nObjects; ++i)
	{
		CDiffuseNormalVertex *mVertices = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->pVertices; //(8��)
		D3DXVECTOR3 d3dxvMax = { mVertices[1].m_d3dxvPosition.x , mVertices[4].m_d3dxvPosition.y, mVertices[2].m_d3dxvPosition.z };
		D3DXVECTOR3 d3dxvMin = { mVertices[0].m_d3dxvPosition.x , mVertices[0].m_d3dxvPosition.y,  mVertices[0].m_d3dxvPosition.z };
		bool x, y, z;
		OBJECTTAG tag = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->m_tag;
		if (true == CollisionCheck(d3dxv_cMax, d3dxv_cMin, d3dxvMax, d3dxvMin, dxvShift, x, y, z) && tag != BOX && tag != REALBOX)
		{

			// ��?
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
			//CDiffuseNormalVertex *mVertices = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->pVertices; //(8��)
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
			//CDiffuseNormalVertex *mVertices = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->pVertices; //(8��)
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