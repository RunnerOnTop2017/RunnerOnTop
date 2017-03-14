#pragma once

#include "stdafx.h"
#include "Mesh.h"


CMesh::CMesh() {
	m_pd3dRasterizerState = NULL;
	m_pd3dIndexBuffer = NULL;
	m_nIndices = 0;
	m_nStartIndex = 0;
	m_nBaseVertex = 0;
}

CMesh::CMesh(ID3D11Device *pd3dDevice)
{
	m_nStride = sizeof(CVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_nReferences = 1;
	m_pd3dRasterizerState = NULL;

	m_pd3dIndexBuffer = NULL;
	m_nIndices = 0;
	m_nStartIndex = 0;
	m_nBaseVertex = 0;

}

CMesh::~CMesh()
{
	if (m_pd3dVertexBuffer) m_pd3dVertexBuffer->Release();
	if (m_pd3dRasterizerState) m_pd3dRasterizerState->Release();
	if (m_pd3dIndexBuffer) m_pd3dIndexBuffer->Release();
}
void CMesh::CreateRasterizerState(ID3D11Device *pd3dDevice)
{
}

void CMesh::AddRef()
{
	m_nReferences++;
}

void CMesh::Release()
{
	m_nReferences--;
	if (m_nReferences == 0)delete this;
}

void CMesh::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (m_pd3dVertexBuffer) pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dVertexBuffer, &m_nStride, &m_nOffset);
	if (m_pd3dIndexBuffer) pd3dDeviceContext->IASetIndexBuffer(m_pd3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	pd3dDeviceContext->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	//래스터라이저 상태를 디바이스 컨텍스트에 설정한다.
	if (m_pd3dRasterizerState) pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);

	if (m_pd3dIndexBuffer)
		pd3dDeviceContext->DrawIndexed(m_nIndices, m_nStartIndex, m_nBaseVertex);
	else
		pd3dDeviceContext->Draw(m_nVertices, m_nOffset);
}


CAirplaneMesh::CAirplaneMesh(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth, D3DXCOLOR d3dxColor) : CMesh(pd3dDevice)
{
	m_nVertices = 24 * 3;
	m_nStride = sizeof(CDiffusedVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	CDiffusedVertex pVertices[24 * 3];

	float fx = fWidth * 0.5f, fy = fHeight * 0.5f, fz = fDepth * 0.5f;
	float x1 = fx * 0.2f, y1 = fy * 0.2f, x2 = fx * 0.1f, y3 = fy * 0.3f, y2 = ((y1 - (fy - y3)) / x1)*x2 + (fy - y3);
	int i = 0;

	//비행기 메쉬의 정점 위치를 설정한다.
	//비행기 메쉬의 위쪽 평면
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), -fz, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(+x1, -y1, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), -fz, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, -fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+x2, +y2, -fz, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(+fx, -y3, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, -fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x2, +y2, -fz, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(-x1, -y1, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, -fz, RANDOM_COLOR);

	//비행기 메쉬의 아래쪽 평면
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(-x1, -y1, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+x2, +y2, +fz, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(+x1, -y1, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x2, +y2, +fz, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(-fx, -y3, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, +fz, RANDOM_COLOR);

	//비행기 메쉬의 오른쪽 평면
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x2, +y2, -fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+x2, +y2, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x2, +y2, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+x2, +y2, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x2, +y2, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, -fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+fx, -y3, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x2, +y2, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, +fz, RANDOM_COLOR);

	//비행기 메쉬의 뒤쪽/오른쪽 평면
	pVertices[i++] = CDiffusedVertex(+x1, -y1, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+x1, -y1, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x2, +y2, -fz, RANDOM_COLOR);

	//비행기 메쉬의 왼쪽 평면
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x2, +y2, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x2, +y2, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x2, +y2, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x2, +y2, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, -fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x2, +y2, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, +fz, RANDOM_COLOR);

	//비행기 메쉬의 뒤쪽/왼쪽 평면
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, -fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x1, -y1, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x1, -y1, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, -fz, RANDOM_COLOR);

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(CDiffusedVertex)* m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	CreateRasterizerState(pd3dDevice);
}

CAirplaneMesh::~CAirplaneMesh()
{
}

void CAirplaneMesh::CreateRasterizerState(ID3D11Device *pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	d3dRasterizerDesc.DepthClipEnable = true;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

void CAirplaneMesh::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CMesh::Render(pd3dDeviceContext);
}



CMeshIlluminated::CMeshIlluminated(ID3D11Device *pd3dDevice) : CMesh(pd3dDevice)
{
}

CMeshIlluminated::~CMeshIlluminated()
{
}

void CMeshIlluminated::CalculateVertexNormal(BYTE *pVertices, WORD *pIndices)
{
	switch (m_d3dPrimitiveTopology)
	{
		/*프리미티브가 삼각형 리스트일 때 인덱스 버퍼가 있는 경우와 없는 경우를 구분하여 정점의 법선 벡터를 계산한다. 인덱스 버퍼를 사용하지 않는 경우 각 정점의 법선 벡터는 그 정점이 포함된 삼각형의 법선 벡터로 계산한다. 인덱스 버퍼를 사용하는 경우 각 정점의 법선 벡터는 그 정점이 포함된 삼각형들의 법선 벡터의 평균으로(더하여) 계산한다.*/
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		if (!pIndices)
			SetTriAngleListVertexNormal(pVertices);
		else
			SetAverageVertexNormal(pVertices, pIndices, (m_nIndices / 3), 3, false);
		break;
		/*프리미티브가 삼각형 스트립일 때 각 정점의 법선 벡터는 그 정점이 포함된 삼각형들의 법선 벡터의 평균으로(더하여) 계산한다.*/
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		SetAverageVertexNormal(pVertices, pIndices, (pIndices) ? (m_nIndices - 2) : (m_nVertices - 2), 1, true);
		break;
	default:
		break;
	}
}

void CMeshIlluminated::SetTriAngleListVertexNormal(BYTE *pVertices)
{
	D3DXVECTOR3 d3dxvNormal;
	CNormalVertex *pVertex = NULL;
	/*삼각형(프리미티브)의 개수를 구하고 각 삼각형의 법선 벡터를 계산하고 삼각형을 구성하는 각 정점의 법선 벡터로 지정한다.*/
	int nPrimitives = m_nVertices / 3;
	for (int i = 0; i < nPrimitives; i++)
	{
		d3dxvNormal = CalculateTriAngleNormal(pVertices, (i * 3 + 0), (i * 3 + 1), (i * 3 + 2));
		pVertex = (CNormalVertex *)(pVertices + ((i * 3 + 0) * m_nStride));
		pVertex->m_d3dxvNormal = d3dxvNormal;
		pVertex = (CNormalVertex *)(pVertices + ((i * 3 + 1) * m_nStride));
		pVertex->m_d3dxvNormal = d3dxvNormal;
		pVertex = (CNormalVertex *)(pVertices + ((i * 3 + 2) * m_nStride));
		pVertex->m_d3dxvNormal = d3dxvNormal;
	}
}

D3DXVECTOR3 CMeshIlluminated::CalculateTriAngleNormal(BYTE *pVertices, USHORT nIndex0, USHORT nIndex1, USHORT nIndex2)
{
	D3DXVECTOR3 d3dxvNormal(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 d3dxvP0 = *((D3DXVECTOR3 *)(pVertices + (m_nStride * nIndex0)));
	D3DXVECTOR3 d3dxvP1 = *((D3DXVECTOR3 *)(pVertices + (m_nStride * nIndex1)));
	D3DXVECTOR3 d3dxvP2 = *((D3DXVECTOR3 *)(pVertices + (m_nStride * nIndex2)));
	D3DXVECTOR3 d3dxvEdge1 = d3dxvP1 - d3dxvP0;
	D3DXVECTOR3 d3dxvEdge2 = d3dxvP2 - d3dxvP0;
	D3DXVec3Cross(&d3dxvNormal, &d3dxvEdge1, &d3dxvEdge2);
	D3DXVec3Normalize(&d3dxvNormal, &d3dxvNormal);
	return(d3dxvNormal);
}

/*프리미티브가 인덱스 버퍼를 사용하는 삼각형 리스트 또는 삼각형 스트립인 경우 정점의 법선 벡터는 그 정점을 포함하는 삼각형의 법선 벡터들의 평균으로 계산한다.*/
void CMeshIlluminated::SetAverageVertexNormal(BYTE *pVertices, WORD *pIndices, int nPrimitives, int nOffset, bool bStrip)
{
	D3DXVECTOR3 d3dxvSumOfNormal(0.0f, 0.0f, 0.0f);
	CNormalVertex *pVertex = NULL;
	USHORT nIndex0, nIndex1, nIndex2;

	for (UINT j = 0; j < m_nVertices; j++)
	{
		d3dxvSumOfNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < nPrimitives; i++)
		{
			nIndex0 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 0) : (i*nOffset + 1)) : (i*nOffset + 0);
			if (pIndices) nIndex0 = pIndices[nIndex0];
			nIndex1 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 1) : (i*nOffset + 0)) : (i*nOffset + 1);
			if (pIndices) nIndex1 = pIndices[nIndex1];
			nIndex2 = (pIndices) ? pIndices[i*nOffset + 2] : (i*nOffset + 2);
			if ((nIndex0 == j) || (nIndex1 == j) || (nIndex2 == j)) d3dxvSumOfNormal += CalculateTriAngleNormal(pVertices, nIndex0, nIndex1, nIndex2);
		}
		D3DXVec3Normalize(&d3dxvSumOfNormal, &d3dxvSumOfNormal);
		pVertex = (CNormalVertex *)(pVertices + (j * m_nStride));
		pVertex->m_d3dxvNormal = d3dxvSumOfNormal;
	}
}

void CMeshIlluminated::Render(ID3D11DeviceContext *pd3dImmediateDeviceContext)
{
	CMesh::Render(pd3dImmediateDeviceContext);
}

CMeshTextured::CMeshTextured(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth) : CMeshIlluminated(pd3dDevice)
{
	//m_nVertices;
	m_nStride = sizeof(CTexturedNormalVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	FILE *fp = fopen("Data\\Maps\\maps1_s1.arc", "rb");
	int size;
	fread((char*)&size, sizeof(int), 1, fp);
	CTexturedNormalVertex *pVertices = new CTexturedNormalVertex[size];
	fread((char*)pVertices, sizeof(CTexturedNormalVertex), size, fp);
	fclose(fp);

	m_nVertices = size;

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = m_nStride * m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);


	SetRasterizerState(pd3dDevice);
}

CMeshTextured::~CMeshTextured()
{
}

void CMeshTextured::SetRasterizerState(ID3D11Device *pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_FRONT;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

void CMeshTextured::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CMeshIlluminated::Render(pd3dDeviceContext);
}

CCharacterMesh::CCharacterMesh(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth) : CMeshIlluminated(pd3dDevice)
{
	m_nStride = sizeof(CSkinnedVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	AnimaionCount = 0;

	m_nIndices;
	int indexSize = 0;
	FILE *fp = fopen("Data\\character\\Police01.msh", "rb");
	fread((char*)&indexSize, sizeof(int), 1, fp);
	CSkinnedVertex *pVertices = new CSkinnedVertex[indexSize];
	fread((char*)pVertices, sizeof(CSkinnedVertex), indexSize, fp);
	fclose(fp);

	m_nIndices = indexSize;

	fp = fopen("Data\\Animation\\run.anm", "rb");
	int BoneSIze, matrixSize;
	fread((char*)&BoneSIze, sizeof(int), 1, fp);
	fread((char*)&matrixSize, sizeof(int), 1, fp);
	for (int i = 0; i < BoneSIze; ++i)
	{
		D3DXMATRIX* mMatrix = new D3DXMATRIX[matrixSize];
		fread((char*)mMatrix, sizeof(D3DXMATRIX), matrixSize, fp);
		mBoneData.push_back(mMatrix);
	}
	AnimSize = matrixSize;
	fclose(fp);

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = m_nStride * m_nIndices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	
	m_nVertices = m_nIndices;

	
	SetRasterizerState(pd3dDevice);

}
CCharacterMesh::~CCharacterMesh()
{

}
void CCharacterMesh::SetRasterizerState(ID3D11Device *pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

void CCharacterMesh::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CMesh::Render(pd3dDeviceContext);
}