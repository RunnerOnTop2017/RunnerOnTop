#pragma once
#include"stdafx.h"

#define STARTPOINT_X  320.0f
#define STARTPOINT_Z  3480.0f
#define ENDPOINT_X  1617.0f
#define ENDPOINT_Z -2584.0f

#define DISTANCE abs(((ENDPOINT_X - STARTPOINT_X)*(ENDPOINT_X - STARTPOINT_X)) + ((ENDPOINT_Z -STARTPOINT_Z ) * (ENDPOINT_Z -STARTPOINT_Z )))
#define GETDISTANCE(x,z)  abs(((ENDPOINT_X - x)*(ENDPOINT_X - x)) + ((ENDPOINT_Z -z ) * (ENDPOINT_Z -z )))

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
	CUIClass(ID3D11Device *pd3dDevice);
	~CUIClass();

	float GetBarLength() { return 600.0f; }
	
	void RenderBar(ID3D11DeviceContext *pd3dDeviceContext);
	void RenderMarker(ID3D11DeviceContext *pd3dDeviceContext);
};

