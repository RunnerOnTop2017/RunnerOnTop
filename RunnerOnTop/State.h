#pragma once


#define IDLE 0
#define RUN 1
#define JUMP 2
#define STATENUMBER int


class CState
{
	STATENUMBER m_state;
public:
	CState();
	~CState();
	STATENUMBER GetState();
	void ChangeState(STATENUMBER newState);
};

