#include "components/ai/graph/ai_controller.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"
#include "components/common/comp_transform.h"
#include "components/common/physics/comp_collider.h"
#include "ai_wind.h"


class TCompWindEffect : public IAIController
{
	void OnState(float dt);
	void OffState(float dt);
	void BrokenState(float dt);
	void DeadState(float dt);
	void onWindInfoMsg(const TMsgAssignBulletOwner& msg);
	void onDamageInfoMsg(const TMsgDamage& msg);
	void onTriggerEnter(const TMsgEntityTriggerEnter& msg);
	void onTriggerExit(const TMsgEntityTriggerExit& msg);
	void onBattery(const TMsgGravity & msg);
	
	DECL_SIBLING_ACCESS();
	
public:
	
	void Init();
	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	static void registerMsgs();
	



private:
	bool _enabled = false;


	float _windDamage = 0.01f;
	
	
	float _durationOn = 4.f;
	float _durationOn_init = 4.f;

	float _durationOff = 4.f;
	float _durationOff_init = 2.f;
	

	bool activateWind = false;
	bool inWind = false;
	TCompTransform* c_trans;
	TCompCollider* c_col;
	CHandle c_handle;
	//TCompWind* t_wind;

};
