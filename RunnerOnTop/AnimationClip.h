#pragma once
#include"stdafx.h"

class CAnimation
{
	int bonesize;
	int m_size;
	char *name;
	std::vector<D3DXMATRIX*> m_pAnimationData;
	D3DXMATRIX** m_AnimationByFrame;
public:
	CAnimation();
	CAnimation(char *name, int bSize, int mSize, std::vector<D3DXMATRIX*> AnimData);
	CAnimation(const char* pAnimationName);
	CAnimation(const CAnimation& anm);
	
	~CAnimation();

	int GetFrameSize();
	int GetBoneSize();
	
	
	//Get Animation Name
	char* GetName();

	//Get Matrix
	D3DXMATRIX* GetMatrixByFrame(int& frame);

	//
	std::vector<D3DXMATRIX*> GetFullAnimationData();
};

class CAnimationClip
{
private:
	std::map<std::string, CAnimation*> DATA;
	D3DXMATRIX* m_pblendedMatrix;
	
public:
	CAnimationClip();
	~CAnimationClip();
	
	// ���Ϸ� ���� �ִϸ��̼� �߰�
	bool LoadAnimation(char *name);
	// �����۾� �߰�
	bool LoadAnimation(char *name, int len);
	// ���� �������� �ִϸ��̼� ����� ���� �� �ִ�.
	D3DXMATRIX* GetAnimation(char* name, int frame, D3DXMATRIX* buf);

	int GetBoneSize(char*name);

	// ������ �ִϸ��̼��� ���� �� �ִ�.  
	D3DXMATRIX* GetBlendAnimation(char* name_a, char* name_b, int frame_a, int frame_b, float ratio, D3DXMATRIX* buf);
	D3DXMATRIX* GetBlendAnimation(char* name, int frame_a, D3DXMATRIX* other, float ratio);

	int GetCurrentMatirxSize(char * name);
};