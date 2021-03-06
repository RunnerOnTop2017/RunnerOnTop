#pragma once
#include"stdafx.h"
#include"AnimationClip.h"
#include "Timer.h"

#define STATE_NULL -1
#define STATE_IDLE 0
#define STATE_RUN 1
#define STATE_LEFT 2
#define STATE_RIGHT 3
#define STATE_BACK 99
#define STATE_JUMP 10
#define STATE_IDLEJUMP 10
#define STATE_RUNJUMP 11
#define STATE_FALLBACK 21
#define STATE_STANDUP 22
#define STATE_FALLFRONT 23
#define STATE_INTERACTION 100
#define STATE_SLIDE 101
#define STATE_KICK 102
#define STATE_SMASH 102
#define STATENUMBER int

#define TIME_ANIMATE_ELAPSED 1.0f/60.0f

class CPlayer;

class CState
{
	STATENUMBER m_state;
	STATENUMBER m_prev_state;
	STATENUMBER m_next_state;
	STATENUMBER m_sub_state;
	CPlayer *m_pPlayer;
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
	STATENUMBER GetSubState();
	STATENUMBER GetNextState();
	STATENUMBER GetPrevState();

	void SetState(STATENUMBER state);
	void SetSubState(STATENUMBER state);
	void SetPlayer(CPlayer *player);

	void ChangeState(STATENUMBER newState,unsigned int keyBuf = 0);
	void SetTimer(CGameTimer * timer);
	void SetAnimationClip(CAnimationClip* clip);

	void ProcessInput(UINT uMessage, WPARAM wParam, LPARAM lParam);

	D3DXMATRIX* GetAnimation();
	int GetBoneSize();
};

