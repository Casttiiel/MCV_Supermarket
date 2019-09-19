#pragma once
#include "modules/module.h"

class CModuleBoot : public IModule
{
public:
  CModuleBoot(const std::string& name);
  bool start() override;
  bool isLoadAll = false;
  void stop() override;

private:
  float _timer = 0.f;
};


