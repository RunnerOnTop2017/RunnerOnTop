#include "stdafx.h"
#include "Scene.h"


CScene::CScene()
{
	m_ppShaders = NULL;
	m_nShaders = 0;
	//m_ppObjects = NULL;
	//m_nObjects = 0;
	m_pLights = NULL;
	m_pSkybox = NULL;
}


CScene::~CScene()
{
}


void CScene::BuildObjects(ID3D11Device *pd3dDevice)
{
	m_nShaders = 5;
	m_ppShaders = new CShader*[m_nShaders];

	// ��
	m_ppShaders[0] = new CTextureShader();
	m_ppShaders[0]->CreateShader(pd3dDevice);
	m_ppShaders[0]->BuildObjects(pd3dDevice);

	//��Ÿ ������
	m_ppShaders[1] = new CItemShader();
	m_ppShaders[1]->CreateShader(pd3dDevice);
	m_ppShaders[1]->BuildObjects(pd3dDevice);

	// ��
	m_ppShaders[2] = new CItemShader_Door();
	m_ppShaders[2]->CreateShader(pd3dDevice);
	m_ppShaders[2]->BuildObjects(pd3dDevice);

	// �ٿ���ڽ�
	m_ppShaders[3] = new CDiffusedShader();
	m_ppShaders[3]->CreateShader(pd3dDevice);
	m_ppShaders[3]->BuildObjects(pd3dDevice);

	// ������ ���ĸ�
	m_ppShaders[4] = new CItemShader_Alpha();
	m_ppShaders[4]->CreateShader(pd3dDevice);
	m_ppShaders[4]->BuildObjects(pd3dDevice);

	//��ī�̹ڽ�
	m_pSkybox = new CSkyBoxShader();
	m_pSkybox->CreateShader(pd3dDevice);
	m_pSkybox->BuildObjects(pd3dDevice);

	//UI
	m_pUIShader = new CUIShader();
	m_pUIShader->CreateShader(pd3dDevice);
	m_pUIShader->BuildObjects(pd3dDevice);

	 //��
	m_ppShaders[3]->m_ppObjects[60]->ref = m_ppShaders[2]->m_ppObjects[0];
	m_ppShaders[3]->m_ppObjects[61]->ref = m_ppShaders[2]->m_ppObjects[0];

	m_ppShaders[3]->m_ppObjects[72]->ref = m_ppShaders[2]->m_ppObjects[1];
	m_ppShaders[3]->m_ppObjects[73]->ref = m_ppShaders[2]->m_ppObjects[1];


	//�潺
	m_ppShaders[3]->m_ppObjects[65]->ref = m_ppShaders[4]->m_ppObjects[0];
	m_ppShaders[3]->m_ppObjects[66]->ref = m_ppShaders[4]->m_ppObjects[0];

	m_ppShaders[3]->m_ppObjects[77]->ref = m_ppShaders[4]->m_ppObjects[8];
	m_ppShaders[3]->m_ppObjects[78]->ref = m_ppShaders[4]->m_ppObjects[8];

	//������
	m_ppShaders[3]->m_ppObjects[70]->ref = m_ppShaders[1]->m_ppObjects[1];

	//ĳ���� ���̴�
	m_pCharacters = new CAnimateShader();
	m_pCharacters->CreateShader(pd3dDevice);
	m_pCharacters->BuildObjects(pd3dDevice);

	BuildLights(pd3dDevice);
}


void CScene::ReleaseObjects()
{
	if (m_ppShaders)
	{
		//delete m_pShaders;
		for (int i = 0; i < m_nShaders; ++i)	m_ppShaders[i]->ReleaseObjects();

		delete[]m_ppShaders;
	}

	if (m_pSkybox)
	{
		delete m_pSkybox;
	}

	
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}
bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::ProcessInput()
{
	return(false);
}

void CScene::AnimateObjects(ID3D11Device *pd3dDevice,float fTimeElapsed)
{
	for (int i = 0; i < m_nShaders; i++)
	{
		m_ppShaders[i]->AnimateObjects(pd3dDevice,fTimeElapsed);
	}
}
void CScene::Render(ID3D11DeviceContext*pd3dDeviceContext, CCamera *pCamera)
{
	if (m_pLights && m_pd3dcbLights)
	{
		/*�� ��° ������ �÷��̾ ������ �ִ� �������̴�. �׷��Ƿ� ī�޶��� ��ġ�� �ٲ�� ������ ��ġ�� ������ ī�޶��� ��ġ�� �������� �����Ѵ�.*/
		D3DXVECTOR3 d3dxvCameraPosition = pCamera->GetPosition();
		m_pLights->m_d3dxvCameraPosition = D3DXVECTOR4(d3dxvCameraPosition, 1.0f);


		
		//m_pLights->m_pLights[1].m_d3dxvPosition = d3dxvCameraPosition;
		//m_pLights->m_pLights[1].m_d3dxvDirection = pCamera->GetLookVector();

		UpdateLights(pd3dDeviceContext);
	}
	if (m_pSkybox)
	{
		m_pSkybox->Render(pd3dDeviceContext, pCamera);
	}

	

	for (int i = 0; i < m_nShaders; i++)
	{
#ifdef _DEBUG
		if(3==i)
			m_ppShaders[i]->Render(pd3dDeviceContext, pCamera);
#endif
	if (3 != i)
		m_ppShaders[i]->Render(pd3dDeviceContext, pCamera);
	}

	

}
void CScene::DrawUI(ID3D11DeviceContext * pd3dDeviceContext, CCamera *pCamera)
{
	if (m_pUIShader)
	{
		m_pUIShader->Render(pd3dDeviceContext, pCamera);
	}
}
void CScene::BuildLights(ID3D11Device *pd3dDevice)
{
	m_pLights = new LIGHTS;
	::ZeroMemory(m_pLights, sizeof(LIGHTS));
	//���� ���� ��ü�� ���ߴ� �ֺ������� �����Ѵ�.
	m_pLights->m_d3dxcGlobalAmbient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.1f);

	//3���� ����(�� ����, ���� ����, ���⼺ ����)�� �����Ѵ�.
	m_pLights->m_pLights[0].m_bEnable = 1.0f;
	m_pLights->m_pLights[0].m_nType = DIRECTIONAL_LIGHT;
	m_pLights->m_pLights[0].m_d3dxcAmbient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.3f);
	m_pLights->m_pLights[0].m_d3dxcDiffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.3f);
	m_pLights->m_pLights[0].m_d3dxcSpecular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights->m_pLights[0].m_d3dxvDirection = D3DXVECTOR3(0.0f, -1.0f, -1.0f);
	
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(LIGHTS);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = m_pLights;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dcbLights);
}

void CScene::ReleaseLights()
{
	if (m_pLights) delete m_pLights;
	if (m_pd3dcbLights) m_pd3dcbLights->Release();
}

void CScene::UpdateLights(ID3D11DeviceContext *pd3dDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbLights, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	LIGHTS *pcbLight = (LIGHTS *)d3dMappedResource.pData;
	memcpy(pcbLight, m_pLights, sizeof(LIGHTS));
	pd3dDeviceContext->Unmap(m_pd3dcbLights, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_SLOT_LIGHT, 1, &m_pd3dcbLights);
}

