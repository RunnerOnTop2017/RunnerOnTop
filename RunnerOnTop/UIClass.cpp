#include "stdafx.h"
#include "UIClass.h"


CUIClass::CUIClass(ID3D11Device *pd3dDevice)
{
	m_nVertices = 6;
	m_nStride = sizeof(UIVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UIVertex vertex[6];


	// 바
	vertex[0] = { D3DXVECTOR3(-300.0f, 300.0f, 20.0f),D3DXCOLOR(0.3f,0.3f,0.3f,1.0f) };
	vertex[1] = { D3DXVECTOR3(300.0f, 300.0f, 20.0f),D3DXCOLOR(0.3f,0.3f,0.3f,1.0f) };
	vertex[2] = { D3DXVECTOR3(-300.0f, 290.0f, 20.0f),D3DXCOLOR(0.3f,0.3f,0.3f,1.0f) };
	vertex[3] = { D3DXVECTOR3(300.0f, 300.0f, 20.0f),D3DXCOLOR(0.3f,0.3f,0.3f,1.0f) };
	vertex[4] = { D3DXVECTOR3(300.0f, 290.0f, 20.0f),D3DXCOLOR(0.3f,0.3f,0.3f,1.0f) };
	vertex[5] = { D3DXVECTOR3(-300.0f, 290.0f, 20.0f),D3DXCOLOR(0.3f,0.3f,0.3f,1.0f) };


	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(UIVertex)* m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = vertex;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);


	// 위치표시
	vertex[0] = { D3DXVECTOR3(-310.0f, 310.0f, 20.0f),D3DXCOLOR(0.47f,0.84f,0.69f,1.0f) };
	vertex[1] = { D3DXVECTOR3(-280.0f, 310.0f, 20.0f),D3DXCOLOR(0.47f,0.84f,0.69f,1.0f) };
	vertex[2] = { D3DXVECTOR3(-310.0f, 280.0f, 20.0f),D3DXCOLOR(0.47f,0.84f,0.69f,1.0f) };
	vertex[3] = { D3DXVECTOR3(-280.0f, 310.0f, 20.0f),D3DXCOLOR(0.47f,0.84f,0.69f,1.0f) };
	vertex[4] = { D3DXVECTOR3(-280.0f, 280.0f, 20.0f),D3DXCOLOR(0.47f,0.84f,0.69f,1.0f) };
	vertex[5] = { D3DXVECTOR3(-310.0f, 280.0f, 20.0f),D3DXCOLOR(0.47f,0.84f,0.69f,1.0f) };



	D3D11_BUFFER_DESC d3dBufferDesc2;
	ZeroMemory(&d3dBufferDesc2, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc2.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc2.ByteWidth = sizeof(UIVertex)* m_nVertices;
	d3dBufferDesc2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc2.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData2;
	ZeroMemory(&d3dBufferData2, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData2.pSysMem = vertex;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer_Circle);


	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.AntialiasedLineEnable = false;
	d3dRasterizerDesc.CullMode = D3D11_CULL_NONE;
	d3dRasterizerDesc.DepthBias = 0;
	d3dRasterizerDesc.DepthBiasClamp = 0.0f;
	d3dRasterizerDesc.DepthClipEnable = true;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	d3dRasterizerDesc.FrontCounterClockwise = false;
	d3dRasterizerDesc.MultisampleEnable = false;
	d3dRasterizerDesc.ScissorEnable = false;
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);

}


CUIClass::~CUIClass()
{
}

void CUIClass::RenderBar(ID3D11DeviceContext * pd3dDeviceContext)
{
	if (m_pd3dVertexBuffer) pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dVertexBuffer, &m_nStride, &m_nOffset);
	pd3dDeviceContext->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	if (m_pd3dRasterizerState) pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);

	pd3dDeviceContext->Draw(m_nVertices, m_nOffset);
}

void CUIClass::RenderMarker(ID3D11DeviceContext * pd3dDeviceContext)
{
	

	if (m_pd3dVertexBuffer_Circle) pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dVertexBuffer_Circle, &m_nStride, &m_nOffset);
	pd3dDeviceContext->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	if (m_pd3dRasterizerState) pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);

	pd3dDeviceContext->Draw(m_nVertices, m_nOffset);

}

