#include "mcv_platform.h"
#include "comp_player_animation.h"
#include "components/common/comp_transform.h"


DECL_OBJ_MANAGER("player_animator", TCompPlayerAnimator);

void TCompPlayerAnimator::debugInMenu() {

    static float delta_movement = 0.0f;
    static float speed = 1.0f;
    ImGui::DragFloat("Speed", &speed, 0.01f, 0, 3.f);
    if (ImGui::SmallButton("IDLE_MELEE")) {
        playAnimation(EAnimation::IDLE_MELEE, speed);
    }
    if (ImGui::SmallButton("IDLE_FIRE")) {
        playAnimation(EAnimation::IDLE_FIRE, speed);
    }
    if (ImGui::SmallButton("IDLE_COMBAT")) {
        playAnimation(EAnimation::IDLE_COMBAT, speed);
    }
    if (ImGui::SmallButton("MELEE1_FULL")) {
        playAnimation(EAnimation::MELEE1_FULL, speed);
    }
    if (ImGui::SmallButton("MELEE1_PARTIAL")) {
        playAnimation(EAnimation::MELEE1_PARTIAL, speed);
    }
    if (ImGui::SmallButton("MELEE2_FULL")) {
        playAnimation(EAnimation::MELEE2_FULL, speed);
    }
    if (ImGui::SmallButton("MELEE2_PARTIAL")) {
        playAnimation(EAnimation::MELEE2_PARTIAL, speed);
    }
    if (ImGui::SmallButton("AIM_THROW")) {
        playAnimation(EAnimation::AIM_THROW, speed);
    }
    if (ImGui::SmallButton("DAMAGED")) {
        playAnimation(EAnimation::DAMAGED, speed);
    }
    if (ImGui::SmallButton("DASH")) {
        playAnimation(EAnimation::DASH, speed);
    }
    if (ImGui::SmallButton("DOUBLE_JUMP")) {
        playAnimation(EAnimation::DOUBLE_JUMP, speed);
    }
    if (ImGui::SmallButton("DRINK")) {
        playAnimation(EAnimation::DRINK, speed);
    }
    if (ImGui::SmallButton("JUMP_END")) {
        playAnimation(EAnimation::JUMP_END, speed);
    }
    if (ImGui::SmallButton("JUMP_START")) {
        playAnimation(EAnimation::JUMP_START, speed);
    }
    if (ImGui::SmallButton("ON_AIR")) {
        playAnimation(EAnimation::ON_AIR, speed);
    }
    if (ImGui::SmallButton("RUN")) {
        playAnimation(EAnimation::RUN, speed);
    }
    if (ImGui::SmallButton("SCAN")) {
        playAnimation(EAnimation::SCAN, speed);
    }
    if (ImGui::SmallButton("THROW")) {
        playAnimation(EAnimation::THROW, speed);
    }
    if (ImGui::SmallButton("WALK")) {
        playAnimation(EAnimation::WALK, speed);
    }


    ImGui::DragFloat("Delta Movement", &delta_movement, 0.01f, 0, 1.f);
    TCompSkeleton* compSkeleton = get<TCompSkeleton>();
    compSkeleton->setCyclicAnimationWeight(delta_movement);
}

void TCompPlayerAnimator::initializeAnimations() {

    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::IDLE_MELEE,
        EAnimationType::CYCLIC,
        EAnimationSize::SINGLE,
        "IDLE_MELEE",
        "",
        "",
        1.0f,
        1.0f
    );
    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::IDLE_COMBAT,
        EAnimationType::CYCLIC,
        EAnimationSize::SINGLE,
        "IDLE_COMBAT",
        "",
        "",
        0.5f,
        1.0f
    );


    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::IDLE_FIRE,
        EAnimationType::CYCLIC,
        EAnimationSize::SINGLE,
        "IDLE_FIRE",
        "",
        "",
        1.0f,
        1.0f
    );

    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::MELEE1_FULL,
        EAnimationType::ACTION,
        EAnimationSize::SINGLE,
        "MELEE1_FULL",
        "",
        "",
        1.0f,
        1.0f
    );

    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::MELEE1_PARTIAL,
        EAnimationType::ACTION,
        EAnimationSize::SINGLE,
        "MELEE1_PARTIAL",
        "",
        "",
        1.0f,
        1.0f
    );

    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::MELEE2_FULL,
        EAnimationType::ACTION,
        EAnimationSize::SINGLE,
        "MELEE2_FULL",
        "",
        "",
        1.0f,
        1.0f
    );

    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::MELEE2_PARTIAL,
        EAnimationType::ACTION,
        EAnimationSize::SINGLE,
        "MELEE2_PARTIAL",
        "",
        "",
        1.0f,
        1.0f
    );

    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::AIM_THROW,
        EAnimationType::ACTION,
        EAnimationSize::SINGLE,
        "AIM_THROW",
        "",
        "",
        1.5f,
        1.0f
    );

    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::DAMAGED,
        EAnimationType::ACTION,
        EAnimationSize::SINGLE,
        "DAMAGED",
        "",
        "",
        1.0f,
        1.0f
    );

    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::DASH,
        EAnimationType::ACTION,
        EAnimationSize::SINGLE,
        "DASH",
        "",
        "",
        1.0f,
        1.0f
    );

    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::DOUBLE_JUMP,
        EAnimationType::ACTION,
        EAnimationSize::SINGLE,
        "DOUBLE_JUMP",
        "",
        "",
        1.0f,
        1.0f
    );

    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::DRINK,
        EAnimationType::ACTION,
        EAnimationSize::SINGLE,
        "DRINK",
        "",
        "",
        1.0f,
        1.0f
    );

    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::JUMP_END,
        EAnimationType::ACTION,
        EAnimationSize::SINGLE,
        "JUMP_END",
        "",
        "",
        1.0f,
        1.0f
    );

    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::JUMP_START,
        EAnimationType::ACTION,
        EAnimationSize::SINGLE,
        "JUMP_START",
        "",
        "",
        1.0f,
        1.0f
    );

    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::ON_AIR,
        EAnimationType::CYCLIC,
        EAnimationSize::SINGLE,
        "ON_AIR",
        "",
        "",
        1.0f,
        1.0f
    );

    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::RUN,
        EAnimationType::CYCLIC,
        EAnimationSize::SINGLE,
        "RUN",
        "",
        "",
        0.15f,
        1.0f
    );

    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::SCAN,
        EAnimationType::ACTION,
        EAnimationSize::SINGLE,
        "SCAN",
        "",
        "",
        1.0f,
        1.0f
    );

    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::THROW,
        EAnimationType::ACTION,
        EAnimationSize::SINGLE,
        "THROW",
        "",
        "",
        1.0f,
        1.0f
    );

    initializeAnimation(
        (TCompAnimator::EAnimation)EAnimation::WALK,
        EAnimationType::CYCLIC,
        EAnimationSize::SINGLE,
        "WALK",
        "",
        "",
        0.5f,
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
        if (!isPlayingAnimation((TCompAnimator::EAnimation) * it))
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
    CEntity* e = ownHandle;
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