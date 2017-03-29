#pragma once
#include"stdafx.h"

class CAnimation
{
	int m_size;
	D3DXMATRIX* m_pAnimationData;
public:
	CAnimation();
	~CAnimation();

	void setSize(int)
};

class CAnimationClip
{
private:
	int count;
	std::vector<std::string> m_NameList;
	std::vector<int> m_SizeList;
	std::vector<D3DXMATRIX*> m_Data;
private:
	int findClip(char* name);
public:
	CAnimationClip();
	~CAnimationClip();
	
	// ���Ϸ� ���� �ִϸ��̼� �߰�
	bool LoadAnimation();
	
	// �ִϸ��̼� �ϳ��� ��ü ����� ���� �� �ִ�.
	bool GetAnimationClip(char *name, D3DXMATRIX* buf);
	
	// ���� �������� �ִϸ��̼� ����� ���� �� �ִ�.
	bool GetAnimation(char* name, int frame, D3DXMATRIX* buf);

	// ������ �ִϸ��̼��� ���� �� �ִ�.  
	bool GetBlenAnimation(char* name_a, char* name_b, int frame_a, int frame_b, float ratio, D3DXMATRIX* buf);
};