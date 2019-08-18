#include "mcv_platform.h"
#include "components/common/comp_transform.h"
#include "components/common/comp_name.h"
#include "components/ai/others/self_destroy.h"
#include "entity/entity_parser.h"
#include "engine.h"
#include "input/input.h"
#include "components/common/comp_tags.h"
#include "components/common/physics/comp_collider.h"
#include "input/module_input.h"
#include "modules/module_physics.h"
#include "components/controllers/comp_parabolic_launch.h"
#include "comp_kinematic_bullet.h"

using namespace physx;

DECL_OBJ_MANAGER("kinematic_bullet", TCompKinematicBullet);
void TCompKinematicBullet::debugInMenu() {
}

void TCompKinematicBullet::load(const json& j, TEntityParseContext& ctx) {
    _speed = j.value("_speed", _speed);
    _destroyOnCollission = j.value("_destroyOnCollission", _destroyOnCollission);
    _audioOnHit = j.value("_audioOnHit", _audioOnHit);
}

void TCompKinematicBullet::registerMsgs() {
    DECL_MSG(TCompKinematicBullet, TMsgAssignBulletOwner, onLaunch);
    DECL_MSG(TCompKinematicBullet, TMsgEntityTriggerEnter, onCollision);
}


void TCompKinematicBullet::onCollision(const TMsgEntityTriggerEnter& msg) {
    if (!_isEnabled)
        return;

    CEntity* e_contact = (CEntity*)msg.h_entity;
    e_contact->sendMsg(_messageToTarget);
    if (_destroyOnCollission)
        _isEnabled = false;
    if (_audioOnHit != "") {
        dbg(_audioOnHit.c_str());
        EngineAudio.playEvent(_audioOnHit);
    }
}


void TCompKinematicBullet::onLaunch(const TMsgAssignBulletOwner& msg) {
    _targetDirection = msg.front;
    _messageToTarget = msg.messageToTarget;
    targetType = msg.messageToTarget.targetType;
}


void TCompKinematicBullet::update(float delta) {
    //If it's enabled
    if (_isEnabled) {
        //Fly in the desired direction
        TCompTransform* c_trans = get<TCompTransform>();
        VEC3 c_pos = c_trans->getPosition();

        //Movement control
        c_pos += (_targetDirection * _speed * delta);

        const PxTransform tr(VEC3_TO_PXVEC3(c_pos), QUAT_TO_PXQUAT(c_trans->getRotation()));
        TCompCollider* c_col = get<TCompCollider>();
        physx::PxRigidDynamic* rigid_dynamic = static_cast<physx::PxRigidDynamic*>(c_col->actor);
        if (rigid_dynamic) {
            rigid_dynamic->setKinematicTarget(tr);
        }
    }
    else {
        destroy();
    }
}

void TCompKinematicBullet::renderDebug() {
}

void TCompKinematicBullet::destroy() {
    CHandle(this).getOwner().destroy();
    CHandle(this).destroy();
}