#include "stdafx.h"
#include "State.h"


CState::CState()
{
	hashMap.insert({ STATE_IDLE,"idle" });
	hashMap.insert({ STATE_RUN, "run" });
	hashMap.insert({ STATE_IDLEJUMP, "jumping" });
	hashMap.insert({ STATE_RUNJUMP, "jump" });
	
	m_prev_state = STATE_IDLE;
	m_state = STATE_IDLE;
	frame = 0;
	frame2 = 0;
	ratio = 0.0f;
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
		

		break;
	case STATE_RUN:
		if(m_state != STATE_RUN)
			m_state = STATE_RUN;
	

		break;
	case STATE_JUMP:
		if (m_state == STATE_IDLE)
		{
			m_state = STATE_IDLEJUMP;
		}
		else if (m_state == STATE_RUN)
		{
			m_state = STATE_RUNJUMP;
		}
		break;
	}
	if (prev != m_state)
	{
		frame2 = frame;
		frame = 0;
		m_prev_state = prev;
	}
}

void CState::SetTimer(CGameTimer * timer)
{
	pTimer = timer;
	time = 0;
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
		case VK_SPACE:
			ChangeState(STATE_JUMP);
			break;
		}
		break;

	case WM_KEYUP:
		
		switch (wParam)
		{
		case 0x57: // W key
			ChangeState(STATE_IDLE);
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
	if (frame != 0 && m_pAnimationClip->GetCurrentMatirxSize((char *)hashMap.find(m_state)->second.c_str()) == frame && (m_state == STATE_IDLEJUMP || m_state == STATE_RUNJUMP))
	{
		if(STATE_IDLEJUMP == m_state)
			ChangeState(STATE_IDLE);
		if (STATE_RUNJUMP == m_state)
			ChangeState(STATE_RUN);

	}
	time += pTimer->GetTimeElapsed();
	std::cout << hashMap.find(m_state)->second <<std::endl;
	if (STATE_RUNJUMP == m_state && time > TIME_ANIMATE_ELAPSED * 3.0f)
	{
		frame += 1;
		frame2 += 1;
		time = 0;
	}
	else if (time > TIME_ANIMATE_ELAPSED * 1.5f && STATE_RUNJUMP != m_state)
	{
		frame2 += 1;
		frame += 1;
		time = 0;
	}
	if (m_prev_state != m_state && m_prev_state!= STATE_RUNJUMP)
	{
		if (ratio >= 1.0f)
		{
			ratio = 0.0f;
			m_prev_state = m_state;
			frame2 = 0;
		}
		else
		{
			ratio += 0.05f;
			return m_pAnimationClip->GetBlendAnimation((char*)hashMap.find(m_state)->second.c_str(), (char*)hashMap.find(m_prev_state)->second.c_str(),
				frame, frame2, ratio, NULL);
		}
	}
	return m_pAnimationClip->GetAnimation((char *)hashMap.find(m_state)->second.c_str(), frame, buf);	
}

int CState::GetBoneSize()
{
	return m_pAnimationClip->GetBoneSize((char *)hashMap.find(m_state)->second.c_str());
}

std::string CState::StateNumberToString(STATENUMBER s)
{
	return std::string();
}
