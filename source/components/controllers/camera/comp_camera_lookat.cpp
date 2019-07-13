#include "mcv_platform.h"
#include "comp_camera_lookat.h"
#include "components/common/comp_transform.h"

DECL_OBJ_MANAGER("camera_lookat", TCompCameraLookat);

void TCompCameraLookat::debugInMenu()
{
  ImGui::Text("Target: %.s", _target.isValid() ? ((CEntity*)_target)->getName() : "...");
}

void TCompCameraLookat::load(const json& j, TEntityParseContext& ctx) {
  _targetName = j.value("target", "");
}

void TCompCameraLookat::update(float scaled_dt)
{
  if (!_target.isValid())
  {
    _target = getEntityByName(_targetName);
    
    if (!_target.isValid())
      return;
  }

  TCompTransform* cTransform = get<TCompTransform>();
  if (!cTransform)
    return;
  CEntity* eTarget = _target;
  TCompTransform* cTargetTransform = eTarget->get<TCompTransform>();
  if (!cTargetTransform)
    return;

  const VEC3& pos = cTransform->getPosition();
  const VEC3& targetPos = cTargetTransform->getPosition();
  if (pos != targetPos)
  {
    cTransform->lookAt(pos, targetPos);
  }
}
