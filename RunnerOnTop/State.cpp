#include "stdafx.h"
#include "State.h"


CState::CState()
{
	hashMap.insert({ STATE_IDLE,"idle" });
	hashMap.insert({ STATE_RUN, "run" });
	hashMap.insert({ STATE_IDLEJUMP, "jumping" });
	hashMap.insert({ STATE_RUNJUMP, "jump" });
	hashMap.insert({ STATE_LEFT, "left" });
	hashMap.insert({ STATE_RIGHT, "right" });
	hashMap.insert({ STATE_BACK, "backward" });
	hashMap.insert({ STATE_SLIDE, "slide" });
	
	m_prev_state = STATE_IDLE;
	m_state = STATE_IDLE;
	m_next_state = STATE_NULL;
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

STATENUMBER CState::GetSubState()
{
	return m_sub_state;
}

void CState::ChangeState(STATENUMBER newState, unsigned int keyBuf)
{
	if (m_state < STATE_INTERACTION)
	{

	}
	STATENUMBER prev = m_state;
	switch (newState)
	{
	case STATE_IDLE: //IDLE로 변경하려 할때
		if (m_state == STATE_RUNJUMP) // 현재상태가 RunJump인가
		{
			m_next_state = STATE_IDLE;
		}
		else if (m_state == STATE_LEFT || m_state == STATE_RIGHT) // 좌우이동중인가
		{
			if( (m_prev_state == STATE_LEFT && m_state == STATE_RIGHT) || (m_prev_state == STATE_RIGHT && m_state == STATE_LEFT))
				break;
			else
			{
				m_prev_state = m_state;
				m_state = STATE_IDLE;
			}
		}
		else if (m_state == STATE_RUN) //현재상태가 달리는중인가
		{
			if (m_sub_state == STATE_LEFT || m_sub_state == STATE_RIGHT) // 좌우이동이 섞여있는가
			{
				if (keyBuf == VK_KEYW) // W키를 떼면
				{
					m_next_state = m_sub_state;
				}
				else if(keyBuf == VK_KEYA || keyBuf == VK_KEYD)
				{
					m_next_state = STATE_RUN;
				}
			}
			else
				m_state = STATE_IDLE;
		}
		else if (m_state > STATE_INTERACTION)
		{
			m_next_state = STATE_IDLE;
		}
		else 
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
			m_next_state = STATE_IDLE;

		}
		else if (m_state == STATE_RUN)
		{
			m_state = STATE_RUNJUMP;
			m_next_state = STATE_RUN;
		}
		break;
	case STATE_BACK:
		if (m_state == STATE_IDLE)
		{
			m_state = STATE_BACK;
			m_prev_state = STATE_IDLE;
		}
		break;
	case STATE_LEFT:
		if (m_state == STATE_IDLE)
		{
			m_state = STATE_LEFT;
			m_prev_state = STATE_IDLE;
		}
		else if (m_state == STATE_RIGHT)
		{
			m_state = STATE_LEFT;
			m_prev_state = STATE_RIGHT;
		}
		else if (m_state == STATE_RUN)
		{
			m_sub_state = STATE_LEFT;
		}
		break;
	case STATE_RIGHT:
		if (m_state == STATE_IDLE)
		{
			m_state = STATE_RIGHT;
			m_prev_state = STATE_IDLE;
		}
		else if (m_state == STATE_LEFT)
		{
			m_state = STATE_RIGHT;
			m_prev_state = STATE_LEFT;
		}
		else if (m_state == STATE_RUN)
		{
			m_sub_state = STATE_RIGHT;
		}
		break;
	case STATE_SLIDE:
		if (m_state == STATE_RUN)
		{
			m_next_state = m_state;
			m_state = STATE_SLIDE;
			m_prev_state = STATE_RUN;
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
		case VK_KEYW: // W key
			ChangeState(STATE_RUN);
			break;
		case VK_SPACE:
			ChangeState(STATE_JUMP);
			break;
		case VK_KEYS:
			ChangeState(STATE_BACK);
			break;
		case VK_KEYA:
			ChangeState(STATE_LEFT);
			break;
		case VK_KEYD:
			ChangeState(STATE_RIGHT);
			break;
		case VK_KEYE:
			ChangeState(STATE_SLIDE);
			break;
		}
		break;

	case WM_KEYUP:
		switch (wParam)
		{
		case VK_KEYW: // W key
			ChangeState(STATE_IDLE, VK_KEYW);
			break;
		case VK_KEYS:
			ChangeState(STATE_IDLE);
			break;
		case VK_KEYA:
			ChangeState(STATE_IDLE, VK_KEYA);
			break;
		case VK_KEYD:
			ChangeState(STATE_IDLE, VK_KEYD);
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

	time += pTimer->GetTimeElapsed();
	std::cout << hashMap.find(m_state)->second <<std::endl;

	 if (time > TIME_ANIMATE_ELAPSED * 1.5f)
	{
		frame2 += 1;
		frame += 1;
		time = 0;
	}

	 // 달리는중
	 if (m_state == STATE_RUN)
	 {
		 if (m_sub_state == STATE_LEFT || m_sub_state == STATE_RIGHT)
		 {
			 if (m_next_state == STATE_LEFT || m_next_state == STATE_RIGHT)
			 {
				 D3DXMATRIX* other = m_pAnimationClip->GetBlendAnimation((char*)hashMap.find(m_state)->second.c_str(), (char*)hashMap.find(m_sub_state)->second.c_str(),
					 frame, frame2, 0.5f, NULL);
				 
				 other = m_pAnimationClip->GetBlendAnimation((char*)hashMap.find(m_next_state)->second.c_str(), frame, other, 0.5f);
				 m_state = m_next_state;
				 m_next_state = STATE_NULL;
				 m_sub_state = STATE_NULL;
			 }
			 else if (m_next_state == STATE_RUN)
			 {
				 D3DXMATRIX* other = m_pAnimationClip->GetBlendAnimation((char*)hashMap.find(m_state)->second.c_str(), (char*)hashMap.find(m_sub_state)->second.c_str(),
					 frame, frame2, 0.5f, NULL);

				 other = m_pAnimationClip->GetBlendAnimation((char*)hashMap.find(m_next_state)->second.c_str(), frame, other, 0.5f);
				 m_state = m_next_state;
				 m_next_state = STATE_NULL;
				 m_sub_state = STATE_NULL;
			 }
		 }
		 if(m_sub_state == STATE_LEFT)
			 return  m_pAnimationClip->GetBlendAnimation((char*)hashMap.find(m_state)->second.c_str(), (char*)hashMap.find(STATE_LEFT)->second.c_str(),
				 frame, frame2, 0.5f, NULL);
		 else if(m_sub_state == STATE_RIGHT)
			 return  m_pAnimationClip->GetBlendAnimation((char*)hashMap.find(m_state)->second.c_str(), (char*)hashMap.find(STATE_RIGHT)->second.c_str(),
				 frame, frame2, 0.5f, NULL);
	 }

	 //상호작용중일때
	 if (m_state > STATE_INTERACTION)
	 {
		 if (frame > m_pAnimationClip->GetCurrentMatirxSize((char*)hashMap.find(m_state)->second.c_str()) - 11)
		 {
			 if (ratio >= 1.0f)
			 {
				 frame = frame2;
				 ratio = 0.0f;
				 m_state = m_next_state;
				 frame2 = 0;
				 m_next_state = STATE_NULL;
			 }
			 else
			 {
				 ratio += 0.1f;
				 return  m_pAnimationClip->GetBlendAnimation((char*)hashMap.find(m_state)->second.c_str(), (char*)hashMap.find(m_next_state)->second.c_str(),
					 frame, frame2, 1.0f - ratio, NULL);
			 }
		 }
	 }

	 // 점프중일때 애니메이션 끝나갈때 블랜딩
	 if ((m_state == STATE_RUNJUMP || m_state == STATE_IDLEJUMP) && m_next_state != m_state)
	 {
		 float stand;
		 if (m_state == STATE_IDLEJUMP)
		 {
			stand = m_pAnimationClip->GetCurrentMatirxSize((char*)hashMap.find(m_state)->second.c_str()) - 11;
		 }
		 else
		 {
			 stand = m_pAnimationClip->GetCurrentMatirxSize((char*)hashMap.find(m_state)->second.c_str()) - 11;// *0.9f;
		 }
		 if (frame >= stand)
		 {
			 if (ratio >= 1.0f)
			 {
				 frame =  frame2;
				 ratio = 0.0f;
				 m_state = m_next_state;
				 frame2 = 0;
				 m_next_state = STATE_NULL;
				 
			 }
			 else
			 {
				 ratio += 0.1f;
				 return  m_pAnimationClip->GetBlendAnimation((char*)hashMap.find(m_state)->second.c_str(), (char*)hashMap.find(m_next_state)->second.c_str(),
					 frame, frame2, 1.0f - ratio, NULL);
			 }
			
		 }
	 }

	
	 // 애니메이션 전환시 뒷부분 블렌딩(단 이전 상태가 RunJump가 아닐경우)
	if (m_prev_state != m_state && m_prev_state!= STATE_RUNJUMP)
	{
		if (ratio >= 1.0f)
		{
			//frame = frame2;
			ratio = 0.0f;
			m_prev_state = m_state;
			frame2 = 0;
		}
		else
		{
			ratio += 0.1f;
			return m_pAnimationClip->GetBlendAnimation((char*)hashMap.find(m_state)->second.c_str(), (char*)hashMap.find(m_prev_state)->second.c_str(),
				frame, frame2, ratio, NULL);
		}
	}

	//애니메이션 반환
	return m_pAnimationClip->GetAnimation((char *)hashMap.find(m_state)->second.c_str(), frame, buf);	
}

int CState::GetBoneSize()
{
	return m_pAnimationClip->GetBoneSize((char *)hashMap.find(m_state)->second.c_str());
}