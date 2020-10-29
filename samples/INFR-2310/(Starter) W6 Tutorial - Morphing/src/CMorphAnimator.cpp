/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CMorphAnimator.cpp
Simple animator component for demonstrating morph target animation.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#include "CMorphAnimator.h"
#include "CMorphMeshRenderer.h"
#include "NOU/Mesh.h"

namespace nou
{
	CMorphAnimator::AnimData::AnimData()
	{
		frameTime = 1.0f;
	}

	CMorphAnimator::CMorphAnimator(Entity& owner)
	{
		m_owner = &owner;
		m_data = std::make_unique<AnimData>();
		m_timer = 0.0f;
		m_currentFrame = 0;
		m_forwards = true;
		m_loopBackwards = true;
	}

	void CMorphAnimator::Update(float deltaTime)
	{
		//TODO: Complete this function.
		if (!m_data->frames.data())
			return;

		float t; 

		if (m_data->frameTime > 0.0f)
		{
			m_timer += deltaTime;

			//when a frame finishes
			if (m_timer > m_data->frameTime)
			{
				//move to the next frame
				m_currentFrame = (m_forwards) ? m_currentFrame + 1: m_currentFrame - 1;

				//if the next frame is the last frame
				if (m_currentFrame == m_data->frames.size() - 1 || m_currentFrame == 0)
				{
					//check if it should go backwards before looping
					if (m_loopBackwards)
						//if it should, swap direction
						m_forwards = !m_forwards;
					else
						//if it shouldn't restart on the first frame 
						//turney because if the animation is going backwards anyways, the first frame will be size-1, and the last will be 0
						m_currentFrame = (m_forwards) ? 0 : m_data->frames.size() - 1;
				}
			}
				
			m_timer = fmod(m_timer, m_data->frameTime);

			t = m_timer / m_data->frameTime;
		}
		else
			t = 0;

		if (m_forwards)
			m_owner->Get<CMorphMeshRenderer>().UpdateData(*m_data->frames[m_currentFrame], *m_data->frames[m_currentFrame+1], t);
		else 
			m_owner->Get<CMorphMeshRenderer>().UpdateData(*m_data->frames[m_currentFrame], *m_data->frames[m_currentFrame-1], t);
	}

	void CMorphAnimator::setFrames(const std::vector<std::unique_ptr<Mesh>>& frames)
	{
		for (int i = 0; i < frames.size(); i++)
			m_data->frames.push_back(frames[i].get());
	}

	void CMorphAnimator::SetFrameTime(float frameTime)
	{
		m_data->frameTime = frameTime;
	}

	void CMorphAnimator::SetLoopBackwards(bool setting)
	{
		m_loopBackwards = setting;
	}
}