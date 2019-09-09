#pragma once

#include "mcv_platform.h"

namespace UI
{
  class CWidget;

  class CEffect
  {
  public:
    virtual void start() {}
    virtual void stop() {}
    virtual void update(float dt) {}
	virtual void onDeactivate() {};
  protected:
    CWidget* _owner = nullptr;
    
    friend class CParser;
  };
}
