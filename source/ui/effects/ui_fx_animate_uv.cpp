#pragma once

#include "mcv_platform.h"
#include "ui/effects/ui_fx_animate_uv.h"
#include "ui/ui_widget.h"

namespace UI
{
  void CFXAnimateUV::update(float dt)
  {
    assert(_owner);

    TImageParams* imageParams = _owner->getImageParams();
    if (!imageParams)
    {
      return;
    }

    imageParams->minUV += _speed * dt;
    imageParams->maxUV += _speed * dt;
  }
}
