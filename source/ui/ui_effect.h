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
	virtual void stopUiFx() {};
	virtual void changeSpeedUV(float x, float y) {};
	virtual void changeDuration(float duration) {};
	virtual void setScale(VEC2 scale) {};
	virtual void setMinUV(VEC2 vector) {};
	virtual void setMaxUV(VEC2 vector) {};
	virtual void setInitialScale(VEC2 initialscale) {};
	virtual void setTime(float time) {};


	std::string getName() {
		return name;
	}
  protected:
    CWidget* _owner = nullptr;
	std::string name = "";
    friend class CParser;
  };
}
