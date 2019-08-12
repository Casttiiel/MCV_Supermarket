#include "mcv_platform.h"
#include "module_boot.h"
#include "engine.h"
#include "entity/entity_parser.h"

CModuleBoot::CModuleBoot(const std::string& name)
  : IModule(name)
{}

bool CModuleBoot::start()
{
	
	//Engine.getNavmesh().createNavmesh("data/navmesh/solo_navmesh.bin");
	//Engine.getNavmesh().createNavmesh("data/navmesh/all_tiles_tilecache.bin");//TODO crar modulo de carga de Mapa
	
	//Engine.getNavmesh().createNavmesh("data/navmesh/navmesh_pequena.bin");
  json json = loadJson("data/boot.json");
  auto prefabs = json["scenes_to_load"].get< std::vector< std::string > >();
  /*for (auto& p : prefabs) {
    TFileContext fc(p);
    TEntityParseContext ctx;
    PROFILE_FUNCTION_COPY_TEXT( p.c_str() );
    dbg("Parsing boot prefab %s\n", p.c_str());
    parseScene(p, ctx);
  }*/

   prefabs = json["prefabs_to_prefaload"].get< std::vector< std::string > >();
  for (auto& p : prefabs) {
	  TFileContext fc(p);
	  TEntityParseContext ctx;
	  PROFILE_FUNCTION_COPY_TEXT(p.c_str());
	  dbg("Parsing boot prefab %s\n", p.c_str());
	  parseScene(p, ctx);
	  for (auto h : ctx.entities_loaded)
		  h.destroy();
  }

  CHandleManager::destroyAllPendingObjects();

  //loadJsonScenes("data/boot.json");//cambiar a bootScenes.json

  return true;
}



