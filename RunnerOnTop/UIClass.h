#pragma once
#include"stdafx.h"


float DISTANCE(float STARTPOINT_X, float STARTPOINT_Z, float ENDPOINT_X, float ENDPOINT_Z);
float GETDISTANCE(float x, float z, float ENDPOINT_X, float ENDPOINT_Z);
struct UIVertex {
	D3DXVECTOR3 position;
	D3DXCOLOR color;
};

class CUIClass
{
	// 바
	ID3D11Buffer *m_pd3dVertexBuffer;

	// 위치
	ID3D11Buffer *m_pd3dVertexBuffer_Circle;

	
	//레스터라이저
	ID3D11RasterizerState *m_pd3dRasterizerState;

	// 내 위치 이동지점
	D3DXMATRIX *m_pmoveMatrix;

	UINT m_nStride;			//바이트수
	UINT m_nOffset;
	UINT m_nVertices;

	D3D11_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology;
public:


	float STARTPOINT_X;
	float STARTPOINT_Z;
	float ENDPOINT_X  ;
	float ENDPOINT_Z ;



	CUIClass(ID3D11Device *pd3dDevice, int mapNumber = 1);
	~CUIClass();

	float GetBarLength() { return 600.0f; }
	
	void RenderBar(ID3D11DeviceContext *pd3dDeviceContext);
	void RenderMarker(ID3D11DeviceContext *pd3dDeviceContext);
};

