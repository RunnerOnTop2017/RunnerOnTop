#pragma once

#ifndef SHADER_H
#define SHADER_H

#include"GameObject.h"
#include "UIClass.h"



struct VS_CB_WORLD_MATRIX
{
	D3DXMATRIX m_d3dxmtxWorld;
};

struct VS_CB_SKINNED
{
	D3DXMATRIX BoneTransform[96];
};

struct VS_CB_ISALPHABLEND
{
	bool isAlphaBlend;
};
class CShader
{
public:
	CShader();
	~CShader();

	void CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11VertexShader **ppd3dVertexShader, D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements, ID3D11InputLayout **ppd3dVertexLayout);
	void CreatePixelShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **ppd3dPixelShader);
	void CreateGeometryShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dGeometryShader);

	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, MATERIAL *pMaterial = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, CTexture *pTexture);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dTransform, int AnimationSize);

	virtual void BuildObjects(ID3D11Device *pd3dDevice);
	virtual void BuildObjects(ID3D11Device *pd3dDevice, int a) {};

	virtual void ReleaseObjects();
	virtual void AnimateObjects(ID3D11Device *pd3dDevice, float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera = NULL, int index = -1);
public:
	CGameObject* GetGameObject(int index);

public:
	ID3D11InputLayout *m_pd3dVertexLayout;
	ID3D11VertexShader *m_pd3dVertexShader;
	ID3D11PixelShader *m_pd3dPixelShader;
	ID3D11GeometryShader *m_pd3dGeometryShader;

	CGameObject **m_ppObjects;
	int m_nObjects;

	ID3D11Buffer *m_pd3dcbWorldMatrix;
	ID3D11Buffer *m_pd3dcbSkinned;

	ID3D11Buffer *m_pd3dcbMaterial;

	ID3D11Buffer *m_isAlphaBlend;

	ID3D11Buffer *m_pd3dcbTexture;
};

class CDiffusedShader : public CShader
{
public:
	CDiffusedShader();
	~CDiffusedShader();
	// 바운딩 박스 개수
	int OBJECT_CNT; 
		// 바닥 개수
	int FLOOR_CNT; 

		//건물 개수
	int WALL_CNT; 


	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld = NULL);
	virtual void BuildObjects(ID3D11Device *pd3dDevice, int mapNumber=1);
	void BuildMap1(ID3D11Device *pd3dDevice);
	void BuildMap2(ID3D11Device *pd3dDevice);

	virtual void ReleaseObjects();
	virtual void AnimateObjects(ID3D11Device *pd3dDevice, float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera = NULL);
};

class CTextureShader : public CShader
{
public:
	CTextureShader();
	~CTextureShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, MATERIAL *pMaterial = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, CTexture *pTexture);
	
	virtual void BuildObjects(ID3D11Device *pd3dDevice, int mapNumber = 1);
	virtual void ReleaseObjects();
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera = NULL, int index=-1);
};

class CAnimateShader : public CTextureShader
{
public:
	CAnimateShader();
	~CAnimateShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, MATERIAL *pMaterial = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, CTexture *pTexture);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dTransform, int AnimationSize);


	virtual void BuildObjects(ID3D11Device *pd3dDevice);
};
class CSkyBoxShader : public CTextureShader
{
public:
	CSkyBoxShader();
	~CSkyBoxShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, MATERIAL *pMaterial = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, CTexture *pTexture);

	virtual void BuildObjects(ID3D11Device *pd3dDevice);
	virtual void ReleaseObjects();
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera = NULL);
};

class CItemShader : public CTextureShader
{
public:
	CItemShader();
	~CItemShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, MATERIAL *pMaterial = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, CTexture *pTexture);
	virtual void AnimateObjects(ID3D11Device *pd3dDevice, float fTimeElapsed);

	virtual void BuildObjects(ID3D11Device *pd3dDevice, int mapNumber = 1);
	virtual void ReleaseObjects();
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera = NULL, int index = -1);
};

class CItemShader_Alpha : public CTextureShader
{
public:
	CItemShader_Alpha();
	~CItemShader_Alpha();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, MATERIAL *pMaterial = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, CTexture *pTexture);

	virtual void BuildObjects(ID3D11Device *pd3dDevice, int mapNumber = 1);
	virtual void ReleaseObjects();
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera = NULL, int index = -1);
};
class CItemShader_Door : public CTextureShader
{
	bool *doorDir;
public:
	CItemShader_Door();
	~CItemShader_Door();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, MATERIAL *pMaterial = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, CTexture *pTexture);

	virtual void BuildObjects(ID3D11Device *pd3dDevice, int mapNumber = 1);
	virtual void ReleaseObjects();
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera = NULL, int index= -1);
};

class CUIShader : public CShader {
	D3DXMATRIX mtxWorld;
public:
	CUIClass *m_pUI;
	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);

	virtual void BuildObjects(ID3D11Device *pd3dDevice, int mapNumber = 1);
	virtual void ReleaseObjects();
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera = NULL, int index = -1);
};


#endif