#include "stdafx.h"
#include "State.h"


CState::CState()
{
	m_state = STATE_IDLE;
}


CState::~CState()
{
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
