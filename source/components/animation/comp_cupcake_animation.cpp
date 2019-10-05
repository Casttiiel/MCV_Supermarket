#include "mcv_platform.h"
#include "comp_cupcake_animation.h"
#include "components/common/comp_transform.h"


DECL_OBJ_MANAGER("cupcake_animator", TCompCupcakeAnimator);

void TCompCupcakeAnimator::debugInMenu() {

    static float delta_movement = 0.0f;
    static float speed = 1.0f;
    ImGui::DragFloat("Speed", &speed, 0.01f, 0, 3.f);
    if (ImGui::SmallButton("Idle")) {
        playAnimation(EAnimation::IDLE_LOOP, speed);
    }


    ImGui::DragFloat("Delta Movement", &delta_movement, 0.01f, 0, 1.f);
    TCompSkeleton * compSkeleton = get<TCompSkeleton>();
    compSkeleton->setCyclicAnimationWeight(delta_movement);
}

void TCompCupcakeAnimator::initializeAnimations() {

    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::IDLE_LOOP,
        EAnimationType::CYCLIC,
        EAnimationSize::SINGLE,
        "CUPCAKE_IDLE",
        "",
        "",
        1.0f,
        1.0f
    );
		
    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::JUMP,
        EAnimationType::ACTION,
        EAnimationSize::SINGLE,
        "CUPCAKE_JUMP",
        "",
        "",
        1.0f,
        1.0f
    );

}

void TCompCupcakeAnimator::registerMsgs() {
    DECL_MSG(TCompCupcakeAnimator, TMsgEntityCreated, onCreated);
    DECL_MSG(TCompCupcakeAnimator, TMsgExecuteAnimation, playMsgAnimation);

}


void TCompCupcakeAnimator::onCreated(const TMsgEntityCreated& msg) {
	

		ownHandle = CHandle(this).getOwner();
		CEntity *e = ownHandle;
		TCompSkeleton * compSkeleton = e->get<TCompSkeleton>();
		assert(compSkeleton);
		initializeAnimations();
		setFeetNumAndCalculate(2);
		
}

bool TCompCupcakeAnimator::playAnimation(TCompCupcakeAnimator::EAnimation animation, float speed) {

    return playAnimationConverted((TCompAnimator::EAnimation)animation, speed);
}

bool TCompCupcakeAnimator::isPlaying(TCompCupcakeAnimator::EAnimation animation) {

    return isPlayingAnimation((TCompAnimator::EAnimation)animation);
}

void TCompCupcakeAnimator::playMsgAnimation(const TMsgExecuteAnimation& msg) {

    playAnimation(msg.animation, msg.speed);
}