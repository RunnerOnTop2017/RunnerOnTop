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
	CAnimation(const char* pAnimationName);
	~CAnimation();

	int GetFrameSize();
	int GetBoneSize();
	
	
	//Get Animation Name
	char* GetName();

	//Get Matrix
	D3DXMATRIX* GetMatrixByFrame(int& frame);
};

class CAnimationClip
{
private:
	std::map<std::string, CAnimation> DATA;
public:
	CAnimationClip();
	~CAnimationClip();
	
	// 파일로 부터 애니메이션 추가
	bool LoadAnimation(char *name);
	
	// 일정 프레임의 애니메이션 행렬을 얻어올 수 있다.
	bool GetAnimation(char* name, int frame, D3DXMATRIX* buf);

	// 블랜딩된 애니메이션을 얻어올 수 있다.  
	bool GetBlenAnimation(char* name_a, char* name_b, int frame_a, int frame_b, float ratio, D3DXMATRIX* buf);
};