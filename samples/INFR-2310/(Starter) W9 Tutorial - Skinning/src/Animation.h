/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

Animation.h
Classes for managing animation data and blendtrees.
*/

#pragma once

#include "SkinnedMesh.h"

#include "GLM/glm.hpp"

#include <vector>
#include <deque>

namespace nou
{	
	//Store the data for an animation clip for one joint.
	struct JointAnim
	{
		int jointInd;

		int rotFrames;
		std::vector<float> rotTimes;
		std::vector<glm::quat> rotKeys;

		int posFrames;
		std::vector<float> posTimes;
		std::vector<glm::vec3> posKeys;

		JointAnim();
	};

	//Store the data for an animation clip for an entire skeleton.
	struct SkeletalAnim
	{
		float duration;
		std::vector<JointAnim> data;

		SkeletalAnim();
	};

	//Manage an animation clip.
	class SkeletalAnimClip
	{
		public:

		struct JointPose
		{
			glm::vec3 pos;
			glm::quat rotation;

			JointPose();
			~JointPose() = default;
		};

		SkeletalAnimClip(const SkeletalAnim& anim, const Skeleton& skeleton);
		~SkeletalAnimClip() = default;

		//Update this node's animation.
		void Update(float deltaTime, const Skeleton& skeleton);
		//Apply the output of this node to a skeleton.
		void Apply(Skeleton& skeleton);

		//retstarts this node's animation
		void Restart();

		//sets if it's paused or not
		void SetIsPaused(bool _paused);
		//sets if it should loop or not
		void SetShouldLoop(bool _shouldLoop);
		//sets the playerback speed
		void SetPlaybackSpeed(float _playbackSpeed);

		//getters
		bool GetIsPaused() { return paused; }
		bool GetShouldLoop() { return shouldLoop; }
		float GetPlaybackSpeed() { return playBackFactor; }

		protected:

		//Our local timer.
		float m_timer;
		//The data for our animation clip.
		const SkeletalAnim& m_anim;
		//Which frame indices we are on for each joint.
		std::vector<int> m_rotFrame;
		std::vector<int> m_posFrame;

		//The result of our animation update.
		std::vector<JointPose> m_result;

		//if the animation is paused
		bool paused;
		//if the animation should loop
		bool shouldLoop;
		//rate at which the animation should playback
		float playBackFactor;

		void UpdateRotations();
		void UpdatePositions();
	};
}