#ifndef _AI_PLATFORM
#define _AI_PLATFORM
#include "ai_controller.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"
#include "geometry/curve.h"



#define POINT_TO_POINT 0 //translacion
#define TRAVELLING 1 //plataforma sigue camino camino de vias
#define ROTATION 2 //rotacion cierto angulo
#define ROTATION_ALWAYS 3 //rotacion infinita
#define POINT_TO_POINT_ROTATION_ALWAYS 4//de punto a punto con giro infinito
#define TRAVELLING_ROTATION_ALWAYS 5 //plataforma sigue camino
#define TRAVELLING_NOT_ROTATION 6 //platafarma sigue camino de vias sin rotar

class CAIMobilePlatform : public IAIController
{
	//Estados POINT_TO_PO
	void SeekwptState(float dt);
	void NextwptState(float dt);
	//Estados ROTATION INFINITY
	void TurnInfinity(float dt);
	//ESTADOS ROTATION
	void Turn(float dt);
	void WaitState(float dt);
	//EStados TRAVELL
	void SeekwptTravelState(float dt);
	//void NextwptTravelState(float dt); no hace falta ya
	void InitialPositionTravelState(float dt);

	//EStados Point to Point rotation infinity
	void SeekwPoinToPointRotationInfinity(float dt);
	void NextwPoinToPointRotationInfinity(float dt);

	//estos travel rotation infinity
	void SeekwTravelRotationInfinity(float dt);

	void InitialPositionRotationTravelState(float dt);

	//plataformas que solo se desplazan siguiendo curva, no rotan
	void InitialPositionTravelNotRotationState(float dt);
	void SeekwptTravelNotRotationState(float dt);
	
	

	DECL_SIBLING_ACCESS();
public:
	void InitTravel();
	void InitPoinToPoint();
	void InitRotationInfinity();
	void InitRotation();
	void InitPoinToPointRotationInfinity();
	void InitTravelRotationInfinity();
	void InitialPositionTravelStateNotRotation();

	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	void renderDebug();
	float computeLength() const;

	static void registerMsgs();
	void onActiveMsgReceived(const TMsgSetActive & msg);
	bool active = true;

private:
	std::vector<VEC3> positions;
	int i = 0;
	VEC3 nextPoint;
	//PxQuat					mLocalRot;		// Local rotation (const data)
	int					mNbPts;
	//PxVec3Alloc*			mPts;
	//const PxBoxObstacle*	mBoxObstacle;
	float					mTravelTime = 0.02f;
	float					mRotationSpeed = 0.5f;
	float					angleTurn = 178.f;
	//LoopMode				mMode;


	int platformType = POINT_TO_POINT;
	float travelTime = 10;
	float currentTime = 0;
	Vector3 currentPosition;

	
	bool controlledByTrigger = false;
	float ratio = 0.f;
	const CCurve* _curve = nullptr;
	MAT44 _curveTransform;
	std::vector<VEC3> _knots;
	float i_ang = 0;
	float rotationTime = 5.f;
	float rotationTimeActual = rotationTime;
	VEC3 axis = VEC3(0, 1, 0);
	int direction = -1;//sentido de rotacion
	void onTriggerEnter(const TMsgEntityTriggerEnter& msg);

	int trigger_id = 19; //trigger que genera el area de damage
	int pair_object_id = 17; //elemento con el que chocara


	

};

#endif _AI_PLATFORM