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
	std::cout << "";
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

void CShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);

	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j])
		{
			UpdateShaderVariables(pd3dDeviceContext, &m_ppObjects[j]->m_d3dxmtxWorld);
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

CGameObject * CShader::GetGameObject(int index)
{
	return m_ppObjects[index];
}


CDiffusedShader::CDiffusedShader()
{
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

void CDiffusedShader::BuildObjects(ID3D11Device *pd3dDevice)
{
	
	m_nObjects = OBJECT_CNT;
	m_ppObjects = new CGameObject*[m_nObjects];
	int n = 0;
	CGameObject *pRotatingObject = new CGameObject();
	CMesh *pCubeMesh = new CCubeMesh(pd3dDevice, -3000.0f, 3000.0f, 0.0f, 50.0f, 0.0f, 3800.0f);

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

	pCubeMesh = new CCubeMesh(pd3dDevice, -55.0f, 595.0f, 3050.0f, 3275.0f, 1705.0f, 2653.0f);
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

	pCubeMesh = new CCubeMesh(pd3dDevice, 1340.0f, 1960.0f, 3038.0f, 3263.0f, 1435.0f, 1735.0f);
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

	pCubeMesh = new CCubeMesh(pd3dDevice, 625.0f, 1280.0f, 2918.0f, 3143.0f, 490.0f, 810.0f);
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
	pCubeMesh = new CCubeMesh(pd3dDevice, -35.0f, 130.0f, 3275.0f, 3321.0f, 3325.0f, 3415.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 635.0f, 810.f, 3230.0f, 3278.0f, 1340.0f, 1430.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1115.0f, 1290.0f, 3188.0f, 3234.0f, 810.0f, 900.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 635.0f, 810.0f, 3143.0f, 3189.0f, 720.0f, 810.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 635.0f, 810.0f, 3078.0f, 3124.0f, 83.0f, 173.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 635.0f, 810.0f, 3028.f, 3074.0f, -460.0f, -350.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1780.0f, 1955.0f, 2980.f, 3026.0f, -550.0f, -460.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1345.0f, 1520.0f, 2923.f, 2969.0f, -870.0f, -780.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1345.0f, 1520.0f, 2778.f, 2824.0f, -1825.0f, -1735.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1845.0f, 2020.0f, 2743.f, 2789.0f, -2355.0f, -2265.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 1345.0f, 1520.0f, 2723.f, 2769.0f, -2460.0f, -2370.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	// X축 175 Y축 46 Z축 90 (메모용)

	pCubeMesh = new CCubeMesh(pd3dDevice, 670.0f, 1325.0f, 3140.0f, 3365.0f, 2980.0f, 3605.0f);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	pCubeMesh = new CCubeMesh(pd3dDevice, 710.0f, 1220.0f, 3140.0f, 3500.0f, 2718.0f, 2955.0f);
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


	// 첫번째 문 인터랙트 영역
	pCubeMesh = new CCubeMesh(pd3dDevice, 270.0f, 330.0f, 3275.0f, 3360.0f, 3140.0f, 3180.0f, DOOR);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);

	m_ppObjects[n++] = pRotatingObject;

	// 첫번째 문 
	pCubeMesh = new CCubeMesh(pd3dDevice, 270.0f, 330.0f, 3275.0f, 3360.0f, 3100.0f, 3110.0f, REALDOOR);
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

	//구멍뚫린 펜스
	pCubeMesh = new CCubeMesh(pd3dDevice, 240.0f, 360.0f, 3275.0f, 3360.0f, 2000.0f, 2010.0f, FENCEHOLE);
	pRotatingObject = new CGameObject();

	pRotatingObject->SetMesh(pCubeMesh);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	

	m_ppObjects[n++] = pRotatingObject;


	CreateShaderVariables(pd3dDevice);
}

void CDiffusedShader::ReleaseObjects()
{
	CShader::ReleaseObjects();
}

void CDiffusedShader::AnimateObjects(ID3D11Device *pd3dDevice, float fTimeElapsed)
{
	CShader::AnimateObjects(pd3dDevice, fTimeElapsed);
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
void CTextureShader::BuildObjects(ID3D11Device * pd3dDevice)
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

	CTexture *p_Texture = new CTexture(3);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Maps\\Texture\\map01_3.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(0, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Maps\\Texture\\map01_1.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(1, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Maps\\Texture\\map01_2.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(2, pd3dTexture, pd3dSamplerState);
	
	CMesh *pMeshIlluminatedTextured = new CMeshTextured(pd3dDevice, 12.0f, 12.0f, 12.0f);//CCubeMeshIlluminatedTextured(pd3dDevice, 12.0f, 12.0f, 12.0f);

	int i = 0, nObjectTypes = 2;
	m_nObjects = 1;//((xObjects * 2) + 1) * ((yObjects * 2) + 1) * ((zObjects * 2) + 1);
	m_ppObjects = new CGameObject*[m_nObjects];
	CGameObject *pObject = NULL;
	pObject = new CGameObject();
	pObject->SetMesh(pMeshIlluminatedTextured);
	pObject->SetMaterial(ppMaterials[0]);
	pObject->SetTexture(p_Texture);

	pObject->SetPosition(0.0f, 0.0f, 0.0f);
	//pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
	//pRotatingObject->
	pObject->Scale(1000.0f);
	m_ppObjects[i++] = pObject;


	CreateShaderVariables(pd3dDevice);

	//delete[] ppTextures;
	//delete p_Texture;
	delete[] ppMaterials;
}
void CTextureShader::ReleaseObjects()
{
}

void CTextureShader::Render(ID3D11DeviceContext * pd3dDeviceContext, CCamera * pCamera)
{
	CShader::Render(pd3dDeviceContext, pCamera);
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

	ID3D11ShaderResourceView *pd3dTexture = NULL;

	CTexture *p_Texture = new CTexture(8);
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


	CTexture *pBump = new CTexture(6);
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

	CMesh *pMeshIlluminatedTextured = new CCharacterMesh(pd3dDevice, 12.0f, 12.0f, 12.0f);//CCubeMeshIlluminatedTextured(pd3dDevice, 12.0f, 12.0f, 12.0f);

	int xObjects = 3, yObjects = 3, zObjects = 3, i = 0, nObjectTypes = 2;
	m_nObjects = 1;//((xObjects * 2) + 1) * ((yObjects * 2) + 1) * ((zObjects * 2) + 1);
	m_ppObjects = new CGameObject*[m_nObjects];

	float fxPitch = 12.0f * 1.7f;
	float fyPitch = 12.0f * 1.7f;
	float fzPitch = 12.0f * 1.7f;
	CGameObject *pRotatingObject = NULL;

	pRotatingObject = new CGameObject();
	pRotatingObject->SetMesh(pMeshIlluminatedTextured);
	pRotatingObject->SetMaterial(ppMaterials[i % 3]);
	pRotatingObject->SetTexture(p_Texture);
	pRotatingObject->SetBump(pBump);
	pRotatingObject->SetPosition(0.0f, 0.0f, 0.0f);
	//pRotatingObject->SetRotationAxis(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	//pRotatingObject->SetRotationSpeed(1.0f);// *(i % 10));
	//pRotatingObject->Scale(1.0f);
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

void CItemShader::BuildObjects(ID3D11Device * pd3dDevice)
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


	m_nObjects = 2;
	m_ppObjects = new CGameObject*[m_nObjects];

	CGameObject *pObject = NULL;


	CMesh *pMesh = new CItemMesh(pd3dDevice, "conditioner.src");
	CTexture *p_Texture = new CTexture(8);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\texture\\conditioner_4.jpg"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(0, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\texture\\conditioner_2.jpg"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(1, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\texture\\conditioner_3.jpg"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(2, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\texture\\conditioner_1.jpg"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(3, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\texture\\conditioner_5.jpg"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(4, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\texture\\conditioner_6.jpg"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(5, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\texture\\conditioner_7.jpg"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(6, pd3dTexture, pd3dSamplerState);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\texture\\conditioner_8.jpg"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(7, pd3dTexture, pd3dSamplerState);

	pObject = new CGameObject();
	pObject->SetMesh(pMesh);
	pObject->SetMaterial(ppMaterials[0]);
	pObject->SetTexture(p_Texture);
	pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), 90.0f);
	pObject->SetPosition(50.0f, 3270.0f, 2500.0f);
	
	pObject->Scale(0.3f);
	m_ppObjects[0] = pObject;
	

	pMesh = new CItemMesh(pd3dDevice, "pipe.src");
	p_Texture = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\texture\\pipe_1.jpg"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(0, pd3dTexture, pd3dSamplerState);

	pObject = new CGameObject();
	pObject->SetMesh(pMesh);
	pObject->SetMaterial(ppMaterials[0]);
	pObject->SetTexture(p_Texture);
	pObject->Rotate(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), 90.0f);

	pObject->SetPosition(300.0f, 3280.0f, 3000.0f);

	pObject->Scale(0.4f);
	m_ppObjects[1] = pObject;


	CreateShaderVariables(pd3dDevice);
	delete[] ppMaterials;
}

void CItemShader::ReleaseObjects()
{
	CTextureShader::ReleaseObjects();
}

void CItemShader::Render(ID3D11DeviceContext * pd3dDeviceContext, CCamera * pCamera)
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

void CItemShader_Alpha::BuildObjects(ID3D11Device * pd3dDevice)
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


	m_nObjects = 5;
	m_ppObjects = new CGameObject*[m_nObjects];

	CGameObject *pObject = NULL;

	CTexture*  p_Texture = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\Texture\\fence_2.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(0, pd3dTexture, pd3dSamplerState);

	CMesh *pMesh = new CItemMesh(pd3dDevice, "fence.src", true);


	pObject = new CGameObject();
	pObject->SetMesh(pMesh);
	pObject->SetMaterial(ppMaterials[0]);
	pObject->SetTexture(p_Texture);
	pObject->SetPosition(300.0f, 3270.0f, 2000.0f);
	pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
	p_Texture = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\texture\\fence_2_alpha.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(0, pd3dTexture, pd3dSamplerState);
	pObject->SetAlphaMap(p_Texture);

	pObject->Scale(2.0f);

	m_ppObjects[0] = pObject;

	//300.0f, 3260.0f, 3100.0f
	pObject = new CGameObject();
	pObject->SetMesh(pMesh);
	pObject->SetMaterial(ppMaterials[0]);
	pObject->SetTexture(p_Texture);
	pObject->SetPosition(200.0f, 3270.0f, 3100.0f);
	pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
	
	p_Texture = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\Texture\\fence_1.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(0, pd3dTexture, pd3dSamplerState);

	pObject->SetTexture(p_Texture);

	CTexture* p_Texture2 = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\Texture\\fence_alpha.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture2->SetTexture(0, pd3dTexture, pd3dSamplerState);

	pObject->SetAlphaMap(p_Texture2);
	pObject->Scale(2.0f);
	
	m_ppObjects[1] = pObject;


	//300.0f, 3260.0f, 3100.0f
	pObject = new CGameObject();
	pObject->SetMesh(pMesh);
	pObject->SetMaterial(ppMaterials[0]);
	pObject->SetTexture(p_Texture);
	pObject->SetPosition(395.0f, 3270.0f, 3100.0f);
	pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);

	p_Texture = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\Texture\\fence_1.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(0, pd3dTexture, pd3dSamplerState);

	pObject->SetTexture(p_Texture);

	p_Texture2 = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\Texture\\fence_alpha.png"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture2->SetTexture(0, pd3dTexture, pd3dSamplerState);

	pObject->SetAlphaMap(p_Texture2);
	pObject->Scale(2.0f);

	m_ppObjects[2] = pObject;

	pObject = new CGameObject();
	pMesh = new CItemMesh(pd3dDevice, "fence.src", true);

	pObject->SetMesh(pMesh);
	pObject->SetMaterial(ppMaterials[0]);
	pObject->SetTexture(p_Texture);
	pObject->SetAlphaMap(p_Texture2);

	pObject->SetPosition(530.0f, 3270.0f, 3100.0f);
	pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
	pObject->Scale(2.0f);

	m_ppObjects[3] = pObject;


	pObject = new CGameObject();
	pMesh = new CItemMesh(pd3dDevice, "fence.src", true);

	pObject->SetMesh(pMesh);
	pObject->SetMaterial(ppMaterials[0]);
	pObject->SetTexture(p_Texture);
	pObject->SetAlphaMap(p_Texture2);

	pObject->SetPosition(65.0f, 3270.0f, 3100.0f);
	pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
	pObject->Scale(2.0f);

	m_ppObjects[4] = pObject;



	CreateShaderVariables(pd3dDevice);
	delete[] ppMaterials;
}

void CItemShader_Alpha::ReleaseObjects()
{
	CTextureShader::ReleaseObjects();
}

void CItemShader_Alpha::Render(ID3D11DeviceContext * pd3dDeviceContext, CCamera * pCamera)
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

void CItemShader_Door::BuildObjects(ID3D11Device * pd3dDevice)
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


	m_nObjects = 1;
	m_ppObjects = new CGameObject*[m_nObjects];
	
	CGameObject *pObject = NULL;




	CTexture*  p_Texture = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Object\\Texture\\pipe_1.jpg"), NULL, NULL, &pd3dTexture, NULL);
	p_Texture->SetTexture(0, pd3dTexture, pd3dSamplerState);

	CMesh *pMesh = new CItemMesh(pd3dDevice, "door.src", true);


	pObject = new CGameObject();
	pObject->SetMesh(pMesh);
	pObject->SetMaterial(ppMaterials[0]);
	pObject->SetTexture(p_Texture);
	pObject->SetPosition(297.0f, 3260.0f, 3100.0f);
	pObject->Rotate(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90.0f);
	pObject->Scale(0.5f);

	m_ppObjects[0] = pObject;

	CreateShaderVariables(pd3dDevice);
	delete[] ppMaterials;
}

void CItemShader_Door::ReleaseObjects()
{
	CTextureShader::ReleaseObjects();
}

void CItemShader_Door::Render(ID3D11DeviceContext * pd3dDeviceContext, CCamera * pCamera)
{
	if (m_ppObjects[0]->bInteracted && m_ppObjects[0]->fAngeYaw >= -90.0f)
	{
		m_ppObjects[0]->MoveStrafe(30.0f);
		m_ppObjects[0]->Rotate(&D3DXVECTOR3(0.0f, 0.0f, 1.0f), -5.0f);
		m_ppObjects[0]->MoveStrafe(-30.0f);
	}
	CTextureShader::Render(pd3dDeviceContext, pCamera);

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

void CUIShader::BuildObjects(ID3D11Device * pd3dDevice)
{
	D3DXMatrixIdentity(&mtxWorld);
	m_pUI = new CUIClass(pd3dDevice);
	CreateShaderVariables(pd3dDevice);
}

void CUIShader::ReleaseObjects()
{
	delete m_pUI;
}

void CUIShader::Render(ID3D11DeviceContext * pd3dDeviceContext, CCamera * pCamera)
{
	pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);
	//D3DXMATRIX matrix;// = pCamera->GetCameraWorldMatrix();
	if (pCamera)
	{
		D3DXMATRIX matrix;
		CPlayer *player = pCamera->GetPlayer();
		float len = DISTANCE;
		float myLen = GETDISTANCE(player->GetPosition().x, player->GetPosition().z);

		float ratio = myLen / len;

		float bar = m_pUI->GetBarLength();

		ratio = bar*(1.0f-ratio);

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
