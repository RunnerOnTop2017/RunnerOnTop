#pragma once


#include"stdafx.h"
#include "Bone.h"
#define RANDOM_COLOR D3DXCOLOR((rand() * 0xFFFFFF) / RAND_MAX)

enum OBJECTTAG{ MAP, DOOR, CONDITIONER, FENCE, FENCEHOLE ,PIPE, REALDOOR, FALL, OBJ };

class CVertex
{
	//������ ��ġ ����(3���� ����)�� �����ϴ� ��� ����
	D3DXVECTOR3 m_d3dxvPosition;
public:
	//������, �Ҹ��� ����
	CVertex() { m_d3dxvPosition = D3DXVECTOR3(0, 0, 0); }
	CVertex(D3DXVECTOR3 d3dxvPosition) { m_d3dxvPosition = d3dxvPosition; }
	~CVertex() {}
};

class CDiffusedVertex
{
	D3DXVECTOR3 m_d3dxvPosition;
	//������ ������ ��Ÿ���� ��� ����(D3DXCOLOR ����ü)�� �����Ѵ�. 
	D3DXCOLOR m_d3dxcDiffuse;
public:
	//�����ڿ� �Ҹ��ڸ� �����Ѵ�.
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
	//������ ������ ��Ÿ���� ��� ����(D3DXCOLOR ����ü)�� �����Ѵ�. 
	D3DXCOLOR m_d3dxcDiffuse;
	CDiffuseNormalVertex() {}
	CDiffuseNormalVertex(D3DXVECTOR3 vPosition, D3DXVECTOR3 vNormal, D3DXCOLOR Color) { m_d3dxvPosition = vPosition; m_d3dxvNormal = vNormal;  m_d3dxcDiffuse = Color; }
	~CDiffuseNormalVertex() {}
};

class CNormalVertex
{
	D3DXVECTOR3 m_d3dxvPosition;
public:
	//������ ������ ����ϱ� ���Ͽ� �������Ͱ� �ʿ��ϴ�.
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
	D3DXVECTOR3 m_d3dxvPosition; //����
	D3DXVECTOR3 m_d3dxvNormal; // �븻
	D3DXVECTOR3 m_d3dxvBinormal; // ���̳븻
	D3DXVECTOR3 m_d3dxvTangent; // ź��Ʈ
	D3DXVECTOR2 m_d3dxvTexCoord; //�ؽ�����ǥ
	D3DXVECTOR3 m_weights; // ����
	D3DXVECTOR4 m_boneIndices; // ����ȣ
	int TextureNumber; // �ؽ��� �ε���
	

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
	//������, �Ҹ���
public:
	CMesh();
	CMesh(ID3D11Device *pd3dDevice);
	virtual ~CMesh();
	//CMesh Ŭ���� ��ü�� ������ ���õ� ��� ������ �Լ��� �����Ѵ�.
private:
	int m_nReferences;
public:
	void AddRef();
	void Release();
protected:
	//���� �����͸� �����ϱ� ���� ���� ���� �������̽� ������
	ID3D11Buffer *m_pd3dVertexBuffer;
	ID3D11RasterizerState *m_pd3dRasterizerState;
	ID3D11BlendState *m_pd3dBlendState;

	//���� ������ ���� ����, ������ ����Ʈ ��, ���� �����Ͱ� ���� ������ ��𿡼�����
	//�����ϴ°��� ��Ÿ���� ������ �����Ѵ�.
	UINT m_nStride;			//����Ʈ��
	UINT m_nOffset;			//��𿡼����� ��𿡼� �����ϴ°�
	
	std::vector<D3DXMATRIX*> mBoneData;

	/*�ε��� ����(�ε����� �迭)�� ���� �������̽� �������̴�. �ε��� ���۴� ���� ����(�迭)�� ���� �ε����� ������.*/
	ID3D11Buffer *m_pd3dIndexBuffer;
	//�ε��� ���۰� �����ϴ� �ε����� �����̴�. 
	
	//�ε��� ���ۿ��� �޽��� ǥ���ϱ� ���� ���Ǵ� ���� �ε����̴�. 
	UINT m_nStartIndex;
	//�� �ε����� ������ �ε����̴�. 
	int m_nBaseVertex;

	//���� �����Ͱ� � ������Ƽ�긦 ǥ���ϰ� �ִ� ���� ��Ÿ���� ��������� �����Ѵ�.
	D3D11_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology;
public:
	CTexturedNormalVertexUVW *m_pVertices;
	UINT m_nVertices;		//����
	UINT m_nIndices;
public:
	//�������ϴ� �޼ҵ�
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


	//������ ���Ե� �ﰢ���� �������͸� ����ϴ� �Լ��̴�.
	D3DXVECTOR3 CalculateTriAngleNormal(BYTE *pVertices, USHORT nIndex0, USHORT nIndex1, USHORT nIndex2);
	void SetTriAngleListVertexNormal(BYTE *pVertices);
	//������ ���������� ����� ����ϴ� �Լ��̴�.
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