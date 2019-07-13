#include "mcv_platform.h"
#include "comp_player_animation.h"
#include "components/common/comp_transform.h"


DECL_OBJ_MANAGER("player_animator", TCompPlayerAnimator);

void TCompPlayerAnimator::debugInMenu() {

	static float delta_movement = 0.0f;
	static float speed = 1.0f;
	ImGui::DragFloat("Speed", &speed, 0.01f, 0, 3.f);
	if (ImGui::SmallButton("Idle")) {
		playAnimation(EAnimation::IDLE, speed);
	}
	if (ImGui::SmallButton("Run")) {
		playAnimation(EAnimation::RUN, speed);
	}
	if (ImGui::SmallButton("Attack")) {
		playAnimation(EAnimation::ATTACK, speed);
	}
	if (ImGui::SmallButton("Death")) {
		playAnimation(EAnimation::DEAD, speed);
	}
	if (ImGui::SmallButton("Prueba")) {
		playAnimation(EAnimation::PRUEBA, speed);
	}


	ImGui::DragFloat("Delta Movement", &delta_movement, 0.01f, 0, 1.f);
	TCompSkeleton * compSkeleton = get<TCompSkeleton>();
	compSkeleton->setCyclicAnimationWeight(delta_movement);
}

void TCompPlayerAnimator::initializeAnimations() {

	initializeAnimation(
		(TCompAnimator::EAnimation)EAnimation::IDLE,
		EAnimationType::CYCLIC,
		EAnimationSize::SINGLE,
		"idle",
		"",
    "",
		1.0f,
		1.0f
	);

	initializeAnimation(
		(TCompAnimator::EAnimation)EAnimation::RUN,
		EAnimationType::CYCLIC,
		EAnimationSize::SINGLE,
		"run",
		"",
    "",
		1.0f,
		1.0f
	);

	initializeAnimation(
		(TCompAnimator::EAnimation)EAnimation::ATTACK,
		EAnimationType::ACTION,
		EAnimationSize::SINGLE,
		"attack",
		"",
    "",
		1.0f,
		1.0f
	);

	initializeAnimation(
		(TCompAnimator::EAnimation)EAnimation::JUMP,
		EAnimationType::ACTION,
		EAnimationSize::SINGLE,
		"jump",
		"",
    "",
		1.0f,
		1.0f
	);

	initializeAnimation(
		(TCompAnimator::EAnimation)EAnimation::THROW,
		EAnimationType::ACTION,
		EAnimationSize::SINGLE,
		"throw",
		"",
    "",
		1.0f,
		1.0f
	);

	initializeAnimation(
		(TCompAnimator::EAnimation)EAnimation::SCAN,
		EAnimationType::ACTION,
		EAnimationSize::SINGLE,
		"scan",
		"",
    "",
		1.0f,
		1.0f
	);

  /*
	initializeAnimation(
		(TCompAnimator::EAnimation)EAnimation::DEAD,
		EAnimationType::CYCLIC,
		EAnimationSize::SINGLE,
		"dead",
		"",
		1.0f,
		1.0f
	);
	initializeAnimation(
		(TCompAnimator::EAnimation)EAnimation::PRUEBA,
		EAnimationType::CYCLIC,
		EAnimationSize::DOUBLE,
		"run",
		"walk",
		0.9f,
		1.0f
	);*/
}

void TCompPlayerAnimator::registerMsgs() {
	DECL_MSG(TCompPlayerAnimator, TMsgEntityCreated, onCreated);
	DECL_MSG(TCompPlayerAnimator, TMsgExecuteAnimation, playMsgAnimation);
	
}

void TCompPlayerAnimator::update(float dt) {

	std::list<EAnimation>::iterator it = animationCallbackRequests.begin();
	while (it != animationCallbackRequests.end())
	{
		if (!isPlayingAnimation((TCompAnimator::EAnimation)*it))
		{
			//Send animation finished message
			TMsgPlayerAnimationFinished msg;
			msg.animation = *it;
			((CEntity*)CHandle(this).getOwner())->sendMsg(msg);
			//Remove callback request
			animationCallbackRequests.erase(it++);
		}
		else
		{
			++it;
		}
	}
}

void TCompPlayerAnimator::onCreated(const TMsgEntityCreated& msg) {
	
	ownHandle = CHandle(this).getOwner();
	CEntity *e = ownHandle;
	compSkeleton = e->get<TCompSkeleton>();
	assert(compSkeleton);
	initializeAnimations();
	setFeetNumAndCalculate(2);
}

bool TCompPlayerAnimator::playAnimation(TCompPlayerAnimator::EAnimation animation, float speed, bool callbackWanted) {
	if (callbackWanted)
		animationCallbackRequests.push_back(animation);

	return playAnimationConverted((TCompAnimator::EAnimation)animation, speed);
}

void TCompPlayerAnimator::playMsgAnimation(const TMsgExecuteAnimation& msg) {

	playAnimation(msg.animation, msg.speed);
}