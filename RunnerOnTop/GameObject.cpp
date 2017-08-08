#include "stdafx.h"
#include "GameObject.h"
#include "Shader.h"

CGameObject::CGameObject()
{
	D3DXMatrixIdentity(&m_d3dxmtxWorld);
	m_pMesh = NULL;
	m_pMaterial = NULL;
	m_pTexture = NULL;
	m_pBump = NULL;
	m_pAlpha = NULL;
	transform = NULL;
	framenumber = 0;
	m_pState = NULL;
	ref = NULL;
	bInteracted = false;
	fAngeYaw = fAngePitch = fAngeRoll = 0.0f;
	collisionShader = NULL;
}

CGameObject::~CGameObject()
{
	if (m_pMesh) m_pMesh->Release();
	if (m_pMaterial) m_pMaterial->Release();
	if (m_pTexture) m_pTexture->Release();
}


void CGameObject::AddRef()
{
	m_nReferences++;
}

void CGameObject::Release()
{
	if (m_nReferences > 0) m_nReferences--;
	if (m_nReferences <= 0) delete this;
}

void CGameObject::SetMesh(CMesh *pMesh)
{
	if (m_pMesh) m_pMesh->Release();
	m_pMesh = pMesh;
	if (m_pMesh)m_pMesh->AddRef();
	
}

void CGameObject::Animate(ID3D11Device *pd3dDevice,float fTimeElapsed)
{
	//MoveForward(100.0f);
	if (m_physics.isValid == true)
	{
		D3DXMATRIX move;
		D3DXVECTOR3 vel = fTimeElapsed*m_physics.velocity;
		D3DXMatrixTranslation(&move, vel.x, vel.y, vel.z);

		//이동 거리 계산
		//move *= fTimeElapsed;

		// 이동
		ResetUpLookRight();
		m_d3dxmtxWorld = move * m_d3dxmtxWorld;
		D3DXMATRIX mtxRotate;
		rotateValue += 0.5f;
		
		D3DXMatrixRotationYawPitchRoll(&mtxRotate, 0.0f, rotateValue, 0.0f);

		m_d3dxmtxWorld = mtxRotate * m_d3dxmtxWorld;

		// 다음 이동 준비
		float weight = m_physics.weight;
		float xfrict = m_physics.friction;
		float zfrict = m_physics.friction;

		if (m_physics.velocity.x < 0)
		{
			xfrict *= -1.0f;
		}

		if (m_physics.velocity.z < 0)
		{
			zfrict *= -1.0f;
		}

		m_physics.velocity -= weight * D3DXVECTOR3(xfrict, m_physics.gravity, zfrict);
		D3DXVECTOR3 pos = { 0.0f, 0.0f ,0.0f };// GetPosition();
		D3DXVECTOR4 mPos;
		D3DXVec3Transform(&mPos, &pos, &m_d3dxmtxWorld);
		pos = { mPos.x, mPos.y, mPos.z };
		//충돌체크
		if (collisionShader)
		{
			CDiffusedShader *pShader = (CDiffusedShader *)collisionShader;
			int floor = pShader->FLOOR_CNT;
			for (int i = 0; i < floor; ++i)
			{
				CDiffuseNormalVertex *mVertices = ((CCubeMesh*)pShader->m_ppObjects[i]->m_pMesh)->pVertices; //(8개)

				D3DXVECTOR3 d3dxvMax = { mVertices[1].m_d3dxvPosition.x , mVertices[4].m_d3dxvPosition.y, mVertices[2].m_d3dxvPosition.z };
				D3DXVECTOR3 d3dxvMin = { mVertices[0].m_d3dxvPosition.x , mVertices[0].m_d3dxvPosition.y,  mVertices[0].m_d3dxvPosition.z };


				D3DXVECTOR3 max = { 15.0f, 15.0f, 30.0f };
				D3DXVECTOR3 min = { -15.0f, -15.0f, 0.0f };
				
				bool x, y, z;
				if (CollisionCheck(pos + max, pos + min, d3dxvMax, d3dxvMin, fTimeElapsed * m_physics.velocity, x, y, z))
				{
					std::cout << "충돌 : " << i << std::endl;
					std::cout << "Pos : " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
					pos = fTimeElapsed * m_physics.velocity;
					std::cout<< "Vec : " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
					ResetUpLookRight();
					if (y) m_physics.isValid = false;
					break;
				}
			}

		}
		
	}
}

void CGameObject::SetPosition(float x, float y, float z)
{
	m_d3dxmtxWorld._41 = x;
	m_d3dxmtxWorld._42 = y;
	m_d3dxmtxWorld._43 = z;
}

void CGameObject::SetPosition(D3DXVECTOR3 d3dxvPosition)
{
	m_d3dxmtxWorld._41 = d3dxvPosition.x;
	m_d3dxmtxWorld._42 = d3dxvPosition.y;
	m_d3dxmtxWorld._43 = d3dxvPosition.z;
}
D3DXVECTOR3 CGameObject::GetPosition()
{
	return(D3DXVECTOR3(m_d3dxmtxWorld._41, m_d3dxmtxWorld._42, m_d3dxmtxWorld._43));
}

D3DXVECTOR3 CGameObject::GetLookAt()
{
	D3DXVECTOR3 d3dxvLookAt(m_d3dxmtxWorld._31, m_d3dxmtxWorld._32, m_d3dxmtxWorld._33);
	D3DXVec3Normalize(&d3dxvLookAt, &d3dxvLookAt);
	return(d3dxvLookAt);
}

D3DXVECTOR3 CGameObject::GetUp()
{
	D3DXVECTOR3 d3dxvUp(m_d3dxmtxWorld._21, m_d3dxmtxWorld._22, m_d3dxmtxWorld._23);
	D3DXVec3Normalize(&d3dxvUp, &d3dxvUp);
	return(d3dxvUp);
}

D3DXVECTOR3 CGameObject::GetRight()
{
	D3DXVECTOR3 d3dxvRight(m_d3dxmtxWorld._11, m_d3dxmtxWorld._12, m_d3dxmtxWorld._13);
	D3DXVec3Normalize(&d3dxvRight, &d3dxvRight);
	return(d3dxvRight);
}

void CGameObject::ResetUpLookRight()
{
	D3DXVECTOR3 m_d3dxvRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_d3dxvUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 m_d3dxvLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 m_d3dxvPosition = GetPosition();
	
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

void CGameObject::MoveStrafe(float fDistance)
{
	D3DXVECTOR3 d3dxvPosition = GetPosition();
	D3DXVECTOR3 d3dxvRight = GetRight();
	d3dxvPosition += fDistance * d3dxvRight;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::MoveUp(float fDistance)
{
	D3DXVECTOR3 d3dxvPosition = GetPosition();
	D3DXVECTOR3 d3dxvUp = GetUp();
	d3dxvPosition += fDistance * d3dxvUp;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::MoveForward(float fDistance)
{
	D3DXVECTOR3 d3dxvPosition = GetPosition();
	D3DXVECTOR3 d3dxvLookAt = GetLookAt();
	d3dxvPosition += fDistance * d3dxvLookAt;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationYawPitchRoll(&mtxRotate, (float)D3DXToRadian(fYaw), (float)D3DXToRadian(fPitch), (float)D3DXToRadian(fRoll));
	m_d3dxmtxWorld = mtxRotate * m_d3dxmtxWorld;
}

void CGameObject::Rotate(D3DXVECTOR3 *pd3dxvAxis, float fAngle)
{
	D3DXMATRIX mtxRotate;
	if (pd3dxvAxis->x == 1.0f) fAngePitch += fAngle;
	if (pd3dxvAxis->y == 1.0f) fAngeRoll += fAngle;
	if (pd3dxvAxis->z == 1.0f) fAngeYaw += fAngle;

	D3DXMatrixRotationAxis(&mtxRotate, pd3dxvAxis, (float)D3DXToRadian(fAngle));
	m_d3dxmtxWorld = mtxRotate * m_d3dxmtxWorld;
}




void CGameObject::SetMaterial(CMaterial *pMaterial)
{
	if (m_pMaterial) m_pMaterial->Release();
	m_pMaterial = pMaterial;
	if (m_pMaterial) m_pMaterial->AddRef();
}

void CGameObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	if (m_pMesh)m_pMesh->Render(pd3dDeviceContext);
	/*if (!transform)
	{
		transform = m_pMesh->getTransform(framenumber++);
	}*/
}

void CGameObject::SetTexture(CTexture *pTexture)
{
	if (m_pTexture) m_pTexture->Release();
	m_pTexture = pTexture;
	if (m_pTexture) m_pTexture->AddRef();
}

void CGameObject::SetBump(CTexture * pBump)
{
	if (m_pBump) m_pBump->Release();
	m_pBump = pBump;
	if (m_pBump) m_pBump->AddRef();
}

void CGameObject::SetAlphaMap(CTexture * pAlpha)
{
	if (m_pAlpha) m_pAlpha->Release();
	m_pAlpha = pAlpha;
	if (m_pAlpha) m_pAlpha->AddRef();
}




CRotatingObject::CRotatingObject()
{
	m_fRotationSpeed = 15.0f;
}
CRotatingObject::~CRotatingObject()
{
}
void CRotatingObject::Animate(ID3D11Device *pd3dDevice,float fTimeElapsed)
{
	//Rotate(&m_d3dxvRotationAxis, m_fRotationSpeed);

}

void CRotatingObject::Render(ID3D11DeviceContext *pd3dDeviceContext,CCamera *pCamera)
{
	CGameObject::Render(pd3dDeviceContext, pCamera);
}



CMaterial::CMaterial()
{
	m_nReferences = 0;
}

CMaterial::~CMaterial()
{
}

void CMaterial::AddRef()
{
	m_nReferences++;
}

void CMaterial::Release()
{
	if (m_nReferences > 0) m_nReferences--;
	if (m_nReferences == 0) delete this;
}

CTexture::CTexture(int nTextures)
{
	m_nReferences = 0;
	m_nTextures = nTextures;
	m_ppd3dsrvTextures = new ID3D11ShaderResourceView*[m_nTextures];
	for (int i = 0; i < m_nTextures; i++) m_ppd3dsrvTextures[i] = NULL;
	m_ppd3dSamplerStates = new ID3D11SamplerState*[m_nTextures];
	for (int i = 0; i < m_nTextures; i++) m_ppd3dSamplerStates[i] = NULL;
}

CTexture::~CTexture()
{
	if (m_ppd3dsrvTextures) delete[] m_ppd3dsrvTextures;
	if (m_ppd3dSamplerStates) delete[] m_ppd3dSamplerStates;
}

void CTexture::AddRef()
{
	m_nReferences++;
}

void CTexture::Release()
{
	if (m_nReferences > 0) m_nReferences--;
	for (int i = 0; i < m_nTextures; i++)
	{
		if (m_ppd3dsrvTextures[i]) m_ppd3dsrvTextures[i]->Release();
		if (m_ppd3dSamplerStates[i]) m_ppd3dSamplerStates[i]->Release();
	}
	if (m_nReferences == 0) delete this;
}

void CTexture::SetTexture(int nIndex, ID3D11ShaderResourceView *pd3dsrvTexture, ID3D11SamplerState *pd3dSamplerState)
{
	if (m_ppd3dsrvTextures[nIndex]) m_ppd3dsrvTextures[nIndex]->Release();
	if (m_ppd3dSamplerStates[nIndex]) m_ppd3dSamplerStates[nIndex]->Release();
	m_ppd3dsrvTextures[nIndex] = pd3dsrvTexture;
	m_ppd3dSamplerStates[nIndex] = pd3dSamplerState;
	if (m_ppd3dsrvTextures[nIndex]) m_ppd3dsrvTextures[nIndex]->AddRef();
	if (m_ppd3dSamplerStates[nIndex]) m_ppd3dSamplerStates[nIndex]->AddRef();
}

CPhysics::CPhysics()
{
	isValid = false;
	velocity = { 0.0, 0.0, 0.0 };
	powersource = { 0.0, 0.0, 0.0 };
	gravity = 9.8f;
	weight = 2.0f;
	friction = 3.0f;
}
