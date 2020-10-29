/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CMorphAnimator.h
Simple animator component for demonstrating morph target animation.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#pragma once

#include "NOU/Entity.h"

namespace nou
{
	class Mesh;

	class CMorphAnimator
	{
		public:

		CMorphAnimator(Entity& owner);
		~CMorphAnimator() = default;

		CMorphAnimator(CMorphAnimator&&) = default;
		CMorphAnimator& operator=(CMorphAnimator&&) = default;

		void Update(float deltaTime);

		//TODO: Add functions to set frames and in-between frame time.
		void setFrames(const std::vector<std::unique_ptr<Mesh>>& frames);
		void SetFrameTime(float frameTime);
		void SetLoopBackwards(bool setting);

		protected:

		Entity* m_owner;

		class AnimData
		{
			public:

			//TODO: You'll need to define a way to store and manage full
			//animation clips for the exercise.
			std::vector<const Mesh*> frames;
			//The time inbetween frames.
			float frameTime;

			AnimData();
			~AnimData() = default;
		};

		std::unique_ptr<AnimData> m_data;

		float m_timer;
		int m_currentFrame;
		bool m_forwards;
		//bool to control wheter or not the animation should play backwards when looping (true), or just restart at the first frame (false)
		bool m_loopBackwards; 
	};
}