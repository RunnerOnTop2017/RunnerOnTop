#pragma once
#include"stdafx.h"
#include"AnimationClip.h"
#include "Timer.h"
#define STATE_IDLE 0
#define STATE_RUN 1
#define STATE_JUMP 10
#define STATE_IDLEJUMP 10
#define STATE_RUNJUMP 11
#define STATENUMBER int

#define TIME_ANIMATE_ELAPSED 1.0f/60.0f


class CState
{
	STATENUMBER m_state;
	STATENUMBER m_prev_state;
	CAnimationClip * m_pAnimationClip;
	CGameTimer * pTimer;
	float time;
	float ratio;
	int frame;
	int frame2;
	std::map<int, std::string> hashMap;
public:
	CState();
	~CState();
	STATENUMBER GetState();
	void ChangeState(STATENUMBER newState);
	void SetTimer(CGameTimer * timer);
	void SetAnimationClip(CAnimationClip* clip);

	void ProcessInput(UINT uMessage, WPARAM wParam, LPARAM lParam);

	D3DXMATRIX* GetAnimation();
	int GetBoneSize();
	std::string StateNumberToString(STATENUMBER s);
};

