/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CPathAnimator.cpp
Simple component for animating an object along a path.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#include "CPathAnimator.h"


namespace nou
{
	CPathAnimator::CPathAnimator(Entity& owner)
	{
		m_owner = &owner;
		//TODO: How should we set this up?

		m_segmentIndex = 0;
		m_segmentTimer = 0;

		m_segmentTravelTime = 1;
		m_mode = PathSampler::PathMode::CATMULL;
	}

	void CPathAnimator::SetMode(PathSampler::PathMode mode)
	{
		m_mode = mode;
		m_segmentIndex = 0;
	}

	void CPathAnimator::Update(const PathSampler::KeypointSet& keypoints, float deltaTime)
	{
		//TODO: Complete this function.
		if (keypoints.size() == 0 || m_segmentTravelTime == 0)
			return;

		m_segmentTimer += deltaTime;

		while (m_segmentTimer > m_segmentTravelTime) {
			m_segmentTimer -= m_segmentTravelTime;

			//if we're using catmull-rom add 1 to the index
			if (m_mode == PathSampler::PathMode::CATMULL)
				m_segmentIndex += 1;
			//if we're using beizer, we need to skip the control points so add 3 instead 
			else
				m_segmentIndex += 3;

			if (m_segmentIndex >= keypoints.size()) 
				m_segmentIndex = 0;
		}

		float t = m_segmentTimer / m_segmentTravelTime;

		//if the mode is catmull check if it's a valid number of points and if it is, then run the update
		if (m_mode == PathSampler::PathMode::CATMULL) {

			if (keypoints.size() < 4) {
				m_owner->transform.m_pos = keypoints[0]->transform.m_pos;
				return;
			}

			size_t p0_ind, p1_ind, p2_ind, p3_ind;
			glm::vec3 p0, p1, p2, p3;

			//For catmull, the path segment is p1 and p2
			//our segment index will be p1
			p1_ind = m_segmentIndex;

			p0_ind = (p1_ind == 0) ? keypoints.size() - 1 : p1_ind - 1;

			p2_ind = (p1_ind + 1) % keypoints.size();

			p3_ind = (p2_ind + 1) % keypoints.size();

			//setting the vec3s 
			p0 = keypoints[p0_ind]->transform.m_pos;
			p1 = keypoints[p1_ind]->transform.m_pos;
			p2 = keypoints[p2_ind]->transform.m_pos;
			p3 = keypoints[p3_ind]->transform.m_pos;

			m_owner->transform.m_pos = PathSampler::Catmull(p0, p1, p2, p3, t);
		}
		//if the mode isn't catmull, then it must be beizer, check if it's a valid number of points for beizer and if it is then run the update
		else {
			if ((keypoints.size() < 6) || (keypoints.size() % 3 != 0)) {
				m_owner->transform.m_pos = keypoints[0]->transform.m_pos;
				return;
			}

			size_t p0_ind, p1_ind, p2_ind, p3_ind;
			glm::vec3 p0, p1, p2, p3;

			p0_ind = m_segmentIndex;

			p1_ind = (p0_ind + 1) % keypoints.size();

			p2_ind = (p1_ind + 1) % keypoints.size();

			p3_ind = (p2_ind + 1) % keypoints.size();

			//setting the vec3s 
			p0 = keypoints[p0_ind]->transform.m_pos;
			p1 = keypoints[p1_ind]->transform.m_pos;
			p2 = keypoints[p2_ind]->transform.m_pos;
			p3 = keypoints[p3_ind]->transform.m_pos;

			m_owner->transform.m_pos = PathSampler::Bezier(p0, p1, p2, p3, t);
		}
	}

	PathSampler::PathMode CPathAnimator::GetMode()
	{
		return m_mode;
	}
}