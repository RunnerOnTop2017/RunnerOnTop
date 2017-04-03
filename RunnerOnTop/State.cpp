#include "stdafx.h"
#include "State.h"


CState::CState()
{
	m_state = STATE_IDLE;
	frame = 0;
	frame2 = 0;
	currentStateName = "idle";

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
	STATENUMBER prev = m_state;
	switch (newState)
	{
	case STATE_IDLE:
		m_state = STATE_IDLE;
		currentStateName.clear();
		currentStateName = "idle";

		break;
	case STATE_RUN:
		if(m_state != STATE_RUN)
			m_state = STATE_RUN;
		currentStateName.clear();
		currentStateName = "run";

		break;
	case STATE_JUMP:
		m_state = m_state % 10 + STATE_JUMP;
		currentStateName.clear();
		currentStateName = "jump";
		break;
	}
	if(prev!= m_state)
		frame = 0;
}

void CState::SetTimer(CGameTimer * timer)
{
	pTimer = timer;
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
		
		switch (wParam)
		{
		case 0x57: // W key
			ChangeState(STATE_RUN);
			break;
		}

		break;

	case WM_KEYUP:
		
		switch (wParam)
		{
		case 0x57: // W key
			ChangeState(STATE_IDLE);
			break;
		case VK_SPACE:
			ChangeState(STATE_JUMP);
			break;
		}

		break;

	case WM_CHAR:
		break;
	default:
		break;
	}
}

D3DXMATRIX * CState::GetAnimation()
{
	
	D3DXMATRIX *buf = NULL;
	if (frame != 0 && m_pAnimationClip->GetCurrentMatirxSize((char *)currentStateName.c_str()) == frame && m_state == STATE_JUMP)
		ChangeState(STATE_IDLE);

	if (m_state == STATE_RUN)
	{
		//return m_pAnimationClip->GetBlenAnimation((char *)str.c_str(), "idle", frame++, frame2++, 0.5f, buf);
	}
	pTimer->GetTimeElapsed();
	return m_pAnimationClip->GetAnimation((char *)currentStateName.c_str(), frame++, buf);
	
}

int CState::GetBoneSize()
{
	return m_pAnimationClip->GetBoneSize((char*)currentStateName.c_str());
}
