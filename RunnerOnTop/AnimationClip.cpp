#include "stdafx.h"
#include "AnimationClip.h"

CAnimation::CAnimation()
{
	bonesize = 0;
	m_size = 0;
	m_AnimationByFrame = NULL;
}

CAnimation::CAnimation(char * pname, int bSize, int mSize, std::vector<D3DXMATRIX*> AnimData)
{
	bonesize = bSize;
	m_size = mSize;
	name = pname;
	m_pAnimationData = AnimData;

	m_AnimationByFrame = new D3DXMATRIX*[m_size];
	//for (int i = 0; i < m_size; ++i)
	//{
	//	m_AnimationByFrame[i] = new D3DXMATRIX[bonesize];
		/*for (int j = 0; j < bonesize; ++j)
		{
			m_AnimationByFrame[i][j] = m_pAnimationData[j][i];
		}*/
	//}

		for (int j = 0; j < m_pAnimationData.size(); ++j)
		{
			m_AnimationByFrame[j] = m_pAnimationData[j];
		}
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

CAnimation::CAnimation(const CAnimation & anm)
{
	bonesize = anm.bonesize;
	m_size = anm.bonesize;
	name = anm.name;
	m_pAnimationData = anm.m_pAnimationData;
	m_AnimationByFrame = anm.m_AnimationByFrame;
}

CAnimation::~CAnimation()
{
	for (int i = 0; i < m_pAnimationData.size(); ++i)
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

std::vector<D3DXMATRIX*> CAnimation::GetFullAnimationData()
{
	return m_pAnimationData;
}


CAnimationClip::CAnimationClip()
{
	m_pblendedMatrix = NULL;
}


CAnimationClip::~CAnimationClip()
{
	DATA.clear();
}

bool CAnimationClip::LoadAnimation(char * name)
{
	//CAnimation *pAnimation = new CAnimation(name);
	auto ret = DATA.insert({ name, nullptr });
	if (ret.second)
	{
		ret.first->second = new CAnimation(name);
	}
	return true;
}

bool CAnimationClip::LoadAnimation(char * name, int len)
{
	CAnimation anm(name);

	int fSize = anm.GetFrameSize();
	int bSize = anm.GetBoneSize();
	D3DXMATRIX** Animation = new D3DXMATRIX*[fSize];
	for (int i = 0; i < fSize; ++i)
		Animation[i] = anm.GetMatrixByFrame(i);

	float elapse = (float)fSize / float(len);
	float fTime = elapse;
	std::vector<D3DXMATRIX*> clip;
	clip.push_back(Animation[0]);
	int n = 1;
	for(int k = 0; k<len-1; ++k)
	{

		if (fTime + elapse >n && n< fSize-1)
		{
			++n;
		}

		float ratio = fTime - (n - 1);

		D3DXMATRIX *matrixByBone = new D3DXMATRIX[bSize];

		for (int i = 0; i < bSize; ++i)
		{
			matrixByBone[i] = ((Animation[n-1][i]*(1.0f-ratio)) + (Animation[n][i]*ratio));
		}
		clip.push_back(matrixByBone);

		fTime += elapse;
	}
	auto ret = DATA.insert({ name, nullptr });
	if (ret.second)
	{
		ret.first->second = new CAnimation(name, bSize, clip.size(), clip);
	}
	return true;
}

D3DXMATRIX* CAnimationClip::GetAnimation(char * name, int frame, D3DXMATRIX * buf)
{
 	return DATA.find(name)->second->GetMatrixByFrame(frame);
}

int CAnimationClip::GetBoneSize(char * name)
{
	return DATA.find(name)->second->GetBoneSize();
}

D3DXMATRIX* CAnimationClip::GetBlendAnimation(char * name_a, char * name_b, int frame_a, int frame_b, float ratio, D3DXMATRIX * buf)
{
	if (m_pblendedMatrix)
	{
		delete m_pblendedMatrix;
	}
	int size = DATA.find(name_a)->second->GetBoneSize();
	m_pblendedMatrix = new D3DXMATRIX[size];
	D3DXMATRIX *tmp1 = DATA.find(name_a)->second->GetMatrixByFrame(frame_a);
	D3DXMATRIX *tmp2 = DATA.find(name_b)->second->GetMatrixByFrame(frame_b);

	for (int i = 0; i < size; ++i)
	{
		m_pblendedMatrix[i] = (tmp1[i] * ratio) + (tmp2[i] * (1.0f - ratio));
	}
	
	return m_pblendedMatrix;
}

int CAnimationClip::GetCurrentMatirxSize(char * name)
{
	return DATA.find(name)->second->GetFrameSize();
}
