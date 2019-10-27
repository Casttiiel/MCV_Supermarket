#include "mcv_platform.h"
#include "comp_cupcake.h"
#include "components/common/comp_transform.h"


DECL_OBJ_MANAGER("cupcake_animator", TCompCupcakeAnimator);

void TCompCupcakeAnimator::debugInMenu() {

}

void TCompCupcakeAnimator::initializeAnimations() {
	
    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::CUPCAKE_IDLE,
        EAnimationType::CYCLIC,
        EAnimationSize::SINGLE,
        "CUPCAKE_IDLE",
        "",
        "",
        1.0f,
        1.0f
    );

    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::CUPCAKE_JUMP,
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