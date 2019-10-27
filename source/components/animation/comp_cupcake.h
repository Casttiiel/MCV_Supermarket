#pragma once

#include "components/common/comp_base.h"
#include "skeleton/comp_skeleton.h"
#include "skeleton/comp_animator.h"
#include "entity/common_msgs.h"

class TCompCupcakeAnimator;

class TCompCupcakeAnimator : public TCompAnimator
{
public:
	enum EAnimation { 
      CUPCAKE_IDLE = 0,
			CUPCAKE_JUMP
	};
	struct TMsgExecuteAnimation {

		TCompCupcakeAnimator::EAnimation animation;
		float speed;
		DECL_MSG_ID();
	};

	DECL_SIBLING_ACCESS();

	void debugInMenu();
	static void registerMsgs();
	void onCreated(const TMsgEntityCreated& msg);
	void playMsgAnimation(const TMsgExecuteAnimation& msg);
  bool isPlaying(TCompCupcakeAnimator::EAnimation animation);
	void initializeAnimations();
	bool playAnimation(TCompCupcakeAnimator::EAnimation animation, float speed = 1.0f);
};
