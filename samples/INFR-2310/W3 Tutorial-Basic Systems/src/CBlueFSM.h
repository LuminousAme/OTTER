#pragma once

#include "FSM/FSM.h"
#include "NOU/Entity.h"

namespace nou
{
	class CBlueFSM : public FSM
	{
	public:

		static const std::string idleClip;
		static const std::string runClip;
		static const std::string slideClip;

		enum class AnimState
		{
			IDLE = 0,
			RUN,
			SLIDE
		};

		CBlueFSM(Entity& owner);
		~CBlueFSM() = default;

		CBlueFSM(CBlueFSM&&) = default;
		CBlueFSM& operator=(CBlueFSM&&) = default;

		void SetState(AnimState state);
		void Update();

	private:

		Entity* m_owner;
		AnimState m_state;
	};
}