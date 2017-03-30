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
	
	// ���Ϸ� ���� �ִϸ��̼� �߰�
	bool LoadAnimation(char *name);
	
	// ���� �������� �ִϸ��̼� ����� ���� �� �ִ�.
	bool GetAnimation(char* name, int frame, D3DXMATRIX* buf);

	// ������ �ִϸ��̼��� ���� �� �ִ�.  
	bool GetBlenAnimation(char* name_a, char* name_b, int frame_a, int frame_b, float ratio, D3DXMATRIX* buf);
};