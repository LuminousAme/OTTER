/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

Animation.cpp
Classes for managing animation data and blendtrees.
*/

#include "Animation.h"

namespace nou
{
	JointAnim::JointAnim()
	{
		jointInd = 0;
		rotFrames = 0;
		posFrames = 0;
	}

	SkeletalAnim::SkeletalAnim()
	{
		duration = 0.0f;
	}

	SkeletalAnimClip::JointPose::JointPose()
	{
		pos = glm::vec3(0.0f, 0.0f, 0.0f);
		rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	}

	SkeletalAnimClip::SkeletalAnimClip(const SkeletalAnim& anim, const Skeleton& skeleton)
		: m_anim(anim)
	{
		m_timer = 0.0f;

		//Should have output for each joint in our skeleton.
		m_result.resize(skeleton.m_joints.size());

		for (size_t i = 0; i < skeleton.m_joints.size(); ++i)
		{
			//Output should default to the base pose of our skeleton.
			m_result[i].pos = skeleton.m_joints[i].m_basePos;
			m_result[i].rotation = skeleton.m_joints[i].m_baseRotation;
		}

		m_rotFrame.resize(m_anim.data.size());
		std::fill(m_rotFrame.begin(), m_rotFrame.end(), 0);

		m_posFrame.resize(m_anim.data.size());
		std::fill(m_posFrame.begin(), m_posFrame.end(), 0);
	}

	void SkeletalAnimClip::Update(float deltaTime, const Skeleton& skeleton)
	{
		//TODO: Implement this function!
		//(You will be REPLACING what is here,
		//not just adding to it.)

		//add to the timer
		m_timer += deltaTime;
		//if the animation has ended, restart it 
		while (m_timer >= m_anim.duration)
			m_timer -= m_anim.duration;


		for (size_t i = 0; i < m_anim.data.size(); i++) {
			JointPose& pose = m_result[m_anim.data[i].jointInd];

			if (m_anim.data[i].posFrames > 0) {
				pose.pos = m_anim.data[i].posKeys[0];
			}

			//check if there is only 1 rotation keyframe
			if (m_anim.data[i].rotFrames == 1) {
				//if there is just set it to that frame
				pose.rotation = m_anim.data[i].rotKeys[0];
			}
			//otherwise, if there's more than 1 cycle through them
			else if (m_anim.data[i].rotFrames > 1) {
				//get the current and next frame
				//grab the current frame out of the member variable
				int currentFrame = m_rotFrame[i]; 
				//update the current frame if the time is now beyond the end of the current keyframe
				while (m_timer > m_anim.data[i].rotTimes[currentFrame]) {
					currentFrame++;
					m_rotFrame[i]++;
				}

				//calculate the next keyframe
				int nextFrame = currentFrame + 1;
				//if the next keyframe would go out of outbounds, reset it to zero
				if (nextFrame >= m_anim.data[i].rotFrames)
					nextFrame = 0;

				//having the current and next keyframe indices, we can calculate t 
				float t = (m_timer - m_anim.data[i].rotTimes[currentFrame]) / 
					(m_anim.data[i].rotTimes[nextFrame] - m_anim.data[i].rotTimes[currentFrame]);

				//with that we can slerp the rotations
				pose.rotation = glm::mix(m_anim.data[i].rotKeys[currentFrame], m_anim.data[i].rotKeys[nextFrame], t);

				//reset to current frame to zero if it's reached the end of the animation
				if (currentFrame >= m_anim.data[i].rotFrames - 1) {
					m_rotFrame[i] = 0;
				}
			}
		}
	}

	void SkeletalAnimClip::Apply(Skeleton& skeleton)
	{
		//Indices of output match joint indices.
		for (size_t i = 0; i < m_result.size(); ++i)
		{
			Joint& joint = skeleton.m_joints[i];
			joint.m_pos = m_result[i].pos;
			joint.m_rotation = m_result[i].rotation;

		}
	}
}