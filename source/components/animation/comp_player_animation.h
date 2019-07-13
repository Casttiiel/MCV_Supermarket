#pragma once

#include "components/common/comp_base.h"
#include "skeleton/comp_skeleton.h"
#include "skeleton/comp_animator.h"
#include "entity/common_msgs.h"

class TCompPlayerAnimator;

class TCompPlayerAnimator : public TCompAnimator
{
public:
	enum EAnimation { IDLE = 0, ATTACK, RUN, JUMP, THROW, SCAN, DEAD, PRUEBA};
	struct TMsgExecuteAnimation {

		TCompPlayerAnimator::EAnimation animation;
		float speed;
		DECL_MSG_ID();
	};

	struct TMsgPlayerAnimationFinished {

		TCompPlayerAnimator::EAnimation animation;
		DECL_MSG_ID();
	};

	DECL_SIBLING_ACCESS();

	void debugInMenu();
	static void registerMsgs();
	void onCreated(const TMsgEntityCreated& msg);
	void playMsgAnimation(const TMsgExecuteAnimation& msg);
	void update(float dt);
	
	void initializeAnimations();
	bool playAnimation(TCompPlayerAnimator::EAnimation animation, float speed = 1.0f, bool callbackWanted = false);

	TCompSkeleton * compSkeleton;
	//A vector where we include those animations whose finish we want to be notified of 
	//We'll traverse the vector on each frame, comparing it with the mixer, to see if the animation
	//has left it. Once it leaves, we remove it from the vector and notify our entity
	std::list< EAnimation> animationCallbackRequests;
};
