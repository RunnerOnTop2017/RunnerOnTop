#include "stdafx.h"
#include "Shader.h"
#include "Player.h"


CShader::CShader()
{
	m_pd3dVertexShader = NULL;
	m_pd3dPixelShader = NULL;
	m_pd3dVertexLayout = NULL;
	m_pd3dcbWorldMatrix = NULL;
	m_pd3dcbMaterial = NULL;
	m_isAlphaBlend = NULL;
	m_ppObjects = NULL;
	m_nObjects = 0;

}


CShader::~CShader()
{
	if (m_pd3dVertexShader) m_pd3dVertexShader->Release();
	if (m_pd3dPixelShader) m_pd3dPixelShader->Release();
	if (m_pd3dVertexLayout) m_pd3dVertexLayout->Release();

}

void CShader::CreateVertexShaderFromFile(ID3D11Device *pd3dDevice,
	WCHAR *pszFileName,
	LPCSTR pszShaderName,
	LPCSTR pszShaderModel,
	ID3D11VertexShader **ppd3dVertexShader,
	D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout,
	UINT nElements,
	ID3D11InputLayout **ppd3dVertexLayout)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;

	//파일(pszFileName)에서 쉐이더함수(pszShader Name)을 컴파일 하여 컴파일된 쉐이더 코드의 메모리 주소(pd3dShaderBlob)를 반환한다.
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel,
		dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		// 컴파일된 쉐이더 코드의 메모리 주소에서 정점-쉐이더를 생성한다.
		pd3dDevice->CreateVertexShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dVertexShader);
		// 컴파일된 쉐이더 코드의 메모리 주소와 입력 레이아웃에서 정점 레이아웃을 생성한다.
		pd3dDevice->CreateInputLayout(pd3dInputLayout, nElements, pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), ppd3dVertexLayout);
		pd3dShaderBlob->Release();


	}
}

void CShader::CreatePixelShaderFromFile(ID3D11Device *pd3dDevice,
	WCHAR *pszFileName,
	LPCSTR pszShaderName,
	LPCSTR pszShaderModel,
	ID3D11PixelShader **ppd3dPixelShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/*파일(pszFileName)에서 쉐이더 함수(pszShaderName)를 컴파일하여 컴파일된 쉐이더 코드의 메모리 주소(pd3dShaderBlob)를 반환한다.*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel,
		dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		// 컴파일된 쉐이더 코드의 메모리 주소에서 픽셀 - 쉐이더를 생성한다.
		pd3dDevice->CreatePixelShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dPixelShader);
		pd3dShaderBlob->Release();

	}
}

void CShader::BuildObjects(ID3D11Device *pd3dDevice)
{
}

void CShader::ReleaseObjects()
{
	if (m_pd3dcbWorldMatrix) m_pd3dcbWorldMatrix->Release();
	if (m_pd3dcbMaterial) m_pd3dcbMaterial->Release();

	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) delete m_ppObjects[j];
		delete[] m_ppObjects;
	}
}

void CShader::AnimateObjects(ID3D11Device *pd3dDevice, float fTimeElapsed)
{
	for (int j = 0; j < m_nObjects; j++)
	{
		m_ppObjects[j]->Animate(pd3dDevice, fTimeElapsed);
	}
}

void CShader::CreateShader(ID3D11Device *pd3dDevice)
{
}


void CShader::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(VS_CB_WORLD_MATRIX);
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbWorldMatrix);


	D3D11_BUFFER_DESC d3dBufferDesc3;
	ZeroMemory(&d3dBufferDesc3, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc3.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc3.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc3.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc3.ByteWidth = sizeof(MATERIAL);
	pd3dDevice->CreateBuffer(&d3dBufferDesc3, NULL, &m_pd3dcbMaterial);


	D3D11_BUFFER_DESC d3dBufferDesc4;
	ZeroMemory(&d3dBufferDesc4, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc4.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc4.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc4.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc4.ByteWidth = sizeof(VS_CB_ISALPHABLEND);
	pd3dDevice->CreateBuffer(&d3dBufferDesc4, NULL, &m_isAlphaBlend);
	
}


void CShader::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbWorldMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_WORLD_MATRIX *pcbWorldMatrix = (VS_CB_WORLD_MATRIX *)d3dMappedResource.pData;
	D3DXMatrixTranspose(&pcbWorldMatrix->m_d3dxmtxWorld, pd3dxmtxWorld);
	pd3dDeviceContext->Unmap(m_pd3dcbWorldMatrix, 0);
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_WORLD_MATRIX, 1, &m_pd3dcbWorldMatrix);
}

void CShader::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, MATERIAL *pMaterial)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	MATERIAL *pcbMaterial = (MATERIAL *)d3dMappedResource.pData;
	memcpy(pcbMaterial, pMaterial, sizeof(MATERIAL));
	pd3dDeviceContext->Unmap(m_pd3dcbMaterial, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_SLOT_MATERIAL, 1, &m_pd3dcbMaterial);
}

void CShader::UpdateShaderVariables(ID3D11DeviceContext * pd3dDeviceContext, CTexture * pTexture)
{
}

void CShader::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dTransform, int AnimationSize)
{
}

void CShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera, int index)
{
	pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);
	if (index == -1)
	{
		for (int j = 0; j < m_nObjects; j++)
		{
			if (m_ppObjects[j])
			{
				if (m_ppObjects[j]->m_pMesh->m_tag == REALBOX || m_ppObjects[j]->m_pMesh->m_tag == BOX)
				{
					UpdateShaderVariables(pd3dDeviceContext, &m_ppObjects[j]->ref->m_d3dxmtxWorld);

				}
				else
				{

					UpdateShaderVariables(pd3dDeviceContext, &m_ppObjects[j]->m_d3dxmtxWorld);
				}
				//객체의 물질 정보를 쉐이더 프로그램으로 전달한다.
				if (m_ppObjects[j]->m_pMaterial) UpdateShaderVariables(pd3dDeviceContext, &m_ppObjects[j]->m_pMaterial->m_Material);
				if (m_ppObjects[j]->m_pTexture)
				{
					UpdateShaderVariables(pd3dDeviceContext, m_ppObjects[j]->m_pTexture);
				}
				if (m_ppObjects[j]->m_pBump)
				{
					pd3dDeviceContext->PSSetShaderResources(0x09, m_ppObjects[j]->m_pBump->m_nTextures, m_ppObjects[j]->m_pBump->m_ppd3dsrvTextures);
					pd3dDeviceContext->PSSetSamplers(0x01, 1, &m_ppObjects[j]->m_pBump->m_ppd3dSamplerStates[0]);
				}

				if (m_ppObjects[j]->m_pAlpha)
				{
					pd3dDeviceContext->PSSetShaderResources(0x09, m_ppObjects[j]->m_pAlpha->m_nTextures, m_ppObjects[j]->m_pAlpha->m_ppd3dsrvTextures);
					pd3dDeviceContext->PSSetSamplers(0x02, 1, &m_ppObjects[j]->m_pAlpha->m_ppd3dSamplerStates[0]);

				}


				if (m_ppObjects[j]->m_pState)
				{

					m_ppObjects[j]->transform = m_ppObjects[j]->m_pState->GetAnimation();
					UpdateShaderVariables(pd3dDeviceContext, m_ppObjects[j]->transform, m_ppObjects[j]->m_pState->GetBoneSize());
				}
				m_ppObjects[j]->Render(pd3dDeviceContext);
			}
		}
	}
	else
	{
		if (m_ppObjects[index])
		{

			UpdateShaderVariables(pd3dDeviceContext, &m_ppObjects[index]->m_d3dxmtxWorld);

			//객체의 물질 정보를 쉐이더 프로그램으로 전달한다.
			if (m_ppObjects[index]->m_pMaterial) UpdateShaderVariables(pd3dDeviceContext, &m_ppObjects[index]->m_pMaterial->m_Material);
			if (m_ppObjects[index]->m_pTexture)
			{
				UpdateShaderVariables(pd3dDeviceContext, m_ppObjects[index]->m_pTexture);
			}
			if (m_ppObjects[index]->m_pBump)
			{
				pd3dDeviceContext->PSSetShaderResources(0x09, m_ppObjects[index]->m_pBump->m_nTextures, m_ppObjects[index]->m_pBump->m_ppd3dsrvTextures);
				pd3dDeviceContext->PSSetSamplers(0x01, 1, &m_ppObjects[index]->m_pBump->m_ppd3dSamplerStates[0]);
			}

			if (m_ppObjects[index]->m_pAlpha)
			{
				pd3dDeviceContext->PSSetShaderResources(0x09, m_ppObjects[index]->m_pAlpha->m_nTextures, m_ppObjects[index]->m_pAlpha->m_ppd3dsrvTextures);
				pd3dDeviceContext->PSSetSamplers(0x02, 1, &m_ppObjects[index]->m_pAlpha->m_ppd3dSamplerStates[0]);

			}


			if (m_ppObjects[index]->m_pState)
			{

				m_ppObjects[index]->transform = m_ppObjects[index]->m_pState->GetAnimation();
				UpdateShaderVariables(pd3dDeviceContext, m_ppObjects[index]->transform, m_ppObjects[index]->m_pState->GetBoneSize());
			}
			m_ppObjects[index]->Render(pd3dDeviceContext);
		}
	}

}

CGameObject * CShader::GetGameObject(int index)
{
	return m_ppObjects[index];
}


CDiffusedShader::CDiffusedShader()
{
	OBJECT_CNT = 81;
	FLOOR_CNT = 30;
	WALL_CNT = 60;
}

CDiffusedShader::~CDiffusedShader()
{
}

void CDiffusedShader::CreateShader(ID3D11Device *pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR0", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"Effect.fx", "VSLightingColor", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"Effect.fx", "PSLightingColor", "ps_4_0", &m_pd3dPixelShader);
}

void CDiffusedShader::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	CShader::CreateShaderVariables(pd3dDevice);
}

void CDiffusedShader::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld)
{
	CShader::UpdateShaderVariables(pd3dDeviceContext, pd3dxmtxWorld);
}

void CDiffusedShader::BuildObjects(ID3D11Device *pd3dDevice, int mapNumber)
{
	if (mapNumber == 1)
	{
		OBJECT_CNT = 91;
		FLOOR_CNT = 30;
		WALL_CNT = 60;
		m_nObjects = OBJECT_CNT;
		BuildMap1(pd3dDevice);
	}
	else
	{
		OBJECT_CNT = 250;
		FLOOR_CNT = 146;
		WALL_CNT = 158;
		m_nObjects = OBJECT_CNT;
		BuildMap2(pd3dDevice);
	}

	CreateShaderVariables(pd3dDevice);
}

void CDiffusedShader::BuildMap1(ID3D11Device *pd3dDevice)
{
	m_ppObjects = new CGameObject*[m_nObjects];
	int n = 0;
	CGameObject *pRotatingObject = new CGameObject();
	CMesh *pCubeMesh = new CCubeMesh(pd3dDevice, -3000.0f, 3000.0f, 0.0f, 3050.0f, 0.0f, 3800.0f, FALL);

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -3000.0f, 3000.0f, 0.0f, 2580.0f, -3800.0f, 0.0f, FALL);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -55.0f, 590.0f, 3050.0f, 3275.0f, 3300.0f, 3605.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -55.0f, 590.0f, 3050.0f, 3275.0f, 2980.0f, 3285.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 190.0f, 605.0f, 3050.0f, 3275.0f, 2660.0f, 2963.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;


	pCubeMesh = new CCubeMesh(pd3dDevice, -55.0f, 595.0f, 3050.0f, 3275.0f, 1705.0f, 2025.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -55.0f, 595.0f, 3050.0f, 3275.0f, 2025.0f, 2333.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -55.0f, 595.0f, 3050.0f, 3275.0f, 2333.0f, 2653.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 210.0f, 860.0f, 3038.0f, 3263.0f, 1435.0f, 1700.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 880.0f, 1300.0f, 3038.0f, 3263.0f, 1435.0f, 1735.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1340.0f, 1990.0f, 3038.0f, 3263.0f, 1435.0f, 1735.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	// 경찰 출발하는 곳
	pCubeMesh = new CCubeMesh(pd3dDevice, 1340.0f, 1990.0f, 3043.0f, 3268.0f, 1735.0f, 2360.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1340.0f, 1990.0f, 3040.0f, 3265.0f, 2360.0f, 2660.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1340.0f, 1990.0f, 3033.0f, 3258.0f, 2660.0f, 2980.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1340.0f, 1990.0f, 3028.0f, 3253.0f, 2980.0f, 3605.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 625.0f, 1280.0f, 3005.0f, 3230.0f, 1120.0f, 1430.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 625.0f, 1280.0f, 2963.0f, 3188.0f, 810.0f, 1120.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;


	pCubeMesh = new CCubeMesh(pd3dDevice, 625.0f, 1280.0f, 2918.0f, 3143.0f, 490.0f, 810.0f);//index =  15
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 625.0f, 1280.0f, 2883.0f, 3108.f, 173.0f, 490.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 625.0f, 1280.0f, 2853.0f, 3078.0f, -145.0f, 173.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 625.0f, 1280.0f, 2813.0f, 3028.f, -460.0f, -145.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 650.0f, 1295.0f, 2755.0f, 2980.f, -770.0f, -460.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1340.0f, 1990.0f, 2755.0f, 2980.f, -770.0f, -460.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1340.0f, 1990.0f, 2698.0f, 2923.f, -1090.0f, -770.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1340.0f, 1990.0f, 2643.0f, 2868.f, -1400.0f, -1090.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1340.0f, 1990.0f, 2598.0f, 2823.f, -1725.0f, -1400.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1340.0f, 1990.0f, 2553.0f, 2778.f, -2045.0f, -1725.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1340.0f, 1990.0f, 2518.0f, 2743.f, -2355.0f, -2045.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1340.0f, 1990.0f, 2498.0f, 2723.f, -2675.0f, -2355.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -870.0f, 400.0f, 3035.0f, 3260.0f, 770.0f, 1360.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	// 건물
	pCubeMesh = new CCubeMesh(pd3dDevice, -35.0f, 130.0f, 3275.0f, 3321.0f, 3325.0f, 3415.0f, OBJ);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 635.0f, 810.f, 3230.0f, 3278.0f, 1340.0f, 1430.0f, OBJ);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1115.0f, 1290.0f, 3188.0f, 3234.0f, 810.0f, 900.0f, OBJ); // index = 30
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 635.0f, 810.0f, 3143.0f, 3189.0f, 720.0f, 810.0f, OBJ);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 635.0f, 810.0f, 3078.0f, 3124.0f, 83.0f, 173.0f, OBJ);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 635.0f, 810.0f, 3028.f, 3074.0f, -460.0f, -350.0f, OBJ);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1780.0f, 1955.0f, 2980.f, 3026.0f, -550.0f, -460.0f, OBJ);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1345.0f, 1520.0f, 2923.f, 2969.0f, -870.0f, -780.0f, OBJ);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1345.0f, 1520.0f, 2778.f, 2824.0f, -1825.0f, -1735.0f, OBJ);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1845.0f, 2020.0f, 2743.f, 2789.0f, -2355.0f, -2265.0f, OBJ);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1345.0f, 1520.0f, 2723.f, 2769.0f, -2460.0f, -2370.0f, OBJ);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 670.0f, 1325.0f, 3140.0f, 3365.0f, 2980.0f, 3605.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 710.0f, 1220.0f, 3140.0f, 3435.0f, 2718.0f, 2955.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 670.0f, 1325.0f, 3140.0f, 3365.0f, 1780.0f, 2710.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -55.0f, 148.0f, 3275.0f, 3370.0f, 2370.0f, 2600.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	//오늘
	pCubeMesh = new CCubeMesh(pd3dDevice, -1380.0f, -115.0f, 3000.0f, 3380.0f, 2975.0f, 3570.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -310.0f, 155.0f, 3000.0f, 3435.0f, 2715.0f, 2935.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -1380.0f, -115.0f, 3000.0f, 3530.0f, 2060.0f, 2653.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -1380.0f, -115.0f, 3000.0f, 3445.0f, 1405.0f, 1993.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	// 경찰 장애물
	pCubeMesh = new CCubeMesh(pd3dDevice, 1345.0f, 1520.0f, 3267.0f, 3314.0f, 1960.0f, 2040.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1810.0f, 1985.0f, 3267.0f, 3314.0f, 2270.0f, 2350.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1345.0f, 1520.0f, 3267.0f, 3317.0f, 2360.0f, 2450.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1345.0f, 1520.0f, 3252.0f, 3299.0f, 3190.0f, 3280.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1400.0f, 1905.0f, 3000.0f, 3430.0f, 1175.0f, 1405.f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1360.0f, 2040.0f, 3000.0f, 3350.0f, 110.0f, 700.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1400.0f, 1900.0f, 2900.0f, 3220.0f, -420.0f, -190.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -30.0f, 1245.0f, 2500.0f, 3103.0f, -1415.0f, -825.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -700.0f, 570.0f, 2500.0f, 3360.0f, 100.0f, 700.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1240.0f, 1900.0f, 2500.0f, 2820.0f, -3000.0f, -2700.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 690.0f, 1205.0f, 2500.0f, 2950.0f, -3000.0f, -2700.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	// 첫번째 문 인터랙트 영역
	pCubeMesh = new CCubeMesh(pd3dDevice, 270.0f, 330.0f, 3275.0f, 3360.0f, 3140.0f, 3180.0f, DOOR); // index = 60
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	// 첫번째 문 
	pCubeMesh = new CCubeMesh(pd3dDevice, 270.0f, 330.0f, 3275.0f, 3360.0f, 3100.0f, 3110.0f, REALDOOR); // index = 61
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;


	// 첫번째 문 옆 펜스들
	pCubeMesh = new CCubeMesh(pd3dDevice, 0.0f, 260.0f, 3275.0f, 3360.0f, 3100.0f, 3110.0f, FENCE);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 340.0f, 600.0f, 3275.0f, 3360.0f, 3100.0f, 3110.0f, FENCE);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;


	//컨디셔너 인터랙트 영역
	pCubeMesh = new CCubeMesh(pd3dDevice, 180, 600.0f, 3275.0f, 3360.0f, 2400.0f, 2600.0f, CONDITIONER);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	//구멍뚫린 펜스 인터랙티브 영역

	//앞
	pCubeMesh = new CCubeMesh(pd3dDevice, 235.0f, 275.0f, 3275.0f, 3360.0f, 2020.0f, 2050.0f, FENCEHOLE);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	//뒤
	pCubeMesh = new CCubeMesh(pd3dDevice, 235.0f, 275.0f, 3275.0f, 3360.0f, 1950.0f, 1980.0f, FENCEHOLE);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	m_ppObjects[n - 1]->ref = m_ppObjects[n - 2];
	m_ppObjects[n - 2]->ref = m_ppObjects[n - 1];



	//구멍뚫린 펜스 막힌 영역
	pCubeMesh = new CCubeMesh(pd3dDevice, 190.0f, 320.0f, 3275.0f, 3360.0f, 1990.0f, 2000.0f, FENCE);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	//그 오른쪽
	pCubeMesh = new CCubeMesh(pd3dDevice, 60.0f, 190.0f, 3275.0f, 3360.0f, 1990.0f, 2000.0f, FENCE);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	// 그 왼쪽
	pCubeMesh = new CCubeMesh(pd3dDevice, 320.0f, 580.0f, 3275.0f, 3360.0f, 1990.0f, 2000.0f, FENCE);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	// 문 뒤 파이프
	pCubeMesh = new CCubeMesh(pd3dDevice, 240.0f, 360.0f, 3275.0f, 3285.0f, 2995.0f, 3005.0f, PIPE); // 70
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	//점프 영역
	pCubeMesh = new CCubeMesh(pd3dDevice, 1300, 1335.0f, 2900.0f, 3050.0f, -750.0f, -450.0f, JUMPAREA);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	//문
	pCubeMesh = new CCubeMesh(pd3dDevice, 1570.0f, 1630.0f, 2985.0f, 3070.0f, -750.0f, -740.0f, REALDOOR);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1570.0f, 1630.0f, 2985.0f, 3070.0f, -710.0f, -670.0f, DOOR);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	//문옆
	pCubeMesh = new CCubeMesh(pd3dDevice, 1630.0f, 1890.0f, 2985.0f, 3070.0f, -750.0f, -740.0f, FENCE);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1310.0f, 1570.0f, 2985.0f, 3070.0f, -750.0f, -740.0f, FENCE);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;



	//골인지점
	pCubeMesh = new CCubeMesh(pd3dDevice, 1550, 1850.0f, 2700.0f, 2800.0f, -2600.0f, -2500.0f, GOAL);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//펜스
	pCubeMesh = new CCubeMesh(pd3dDevice, 920.0f, 990.0f, 3230.0f, 3315.0f, 1140.0f, 1180.0f, FENCEHOLE); // 77
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	//구멍뚫린 펜스 막힌 영역
	pCubeMesh = new CCubeMesh(pd3dDevice, 890.0f, 1020.0f, 3230.0f, 3315.0f, 1120.0f, 1130.0f, FENCE);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	// 그 옆들
	pCubeMesh = new CCubeMesh(pd3dDevice, 1020.0f, 1280.0f, 3230.0f, 3315.0f, 1120.0f, 1130.0f, FENCE);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 630.0f, 890.0f, 3230.0f, 3315.0f, 1120.0f, 1130.0f, FENCE); //80
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	//박스
	pCubeMesh = new CCubeMesh(pd3dDevice, -50.0f, 50.0f, -50.0f, 50.0f, -25.0f, 75.0f, BOX); // 81
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -15.0f, 15.0f, -15.0f, 15.0f, 0.0f, 30.0f, REALBOX); //82
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	// 두번째 박스
	pCubeMesh = new CCubeMesh(pd3dDevice, -50.0f, 50.0f, -50.0f, 50.0f, -25.0f, 75.0f, BOX); // 83
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -15.0f, 15.0f, -15.0f, 15.0f, 0.0f, 30.0f, REALBOX); //84
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	// 파이프

	pCubeMesh = new CCubeMesh(pd3dDevice, 798.0f, 808.0f, 3258.0f, 3268.0f, 1499.0f, 1619.0f, PIPE); //85
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 891.0f, 1011.0f, 3073.0f, 3083.0f, -119.0f, -109.0f, PIPE); //86
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;



	// 철조망

	pCubeMesh = new CCubeMesh(pd3dDevice, 1532.5f, 1662.5f, 2823.0f, 2908.0f, -1675.0f, -1665.0f, FENCE); // 87
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	pCubeMesh = new CCubeMesh(pd3dDevice, 1402.5f, 1532.5f, 2823.0f, 2908.0f, -1675.0f, -1665.0f, FENCE); // 88
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1662.5f, 1922.5f, 2823.0f, 2908.0f, -1675.0f, -1665.0f, FENCE); // 89
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	//뚫린 철조망

	pCubeMesh = new CCubeMesh(pd3dDevice, 1562.5f, 1632.5f, 2823.0f, 2908.0f, -1660.0f, -1630.0f, FENCEHOLE); // 90
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


}


 void CDiffusedShader::BuildMap2(ID3D11Device * pd3dDevice)
 {
	m_ppObjects = new CGameObject*[m_nObjects];
	int n = 0;
	{
		// 0
		CGameObject *pRotatingObject = new CGameObject();
		CMesh *pCubeMesh = new CCubeMesh(pd3dDevice, -5000.0f, 5000.0f, 0.0f, 3100.0f, -13000.0f, 5000.0f, FALL);

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 1
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3290.0f, 2970.0f, 3790.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 1-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, -370.0f, 2280.0f, 3310.0f, 2970.0f, 3790.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 1-2
		pCubeMesh = new CCubeMesh(pd3dDevice, 315.0f, 345.0f, 2280.0f, 3310.0f, 2970.0f, 3790.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 1-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3310.0f, 2940.0f, 2970.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 1-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3310.0f, 3780.0f, 3810.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 2
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3290.0f, 2970.0f, 3790.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 2-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -1175.0f, 2280.0f, 3310.0f, 2970.0f, 3790.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 2-2
		pCubeMesh = new CCubeMesh(pd3dDevice, -490.0f, -460.0f, 2280.0f, 3310.0f, 2970.0f, 3790.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 2-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3310.0f, 2940.0f, 2970.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 2-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3310.0f, 3780.0f, 3810.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 3
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3290.0f, 2970.0f, 3790.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 3-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1975.0f, 2280.0f, 3310.0f, 2970.0f, 3790.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 3-2
		pCubeMesh = new CCubeMesh(pd3dDevice, -1295.0f, -1265.0f, 2280.0f, 3310.0f, 2970.0f, 3790.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 3-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, 2940.0f, 2970.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 3-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, 3780.0f, 3810.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 4
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3290.0f, 2085.0f, 2925.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 4-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1975.0f, 2280.0f, 3310.0f, 2085.0f, 2925.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 4-2
		pCubeMesh = new CCubeMesh(pd3dDevice, -1295.0f, -1265.0f, 2280.0f, 3310.0f, 2085.0f, 2925.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 4-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, 2055.0f, 2085.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 4-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, 2900.0f, 2930.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 5
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3290.0f, 1200.0f, 2040.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 5-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1975.0f, 2280.0f, 3310.0f, 1200.0f, 2040.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 5-2
		pCubeMesh = new CCubeMesh(pd3dDevice, -1295.0f, -1265.0f, 2280.0f, 3310.0f, 1200.0f, 2040.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 5-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, 1170.0f, 1200.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 5-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, 2015.0f, 2045.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 6
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3290.0f, 1200.0f, 2040.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 6-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -1175.0f, 2280.0f, 3310.0f, 1200.0f, 2040.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 6-2
		pCubeMesh = new CCubeMesh(pd3dDevice, -490.0f, -460.0f, 2280.0f, 3310.0f, 1200.0f, 2040.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 6-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3310.0f, 1170.0f, 1200.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 6-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3310.0f, 2015.0f, 2045.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 7
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3290.0f, 1200.0f, 2040.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 7-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, -370.0f, 2280.0f, 3310.0f, 1200.0f, 2040.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 7-2
		pCubeMesh = new CCubeMesh(pd3dDevice, 315.0f, 345.0f, 2280.0f, 3310.0f, 1200.0f, 2040.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 7-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3310.0f, 1170.0f, 1200.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 7-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3310.0f, 2015.0f, 2045.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 8
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3290.0f, 315.0f, 1155.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 8-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, -370.0f, 2280.0f, 3310.0f, 315.0f, 1155.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 8-2
		pCubeMesh = new CCubeMesh(pd3dDevice, 315.0f, 345.0f, 2280.0f, 3310.0f, 315.0f, 1155.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 8-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3310.0f, 280.0f, 310.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 8-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3310.0f, 1135.0f, 1165.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 9
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3290.0f, -570.0f, 270.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 9-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, -370.0f, 2280.0f, 3310.0f, -570.0f, 270.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 9-2
		pCubeMesh = new CCubeMesh(pd3dDevice, 315.0f, 345.0f, 2280.0f, 3310.0f, -570.0f, 270.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 9-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3310.0f, -600.0f, -570.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 9-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3310.0f, 245.0f, 275.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 10
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3290.0f, -570.0f, 270.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 10-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -1175.0f, 2280.0f, 3310.0f, -570.0f, 270.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 10-2
		pCubeMesh = new CCubeMesh(pd3dDevice, -490.0f, -460.0f, 2280.0f, 3310.0f, -570.0f, 270.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 10-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3310.0f, -600.0f, -570.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 10-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3310.0f, 245.0f, 275.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 11
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3290.0f, -570.0f, 270.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 11-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1975.0f, 2280.0f, 3310.0f, -570.0f, 270.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 11-2
		pCubeMesh = new CCubeMesh(pd3dDevice, -1295.0f, -1265.0f, 2280.0f, 3310.0f, -570.0f, 270.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 11-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, -600.0f, -570.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 11-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, 245.0f, 275.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 12
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3290.0f, -1455.0f, -615.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 12-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1975.0f, 2280.0f, 3310.0f, -1455.0f, -615.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 12-2
		pCubeMesh = new CCubeMesh(pd3dDevice, -1295.0f, -1265.0f, 2280.0f, 3310.0f, -1455.0f, -615.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 12-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, -1485.0f, -1455.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 12-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, -640.0f, -610.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 13
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3290.0f, -2340.0f, -1500.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 13-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1975.0f, 2280.0f, 3310.0f, -2340.0f, -1500.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 13-2
		pCubeMesh = new CCubeMesh(pd3dDevice, -1295.0f, -1265.0f, 2280.0f, 3310.0f, -2340.0f, -1500.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 13-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, -2370.0f, -2340.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 13-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, -1525.0f, -1495.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 14
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3290.0f, -2340.0f, -1500.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 14-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -1175.0f, 2280.0f, 3310.0f, -2340.0f, -1500.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 14-2
		pCubeMesh = new CCubeMesh(pd3dDevice, -490.0f, -460.0f, 2280.0f, 3310.0f, -2340.0f, -1500.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 14-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3310.0f, -2370.0f, -2340.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 14-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3310.0f, -1525.0f, -1495.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 15
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3290.0f, -3235.0f, -2385.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 15-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -1175.0f, 2280.0f, 3310.0f, -3235.0f, -2385.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 15-2
		pCubeMesh = new CCubeMesh(pd3dDevice, -490.0f, -460.0f, 2280.0f, 3310.0f, -3235.0f, -2385.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 15-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3310.0f, -3265.0f, -3235.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 15-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3310.0f, -2410.0f, -2380.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 16
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3290.0f, -4130.0f, -3280.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 16-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -1175.0f, 2280.0f, 3310.0f, -4130.0f, -3280.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 16-2
		pCubeMesh = new CCubeMesh(pd3dDevice, -490.0f, -460.0f, 2280.0f, 3310.0f, -4130.0f, -3280.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 16-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3310.0f, -4155.0f, -4125.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 16-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3310.0f, -3305.0f, -3275.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 17
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3290.0f, -4130.0f, -3280.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 17-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1975.0f, 2280.0f, 3310.0f, -4130.0f, -3280.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 17-2
		pCubeMesh = new CCubeMesh(pd3dDevice, -1295.0f, -1265.0f, 2280.0f, 3310.0f, -4130.0f, -3280.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 17-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, -4155.0f, -4125.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 17-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, -3305.0f, -3275.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 18
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3290.0f, -5025.0f, -4175.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 18-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1975.0f, 2280.0f, 3310.0f, -5025.0f, -4175.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 18-2
		pCubeMesh = new CCubeMesh(pd3dDevice, -1295.0f, -1265.0f, 2280.0f, 3310.0f, -5025.0f, -4175.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 18-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, -5040.0f, -5010.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 18-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, -4190.0f, -4160.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 19
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3290.0f, -5910.0f, -5070.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 19-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1975.0f, 2280.0f, 3310.0f, -5910.0f, -5070.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 19-2
		pCubeMesh = new CCubeMesh(pd3dDevice, -1295.0f, -1265.0f, 2280.0f, 3310.0f, -5910.0f, -5070.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 19-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, -5925.0f, -5895.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 19-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, -5085.0f, -5055.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 20
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3290.0f, -5910.0f, -5070.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 20-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -1175.0f, 2280.0f, 3310.0f, -5910.0f, -5070.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 20-2
		pCubeMesh = new CCubeMesh(pd3dDevice, -490.0f, -460.0f, 2280.0f, 3310.0f, -5910.0f, -5070.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 20-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3310.0f, -5925.0f, -5895.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 20-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3310.0f, -5085.0f, -5055.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 21
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3290.0f, -5910.0f, -5070.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 21-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, -370.0f, 2280.0f, 3310.0f, -5910.0f, -5070.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 21-2
		pCubeMesh = new CCubeMesh(pd3dDevice, 315.0f, 345.0f, 2280.0f, 3310.0f, -5910.0f, -5070.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 21-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3310.0f, -5925.0f, -5895.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 21-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3310.0f, -5085.0f, -5055.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 22
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3290.0f, -6795.0f, -5955.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 22-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, -370.0f, 2280.0f, 3310.0f, -6795.0f, -5955.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 22-2
		pCubeMesh = new CCubeMesh(pd3dDevice, 315.0f, 345.0f, 2280.0f, 3310.0f, -6795.0f, -5955.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 22-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3310.0f, -6810.0f, -6780.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 22-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3310.0f, -5970.0f, -5940.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 23
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3290.0f, -7680.0f, -6840.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 23-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, -370.0f, 2280.0f, 3310.0f, -7680.0f, -6840.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 23-2
		pCubeMesh = new CCubeMesh(pd3dDevice, 315.0f, 345.0f, 2280.0f, 3310.0f, -7680.0f, -6840.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 23-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3310.0f, -7695.0f, -7665.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 23-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3310.0f, -6855.0f, -6825.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 24
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3290.0f, -8565.0f, -7725.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 24-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, -370.0f, 2280.0f, 3310.0f, -8565.0f, -7725.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 24-2
		pCubeMesh = new CCubeMesh(pd3dDevice, 315.0f, 345.0f, 2280.0f, 3310.0f, -8565.0f, -7725.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 24-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3310.0f, -8580.0f, -8550.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 24-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2280.0f, 3310.0f, -7740.0f, -7710.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 25
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3290.0f, -7680.0f, -6840.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 25-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -1175.0f, 2280.0f, 3310.0f, -7680.0f, -6840.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 25-2
		pCubeMesh = new CCubeMesh(pd3dDevice, -490.0f, -460.0f, 2280.0f, 3310.0f, -7680.0f, -6840.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 25-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3310.0f, -7695.0f, -7665.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 25-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3310.0f, -6855.0f, -6825.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;


		// 26
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3290.0f, -8565.0f, -7725.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 26-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -1175.0f, 2280.0f, 3310.0f, -8565.0f, -7725.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 26-2
		pCubeMesh = new CCubeMesh(pd3dDevice, -490.0f, -460.0f, 2280.0f, 3310.0f, -8565.0f, -7725.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 26-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3310.0f, -8580.0f, -8550.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 26-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2280.0f, 3310.0f, -7740.0f, -7710.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 27
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3290.0f, -7680.0f, -6840.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 27-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1975.0f, 2280.0f, 3310.0f, -7680.0f, -6840.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 27-2
		pCubeMesh = new CCubeMesh(pd3dDevice, -1295.0f, -1265.0f, 2280.0f, 3310.0f, -7680.0f, -6840.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 27-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, -7695.0f, -7665.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 27-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, -6855.0f, -6825.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 28
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3290.0f, -8565.0f, -7725.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 28-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1975.0f, 2280.0f, 3310.0f, -8565.0f, -7725.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 28-2
		pCubeMesh = new CCubeMesh(pd3dDevice, -1295.0f, -1265.0f, 2280.0f, 3310.0f, -8565.0f, -7725.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 28-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, -8580.0f, -8550.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 28-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, -7740.0f, -7710.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 29
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3290.0f, -9450.0f, -8610.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 29-1
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1975.0f, 2280.0f, 3310.0f, -9450.0f, -8610.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 29-2
		pCubeMesh = new CCubeMesh(pd3dDevice, -1295.0f, -1265.0f, 2280.0f, 3310.0f, -9450.0f, -8610.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 29-3
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, -9465.0f, -9435.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 29-4
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 2280.0f, 3310.0f, -8625.0f, -8595.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 벽 1
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 1680.0f, 2680.0f, 2200.0f, 2800.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 벽 2
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -460.0f, 2080.0f, 3080.0f, 2200.0f, 2800.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 벽 3
		pCubeMesh = new CCubeMesh(pd3dDevice, -1175.0f, -430.0f, 2880.0f, 3880.0f, 315.0f, 1155.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 벽 4
		pCubeMesh = new CCubeMesh(pd3dDevice, -1915.0f, -1265.0f, 2630.0f, 3630.0f, 315.0f, 1155.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 벽 5
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, 345.0f, 2880.0f, 3880.0f, -1455.0f, -615.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 벽 6
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2880.0f, 3880.0f, -2885.0f, -1455.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 벽 7
		pCubeMesh = new CCubeMesh(pd3dDevice, -400.0f, 345.0f, 2430.0f, 3430.0f, -4130.0f, -3135.0);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 벽 8
		pCubeMesh = new CCubeMesh(pd3dDevice, -1175.0f, 345.0f, 2430.0f, 3430.0f, -5025.0f, -4175.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 벽 9
		pCubeMesh = new CCubeMesh(pd3dDevice, -1915.0f, -1265.0f, 2630.0f, 3630.0f, -3235.0f, -2385.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 벽 10
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, -430.0f, 2880.0f, 3880.0f, -6795.0f, -5955.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 벽 11
		pCubeMesh = new CCubeMesh(pd3dDevice, -2005.0f, -1255.0f, 1680.0f, 2680.0f, -6795.0f, -5955.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;

		// 벽 12
		pCubeMesh = new CCubeMesh(pd3dDevice, -1205.0f, 315.0f, 2430.0f, 3430.0f, -9450.0f, -8610.0f);
		pRotatingObject = new CGameObject();

		pRotatingObject->SetMesh(pCubeMesh);
		pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

		m_ppObjects[n++] = pRotatingObject;
	}
	

	//건물1번 박스
	CMesh* pCubeMesh = new CCubeMesh(pd3dDevice, -50.0f, 50.0f, -50.0f, 50.0f, -25.0f, 75.0f, BOX); // 158
	CGameObject* pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -15.0f, 15.0f, -15.0f, 15.0f, 0.0f, 30.0f, REALBOX); //159
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;
	
	//건물1번 파이프
	pCubeMesh = new CCubeMesh(pd3dDevice,-205.0f, -195.0f, 3285.0f, 3295.0f, 3440.0f, 3560.0f, PIPE ); // 160
	pRotatingObject = new CGameObject();
	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	//4번 건물
	//실외기 인터랙트영역
	pCubeMesh = new CCubeMesh(pd3dDevice, -1775.0f, -1455.0f, 3280.0f, 3365.0f, 2125.0f, 2325.0f, CONDITIONER); // 161
	pRotatingObject = new CGameObject();
	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	//실외기 몸체ㅐ
	pCubeMesh = new CCubeMesh(pd3dDevice, -1975.0f, -1775.0f, 3280.0f, 3365.0f, 2105.0f, 2325.0f, OBJ); // 162
	pRotatingObject = new CGameObject();
	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;



	//5번 건물 박스1
	pCubeMesh = new CCubeMesh(pd3dDevice, -50.0f, 50.0f, -50.0f, 50.0f, -25.0f, 75.0f, BOX); // 163
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -15.0f, 15.0f, -15.0f, 15.0f, 0.0f, 30.0f, REALBOX); //164
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//5번 건물 박스2
	pCubeMesh = new CCubeMesh(pd3dDevice, -50.0f, 50.0f, -50.0f, 50.0f, -25.0f, 75.0f, BOX); // 165
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -15.0f, 15.0f, -15.0f, 15.0f, 0.0f, 30.0f, REALBOX); //166
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//6번 건물 파이프
	pCubeMesh = new CCubeMesh(pd3dDevice, -1110.0f, -1090.0f, 3285.0f, 3295.0f, 1740.0f, 1860.0f, PIPE); // 167
	pRotatingObject = new CGameObject();
	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	//7번 건물 박스
	pCubeMesh = new CCubeMesh(pd3dDevice, -50.0f, 50.0f, -50.0f, 50.0f, -25.0f, 75.0f, BOX); // 168
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -15.0f, 15.0f, -15.0f, 15.0f, 0.0f, 30.0f, REALBOX); //169
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	//8번 건물 파이프
	pCubeMesh = new CCubeMesh(pd3dDevice, -60.0f, 60.0f, 3285.0f, 3295.0f, 995.0f, 1005.0f, PIPE); //170
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	//8번 건물 실외기

	//인터랙트 영역
	pCubeMesh = new CCubeMesh(pd3dDevice, -180.0f, 240.0f, 3280.0f, 3365.0f, 890.0f, 1090.0f, CONDITIONER); // 171
	pRotatingObject = new CGameObject();
	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	//실외기 몸체ㅐ
	pCubeMesh = new CCubeMesh(pd3dDevice, -380.0f, -180.0f, 3280.0f, 3365.0f, 870.0f, 1090.0f, OBJ); // 172
	pRotatingObject = new CGameObject();
	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	//9번건물 박스 2개

	pCubeMesh = new CCubeMesh(pd3dDevice, -50.0f, 50.0f, -50.0f, 50.0f, -25.0f, 75.0f, BOX); // 173
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -15.0f, 15.0f, -15.0f, 15.0f, 0.0f, 30.0f, REALBOX); //174
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	pCubeMesh = new CCubeMesh(pd3dDevice, -50.0f, 50.0f, -50.0f, 50.0f, -25.0f, 75.0f, BOX); // 175
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -15.0f, 15.0f, -15.0f, 15.0f, 0.0f, 30.0f, REALBOX); //176
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//10번 건물 파이프
	pCubeMesh = new CCubeMesh(pd3dDevice, -685.0f, -675.0f, 3285.0f, 3295.0f, -200.0f, -80.0f, PIPE); //177
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//12번 건물 상자
	pCubeMesh = new CCubeMesh(pd3dDevice, -50.0f, 50.0f, -50.0f, 50.0f, -25.0f, 75.0f, BOX); // 178
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -15.0f, 15.0f, -15.0f, 15.0f, 0.0f, 30.0f, REALBOX); //179
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//13번 건물 실외기
	//실외기 인터랙트영역
	pCubeMesh = new CCubeMesh(pd3dDevice, -1775.0f, -1455.0f, 3280.0f, 3365.0f, -1730.0f, -1530.0f, CONDITIONER); // 180
	pRotatingObject = new CGameObject();
	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	//실외기 몸체ㅐ
	pCubeMesh = new CCubeMesh(pd3dDevice, -1975.0f, -1775.0f, 3280.0f, 3365.0f, -1750.0f, -1530.0f, OBJ); // 181
	pRotatingObject = new CGameObject();
	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;
	

	//13번 건물 상자
	pCubeMesh = new CCubeMesh(pd3dDevice, -50.0f, 50.0f, -50.0f, 50.0f, -25.0f, 75.0f, BOX); // 182
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -15.0f, 15.0f, -15.0f, 15.0f, 0.0f, 30.0f, REALBOX); //183
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//15번 건물 상자2개
	pCubeMesh = new CCubeMesh(pd3dDevice, -50.0f, 50.0f, -50.0f, 50.0f, -25.0f, 75.0f, BOX); // 184
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -15.0f, 15.0f, -15.0f, 15.0f, 0.0f, 30.0f, REALBOX); //185
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	pCubeMesh = new CCubeMesh(pd3dDevice, -50.0f, 50.0f, -50.0f, 50.0f, -25.0f, 75.0f, BOX); // 186
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -15.0f, 15.0f, -15.0f, 15.0f, 0.0f, 30.0f, REALBOX); //187
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//17번 건물 상자
	pCubeMesh = new CCubeMesh(pd3dDevice, -50.0f, 50.0f, -50.0f, 50.0f, -25.0f, 75.0f, BOX); // 188
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -15.0f, 15.0f, -15.0f, 15.0f, 0.0f, 30.0f, REALBOX); //189
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//20번 건물 파이프
	pCubeMesh = new CCubeMesh(pd3dDevice, -1110.0f, -1090.0f, 3285.0f, 3295.0f, -5560.0f, -5440.0f, PIPE); // 190
	pRotatingObject = new CGameObject();
	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//21번 건물 실외기
	//인터랙트 영역
	pCubeMesh = new CCubeMesh(pd3dDevice, -180.0f, 240.0f, 3280.0f, 3365.0f, -5880.0f, -5680.0f, CONDITIONER); // 191
	pRotatingObject = new CGameObject();
	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	//실외기 몸체ㅐ
	pCubeMesh = new CCubeMesh(pd3dDevice, -380.0f, -180.0f, 3280.0f, 3365.0f, -5900.0f, -5680.0f, OBJ); // 192
	pRotatingObject = new CGameObject();
	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//23번 건물 파이프
	pCubeMesh = new CCubeMesh(pd3dDevice, -60.0f, 60.0f, 3285.0f, 3295.0f, -6995.0f, -7005.0f, PIPE); //193
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//24번 건물 상자
	pCubeMesh = new CCubeMesh(pd3dDevice, -50.0f, 50.0f, -50.0f, 50.0f, -25.0f, 75.0f, BOX); // 194
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -15.0f, 15.0f, -15.0f, 15.0f, 0.0f, 30.0f, REALBOX); //195
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//25번 건물 상자
	pCubeMesh = new CCubeMesh(pd3dDevice, -50.0f, 50.0f, -50.0f, 50.0f, -25.0f, 75.0f, BOX); // 196
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -15.0f, 15.0f, -15.0f, 15.0f, 0.0f, 30.0f, REALBOX); //197
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//26번 건물 파이프
	pCubeMesh = new CCubeMesh(pd3dDevice, -1060.0f, -1070.0f, 3285.0f, 3295.0f, -8215.0f, -8095, PIPE); //198
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//27번 건물 실외기
	//실외기 인터랙트영역
	pCubeMesh = new CCubeMesh(pd3dDevice, -1775.0f, -1455.0f, 3280.0f, 3365.0f, -7060.0f, -6860.0f, CONDITIONER); // 199
	pRotatingObject = new CGameObject();
	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	//실외기 몸체ㅐ
	pCubeMesh = new CCubeMesh(pd3dDevice, -1975.0f, -1775.0f, 3280.0f, 3365.0f, -7090.0f, -6860.0f, OBJ); // 200
	pRotatingObject = new CGameObject();
	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//28번 건물 상자
	pCubeMesh = new CCubeMesh(pd3dDevice, -50.0f, 50.0f, -50.0f, 50.0f, -25.0f, 75.0f, BOX); // 201
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -15.0f, 15.0f, -15.0f, 15.0f, 0.0f, 30.0f, REALBOX); //202
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//29번 골인 지점

	pCubeMesh = new CCubeMesh(pd3dDevice, -1805, -1405.0f, 3000.0f, 3400.0f, -9250.0f, -8810.0f, GOAL); //203
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//2번건물 철조망
	pCubeMesh = new CCubeMesh(pd3dDevice, -810.0f, -790.0f, 3280.0f, 3370.0f, 3315.0f, 3445.0f, FENCE); //204
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -810.0f, -790.0f, 3280.0f, 3370.0f, 3055.0f, 3315.0f, FENCE); //205
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -810.0f, -790.0f, 3280.0f, 3370.0f, 3445.0f, 3705.0f, FENCE); //206
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//2번건물철조망 입구
	pCubeMesh = new CCubeMesh(pd3dDevice, -790.0f, -740.0f, 3280.0f, 3370.0f, 3335.0f, 3425.0f, FENCEHOLE); //207
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -860.0f, -810.0f, 3280.0f, 3370.0f, 3335.0f, 3425.0f, FENCEHOLE); //208
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;



	//6번건물 철조망
	pCubeMesh = new CCubeMesh(pd3dDevice, -810.0f, -790.0f, 3280.0f, 3370.0f, 1555.0f, 1685.0f, FENCE); //209
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -810.0f, -790.0f, 3280.0f, 3370.0f, 1685.0f, 1945.0f, FENCE); //210
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -810.0f, -790.0f, 3280.0f, 3370.0f, 1295.0f, 1555.0f, FENCE); //211
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//6번건물철조망 입구
	pCubeMesh = new CCubeMesh(pd3dDevice, -790.0f, -740.0f, 3280.0f, 3370.0f, 1575.0f, 1645.0f, FENCEHOLE); //212
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -860.0f, -810.0f, 3280.0f, 3370.0f, 1575.0f, 1645.0f, FENCEHOLE); //213
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;




	//10번건물 철조망
	pCubeMesh = new CCubeMesh(pd3dDevice, -910.0f, -890.0f, 3280.0f, 3370.0f, -215.0f, -85.0f, FENCE); //214
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -910.0f, -890.0f, 3280.0f, 3370.0f, -85.0f, 175.0f, FENCE); //215
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -910.0f, -890.0f, 3280.0f, 3370.0f, -475.0f, -215.0f, FENCE); //216
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//10번건물철조망 입구
	pCubeMesh = new CCubeMesh(pd3dDevice, -890.0f, -840.0f, 3280.0f, 3370.0f, -195.0f, -105.0f, FENCEHOLE); //217
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -960.0f, -910.0f, 3280.0f, 3370.0f, -195.0f, -105.0f, FENCEHOLE); //218
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;




	//16번 건물 -850.0f, 3280.0f, -3700.0f
	//16번건물 철조망
	pCubeMesh = new CCubeMesh(pd3dDevice, -915.0f, -785.0f, 3280.0f, 3370.0f, -3710.0f, -3690.0f, FENCE); //219
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -785.0f, -525.0f, 3280.0f, 3370.0f, -3710.0f, -3690.0f, FENCE); //220
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -1175.0f, -915.0f, 3280.0f, 3370.0f, -3710.0f, -3690.0f, FENCE); //221
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//16번건물철조망 입구
	pCubeMesh = new CCubeMesh(pd3dDevice, -895.0f, -805.0f, 3280.0f, 3370.0f, -3690.0f, -3650.0f, FENCEHOLE); //222
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -895.0f, -805.0f, 3280.0f, 3370.0f, -3750.0f, -3710.0f, FENCEHOLE); //223
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;




	//25번건물 철조망
	pCubeMesh = new CCubeMesh(pd3dDevice, -710.0f, -690.0f, 3280.0f, 3370.0f, -7325.0f, -7195.0f, FENCE); //224
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -710.0f, -690.0f, 3280.0f, 3370.0f, -7195.0f, -6935.0f, FENCE); //225
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -710.0f, -690.0f, 3280.0f, 3370.0f, -7585.0f, -7325.0f, FENCE); //226
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//25번건물철조망 입구
	pCubeMesh = new CCubeMesh(pd3dDevice, -750.0f, -710.0f, 3280.0f, 3370.0f, -7305.0f, -7215.0f, FENCEHOLE); //227
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -690.0f, -650.0f, 3280.0f, 3370.0f, -7305.0f, -7215.0f, FENCEHOLE); //228
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;



	//27번 건물 -1670.0f, 3280.0f, -7380.0f
	//27번건물 철조망
	pCubeMesh = new CCubeMesh(pd3dDevice, -1735.0f ,-1605.0f , 3280.0f, 3370.0f, -7390.0f, -7370.0f, FENCE); //229
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -1995.0f, -1735.0f, 3280.0f, 3370.0f, -7390.0f, -7370.0f, FENCE); //230
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -1605.0f, -1345.0f,  3280.0f, 3370.0f, -7390.0f, -7370.0f, FENCE); //231
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//27번건물철조망 입구
	pCubeMesh = new CCubeMesh(pd3dDevice, -1705.0f , -1635.0f , 3280.0f, 3370.0f, -7420.0f, -7390.0f, FENCEHOLE); //232
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -1705.0f, -1635.0f,  3280.0f, 3370.0f, -7370.0f, -7330.0f, FENCEHOLE); //233
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;





	//3번 건물 문
	pCubeMesh = new CCubeMesh(pd3dDevice, -1730.0f, -1670.0f, 3280.0f, 3365.0f, 3165.0f, 3175.0f, REALDOOR); // 234
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -1730.0f, -1670.0f, 3280.0f, 3365.0f, 3195.0f, 3265.0f, DOOR); // 235
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;



	//11번 건물 문
	pCubeMesh = new CCubeMesh(pd3dDevice, -1680.0f, -1620.0f, 3280.0f, 3365.0f, -375.0f, -365.0f, REALDOOR); // 236
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -1680.0f, -1620.0f, 3280.0f, 3365.0f, -365.0f, -325.0f,  DOOR); // 237
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//18번 건물 문
	pCubeMesh = new CCubeMesh(pd3dDevice, -1680.0f, -1620.0f, 3280.0f, 3365.0f, -4805.0f, -4795.0f,  REALDOOR); // 238
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -1680.0f, -1620.0f, 3280.0f, 3365.0f, -4795.0f, -4755.0f,  DOOR); // 239
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;



	//26번 건물 문
	pCubeMesh = new CCubeMesh(pd3dDevice, -1005.0f, -995.0f, 3280.0f, 3365.0f, -8180.0f, -8120.0f, REALDOOR); // 240
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -995.0f, -955.0f,  3280.0f, 3365.0f, -8180.0f, -8120.0f, DOOR); // 241
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	//3번 건물 문 옆 펜스
	pCubeMesh = new CCubeMesh(pd3dDevice, -1990.0f, -1730.0f, 3280.0f, 3370.0f, 3165.0f, 3175.0f, FENCE); //242
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -1670.0f, -1280.0f, 3280.0f, 3370.0f, 3165.0f, 3175.0f, FENCE); //243
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	// 11번 건물 문 옆 펜스
	pCubeMesh = new CCubeMesh(pd3dDevice, -1620.0f, -1360.0f, 3280.0f, 3370.0f, -375.0f, -365.0f, FENCE); //244
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -1940.0f, -1680.0f, 3280.0f, 3370.0f, -375.0f, -365.0f, FENCE); //245
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;


	// 18번 건물 문 옆 펜스
	pCubeMesh = new CCubeMesh(pd3dDevice, -1620.0f, -1360.0f, 3280.0f, 3370.0f, -4805.0f, -4795.0f, FENCE); //246
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -1940.0f, -1680.0f, 3280.0f, 3370.0f, -4805.0f, -4795.0f, FENCE); //247
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	// 26번 건물 문 옆 펜스
	pCubeMesh = new CCubeMesh(pd3dDevice, -1005.0f, -995.0f, 3280.0f, 3370.0f,  -8440.0f, -8180.0f, FENCE); //248
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, -1005.0f, -995.0f, 3280.0f, 3370.0f, -8120.0f, -7860.0f, FENCE); //249
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[n++] = pRotatingObject;



}

void CDiffusedShader::ReleaseObjects()
{
	CShader::ReleaseObjects();
}

void CDiffusedShader::AnimateObjects(ID3D11Device *pd3dDevice, float fTimeElapsed)
{
	//CShader::AnimateObjects(pd3dDevice, fTimeElapsed);
}


void CDiffusedShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	CShader::Render(pd3dDeviceContext, pCamera);
}


CTextureShader::CTextureShader()
{
}

CTextureShader::~CTextureShader()
{
}

void CTextureShader::CreateShader(ID3D11Device * pd3dDevice)
{
	CShader::CreateShader(pd3dDevice);
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXNUM", 0, DXGI_FORMAT_R32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"Effect.fx", "VSTexturedUVWLighting", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"Effect.fx", "PSTexturedUVWLighting", "ps_4_0", &m_pd3dPixelShader);
}

void CTextureShader::CreateShaderVariables(ID3D11Device * pd3dDevice)
{
	CShader::CreateShaderVariables(pd3dDevice);
}

void CTextureShader::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld)
{
	CShader::UpdateShaderVariables(pd3dDeviceContext, pd3dxmtxWorld);
}

void CTextureShader::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, CTexture *pTexture)
{
	pd3dDeviceContext->PSSetShaderResources(PS_SLOT_TEXTURE, pTexture->m_nTextures, pTexture->m_ppd3dsrvTextures);
	pd3dDeviceContext->PSSetSamplers(PS_SLOT_SAMPLER_STATE, 1, &pTexture->m_ppd3dSamplerStates[0]);
}
void CTextureShader::UpdateShaderVariables(ID3D11DeviceContext * pd3dDeviceContext, MATERIAL * pMaterial)
{
	CShader::UpdateShaderVariables(pd3dDeviceContext, pMaterial);
}
void CTextureShader::BuildObjects(ID3D11Device * pd3dDevice, int mapNumber)
{
	CMaterial **ppMaterials = new CMaterial*[1];
	ppMaterials[0] = new CMaterial();
	ppMaterials[0]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcSpecular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.0f);
	ppMaterials[0]->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	ID3D11SamplerState *pd3dSamplerState = NULL;
	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);

	ID3D11ShaderResourceView *pd3dTexture = NULL;

	CTexture *p_Texture;
	if (mapNumber == 1)
	{

		p_Texture = new CTexture(3);
		D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Maps\\Texture\\map01_3.png"), NULL, NULL, &pd3dTexture, NULL);
		p_Texture->SetTexture(0, pd3dTexture, pd3dSamplerState);
		D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Maps\\Texture\\map01_1.png"), NULL, NULL, &pd3dTexture, NULL);
		p_Texture->SetTexture(1, pd3dTexture, pd3dSamplerState);
		D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Maps\\Texture\\map01_2.png"), NULL, NULL, &pd3dTexture, NULL);
		p_Texture->SetTexture(2, pd3dTexture, pd3dSamplerState);
	}
	else
	{
		p_Texture = new CTexture(7);
		D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Maps\\Texture\\build7.png"), NULL, NULL, &pd3dTexture, NULL);
		p_Texture->SetTexture(0, pd3dTexture, pd3dSamplerState);
		D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Maps\\Texture\\build3.jpg"), NULL, NULL, &pd3dTexture, NULL);
		p_Texture->SetTexture(1, pd3dTexture, pd3dSamplerState);
		D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Maps\\Texture\\build9.jpg"), NULL, NULL, &pd3dTexture, NULL);
		p_Texture->SetTexture(2, pd3dTexture, pd3dSamplerState);
		D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Maps\\Texture\\build8.png"), NULL, NULL, &pd3dTexture, NULL);
		p_Texture->SetTexture(3, pd3dTexture, pd3dSamplerState);
		D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Maps\\Texture\\build6.png"), NULL, NULL, &pd3dTexture, NULL);
		p_Texture->SetTexture(4, pd3dTexture, pd3dSamplerState);
		D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Maps\\Texture\\build10.jpg"), NULL, NULL, &pd3dTexture, NULL);
		p_Texture->SetTexture(5, pd3dTexture, pd3dSamplerState);
		D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Maps\\Texture\\build11.jpg"), NULL, NULL, &pd3dTexture, NULL);
		p_Texture->SetTexture(6, pd3dTexture, pd3dSamplerState);
	}

	CMesh *pMeshIlluminatedTextured = new CMeshTextured(pd3dDevice, mapNumber);//CCubeMeshIlluminatedTextured(pd3dDevice, 12.0f, 12.0f, 12.0f);

	int i = 0, nObjectTypes = 2;
	m_nObjects = 1;//((xObjects * 2) + 1) * ((yObjects * 2) + 1) * ((zObjects * 2) + 1);
	m_ppObjects = new CGameObject*[m_nObjects];
	CGameObject *pObject = NULL;
	pObject = new CGameObject();
	pObject->SetMesh(pMeshIlluminatedTextured);
	pObject->SetMaterial(ppMaterials[0]);
	pObject->SetTexture(p_Texture);


	//pRotatingObject->
	if (mapNumber == 1)
	{
		pObject->Scale(1000.0f);
	}
	else
	{
		pObject->SetPosition(0.0f, 1850.0f, 4500.0f);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), 90.0f);
		pObject->Scale(4.0f);

	}

	m_ppObjects[i++] = pObject;


	CreateShaderVariables(pd3dDevice);

	//delete[] ppTextures;
	//delete p_Texture;
	delete[] ppMaterials;
}
void CTextureShader::ReleaseObjects()
{
}

void CTextureShader::Render(ID3D11DeviceContext * pd3dDeviceContext, CCamera * pCamera, int index)
{
	CShader::Render(pd3dDeviceContext, pCamera, index);
}

CAnimateShader::CAnimateShader()
{
}

CAnimateShader::~CAnimateShader()
{
}

void CAnimateShader::CreateShader(ID3D11Device *pd3dDevice)
{
	CShader::CreateShader(pd3dDevice);
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "BONEINDICES",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXNUM", 0, DXGI_FORMAT_R32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"Effect.fx", "SkinnedVS", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"Effect.fx", "SkinnedPS", "ps_4_0", &m_pd3dPixelShader);
}

void CAnimateShader::CreateShaderVariables(ID3D11Device * pd3dDevice)
{
	CShader::CreateShaderVariables(pd3dDevice);
	D3D11_BUFFER_DESC d3dBufferDesc2;
	ZeroMemory(&d3dBufferDesc2, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc2.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc2.ByteWidth = sizeof(VS_CB_SKINNED);
	pd3dDevice->CreateBuffer(&d3dBufferDesc2, NULL, &m_pd3dcbSkinned);

}

void CAnimateShader::UpdateShaderVariables(ID3D11DeviceContext * pd3dDeviceContext, D3DXMATRIX * pd3dxmtxWorld)
{
	CShader::UpdateShaderVariables(pd3dDeviceContext, pd3dxmtxWorld);
}

void CAnimateShader::UpdateShaderVariables(ID3D11DeviceContext * pd3dDeviceContext, MATERIAL * pMaterial)
{
	CShader::UpdateShaderVariables(pd3dDeviceContext, pMaterial);
}

void CAnimateShader::UpdateShaderVariables(ID3D11DeviceContext * pd3dDeviceContext, CTexture * pTexture)
{
	CTextureShader::UpdateShaderVariables(pd3dDeviceContext, pTexture);
}

void CAnimateShader::UpdateShaderVariables(ID3D11DeviceContext * pd3dDeviceContext, D3DXMATRIX * pd3dTransform, int AnimationSize)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbSkinned, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_SKINNED *pcbSkinned = (VS_CB_SKINNED *)d3dMappedResource.pData;


	for (int i = 0; i < AnimationSize; ++i)
	{
		D3DXMatrixTranspose(&(pcbSkinned->BoneTransform[i]), &(pd3dTransform[i]));
	}

	pd3dDeviceContext->Unmap(m_pd3dcbSkinned, 0);
	pd3dDeviceContext->VSSetConstantBuffers(0x02, 1, &m_pd3dcbSkinned);

	//delete pd3dTransform;

}

void CAnimateShader::BuildObjects(ID3D11Device *pd3dDevice)
{
	CMaterial **ppMaterials = new CMaterial*[3];
	ppMaterials[0] = new CMaterial();
	ppMaterials[0]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcSpecular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	ppMaterials[1] = new CMaterial();
	ppMaterials[1]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[1]->m_Material.m_d3dxcAmbient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	ppMaterials[1]->m_Material.m_d3dxcSpecular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 10.0f);
	ppMaterials[1]->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	ppMaterials[2] = new CMaterial();
	ppMaterials[2]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	ppMaterials[2]->m_Material.m_d3dxcAmbient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	ppMaterials[2]->m_Material.m_d3dxcSpecular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 10.0f);
	ppMaterials[2]->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	ID3D11SamplerState *pd3dSamplerState = NULL;
	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);


	int i = 0;
	m_nObjects = 4;
	m_ppObjects = new CGameObject*[m_nObjects];
	CGameObject *pRotatingObject = NULL;
	CMesh *pMeshIlluminatedTextured;
	ID3D11ShaderResourceView *pd3dTexture = NULL;
	CTexture *p_Texture;
	CTexture *pBump;





	p_Texture = new CTexture(8);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\Thief\\ThiefA_Bottom_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(0, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\Thief\\ThiefA_Top_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(1, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\Thief\\ThiefA_Hat_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(2, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\Thief\\ThiefA_Mask_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(3, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\Thief\\ThiefA_Body_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(4, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\Thief\\ThiefA_Body_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(5, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\Thief\\ThiefA_Glove_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(6, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\Thief\\ThiefA_Shoes_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(7, pd3dTexture, pd3dSamplerState);




	pBump = new CTexture(8);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\Thief\\ThiefA_Bottom_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(0, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\Thief\\ThiefA_Top_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(1, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\Thief\\ThiefA_Hat_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(2, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\Thief\\ThiefA_Mask_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(3, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\Thief\\ThiefA_Body_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(4, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\Thief\\ThiefA_Body_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(5, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\Thief\\ThiefA_Glove_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(6, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\Thief\\ThiefA_Shoes_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(7, pd3dTexture, pd3dSamplerState);

	pMeshIlluminatedTextured = new CCharacterMesh(pd3dDevice, "thiefAAA", 12.0f, 12.0f, 12.0f);//CCubeMeshIlluminatedTextured(pd3dDevice, 12.0f, 12.0f, 12.0f);
	pRotatingObject = NULL;




	pRotatingObject = new CGameObject();
	pRotatingObject->SetMesh(pMeshIlluminatedTextured);
	pRotatingObject->SetMaterial(ppMaterials[0]);
	pRotatingObject->SetTexture(p_Texture);
	pRotatingObject->SetBump(pBump);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[i++] = pRotatingObject;


	p_Texture = new CTexture(8);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police1\\Police1_Bottom_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(0, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police1\\Police1_Top_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(1, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police1\\Police1_Shoes_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(2, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police1\\Police1_Body_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(3, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police1\\Police1_Hat_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(4, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police1\\Police1_Moustache_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(5, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\ThiefA.fbm\\ThiefA_Glove_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(6, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\ThiefA.fbm\\ThiefA_Shoes_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(7, pd3dTexture, pd3dSamplerState);


	pBump = new CTexture(6);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police1\\Police1_Bottom_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(0, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police1\\Police1_Top_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(1, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police1\\Police1_Shoes_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(2, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police1\\Police1_Body_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(3, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police1\\Police1_Hat_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(4, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police1\\Police1_Moustache_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(5, pd3dTexture, pd3dSamplerState);

	pMeshIlluminatedTextured = new CCharacterMesh(pd3dDevice, "Police01", 12.0f, 12.0f, 12.0f);//CCubeMeshIlluminatedTextured(pd3dDevice, 12.0f, 12.0f, 12.0f);

	pRotatingObject = NULL;
	pRotatingObject = new CGameObject();
	pRotatingObject->SetMesh(pMeshIlluminatedTextured);
	pRotatingObject->SetMaterial(ppMaterials[0]);
	pRotatingObject->SetTexture(p_Texture);
	pRotatingObject->SetBump(pBump);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[i++] = pRotatingObject;


	p_Texture = new CTexture(4);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police2\\Police2_Bottom_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(0, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police2\\Police2_Top_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(1, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police2\\Police2_Body_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(2, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police2\\Police2_Shoes_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(3, pd3dTexture, pd3dSamplerState);					 
																				 
																				 
																				 
	pBump = new CTexture(4);													 
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police2\\Police2_Bottom_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(0, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police2\\Police2_Top_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(1, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police2\\Police2_Body_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(2, pd3dTexture, pd3dSamplerState);		
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police2\\Police2_Shoes_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(3, pd3dTexture, pd3dSamplerState);
	

	pMeshIlluminatedTextured = new CCharacterMesh(pd3dDevice, "Police02", 12.0f, 12.0f, 12.0f);//CCubeMeshIlluminatedTextured(pd3dDevice, 12.0f, 12.0f, 12.0f);

	pRotatingObject = NULL;
	pRotatingObject = new CGameObject();
	pRotatingObject->SetMesh(pMeshIlluminatedTextured);
	pRotatingObject->SetMaterial(ppMaterials[0]);
	pRotatingObject->SetTexture(p_Texture);
	pRotatingObject->SetBump(pBump);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[i++] = pRotatingObject;



	p_Texture = new CTexture(5);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police3\\Police03_Bottom_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(0, pd3dTexture, pd3dSamplerState);					  		   
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police3\\Police03_Top_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(1, pd3dTexture, pd3dSamplerState);					  		   
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police3\\Police03_Body_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(2, pd3dTexture, pd3dSamplerState);					  		   
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police3\\Police03_Hat_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(3, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police3\\Police03_Shoes_Diffuse.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(4, pd3dTexture, pd3dSamplerState);
																				  		   
																				  		   
	pBump = new CTexture(5);													  		   
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police3\\Police03_Bottom_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(0, pd3dTexture, pd3dSamplerState);						  		   
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police3\\Police03_Top_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(1, pd3dTexture, pd3dSamplerState);						  		   
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police3\\Police03_Body_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(2, pd3dTexture, pd3dSamplerState);						  		   
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police3\\Police03_Hat_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(3, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Character\\police3\\Police03_Shoes_Normal.png"), NULL, NULL, &pd3dTexture, NULL);
	pBump->SetTexture(4, pd3dTexture, pd3dSamplerState);


	pMeshIlluminatedTextured = new CCharacterMesh(pd3dDevice, "Police02", 12.0f, 12.0f, 12.0f);//CCubeMeshIlluminatedTextured(pd3dDevice, 12.0f, 12.0f, 12.0f);

	pRotatingObject = NULL;
	pRotatingObject = new CGameObject();
	pRotatingObject->SetMesh(pMeshIlluminatedTextured);
	pRotatingObject->SetMaterial(ppMaterials[0]);
	pRotatingObject->SetTexture(p_Texture);
	pRotatingObject->SetBump(pBump);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[i++] = pRotatingObject;




	CreateShaderVariables(pd3dDevice);

	//delete[] ppTextures;
	//delete p_Texture;
	delete[] ppMaterials;
}

CSkyBoxShader::CSkyBoxShader()
{
}

CSkyBoxShader::~CSkyBoxShader()
{
}

void CSkyBoxShader::CreateShader(ID3D11Device * pd3dDevice)
{
	CShader::CreateShader(pd3dDevice);
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD0", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"Effect.fx", "VSTexturedLighting", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"Effect.fx", "PSTexturedLighting", "ps_4_0", &m_pd3dPixelShader);
}

void CSkyBoxShader::CreateShaderVariables(ID3D11Device * pd3dDevice)
{
	CTextureShader::CreateShaderVariables(pd3dDevice);

}

void CSkyBoxShader::UpdateShaderVariables(ID3D11DeviceContext * pd3dDeviceContext, D3DXMATRIX * pd3dxmtxWorld)
{
	CTextureShader::UpdateShaderVariables(pd3dDeviceContext, pd3dxmtxWorld);
}

void CSkyBoxShader::UpdateShaderVariables(ID3D11DeviceContext * pd3dDeviceContext, MATERIAL * pMaterial)
{
	CTextureShader::UpdateShaderVariables(pd3dDeviceContext, pMaterial);
}

void CSkyBoxShader::UpdateShaderVariables(ID3D11DeviceContext * pd3dDeviceContext, CTexture * pTexture)
{
	CTextureShader::UpdateShaderVariables(pd3dDeviceContext, pTexture);
}

void CSkyBoxShader::BuildObjects(ID3D11Device * pd3dDevice)
{
	CMaterial **ppMaterials = new CMaterial*[1];
	ppMaterials[0] = new CMaterial();
	ppMaterials[0]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcSpecular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	ID3D11SamplerState *pd3dSamplerState = NULL;
	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);

	ID3D11ShaderResourceView *pd3dTexture = NULL;

	CTexture *p_Texture = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Maps\\Texture\\Sky_03.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(0, pd3dTexture, pd3dSamplerState);

	CMesh *pMesh = new CSkyBoxMesh(pd3dDevice);

	m_nObjects = 1;
	m_ppObjects = new CGameObject*[m_nObjects];

	CGameObject *pObject = NULL;

	pObject = new CGameObject();
	pObject->SetMesh(pMesh);
	pObject->SetMaterial(ppMaterials[0]);
	pObject->SetTexture(p_Texture);
	pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);


	//pObject->SetPosition(0.0f, -1000.0f, 0.0f);
	pObject->Scale(1.0f);
	m_ppObjects[0] = pObject;
	CreateShaderVariables(pd3dDevice);
	delete[] ppMaterials;
}

void CSkyBoxShader::ReleaseObjects()
{
	CTextureShader::ReleaseObjects();
}

void CSkyBoxShader::Render(ID3D11DeviceContext * pd3dDeviceContext, CCamera * pCamera)
{
	if (pCamera)
		m_ppObjects[0]->SetPosition(pCamera->GetPosition());
	CTextureShader::Render(pd3dDeviceContext, pCamera);
}

CItemShader::CItemShader()
{
}

CItemShader::~CItemShader()
{
}

void CItemShader::CreateShader(ID3D11Device * pd3dDevice)
{

	CShader::CreateShader(pd3dDevice);
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXNUM", 0, DXGI_FORMAT_R32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"Effect.fx", "VSTexturedUVWLighting", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"Effect.fx", "PSTexturedUVWLighting", "ps_4_0", &m_pd3dPixelShader);
}

void CItemShader::CreateShaderVariables(ID3D11Device * pd3dDevice)
{
	CTextureShader::CreateShaderVariables(pd3dDevice);

}

void CItemShader::UpdateShaderVariables(ID3D11DeviceContext * pd3dDeviceContext, D3DXMATRIX * pd3dxmtxWorld)
{
	CTextureShader::UpdateShaderVariables(pd3dDeviceContext, pd3dxmtxWorld);

}

void CItemShader::UpdateShaderVariables(ID3D11DeviceContext * pd3dDeviceContext, MATERIAL * pMaterial)
{
	CTextureShader::UpdateShaderVariables(pd3dDeviceContext, pMaterial);

}

void CItemShader::UpdateShaderVariables(ID3D11DeviceContext * pd3dDeviceContext, CTexture * pTexture)
{
	CTextureShader::UpdateShaderVariables(pd3dDeviceContext, pTexture);

}

void CItemShader::AnimateObjects(ID3D11Device * pd3dDevice, float fTimeElapsed)
{

	for (int i = 0; i < m_nObjects; ++i)
	{
		if (m_ppObjects[i]->m_physics.isValid)
		{
			m_ppObjects[i]->Animate(pd3dDevice, fTimeElapsed);
		}
	}
}

void CItemShader::BuildObjects(ID3D11Device * pd3dDevice, int mapNumber)
{

	CMaterial **ppMaterials = new CMaterial*[1];
	ppMaterials[0] = new CMaterial();
	ppMaterials[0]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcSpecular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	ID3D11SamplerState *pd3dSamplerState = NULL;
	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);

	ID3D11ShaderResourceView *pd3dTexture = NULL;



	//실외기 메쉬와 텍스쳐
	CMesh *ConditionerMesh = new CItemMesh(pd3dDevice, "conditioner.src");
	CTexture *ConditionerTexture = new CTexture(8);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\texture\\conditioner_4.jpg"), NULL, NULL, &pd3dTexture, NULL);
	ConditionerTexture->SetTexture(0, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\texture\\conditioner_2.jpg"), NULL, NULL, &pd3dTexture, NULL);
	ConditionerTexture->SetTexture(1, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\texture\\conditioner_3.jpg"), NULL, NULL, &pd3dTexture, NULL);
	ConditionerTexture->SetTexture(2, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\texture\\conditioner_1.jpg"), NULL, NULL, &pd3dTexture, NULL);
	ConditionerTexture->SetTexture(3, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\texture\\conditioner_5.jpg"), NULL, NULL, &pd3dTexture, NULL);
	ConditionerTexture->SetTexture(4, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\texture\\conditioner_6.jpg"), NULL, NULL, &pd3dTexture, NULL);
	ConditionerTexture->SetTexture(5, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\texture\\conditioner_7.jpg"), NULL, NULL, &pd3dTexture, NULL);
	ConditionerTexture->SetTexture(6, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\texture\\conditioner_8.jpg"), NULL, NULL, &pd3dTexture, NULL);
	ConditionerTexture->SetTexture(7, pd3dTexture, pd3dSamplerState);

	//파이프 메쉬와 텍스쳐
	CMesh* pipeMesh = new CItemMesh(pd3dDevice, "pipe.src");
	CTexture *pPipeTexture = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\texture\\pipe_1.jpg"), NULL, NULL, &pd3dTexture, NULL);
	pPipeTexture->SetTexture(0, pd3dTexture, pd3dSamplerState);

	// 상자 메쉬와 텍스쳐
	CMesh *boxMesh = new CItemMesh(pd3dDevice, "box.src");
	CTexture * pBoxTexture = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\texture\\box.jpg"), NULL, NULL, &pd3dTexture, NULL);
	pBoxTexture->SetTexture(0, pd3dTexture, pd3dSamplerState);

	if (mapNumber == 1)
	{
		m_nObjects = 6;
		m_ppObjects = new CGameObject*[m_nObjects];

		CGameObject *pObject = NULL;

		pObject = new CGameObject();
		pObject->SetMesh(ConditionerMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(ConditionerTexture);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), 90.0f);
		pObject->SetPosition(50.0f, 3270.0f, 2500.0f);

		pObject->Scale(0.3f);
		m_ppObjects[0] = pObject;



		pObject = new CGameObject();
		pObject->SetMesh(pipeMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pPipeTexture);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), 90.0f);

		pObject->SetPosition(300.0f, 3280.0f, 3000.0f);

		pObject->Scale(0.4f);
		m_ppObjects[1] = pObject;



		//상자


		pObject = new CGameObject();
		pObject->SetMesh(boxMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pBoxTexture);
		pObject->SetPosition(880.0f, 3158.0f, 542.0f);

		pObject->Scale(1.2f);
		m_ppObjects[2] = pObject;



		// 두번째 파이프
		pObject = new CGameObject();
		pObject->SetMesh(pipeMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pPipeTexture);

		pObject->SetPosition(803.0f, 3263.0f, 1559.0f);

		pObject->Scale(0.4f);
		m_ppObjects[3] = pObject;


		// 세번째 파이프
		pObject = new CGameObject();
		pObject->SetMesh(pipeMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pPipeTexture);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), 90.0f);
		pObject->SetPosition(951.0f, 3078.0f, -114.0f);

		pObject->Scale(0.4f);
		m_ppObjects[4] = pObject;

		// 박스
		pObject = new CGameObject();
		pObject->SetMesh(boxMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pBoxTexture);

		pObject->SetPosition(1656.5f, 2758.0f, -2152.0f);

		pObject->Scale(1.2f);
		m_ppObjects[5] = pObject;


	}
	else
	{
		// 두번째 맵
		m_nObjects = 27;
		m_ppObjects = new CGameObject*[m_nObjects];

		CGameObject *pObject = NULL;
		CMesh *floor = new CFloorMesh(pd3dDevice);
		CTexture *floorTexture = new CTexture(1);
		D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Maps\\texture\\road.jpg"), NULL, NULL, &pd3dTexture, NULL);
		floorTexture->SetTexture(0, pd3dTexture, pd3dSamplerState);

		// 바닥
		pObject = new CGameObject();
		pObject->SetMesh(floor);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(floorTexture);
		
		pObject->SetPosition(0.0f, 1500.0f, 0.0f);

		pObject->Scale(15.0f);
		m_ppObjects[0] = pObject;




		//상자(1번 건물)

		pObject = new CGameObject();
		pObject->SetMesh(boxMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pBoxTexture);

		pObject->SetPosition(-300.0f, 3293.0f, 3200.0f);

		pObject->Scale(1.2f);
		m_ppObjects[1] = pObject;
		
		//파이프(1번 건물)
		pObject = new CGameObject();
		pObject->SetMesh(pipeMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pPipeTexture);

		pObject->SetPosition(-200.0f, 3290.0f, 3500.0f);

		pObject->Scale(0.4f);
		m_ppObjects[2] = pObject;


		//실외기 (4번 건물)
		pObject = new CGameObject();
		pObject->SetMesh(ConditionerMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(ConditionerTexture);

		pObject->SetPosition(-1875.0f, 3280.0f, 2225.0f);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), 90.0f);
		pObject->Scale(0.3f);
		m_ppObjects[3] = pObject;


		//박스(5번 건물 2개)
		pObject = new CGameObject();
		pObject->SetMesh(boxMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pBoxTexture);
		pObject->SetPosition(-1400.0f, 3293.0f, 1600.0f);
		pObject->Scale(1.2f);
		m_ppObjects[4] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(boxMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pBoxTexture);
		pObject->SetPosition(-1400.0f, 3293.0f, 1800.0f);
		pObject->Scale(1.2f);
		m_ppObjects[5] = pObject;


		//파이프(6번건물)
		pObject = new CGameObject();
		pObject->SetMesh(pipeMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pPipeTexture);

		pObject->SetPosition(-1100.0f, 3290.0f, 1800.0f);

		pObject->Scale(0.4f);
		m_ppObjects[6] = pObject;

		// 박스 7번 건물

		pObject = new CGameObject();
		pObject->SetMesh(boxMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pBoxTexture);
		pObject->SetPosition(150.0f, 3293.0f, 1600.0f);
		pObject->Scale(1.2f);
		m_ppObjects[7] = pObject;


		//파이프 8번건물
		pObject = new CGameObject();
		pObject->SetMesh(pipeMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pPipeTexture);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), 90.0f);
		pObject->SetPosition(0.0f, 3290.0f, 1000.0f);
		pObject->Scale(0.4f);
		m_ppObjects[8] = pObject;

		//실외기 (8번 건물)
		pObject = new CGameObject();
		pObject->SetMesh(ConditionerMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(ConditionerTexture);
		pObject->SetPosition(-280.0f, 3280.0f, 990.0f);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), 90.0f);
		pObject->Scale(0.3f);
		m_ppObjects[9] = pObject;

		//상자 9번 건물
		pObject = new CGameObject();
		pObject->SetMesh(boxMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pBoxTexture);
		pObject->SetPosition(150.0f, 3293.0f, -300.0f);
		pObject->Scale(1.2f);
		m_ppObjects[10] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(boxMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pBoxTexture);
		pObject->SetPosition(-200.0f, 3293.0f, -300.0f);
		pObject->Scale(1.2f);
		m_ppObjects[11] = pObject;


		//파이프(10번 건물)
		pObject = new CGameObject();
		pObject->SetMesh(pipeMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pPipeTexture);
		pObject->SetPosition(-680.0f, 3290.0f, -140.0f);
		pObject->Scale(0.4f);
		m_ppObjects[12] = pObject;

		//상자(12번 건물)
		pObject = new CGameObject();
		pObject->SetMesh(boxMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pBoxTexture);
		pObject->SetPosition(-1600.0f, 3293.0f, -1000.0f);
		pObject->Scale(1.2f);
		m_ppObjects[13] = pObject;


		//실외기 (13번 건물)
		pObject = new CGameObject();
		pObject->SetMesh(ConditionerMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(ConditionerTexture);
		pObject->SetPosition(-1875.0f, 3280.0f, -1630.0f);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), 90.0f);
		pObject->Scale(0.3f);
		m_ppObjects[14] = pObject;


		//상자( 13번 건물)
		pObject = new CGameObject();
		pObject->SetMesh(boxMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pBoxTexture);
		pObject->SetPosition(-1555.0f, 3293.0f, -2000.0f);
		pObject->Scale(1.2f);
		m_ppObjects[15] = pObject;


		//상자 (15번건물)
		pObject = new CGameObject();
		pObject->SetMesh(boxMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pBoxTexture);
		pObject->SetPosition(-1005.0f, 3293.0f, -2800.0f);
		pObject->Scale(1.2f);
		m_ppObjects[16] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(boxMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pBoxTexture);
		pObject->SetPosition(-660.0f, 3293.0f, -2800.0f);
		pObject->Scale(1.2f);
		m_ppObjects[17] = pObject;


		//상자 (17번 건물)
		pObject = new CGameObject();
		pObject->SetMesh(boxMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pBoxTexture);
		pObject->SetPosition(-1660.0f, 3293.0f, -3630.0f);
		pObject->Scale(1.2f);
		m_ppObjects[18] = pObject;


		//파이프(20번 건물)
		pObject = new CGameObject();
		pObject->SetMesh(pipeMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pPipeTexture);
		pObject->SetPosition(-1100.0f, 3290.0f, -5500.0f);
		pObject->Scale(0.4f);
		m_ppObjects[19] = pObject;


		//실외기 (21번 건물)
		pObject = new CGameObject();
		pObject->SetMesh(ConditionerMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(ConditionerTexture);
		pObject->SetPosition(-280.0f, 3280.0f, -5780.0f);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), 90.0f);
		pObject->Scale(0.3f);
		m_ppObjects[20] = pObject;


		//파이프 23번 건물
		pObject = new CGameObject();
		pObject->SetMesh(pipeMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pPipeTexture);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), 90.0f);
		pObject->SetPosition(0.0f, 3290.0f, -7000.0f);
		pObject->Scale(0.4f);
		m_ppObjects[21] = pObject;

		// 상자(24번 건물)
		pObject = new CGameObject();
		pObject->SetMesh(boxMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pBoxTexture);
		pObject->SetPosition(-5.0f, 3293.0f, -8155.0f);
		pObject->Scale(1.2f);
		m_ppObjects[22] = pObject;

		//상자 (25번 건물)
		pObject = new CGameObject();
		pObject->SetMesh(boxMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pBoxTexture);
		pObject->SetPosition(-865.0f, 3293.0f, -7355.0f);
		pObject->Scale(1.2f);
		m_ppObjects[23] = pObject;

		//파이프 26번 건물
		pObject = new CGameObject();
		pObject->SetMesh(pipeMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pPipeTexture);
		pObject->SetPosition(-1065.0f, 3290.0f, -8155.0f);
		pObject->Scale(0.4f);
		m_ppObjects[24] = pObject;


		//실외기 27번 건물
		pObject = new CGameObject();
		pObject->SetMesh(ConditionerMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(ConditionerTexture);
		pObject->SetPosition(-1875.0f, 3280.0f, -6960.0f);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), 90.0f);
		pObject->Scale(0.3f);
		m_ppObjects[25] = pObject;

		//박스 28번 건물
		pObject = new CGameObject();
		pObject->SetMesh(boxMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(pBoxTexture);
		pObject->SetPosition(-1665.0f, 3293.0f, -8265.0f);
		pObject->Scale(1.2f);
		m_ppObjects[26] = pObject;


	}

	CreateShaderVariables(pd3dDevice);
	delete[] ppMaterials;
}

void CItemShader::ReleaseObjects()
{
	CTextureShader::ReleaseObjects();
}

void CItemShader::Render(ID3D11DeviceContext * pd3dDeviceContext, CCamera * pCamera, int index)
{

	CTextureShader::Render(pd3dDeviceContext, pCamera);

}



CItemShader_Alpha::CItemShader_Alpha()
{
}

CItemShader_Alpha::~CItemShader_Alpha()
{
}

void CItemShader_Alpha::CreateShader(ID3D11Device * pd3dDevice)
{

	CShader::CreateShader(pd3dDevice);
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXNUM", 0, DXGI_FORMAT_R32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"Effect.fx", "VSTexturedUVWLighting", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"Effect.fx", "PSTexturedUVWLightingAlpha", "ps_4_0", &m_pd3dPixelShader);
}

void CItemShader_Alpha::CreateShaderVariables(ID3D11Device * pd3dDevice)
{
	CTextureShader::CreateShaderVariables(pd3dDevice);

}

void CItemShader_Alpha::UpdateShaderVariables(ID3D11DeviceContext * pd3dDeviceContext, D3DXMATRIX * pd3dxmtxWorld)
{
	CTextureShader::UpdateShaderVariables(pd3dDeviceContext, pd3dxmtxWorld);

}

void CItemShader_Alpha::UpdateShaderVariables(ID3D11DeviceContext * pd3dDeviceContext, MATERIAL * pMaterial)
{
	CTextureShader::UpdateShaderVariables(pd3dDeviceContext, pMaterial);

}

void CItemShader_Alpha::UpdateShaderVariables(ID3D11DeviceContext * pd3dDeviceContext, CTexture * pTexture)
{
	CTextureShader::UpdateShaderVariables(pd3dDeviceContext, pTexture);

}

void CItemShader_Alpha::BuildObjects(ID3D11Device * pd3dDevice, int mapNumber)
{

	CMaterial **ppMaterials = new CMaterial*[1];
	ppMaterials[0] = new CMaterial();
	ppMaterials[0]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcSpecular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	ID3D11SamplerState *pd3dSamplerState = NULL;
	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);

	ID3D11ShaderResourceView *pd3dTexture = NULL;


	CGameObject *pObject = NULL;


	CMesh *pMesh = new CItemMesh(pd3dDevice, "fence.src", true);;

	CTexture* fence1Texture = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\Texture\\fence_1.png"), NULL, NULL, &pd3dTexture, NULL);
	fence1Texture->SetTexture(0, pd3dTexture, pd3dSamplerState);

	CTexture* fence1Alpha = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\Texture\\fence_alpha.png"), NULL, NULL, &pd3dTexture, NULL);
	fence1Alpha->SetTexture(0, pd3dTexture, pd3dSamplerState);

	CTexture* fence2Texture = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\Texture\\fence_2.png"), NULL, NULL, &pd3dTexture, NULL);
	fence2Texture->SetTexture(0, pd3dTexture, pd3dSamplerState);

	CTexture* fence2Alpha = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\texture\\fence_2_alpha.png"), NULL, NULL, &pd3dTexture, NULL);
	fence2Alpha->SetTexture(0, pd3dTexture, pd3dSamplerState);

	if (mapNumber == 1)
	{


		m_nObjects = 21;
		m_ppObjects = new CGameObject*[m_nObjects];

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(fence2Texture);
		pObject->SetPosition(255.0f, 3270.0f, 2000.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetAlphaMap(fence2Alpha);

		pObject->Scale(2.0f);

		m_ppObjects[0] = pObject;



		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(fence1Texture);
		pObject->SetPosition(120.0f, 3270.0f, 2000.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[1] = pObject;



		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(fence1Texture);
		pObject->SetPosition(390.0f, 3270.0f, 2000.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[2] = pObject;


		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(fence1Texture);
		pObject->SetPosition(525.0f, 3270.0f, 2000.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);

		m_ppObjects[3] = pObject;


		//300.0f, 3260.0f, 3100.0f
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(fence1Texture);
		pObject->SetPosition(200.0f, 3270.0f, 3100.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);

		m_ppObjects[4] = pObject;


		//300.0f, 3260.0f, 3100.0f
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(fence1Texture);
		pObject->SetPosition(395.0f, 3270.0f, 3100.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);

		m_ppObjects[5] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);

		pObject->SetPosition(530.0f, 3270.0f, 3100.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->Scale(2.0f);

		m_ppObjects[6] = pObject;


		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->SetPosition(65.0f, 3270.0f, 3100.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->Scale(2.0f);

		m_ppObjects[7] = pObject;


		//두번째 구멍 펜스
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(fence2Texture);
		pObject->SetPosition(955.0f, 3230.0f, 1125.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetAlphaMap(fence2Alpha);
		pObject->Scale(2.0f);

		m_ppObjects[8] = pObject;



		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->SetPosition(825.0f, 3230.0f, 1125.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->Scale(2.0f);

		m_ppObjects[9] = pObject;



		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetPosition(695.0f, 3230.0f, 1125.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);

		m_ppObjects[10] = pObject;



		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);

		pObject->SetPosition(1085.0f, 3230.0f, 1125.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);


		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);

		m_ppObjects[11] = pObject;


		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);

		pObject->SetPosition(1215.0f, 3230.0f, 1125.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);


		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);

		m_ppObjects[12] = pObject;



		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);

		pObject->SetPosition(1695.0f, 2985.0f, -745.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);


		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);

		m_ppObjects[13] = pObject;


		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);

		pObject->SetPosition(1375.0f, 2985.0f, -745.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);


		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);

		m_ppObjects[14] = pObject;



		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);

		pObject->SetPosition(1505.0f, 2985.0f, -745.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);


		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);

		m_ppObjects[15] = pObject;


		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);

		pObject->SetPosition(1825.0f, 2985.0f, -745.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);


		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);

		m_ppObjects[16] = pObject;



		// 마지막 구멍뚫린 철조망
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);

		pObject->SetPosition(1597.5f, 2823.0f, -1670.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);


		pObject->SetTexture(fence2Texture);
		pObject->SetAlphaMap(fence2Alpha);
		pObject->Scale(2.0f);

		m_ppObjects[17] = pObject;

		// 그 옆들
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);

		pObject->SetPosition(1467.5f, 2823.0f, -1670.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);


		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);

		m_ppObjects[18] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);

		pObject->SetPosition(1727.5f, 2823.0f, -1670.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);


		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);

		m_ppObjects[19] = pObject;


		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);

		pObject->SetPosition(1857.5f, 2823.0f, -1670.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);


		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);

		m_ppObjects[20] = pObject;


	}
	else
	{
		m_nObjects = 47;
		m_ppObjects = new CGameObject*[m_nObjects];

		//구멍뚫린 펜스 (2번 건물)
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetPosition(-800.0f, 3280.0f, 3380.0f);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetTexture(fence2Texture);
		pObject->SetAlphaMap(fence2Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[0] = pObject;

		//막힌 펜스 (2번 건물)
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-800.0f, 3280.0f, 3510.0f);
		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[1] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-800.0f, 3280.0f, 3640.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[2] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-800.0f, 3280.0f, 3250.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[3] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-800.0f, 3280.0f, 3120.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[4] = pObject;



		// 6번 건물 펜스
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetPosition(-800.0f, 3280.0f, 1620.0f);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetTexture(fence2Texture);
		pObject->SetAlphaMap(fence2Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[5] = pObject;

		//막힌 펜스 (6번 건물)
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-800.0f, 3280.0f, 1750.0f);
		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[6] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-800.0f, 3280.0f, 1880.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[7] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-800.0f, 3280.0f, 1490.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[8] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-800.0f, 3280.0f, 1360.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[9] = pObject;



		// 10번 건물 펜스
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetPosition(-900.0f, 3280.0f, -150.0f);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetTexture(fence2Texture);
		pObject->SetAlphaMap(fence2Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[10] = pObject;

		//막힌 펜스 (10번 건물)
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-900.0f, 3280.0f, -20.0f);
		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[11] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-900.0f, 3280.0f, 110.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[12] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-900.0f, 3280.0f, -280.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[13] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-900.0f, 3280.0f, -410.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[14] = pObject;





		// 16번 건물 펜스
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetPosition(-850.0f, 3280.0f, -3700.0f);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetTexture(fence2Texture);
		pObject->SetAlphaMap(fence2Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[15] = pObject;

		//막힌 펜스 (16번 건물)
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-720.0f, 3280.0f, -3700.0f);
		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[16] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-590.0f, 3280.0f, -3700.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[17] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-980.0f, 3280.0f, -3700.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[18] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-1110.0f, 3280.0f, -3700.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[19] = pObject;




		// 25번 건물 펜스
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetPosition(-700.0f, 3280.0f, -7260.0f);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetTexture(fence2Texture);
		pObject->SetAlphaMap(fence2Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[20] = pObject;

		//막힌 펜스 (25번 건물)
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-700.0f, 3280.0f, -7130.0f);
		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[21] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-700.0f, 3280.0f, -7000.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[22] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-700.0f, 3280.0f, -7390.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[23] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-700.0f, 3280.0f, -7520.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[24] = pObject;




		// 27번 건물 펜스
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetPosition(-1670.0f, 3280.0f, -7380.0f);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetTexture(fence2Texture);
		pObject->SetAlphaMap(fence2Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[25] = pObject;

		//막힌 펜스 (27번 건물)
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-1540.0f, 3280.0f, -7380.0f);
		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[26] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-1410.0f, 3280.0f, -7380.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[27] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-1800.0f, 3280.0f, -7380.0f);


		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[28] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-1930.0f, 3280.0f, -7380.0f);


		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[29] = pObject;




		//3번 건물 문 옆 펜스
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-1795.0f, 3280.0f, 3170.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[30] = pObject;


		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-1925.0f, 3280.0f, 3170.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[31] = pObject;


		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-1605.0f, 3280.0f, 3170.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[32] = pObject;


		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-1475.0f, 3280.0f, 3170.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[33] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-1345.0f, 3280.0f, 3170.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[34] = pObject;



		//11번 건물 문옆 펜스
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-1745.0f, 3280.0f, -370.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[35] = pObject;


		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-1875.0f, 3280.0f, -370.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[36] = pObject;


		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-1555.0f, 3280.0f, -370.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[37] = pObject;


		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-1425.0f, 3280.0f, -370.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[38] = pObject;


		//18번 건물 문 옆 펜스들
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-1745.0f, 3280.0f, -4800.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[39] = pObject;


		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-1875.0f, 3280.0f, -4800.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[40] = pObject;


		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-1555.0f, 3280.0f, -4800.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[41] = pObject;


		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		//pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-1425.0f, 3280.0f, -4800.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[42] = pObject;




		//26번 건물 문 옆 펜스들
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-1000.0f, 3280.0f, -8245.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[43] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-1000.0f, 3280.0f, -8375.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[44] = pObject;


		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-1000.0f, 3280.0f, -8055.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[45] = pObject;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), -90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->SetPosition(-1000.0f, 3280.0f, -7925.0f);

		pObject->SetTexture(fence1Texture);
		pObject->SetAlphaMap(fence1Alpha);
		pObject->Scale(2.0f);
		m_ppObjects[46] = pObject;
	}


	CreateShaderVariables(pd3dDevice);
	delete[] ppMaterials;
}

void CItemShader_Alpha::ReleaseObjects()
{
	CTextureShader::ReleaseObjects();
}

void CItemShader_Alpha::Render(ID3D11DeviceContext * pd3dDeviceContext, CCamera * pCamera, int index)
{

	CTextureShader::Render(pd3dDeviceContext, pCamera);

}


CItemShader_Door::CItemShader_Door()
{
}

CItemShader_Door::~CItemShader_Door()
{
}

void CItemShader_Door::CreateShader(ID3D11Device * pd3dDevice)
{

	CShader::CreateShader(pd3dDevice);
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXNUM", 0, DXGI_FORMAT_R32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"Effect.fx", "VSTexturedUVWLightingDoor", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"Effect.fx", "PSTexturedUVWLighting", "ps_4_0", &m_pd3dPixelShader);
}

void CItemShader_Door::CreateShaderVariables(ID3D11Device * pd3dDevice)
{
	CTextureShader::CreateShaderVariables(pd3dDevice);

}

void CItemShader_Door::UpdateShaderVariables(ID3D11DeviceContext * pd3dDeviceContext, D3DXMATRIX * pd3dxmtxWorld)
{
	CTextureShader::UpdateShaderVariables(pd3dDeviceContext, pd3dxmtxWorld);

}

void CItemShader_Door::UpdateShaderVariables(ID3D11DeviceContext * pd3dDeviceContext, MATERIAL * pMaterial)
{
	CTextureShader::UpdateShaderVariables(pd3dDeviceContext, pMaterial);

}

void CItemShader_Door::UpdateShaderVariables(ID3D11DeviceContext * pd3dDeviceContext, CTexture * pTexture)
{
	CTextureShader::UpdateShaderVariables(pd3dDeviceContext, pTexture);

}

void CItemShader_Door::BuildObjects(ID3D11Device * pd3dDevice, int mapNumber)
{

	CMaterial **ppMaterials = new CMaterial*[1];
	ppMaterials[0] = new CMaterial();
	ppMaterials[0]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcSpecular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	ID3D11SamplerState *pd3dSamplerState = NULL;
	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);

	ID3D11ShaderResourceView *pd3dTexture = NULL;

	CTexture*  p_Texture = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\Texture\\pipe_1.jpg"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(0, pd3dTexture, pd3dSamplerState);

	CMesh *pMesh = new CItemMesh(pd3dDevice, "door.src", true);


	

	if (mapNumber == 1)
	{
		m_nObjects = 2;
		m_ppObjects = new CGameObject*[m_nObjects];

		CGameObject *pObject = NULL;

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(p_Texture);
		pObject->SetPosition(297.0f, 3260.0f, 3100.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->Scale(0.5f);

		m_ppObjects[0] = pObject;


		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(p_Texture);
		pObject->SetPosition(1597.0f, 2970.0f, -750.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->Scale(0.5f);
		m_ppObjects[1] = pObject;

	}
	else
	{
		m_nObjects = 4;
		m_ppObjects = new CGameObject*[m_nObjects];

		CGameObject *pObject = NULL;

		//3번 건물 문
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(p_Texture);
		pObject->SetPosition(-1700.0f, 3280.0f, 3170.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->Scale(0.5f);
		m_ppObjects[0] = pObject;


		//11번 건물 문

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(p_Texture);
		pObject->SetPosition(-1650.0f, 3280.0f, -370.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->Scale(0.5f);
		m_ppObjects[1] = pObject;



		//18번 건물 문

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(p_Texture);
		pObject->SetPosition(-1650.0f, 3280.0f, -4800.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->Scale(0.5f);
		m_ppObjects[2] = pObject;


		//26번 건물 문

		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetMaterial(ppMaterials[0]);
		pObject->SetTexture(p_Texture);
		pObject->SetPosition(-1000.0f, 3280.0f, -8150.0f);
		pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), 90.0f);
		pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
		pObject->Scale(0.5f);
		m_ppObjects[3] = pObject;
	}


	CreateShaderVariables(pd3dDevice);
	delete[] ppMaterials;
}

void CItemShader_Door::ReleaseObjects()
{
	CTextureShader::ReleaseObjects();
}

void CItemShader_Door::Render(ID3D11DeviceContext * pd3dDeviceContext, CCamera * pCamera, int index)
{
	//int n = 0;
	for (int i = 0; i < m_nObjects; ++i)
	{
		if (m_ppObjects[i]->bInteracted && m_ppObjects[i]->fAngeYaw >= -90.0f)
		{
			m_ppObjects[i]->MoveStrafe(30.0f);
			m_ppObjects[i]->Rotate(&D3DXVECTOR3(0.0f, 0.0f, 1.0f), -5.0f);
			m_ppObjects[i]->MoveStrafe(-30.0f);
		}
	}

	CTextureShader::Render(pd3dDeviceContext, pCamera, index);

}

void CUIShader::CreateShader(ID3D11Device * pd3dDevice)
{
	CShader::CreateShader(pd3dDevice);
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR0", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"Effect.fx", "VS_UI", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"Effect.fx", "PS_UI", "ps_4_0", &m_pd3dPixelShader);
}

void CUIShader::CreateShaderVariables(ID3D11Device * pd3dDevice)
{
	CShader::CreateShaderVariables(pd3dDevice);
}

void CUIShader::BuildObjects(ID3D11Device * pd3dDevice, int mapNumber)
{
	D3DXMatrixIdentity(&mtxWorld);
	m_pUI = new CUIClass(pd3dDevice, mapNumber);
	CreateShaderVariables(pd3dDevice);
}

void CUIShader::ReleaseObjects()
{
	delete m_pUI;
}

void CUIShader::Render(ID3D11DeviceContext * pd3dDeviceContext, CCamera * pCamera, int index)
{
	pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);
	//D3DXMATRIX matrix;// = pCamera->GetCameraWorldMatrix();
	if (pCamera)
	{
		D3DXMATRIX matrix;
		CPlayer *player = pCamera->GetPlayer();
		float len = DISTANCE(m_pUI->STARTPOINT_X, m_pUI->STARTPOINT_Z, m_pUI->ENDPOINT_X, m_pUI->ENDPOINT_Z);
		float myLen = GETDISTANCE(player->GetPosition().x, player->GetPosition().z, m_pUI->ENDPOINT_X, m_pUI->ENDPOINT_Z);

		float ratio = myLen / len;

		float bar = m_pUI->GetBarLength();

		ratio = bar*(1.0f - ratio);

		D3DXMATRIX move;
		D3DXMatrixTranslation(&move, ratio, 0.0f, 0.0f);

		matrix = pCamera->GetCameraWorldMatrix();
		UpdateShaderVariables(pd3dDeviceContext, &matrix);

		m_pUI->RenderBar(pd3dDeviceContext);
		matrix = move*matrix;
		UpdateShaderVariables(pd3dDeviceContext, &matrix);

		m_pUI->RenderMarker(pd3dDeviceContext);

	}


}
