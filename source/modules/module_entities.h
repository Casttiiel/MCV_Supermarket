#pragma once

#include "modules/module.h"
#include "entity/entity.h"

class CModuleEntities : public IModule
{
  float time_scale_factor = 1.f;
  std::vector< CHandleManager* > om_to_update;
  std::vector< CHandleManager* > om_to_render_debug;
  void loadListOfManagers(const json& j, std::vector< CHandleManager* > &managers);
  void renderDebugOfComponents();
  CHandle player_handle;

public:
  CHandle getPlayerHandle();
  CModuleEntities(const std::string& aname) : IModule(aname) { }
  void destroyEntities();
  bool start() override;
  void stop() override;
  void update(float delta) override;
  void renderDebug() override;
  void renderInMenu() override;

};

std::vector<CHandle> getAllEntities();
CHandle getEntityByName(const std::string& name);
CHandle getEntityById(const int& id);
