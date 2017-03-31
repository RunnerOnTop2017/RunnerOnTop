#include "stdafx.h"
#include "State.h"


CState::CState()
{
	m_state = STATE_IDLE;
	frame = 0;
}


CState::~CState()
{
}

STATENUMBER CState::GetState()
{
	return m_state;
}

void CState::ChangeState(STATENUMBER newState)
{
	switch (newState)
	{
	case STATE_IDLE:
		m_state = STATE_IDLE;
		break;
	case STATE_RUN:
		m_state = STATE_RUN;
		break;
	case STATE_JUMP:
		m_state = m_state % 10 + STATE_JUMP;
		break;

	}
	frame = 0;
}

void CState::SetAnimationClip(CAnimationClip* clip)
{
	m_pAnimationClip = clip;
}

void CState::ProcessInput(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
	case WM_KEYDOWN:
		break;
	case WM_KEYUP:
		break;
	default:
		break;
	}
}

D3DXMATRIX * CState::GetAnimation()
{
	std::string str ="";
	switch (m_state)
	{
	case STATE_IDLE:
		str = "idle";
			break;
	case STATE_RUN:
		str = "run";
		break;
	}
	D3DXMATRIX *buf = NULL;
	return m_pAnimationClip->GetAnimation((char *)str.c_str(), frame++, buf);
	//return buf;
}

int CState::GetBoneSize()
{
	std::string str = "";
	switch (m_state)
	{
	case STATE_IDLE:
		str = "idle";
		break;
	case STATE_RUN:
		str = "run";
		break;
	}

	return m_pAnimationClip->GetBoneSize((char*)str.c_str());
}
