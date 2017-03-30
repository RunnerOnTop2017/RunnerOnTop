#include "stdafx.h"
#include "AnimationClip.h"

CAnimation::CAnimation()
{
	bonesize = 0;
	m_size = 0;
	m_AnimationByFrame = NULL;
}

CAnimation::CAnimation(const char * pAnimationName)
{
	bonesize = 0;
	m_size = 0;

	std::string filename = "Data\\Animation\\";
	filename += pAnimationName;
	filename += ".anm";

	FILE *fp = fopen(filename.c_str(), "rb");
	if (fp == NULL)
	{
		MessageBox(NULL, _T("LOAD ERROR"), NULL, MB_OK);
	}
	else
	{
		name = (char*)pAnimationName;
		fread((char*)&bonesize, sizeof(int), 1, fp);
		fread((char*)&m_size, sizeof(int), 1, fp);
		for (int i = 0; i < bonesize; ++i)
		{
			D3DXMATRIX* mMatrix = new D3DXMATRIX[m_size];
			fread((char*)mMatrix, sizeof(D3DXMATRIX), m_size, fp);
			m_pAnimationData.push_back(mMatrix);
		}
		fclose(fp);

		m_AnimationByFrame = new D3DXMATRIX*[m_size];
		for (int i = 0; i < m_size; ++i)
		{
			m_AnimationByFrame[i] = new D3DXMATRIX[bonesize];
			for (int j = 0; j < bonesize; ++j)
			{
				m_AnimationByFrame[i][j] = m_pAnimationData[j][i];
			}
		}
	}
	
}

CAnimation::~CAnimation()
{
	for (int i = 0; i < m_pAnimationData.size; ++i)
	{
		delete m_pAnimationData[i];
	}
	m_pAnimationData.clear();
	
	for (int i = 0; i < m_size; ++i)
	{
		delete m_AnimationByFrame[i];
	}
	delete[] m_AnimationByFrame;
}

int CAnimation::GetFrameSize()
{
	return m_size;
}

int CAnimation::GetBoneSize()
{
	return bonesize;
}

char * CAnimation::GetName()
{
	return name;
}

D3DXMATRIX * CAnimation::GetMatrixByFrame(int & frame)
{
	return m_AnimationByFrame[frame%m_size];
}


CAnimationClip::CAnimationClip()
{
	DATA.clear();
}


CAnimationClip::~CAnimationClip()
{
	DATA.clear();
}

bool CAnimationClip::LoadAnimation(char * name)
{
	DATA.insert(std::make_pair(name, CAnimation(name)));
	if (DATA.find(name)->second.GetBoneSize() == 0 || DATA.find(name) == DATA.end())
		return false;

	return true;
}

bool CAnimationClip::GetAnimation(char * name, int frame, D3DXMATRIX * buf)
{
	buf = DATA.find(name)->second.GetMatrixByFrame(frame);
	return true;
}

bool CAnimationClip::GetBlenAnimation(char * name_a, char * name_b, int frame_a, int frame_b, float ratio, D3DXMATRIX * buf)
{
	int size = DATA.find(name_a)->second.GetBoneSize();
	D3DXMATRIX *tmp1 = DATA.find(name_a)->second.GetMatrixByFrame(frame_a);
	D3DXMATRIX *tmp2 = DATA.find(name_b)->second.GetMatrixByFrame(frame_b);

	for (int i = 0; i < size; ++i)
	{
		buf[i] = (tmp1[i] * ratio) + (tmp2[i] * (1.0f - ratio));
	}
	return true;
}
