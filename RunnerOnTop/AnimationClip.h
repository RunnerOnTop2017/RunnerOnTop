#pragma once
#include"stdafx.h"

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
	
	// 파일로 부터 애니메이션 추가
	bool AddAnimation(char *filename);
	
	// 애니메이션 하나의 전체 행렬을 얻어올 수 있다.
	bool GetAnimationClip(char *name, D3DXMATRIX* buf);
	
	// 일정 프레임의 애니메이션 행렬을 얻어올 수 있다.
	bool GetAnimation(char* name, int frame, D3DXMATRIX* buf);

	// 블랜딩된 애니메이션을 얻어올 수 있다.  
	bool GetBlenAnimation(char* name_a, char* name_b, int frame_a, int frame_b, float ratio, D3DXMATRIX* buf);
};