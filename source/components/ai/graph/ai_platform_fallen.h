#ifndef _AI_PLATFORM_FALLEN
#define _AI_PLATFORM_FALLEN
#include "ai_controller.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"


class CAIFallenPlatform : public IAIController
{

	void IdleState(float dt);
	void DeadState(float dt);
	DECL_SIBLING_ACCESS();
public:

	void Init();
	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	void renderDebug();
	void onCollision(const TMsgOnContact& msg);
	static void registerMsgs();
	float timeRecovery = 7.f;
	float timeRecoveryAux = timeRecovery;

	float timeFallen = 1.f;
	float timeFallenAux;


private:
	bool flagfallen = false;
	VEC3 pos;
	


	

	



};

#endif _AI_PLATFORM_FALLEN