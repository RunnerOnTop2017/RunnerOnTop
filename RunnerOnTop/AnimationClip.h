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
	
	// 파일로 부터 애니메이션 추가
	bool LoadAnimation(char *name);
	// 보간작업 추가
	bool LoadAnimation(char *name, int len);
	// 일정 프레임의 애니메이션 행렬을 얻어올 수 있다.
	D3DXMATRIX* GetAnimation(char* name, int frame, D3DXMATRIX* buf);

	int GetBoneSize(char*name);

	// 블랜딩된 애니메이션을 얻어올 수 있다.  
	D3DXMATRIX* GetBlendAnimation(char* name_a, char* name_b, int frame_a, int frame_b, float ratio, D3DXMATRIX* buf);
	D3DXMATRIX* GetBlendAnimation(char* name, int frame_a, D3DXMATRIX* other, float ratio);

	int GetCurrentMatirxSize(char * name);
};