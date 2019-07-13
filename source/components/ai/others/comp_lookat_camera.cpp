#include "mcv_platform.h"
#include "comp_lookat_camera.h"
#include "components/common/comp_transform.h"
#include "engine.h"

DECL_OBJ_MANAGER("comp_lookat_camera", TCompLookAtCamera);


void TCompLookAtCamera::load(const json& j, TEntityParseContext& ctx) {
}


void TCompLookAtCamera::update(float delta)
 {
	if (!CHandle(this).getOwner().isValid())
		return;
	CEntity* e_player_camera = getEntityByName("PlayerCamera");
	TCompTransform* c_trans = get<TCompTransform>();
	TCompTransform* p_trans = e_player_camera->get<TCompTransform>();
	c_trans->lookAt(c_trans->getPosition(), p_trans->getPosition(), VEC3().Up);
}
