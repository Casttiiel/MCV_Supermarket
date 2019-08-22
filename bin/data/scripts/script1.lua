SLB.using(SLB)

GameController = getGameController()

--variables globales
h_suishi9 = nil
h_suishi10 = nil
h_suishi11 = nil
h_suishi12 = nil
h_suishi13 = nil
h_suishi14 = nil
h_suishi15 = nil



--funciones
function on_heal_player()	
	GameController:healPlayer()
end

function on_restore_madness()
	GameController:restoreMadness()
end

function set_pause_enemy_by_handle(handle,pause)
	GameController:setPauseEnemyByHandle(handle,pause)
end

function delete_grietas(name)
	on_deleteElement(name)
end

function on_spawn_prefab(name)
	GameController:spawnPrefab(name, Vector3(0.0, 0.0, 0.0))
end

function on_god_mode(active)
	GameController:setGodMode(active)
end

function on_active_enemies(active)
	if active == "true" then
		GameController:resumeEnemies()
	else
		GameController:stopEnemies()
	end
end

function on_gameplay_fragment_activate(modl)
	GameController:activateGameplayFragment(modl)
end

function on_gameplay_fragment_deactivate(modl)
	GameController:deactivateGameplayFragment(modl)
end

function on_salute()
	salute()
end

function on_activate_spawner(id)
	GameController:activateSpawner(id)
end

function on_wake_sushi(id)
	GameController:wakeUpSushi(id)
end

function on_deleteElement(id)
	GameController:deleteElement(id)
end

function on_ambush_event_1()
	on_wake_sushi("002")
	on_wake_sushi("003")
	on_deleteElement("pisoFragmento01")
	on_deleteElement("pisoFragmento02")
	on_deleteElement("pisoFragmento03")
	on_activate_spawner("Spawner_dumpling_01")
	on_activate_spawner("Spawner_dumpling_002")
	on_activate_spawner("Spawner_dumpling_003")

end

function on_ambush_event_2()

end


function setPauseEnemyName(name,state)	
	GameController:setPauseEnemyByName(name,state);
end


--MILESTONE 3
--zona panaderia

function on_create_enemies_zone_cupcake_player()

	execDelayedAction("changeScene(\"tutorial_scene\")",0)	

	h_cupcake1 = GameController:spawnPrefab("data/prefabs/enemies/bt_cupcake.json", VEC3(185, -49, -86), QUAT(0, 0, 0, 1),1);
	GameController:updateCupcakeCurveByHandle("curvaCupcakePanaderia1",h_cupcake1);
	GameController:setPauseEnemyByHandle(h_cupcake1,false);
	
	h_cupcake2 = GameController:spawnPrefab("data/prefabs/enemies/bt_cupcake.json", VEC3(184, -49, -75), QUAT(0, 0, 0, 1),1);
	GameController:updateCupcakeCurveByHandle("curvaCupcakePanaderia2",h_cupcake2);
	GameController:setPauseEnemyByHandle(h_cupcake2,false);

	h_cupcake3 = GameController:spawnPrefab("data/prefabs/enemies/bt_cupcake.json", VEC3(176, -49, -81), QUAT(0, 0, 0, 1),1);
	GameController:updateCupcakeCurveByHandle("curvaCupcakePanaderia3",h_cupcake3);
	GameController:setPauseEnemyByHandle(h_cupcake3,false);
	execDelayedAction("on_delete_handle(\"triggerCreacionCupcackes\")",0);

	--Audio
	GameController:updateSoundtrackID(2);
end


--Zona Suishy script parte Cinematica + asignacion de curvas + despertar suishis

function on_blending_camera(name,speed,typeInterpolator)
	GameController:blendingCamera(name,speed,typeInterpolator)
end

function on_lock_camera3(activate)
	GameController:lockCamera3Person(activate)
end

function on_delete_handle(name)
	GameController:destroyCHandleByName(name)
end

function on_cinematic(flag)
	GameController:inCinematic(flag)
end

function on_cinematic_special(flag,type)
	GameController:inCinematicSpecial(flag,type)
end


function on_cinematic_golem(name,flag)
	GameController:inCinematicGolem(name,flag)
end

function destroy_and_wake_up(name_golem,name_wall,intensity)
	-- OLD: GameController:destroyWallByName(name_wall,name_golem, intensity) -- rompe el muro de delante del golem
	GameController:wakeUpGolem(name_golem) -- despierta al golem
end

function setViewDistanceEnemy(distance,handle,type)
	GameController:setViewDistanceEnemyByHandle(distance,handle,type);
end

function script_ice_1_player()
	
	execDelayedAction("changeScene(\"congelados_scene\")",0)	
	GameController:resetCamera(); 
	execDelayedAction("on_cinematic(true)",0);
	execDelayedAction("on_lock_camera3(false)",0);
	execDelayedAction("on_blending_camera(\"CameraPanel001\", 5,\"Quadin\")",0);
	execDelayedAction("on_blending_camera(\"CameraPanel002\", 5,\"Quadin\")",6);
    execDelayedAction("destroy_and_wake_up(\"golem2\",\"Box007\", 20)",15); 
	execDelayedAction("on_blending_camera(\"PlayerCamera\", 5,\"Quadin\")",12);
	execDelayedAction("on_cinematic(false)",16);
	execDelayedAction("on_lock_camera3(true)",16);
	handle = GameController:entityByName("golem2");
	execDelayedAction("setViewDistanceEnemy(80,handle,4)",15.5);
	GameController:setHeightEnemyByHandle(7.0,handle,4);
	execDelayedAction("on_delete_handle(\"trigger001\")",0);
	--Audio
	GameController:updateSoundtrackID(3);
end

function script_ice_2_player()

	--GameController:destroyWallByName("bx4", "golem2", 20) 
	GameController:wakeUpGolem("golem2")
	GameController:sleepGolem("golem1")
	-- despierta al segundo golem y rompe el segundo muro
	-- funcion para despertar al golem y que fije al player pah siempre
	-- funcion que rompa el muro en varios trozos

	execDelayedAction("on_delete_handle(\"trigger002\")",0);
end

function script_ice_3_player()

	--GameController:destroyWallByName("Box016", "golem3", 20)
	execDelayedAction("setPauseEnemyName(\"golem3\",true)", 0);
	execDelayedAction("on_delete_handle(\"trigger003\")",0);
	-- despierta al ultimo golem y rompe el ultimo muro
	-- funcion para despertar al golem y que fije al player pah siempre
	-- funcion que rompa el muro en varios trozos
end



function wake_up_last_golem_player()
	h_golem = GameController:entityByName("golem3"); 
	execDelayedAction("setPauseEnemyName(\"golem3\",false)", 0);
	GameController:setHeightEnemyByHandle(10.0,h_golem,4);
	--execDelayedAction("on_delete_handle(\"trigger007\")",0);
end

function in_trap_tube_enemies_player()
	
	--GameController:resetCamera();
	h_golem = GameController:entityByName("golem3"); 
	execDelayedAction("setPauseEnemyName(\"golem3\",true)", 0);	
	execDelayedAction("on_cinematic(true)",3);
	execDelayedAction("on_lock_camera3(false)",3);
	execDelayedAction("on_blending_camera(\"CameraEnemiesTube\", 5,\"linear\")",3); --poner en mapa la cmara correspondiente a donde estaran ubicados los termoestatos
	execDelayedAction("on_blending_camera(\"PlayerCamera\", 2,\"linear\")",7);
	execDelayedAction("on_lock_camera3(true)", 9);
	execDelayedAction("on_cinematic(false)",9);

	handle = GameController:entityByName("enemies_in_tube");--prefab
	t_compenemiestube = toCompEnemiesInTube(toEntity(handle):getCompByName("enemies_in_tube"));
	t_compenemiestube.activateTrap = true


	execDelayedAction("on_delete_handle(\"trigger008\")",0);



end





function cinematic_scene_termoestatos_player()
	--evento cinematica
	GameController:resetCamera();
	execDelayedAction("on_cinematic(true)",0.1);
	execDelayedAction("on_lock_camera3(false)",0);
	execDelayedAction("on_blending_camera(\"CameraTermoestato1\", 5,\"linear\")",0); --poner en mapa la cmara correspondiente a donde estaran ubicados los termoestatos
	execDelayedAction("on_blending_camera(\"CameraTermoestato2\", 5,\"linear\")",7); --volver a la camara del jugador
	execDelayedAction("on_blending_camera(\"PlayerCamera\", 5,\"linear\")",14);
	execDelayedAction("on_lock_camera3(true)",19);
	execDelayedAction("on_cinematic(false)",19);
	--asignar curvas a enemigos
	--execDelayedAction("curve_to_enemy_zone_first_floor()",0)
	--despertar enemigos
	--execDelayedAction("active_sushi_asiatic_zone_first_floor()",0)

	--prueba de cargar la sgunda escena
	--execDelayedAction("changeScene(\"tutorial_scene\")",0);--Prueba de cambio de escena
	--eliminar trigger de animacion
	execDelayedAction("on_delete_handle(\"trigger3\")",0);
	--execDelayedAction("spawn(\"data/prefabs/enemies/sushi.json\", VEC3(-10, 0, -43.5), QUAT(0, 0, 0, 1),1)",0);
	
	--Suishis piso inferior
	
	--Audio
	GameController:updateSoundtrackID(4);
end


function createEnemies_player()


	execDelayedAction("changeScene(\"asiatic_scene\")",0);
	h_suishi1 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-33,-0.193,-52), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica1",h_suishi1);
	GameController:setPauseEnemyByHandle(h_suishi1,false);
	
	h_suishi2 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-40,-0.193,-62), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica2",h_suishi2);
	GameController:setPauseEnemyByHandle(h_suishi2,false);

	h_suishi3 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-65,-0.193,-69), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica3",h_suishi3);	
	GameController:setPauseEnemyByHandle(h_suishi3,false);

	h_suishi4 = GameController:spawnPrefab("data/prefabs/enemies/bt_ranged_sushi.json", VEC3(-2,-0.193, -73), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica4",h_suishi4);
	GameController:setPauseEnemyByHandle(h_suishi4,false);

	h_suishi5 = GameController:spawnPrefab("data/prefabs/enemies/bt_ranged_sushi.json", VEC3(-11,-0.193,-92), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica5",h_suishi5);
	GameController:setPauseEnemyByHandle(h_suishi5,false);

	h_suishi6 = GameController:spawnPrefab("data/prefabs/enemies/bt_ranged_sushi.json", VEC3(-14,-0.193, -65), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica6",h_suishi6);
	GameController:setPauseEnemyByHandle(h_suishi6,false);

	h_suishi7 = GameController:spawnPrefab("data/prefabs/enemies/bt_ranged_sushi.json", VEC3(-40,-0.193,-80), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica7",h_suishi7);
	GameController:setPauseEnemyByHandle(h_suishi7,false);

	--h_suishi8 = GameController:spawnPrefab("data/prefabs/enemies/bt_ranged_sushi.json", VEC3(104,-0.193,88), QUAT(0, 0, 0, 1),1);--quitar este
	--GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica8",h_suishi8);
	--GameController:setPauseEnemyByHandle(h_suishi8,false); --GameController:setPauseEnemyByHandle(h_suishi8,false);

	--suishis piso superior (Quizas setear el height para que no te ataquen antes de tiempo)
	h_suishi9 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-47,7.632,-18), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica9",h_suishi9);
	GameController:setPauseEnemyByHandle(h_suishi9,true);

	h_suishi10 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-53,7.632,-17), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica10",h_suishi10);
	GameController:setPauseEnemyByHandle(h_suishi10,true);

	h_suishi11 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-64,7.632,-23), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica11",h_suishi11);
	GameController:setPauseEnemyByHandle(h_suishi11,true);

	--h_suishi12 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(80,7.632,137), QUAT(0, 0, 0, 1),1);--quitar este
	--GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica12",h_suishi12);
	--GameController:setPauseEnemyByHandle(h_suishi12,true); --GameController:setPauseEnemyByHandle(h_suishi12,true);

	--suishis que no te atacan hasta el derretir el 2do termoestato

	h_suishi13 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-73,-0.193,-91), QUAT(0, 0, 0, 1),1);
	GameController:setPauseEnemyByHandle(h_suishi13,true);
	h_suishi14 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-58,-0.193,-98), QUAT(0, 0, 0, 1),1);
	GameController:setPauseEnemyByHandle(h_suishi14,true);
	--h_suishi15 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(71,-0.193,61), QUAT(0, 0, 0, 1),1);--quitar este
	--GameController:setPauseEnemyByHandle(h_suishi15,true); --GameController:setPauseEnemyByHandle(h_suishi15,true);
	
	
	execDelayedAction("on_delete_handle(\"trigger2\")",0);


	
end

function setTransformObject_(name,pos,yaw,pith,roll)
	GameController:setTransformObject(name,pos,yaw,pith,roll);
	--GameController:destroyCHandleByName(name)
end




function wakeUpSuishisSecondFloor()
	if h_suishi9 ~= nil then --con uno vale, ya que se instancian a la vez
		
		GameController:setPauseEnemyByHandle(h_suishi9,false);
		
		GameController:setPauseEnemyByHandle(h_suishi10,false);
	
		GameController:setPauseEnemyByHandle(h_suishi11,false);
		
		GameController:setPauseEnemyByHandle(h_suishi12,false);
	end
end

function defrost2()
	
	--execDelayedAction("changeLookAt(\"cubosHielo_033\",0.5)",0);
	--execDelayedAction("on_delete_handle(\"cubosHielo_033\")",5);--simulamos tiempo de animacion morph por ahora
	--execDelayedAction("changeLookAt(\"cubosHielo_033\",0)",4.9);
	--GameController:destroyCHandleByName(name)


	--EN TEORIA ESTO DE AQUI CUANDO ESTE ACABADA LA ESCENA DESCOMENTARLO Y COMENTAR LAS LINEAS DE ARRIBA
	execDelayedAction("changeLookAt(\"cubosHielo_033\",0.5)",0);
	execDelayedAction("playMorph(\"cubosHielo_033\")",0);
	execDelayedAction("changeLookAt(\"cubosHielo_033\",0)",4.9);
	execDelayedAction("on_delete_handle(\"cubosHielo_033\")",5.1);


end

function changeLookAt(name,amount)
	handle = GameController:getPlayerHandle();
	t_compskellook = toCompSkelLookAt(toEntity(handle):getCompByName("skel_lookat"));
	t_compskellook.target_entity_name = name
	t_compskellook.amount= amount
	t_compskellook.flagFirst = true--nuevo, esto es para el giro inicial de cabeza
end

function playMorph(name)
	GameController:playAnimationMorph(name)
end

function stopMorph(name)
	GameController:stopAnimationMorph(name)
end

function defrost1()
	--execDelayedAction("changeLookAt(\"cubosHielo_034\",0.5)",0);
	--execDelayedAction("on_delete_handle(\"cubosHielo_034\")",5);--simulamos tiempo de animacion morph por ahora
	--execDelayedAction("changeLookAt(\"cubosHielo_034\",0)",4.9);

	--EN TEORIA ESTO DE AQUI CUANDO ESTE ACABADA LA ESCENA DESCOMENTARLO Y COMENTAR LAS LINEAS DE ARRIBA
	execDelayedAction("changeLookAt(\"cubosHielo_034\",0.5)",0);
	execDelayedAction("playMorph(\"cubosHielo_034\")",0);
	execDelayedAction("changeLookAt(\"cubosHielo_034\",0)",4.9);
	execDelayedAction("on_delete_handle(\"cubosHielo_034\")",5.1);
end

function on_spawnPrefab(prefab,pos,quat,scale)
	handle = GameController:spawnPrefab(name, pos, quat,scale);
	GameController:setPauseEnemyByHandle(handle,false)
end

function on_ambush()
	--suishis que se despiertan al romper ultimo termoestato
	if h_suishi13 ~= nil then
		GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica13",h_suishi13);
		GameController:setPauseEnemyByHandle(h_suishi13,false);
		GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica14",h_suishi14);
		GameController:setPauseEnemyByHandle(h_suishi14,false);
		GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica15",h_suishi15);
		GameController:setPauseEnemyByHandle(h_suishi15,false);
	end
end


function activePlatformByName(name)
	GameController:activatePlatformByName(name);
end

--MILESTONE 3  zona madalenas Plataforma activada

function activePlataform_player()--activacion de la plataforma zona final
	--GameController:activatePlatformByName("plat1346467");
	execDelayedAction("activePlatformByName(\"plat1346467\")",0.0);
end

--ZONA CARNICERIA
function changeCurvePlatform(nameCurve,namePlatform)
	GameController:updatePlatformCurveByName(nameCurve,namePlatform);
end

--activar plataformas en torre carniceria y creacion de suishis
function activePlataformCarniceria_player()
	execDelayedAction("changeScene(\"carniceria_scene\")",0);
	execDelayedAction("changeCurvePlatform(\"curvaTrampaPinchos001_v2\",\"trampa_pinchos001\")",0);
	execDelayedAction("changeCurvePlatform(\"curvaTrampaPinchos002_v2\",\"trampa_pinchos002\")",0);
	execDelayedAction("changeCurvePlatform(\"curvaTrampaPinchos003_v2\",\"trampa_pinchos003\")",0);
	execDelayedAction("changeCurvePlatform(\"curvaTrampaPinchos004_v2\",\"trampa_pinchos004\")",0);
	execDelayedAction("activePlatformByName(\"trampa_pinchos001\")",0.1);
	execDelayedAction("activePlatformByName(\"trampa_pinchos002\")",0.3);
	execDelayedAction("activePlatformByName(\"trampa_pinchos003\")",0.5);
	execDelayedAction("activePlatformByName(\"trampa_pinchos004\")",0.7);
	execDelayedAction("activePlatformByName(\"trap001\")",0);
	execDelayedAction("activePlatformByName(\"trap002\")",0.5);
	execDelayedAction("activePlatformByName(\"trap003\")",0.3);
	execDelayedAction("on_delete_handle(\"triggerActivarPlataformas\")",0);
	--suishis PB
	h_suishi1 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-146,0.316,-202), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("CurvaSuishiCarn1",h_suishi1);
	GameController:setPauseEnemyByHandle(h_suishi1,true);
	
	h_suishi2 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-126,0.316,-191), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("CurvaSuishiCarn2",h_suishi2);
	GameController:setPauseEnemyByHandle(h_suishi2,true);

	h_suishi3 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-125,0.316,-225), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("CurvaSuishiCarn3",h_suishi3);
	GameController:setPauseEnemyByHandle(h_suishi3,true);
	--suishis P1
	--h_suishi4 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-152,6.157,-222), QUAT(0, 0, 0, 1),1);
	--GameController:updateEnemyCurveByHandle("CurvaSuishiCarn4",h_suishi4);
	--GameController:setPauseEnemyByHandle(h_suishi4,false);
	--t_compname4 = toCompName(toEntity(h_suishi4):getCompByName("name"));
	--t_compname4:setName("Sushi004");
	--suishis P2
	--h_suishi5 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-151,12.182,-189), QUAT(0, 0, 0, 1),1);
	--GameController:setHeightEnemyByHandle(1.0,h_suishi5,1);
	--GameController:updateEnemyCurveByHandle("CurvaSuishiCarn5",h_suishi5);
	--GameController:setPauseEnemyByHandle(h_suishi5,false);
	--t_compname5 = toCompName(toEntity(h_suishi5):getCompByName("name"));
	--t_compname5:setName("Sushi005");
	--suishis P3
	--h_suishi6 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-107,14.962,-192), QUAT(0, 0, 0, 1),1);
	--GameController:setHeightEnemyByHandle(1.0,h_suishi6,1);
	--GameController:updateEnemyCurveByHandle("CurvaSuishiCarn6",h_suishi6);
	--GameController:setPauseEnemyByHandle(h_suishi6,false);
	--t_compname6 = toCompName(toEntity(h_suishi6):getCompByName("name"));
	--t_compname6:setName("Sushi006");
	--suishis P4
	--h_suishi7 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-112,22.063,-231), QUAT(0, 0, 0, 1),1);
	--GameController:setHeightEnemyByHandle(1.0,h_suishi7,1);
	--GameController:updateEnemyCurveByHandle("CurvaSuishiCarn7",h_suishi7);
	--GameController:setPauseEnemyByHandle(h_suishi7,false);
	--creacion de la trampa de los suishis
	trap_sushis = GameController:spawnPrefab("data/prefabs/traps/enemies_in_butcher.json", VEC3(33,100,-65), QUAT(0, 0, 0, 1),1);

	--Audio
	GameController:updateSoundtrackID(5);
	--PRUEBA CAMERA LOCA
	execDelayedAction("cinematica_tower()",0.0);
	execDelayedAction("set_pause_enemy_by_handle(h_suishi1,false)",12.5);
	execDelayedAction("set_pause_enemy_by_handle(h_suishi2,false)",12.5);
	execDelayedAction("set_pause_enemy_by_handle(h_suishi3,false)",12.5);
	
	
end



--activar ascensor final
function activeElevatorFinishCarniceria_player()
	execDelayedAction("activePlatformByName(\"ascensor002\")",0.3);
	execDelayedAction("on_delete_handle(\"triggerActivarAscensorFinal\")",0);
end
--activar ascensor inicial
function activeElevatorInitCarniceria_player()
	execDelayedAction("activePlatformByName(\"ascensor\")",0.3);
	execDelayedAction("on_delete_handle(\"triggerActivarAscensorInicial\")",0);
end


function caidaSuishisPisos_enemy(name)
	--matar/cambiar curvas según el sushi que cae
	
	if name == "Sushi004" then
		h_suishi4 = GameController:entityByName("Sushi004");
		GameController:updateEnemyCurveByHandle("CurvaSuishiCarn1",h_suishi4);
	end
	if name == "Sushi005" then
		h_suishi5 = GameController:entityByName("Sushi005");
		GameController:updateEnemyCurveByHandle("CurvaSuishiCarn2",h_suishi5);
	end
	if name == "Sushi006" then
		h_suishi6 = GameController:entityByName("Sushi006");
		GameController:updateEnemyCurveByHandle("CurvaSuishiCarn3",h_suishi6);
	end
end

function trampaSushisButcher_player()
	execDelayedAction("saveCheckpoint()",0.0);
	--por si queremos poner cinematicas enfocando el fuego o hacer que empiece a llamear el ascensor
end



--Mapa Tutorial panaderia

function activarSalidaPanaderia()

	GameController:resetCamera();
	execDelayedAction("on_cinematic(true)",0.0);
	execDelayedAction("on_lock_camera3(false)",0.0);
	execDelayedAction("on_blending_camera(\"CameraPanaderiaPlat\", 5,\"linear\")",0.1);
	execDelayedAction("on_blending_camera(\"PlayerCamera\",5,\"linear\")",11.5);
	execDelayedAction("on_lock_camera3(true)",16);
	execDelayedAction("on_cinematic(false)",16);


	execDelayedAction("activePlatformByName(\"plat1346469\")",5.2);
	execDelayedAction("activePlatformByName(\"plat1346470\")",7.2);
	execDelayedAction("activePlatformByName(\"plat1346471\")",9.2);

end

function cinematica_tower()
	handleCamera = GameController:entityByName("CameraTower");
	handlePlayer = GameController:getPlayerHandle();
	GameController:resetCamera();
	
	execDelayedAction("on_lock_camera3(false)",0.0);
	execDelayedAction("on_blending_camera(\"CameraTower\", 7,\"Cubicinout\")",0.0);
	execDelayedAction("on_cinematic_special(true,1)",0.0);

	execDelayedAction("on_blending_camera(\"PlayerCamera\", 7,\"Cubicinout\")",7.0);
	execDelayedAction("on_lock_camera3(true)",13.0);
	--t_compCharacterController = toCompCharacterController(toEntity(handlePlayer):getCompByName("character_controller"));
	--t_compCharacterController:ChangeState("ESPECIAL_CINEMATIC");


end

--Prueba carga de escena desde trigger, de momento carga NAVMESH
function changeScene(name)
	GameController:loadScene(name);
end



function saveCheckpoint()
	handlePlayer = GameController:getPlayerHandle();
	t_compTranf = toCompTransform(toEntity(handlePlayer):getCompByName("transform"));
	pos = VEC3(t_compTranf:getPosition().x,t_compTranf:getPosition().y,t_compTranf:getPosition().z);
	rot = QUAT(t_compTranf:getRotation().x,t_compTranf:getRotation().y,t_compTranf:getRotation().z,t_compTranf:getRotation().w);
	GameController:saveCheckpoint(pos, rot);
end