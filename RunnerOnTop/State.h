#pragma once
#include"stdafx.h"
#include"AnimationClip.h"

#define STATE_IDLE 0
#define STATE_RUN 1
#define STATE_JUMP 10
#define STATE_RUNJUMP 11
#define STATENUMBER int


class CState
{
	STATENUMBER m_state;
	CAnimationClip * m_pAnimationClip;
public:
	CState();
	~CState();
	STATENUMBER GetState();
	void ChangeState(STATENUMBER newState);

	void SetAnimationClip(CAnimationClip* clip);

	void ProcessInput(UINT uMessage, WPARAM wParam, LPARAM lParam);
};

