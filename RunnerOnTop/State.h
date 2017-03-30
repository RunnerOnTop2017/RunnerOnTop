#pragma once


#define STATE_IDLE 0
#define STATE_RUN 1
#define STATE_JUMP 10
#define STATE_RUNJUMP 11
#define STATENUMBER int


class CState
{
	STATENUMBER m_state;
public:
	CState();
	~CState();
	STATENUMBER GetState();
	void ChangeState(STATENUMBER newState);

	void ProcessInput(UINT uMessage, WPARAM wParam, LPARAM lParam);
};

