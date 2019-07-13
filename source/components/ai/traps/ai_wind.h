#include "components/ai/graph/ai_controller.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"



class TCompWind : public TCompBase
{

	DECL_SIBLING_ACCESS();
	

public:
	void update(float dt);
	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	static void registerMsgs();
	void onBattery(const TMsgGravity & msg);


	CHandle h_sender;
	CHandle h_bullet;

	float _windDamage = 0.01f;
	float _durationOn = 4.f;
	float _durationOn_init = 4.f;

	float _durationOff = 4.f;
	float _durationOff_init = 2.f;



private:
	bool _enabled = false;
	

	boolean firstTime = true;
	

};
