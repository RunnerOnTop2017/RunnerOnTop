#pragma once


#include"stdafx.h"
#include "Bone.h"
#define RANDOM_COLOR D3DXCOLOR((rand() * 0xFFFFFF) / RAND_MAX)

enum OBJECTTAG{ MAP, DOOR, CONDITIONER, FENCE, FENCEHOLE ,PIPE, REALDOOR, FALL, OBJ };

class CVertex
{
	//정점의 위치 정보(3차원 벡터)를 저장하는 멤버 변수
	D3DXVECTOR3 m_d3dxvPosition;
public:
	//생성자, 소멸자 선언
	CVertex() { m_d3dxvPosition = D3DXVECTOR3(0, 0, 0); }
	CVertex(D3DXVECTOR3 d3dxvPosition) { m_d3dxvPosition = d3dxvPosition; }
	~CVertex() {}
};

class CDiffusedVertex
{
	D3DXVECTOR3 m_d3dxvPosition;
	//정점의 색상을 나타내는 멤버 변수(D3DXCOLOR 구조체)를 선언한다. 
	D3DXCOLOR m_d3dxcDiffuse;
public:
	//생성자와 소멸자를 선언한다.
	CDiffusedVertex(float x, float y, float z, D3DXCOLOR d3dxcDiffuse) { m_d3dxvPosition = D3DXVECTOR3(x, y, z); m_d3dxcDiffuse = d3dxcDiffuse; }
	CDiffusedVertex(D3DXVECTOR3 d3dxvPosition, D3DXCOLOR d3dxcDiffuse) { m_d3dxvPosition = d3dxvPosition; m_d3dxcDiffuse = d3dxcDiffuse; }
	CDiffusedVertex() { m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f); m_d3dxcDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); }
	~CDiffusedVertex() { }
};

class CDiffuseNormalVertex
{
public:
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXVECTOR3 m_d3dxvNormal;
	//정점의 색상을 나타내는 멤버 변수(D3DXCOLOR 구조체)를 선언한다. 
	D3DXCOLOR m_d3dxcDiffuse;
	CDiffuseNormalVertex() {}
	CDiffuseNormalVertex(D3DXVECTOR3 vPosition, D3DXVECTOR3 vNormal, D3DXCOLOR Color) { m_d3dxvPosition = vPosition; m_d3dxvNormal = vNormal;  m_d3dxcDiffuse = Color; }
	~CDiffuseNormalVertex() {}
};

class CNormalVertex
{
	D3DXVECTOR3 m_d3dxvPosition;
public:
	//조명의 영향을 계산하기 위하여 법선벡터가 필요하다.
	D3DXVECTOR3 m_d3dxvNormal;

	CNormalVertex(float x, float y, float z, float nx, float ny, float nz) { m_d3dxvPosition = D3DXVECTOR3(x, y, z); m_d3dxvNormal = D3DXVECTOR3(nx, ny, nz); }
	CNormalVertex(D3DXVECTOR3 d3dxvPosition, D3DXVECTOR3 d3dxvNormal) { m_d3dxvPosition = d3dxvPosition; m_d3dxvNormal = d3dxvNormal; }
	CNormalVertex() { m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f); m_d3dxvNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	~CNormalVertex() { }
};


class CTexturedVertex
{
public:
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXVECTOR2 m_d3dxvTexCoord;

	CTexturedVertex() { m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f); m_d3dxvTexCoord = D3DXVECTOR2(0.0f, 0.0f); }
	CTexturedVertex(float x, float y, float z, float u, float v) { m_d3dxvPosition = D3DXVECTOR3(x, y, z); m_d3dxvTexCoord = D3DXVECTOR2(u, v); }
	CTexturedVertex(D3DXVECTOR3 d3dxvPosition, D3DXVECTOR2 d3dxvTexture) { m_d3dxvPosition = d3dxvPosition; m_d3dxvTexCoord = d3dxvTexture; }
	~CTexturedVertex() { }
};

class CTexturedNormalVertex
{
public:
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXVECTOR3 m_d3dxvNormal;
	D3DXVECTOR2 m_d3dxvTexCoord;

	CTexturedNormalVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v) { m_d3dxvPosition = D3DXVECTOR3(x, y, z); m_d3dxvNormal = D3DXVECTOR3(nx, ny, nz); m_d3dxvTexCoord = D3DXVECTOR2(u, v); }
	CTexturedNormalVertex(D3DXVECTOR3 d3dxvPosition, D3DXVECTOR3 d3dxvNormal, D3DXVECTOR2 d3dxvTexCoord) { m_d3dxvPosition = d3dxvPosition; m_d3dxvNormal = d3dxvNormal; m_d3dxvTexCoord = d3dxvTexCoord; }
	CTexturedNormalVertex() { m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f); m_d3dxvNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f); m_d3dxvTexCoord = D3DXVECTOR2(0.0f, 0.0f); }
	~CTexturedNormalVertex() { }
};

class CTexturedNormalVertexUVW
{
public:
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXVECTOR3 m_d3dxvNormal;
	D3DXVECTOR2 m_d3dxvTexCoord;
	int textureN;

	CTexturedNormalVertexUVW() {}
	CTexturedNormalVertexUVW(float x, float y, float z, float nx, float ny, float nz, float u, float v, int w) {
		m_d3dxvPosition = D3DXVECTOR3(x, y, z); m_d3dxvNormal = D3DXVECTOR3(nx, ny, nz); m_d3dxvTexCoord = D3DXVECTOR2(u, v);  textureN = w;
	}
	~CTexturedNormalVertexUVW() {}
};
class CSkinnedVertex
{
public:
	D3DXVECTOR3 m_d3dxvPosition; //정점
	D3DXVECTOR3 m_d3dxvNormal; // 노말
	D3DXVECTOR3 m_d3dxvBinormal; // 바이노말
	D3DXVECTOR3 m_d3dxvTangent; // 탄젠트
	D3DXVECTOR2 m_d3dxvTexCoord; //텍스쳐좌표
	D3DXVECTOR3 m_weights; // 무게
	D3DXVECTOR4 m_boneIndices; // 본번호
	int TextureNumber; // 텍스쳐 인덱스
	

	CSkinnedVertex()
	{
		m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_d3dxvNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_d3dxvBinormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_d3dxvTangent = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_d3dxvTexCoord = D3DXVECTOR2(0.0f, 0.0f);
		m_weights = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_boneIndices = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
		TextureNumber = -1;
	}

	CSkinnedVertex(float x, float y, float z)
	{
		m_d3dxvPosition = D3DXVECTOR3(x, y, z);
		m_d3dxvNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_d3dxvBinormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_d3dxvTangent = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_d3dxvTexCoord = D3DXVECTOR2(0.0f, 0.0f);
		m_weights = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_boneIndices = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
		TextureNumber = -1;
	}

	~CSkinnedVertex() {};
};

class CMesh
{
	//생성자, 소멸자
public:
	CMesh();
	CMesh(ID3D11Device *pd3dDevice);
	virtual ~CMesh();
	//CMesh 클래스 객체의 참조와 관련된 멤버 변수와 함수를 선언한다.
private:
	int m_nReferences;
public:
	void AddRef();
	void Release();
protected:
	//정점 데이터를 저장하기 위한 정점 버퍼 인터페이스 포인터
	ID3D11Buffer *m_pd3dVertexBuffer;
	ID3D11RasterizerState *m_pd3dRasterizerState;
	ID3D11BlendState *m_pd3dBlendState;

	//정점 버퍼의 정점 개수, 정점의 바이트 수, 정점 데이터가 정점 버퍼의 어디에서부터
	//시작하는가를 나타내는 변수를 선언한다.
	UINT m_nStride;			//바이트수
	UINT m_nOffset;			//어디에서부터 어디에서 시작하는가
	
	std::vector<D3DXMATRIX*> mBoneData;

	/*인덱스 버퍼(인덱스의 배열)에 대한 인터페이스 포인터이다. 인덱스 버퍼는 정점 버퍼(배열)에 대한 인덱스를 가진다.*/
	ID3D11Buffer *m_pd3dIndexBuffer;
	//인덱스 버퍼가 포함하는 인덱스의 개수이다. 
	
	//인덱스 버퍼에서 메쉬를 표현하기 위해 사용되는 시작 인덱스이다. 
	UINT m_nStartIndex;
	//각 인덱스에 더해질 인덱스이다. 
	int m_nBaseVertex;

	//정점 데이터가 어떤 프리미티브를 표현하고 있는 가를 나타내는 멤버변수를 선언한다.
	D3D11_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology;
public:
	CTexturedNormalVertexUVW *m_pVertices;
	UINT m_nVertices;		//개수
	UINT m_nIndices;
public:
	//랜더링하는 메소드
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
	virtual void update(ID3D11Device *pd3dDevice) {};
	int AnimSize;
	D3DXMATRIX* getTransform(int frameNumber) {
		frameNumber %= AnimSize;
		D3DXMATRIX* Transform = new D3DXMATRIX[mBoneData.size()];
		D3DXMATRIX tempM = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		for (unsigned int i = 0; i < mBoneData.size(); ++i)
		{
			
			Transform[i] = mBoneData[i][frameNumber];
		}
		return Transform;
	}
	int GetBoneDataSize() { return mBoneData.size(); }
};



class CAirplaneMesh : public CMesh
{
public:
	CAirplaneMesh(ID3D11Device *pd3dDevice, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 4.0f, D3DXCOLOR d3dxColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
	virtual ~CAirplaneMesh();

	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};

class CCubeMesh : public CMesh
{
public:
	OBJECTTAG m_tag;
	CDiffuseNormalVertex pVertices[8];
	CCubeMesh(ID3D11Device *pd3dDevice, float minX =-1.0f, float maxX = 1.0f, float minY = -1.0f, float maxY = 1.0f, float minZ = -1.0f, float maxZ=1.0f, OBJECTTAG tag = MAP);
	~CCubeMesh();
	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	void GetBottom(D3DXVECTOR3* vPlanes);
	void GetFront(D3DXVECTOR3* vPlanes);
	void GetBack(D3DXVECTOR3* vPlanes);
	void GetRight(D3DXVECTOR3* vPlanes);
	void GetLeft(D3DXVECTOR3* vPlanes);


};

class CMeshIlluminated : public CMesh
{
public:
	CMeshIlluminated(ID3D11Device *pd3dDevice);
	virtual ~CMeshIlluminated();

public:

	void CalculateVertexNormal(BYTE *pVertices, WORD *pIndices);


	//정점이 포함된 삼각형의 법선벡터를 계산하는 함수이다.
	D3DXVECTOR3 CalculateTriAngleNormal(BYTE *pVertices, USHORT nIndex0, USHORT nIndex1, USHORT nIndex2);
	void SetTriAngleListVertexNormal(BYTE *pVertices);
	//정점의 법선벡터의 평균을 계산하는 함수이다.
	void SetAverageVertexNormal(BYTE *pVertices, WORD *pIndices, int nPrimitives, int nOffset, bool bStrip);

	virtual void Render(ID3D11DeviceContext *pd3dImmediateDeviceContext);
};

class CMeshTextured : public CMeshIlluminated
{
public:
	CMeshTextured(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CMeshTextured();

	virtual void SetRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};


class CCharacterMesh : public CMeshIlluminated
{
	std::vector<int> Indices;
	std::vector<D3DXVECTOR2> uv;
	std::vector<CSkinnedVertex> vertex2;
	//ID3D11Device *m_pd3dDevice;
	int AnimaionCount;
public:
	CCharacterMesh(ID3D11Device *pd3dDevice,char* filename ,  float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCharacterMesh();
	virtual void SetRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};

class CSkyBoxMesh : public CMeshTextured
{
public:
	CSkyBoxMesh(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CSkyBoxMesh();

	virtual void SetRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};

class CFloorMesh : public CMeshTextured
{
public:
	CFloorMesh(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CFloorMesh();

	virtual void SetRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};

class CItemMesh : public CMeshTextured
{
public:
	CItemMesh(ID3D11Device *pd3dDevice, const char * filename, bool alphaBlend = false);
	virtual ~CItemMesh();

	virtual void SetRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};