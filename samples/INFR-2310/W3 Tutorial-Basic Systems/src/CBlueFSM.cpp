#include "CBlueFSM.h"
#include "Sprites/CSpriteAnimator.h"


namespace nou
{
	const std::string CBlueFSM::idleClip = "idle";
	const std::string CBlueFSM::runClip = "walk";
	const std::string CBlueFSM::slideClip = "slide";

	CBlueFSM::CBlueFSM(Entity& owner)
	{
		m_owner = &owner;
		m_state = AnimState::IDLE;

		SetState(m_state);
	}

	void CBlueFSM::SetState(AnimState state)
	{
		m_state = state;

		auto& animator = m_owner->Get<CSpriteAnimator>();

		if (GetTrigger("slide"))
			m_state = AnimState::SLIDE;

		ClearTriggers();

		switch (m_state)
		{
		case AnimState::IDLE:
			animator.PlayLoop(idleClip);
			break;
		case AnimState::RUN:
			animator.PlayLoop(runClip);
			break;
		case AnimState::SLIDE:
			animator.PlayOnce(slideClip);
		}
	}

	void CBlueFSM::Update()
	{
		auto& animator = m_owner->Get<CSpriteAnimator>();

		switch (m_state)
		{
		case AnimState::IDLE:

			if (GetVariable("moving"))
				SetState(AnimState::RUN);

			if (GetTrigger("slide"))
				SetState(AnimState::SLIDE);

			break;
		case AnimState::RUN:

			if (!GetVariable("moving"))
				SetState(AnimState::IDLE);

			if (GetTrigger("slide"))
				SetState(AnimState::SLIDE);

			break;
		default:
			if (animator.IsDone()) {
				if (GetVariable("moving"))
					SetState(AnimState::RUN);
				else
					SetState(AnimState::IDLE);
			}

			break;
		}
	}
}