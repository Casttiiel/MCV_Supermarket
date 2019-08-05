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





--MILESTONE 3
--zona panaderia

function on_create_enemies_zone_cupcake_player()

	execDelayedAction("changeScene(\"tutorial_scene\")",0)	

	h_cupcake1 = GameController:spawnPrefab("data/prefabs/enemies/bt_cupcake.json", VEC3(423, -27.247, 64), QUAT(0, 0, 0, 1),1);
	GameController:updateCupcakeCurveByHandle("curvaCupcakePanaderia1",h_cupcake1);
	GameController:setPauseEnemyByHandle(h_cupcake1,false);
	
	h_cupcake2 = GameController:spawnPrefab("data/prefabs/enemies/bt_cupcake.json", VEC3(417, -27.247, 68), QUAT(0, 0, 0, 1),1);
	GameController:updateCupcakeCurveByHandle("curvaCupcakePanaderia2",h_cupcake2);
	GameController:setPauseEnemyByHandle(h_cupcake2,false);

	h_cupcake3 = GameController:spawnPrefab("data/prefabs/enemies/bt_cupcake.json", VEC3(412, -27.247, 60), QUAT(0, 0, 0, 1),1);
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

function on_cinematic_golem(name,flag)
	GameController:inCinematicGolem(name,flag)
end

function destroy_and_wake_up(name_golem,name_wall,intensity)
	-- OLD: GameController:destroyWallByName(name_wall,name_golem, intensity) -- rompe el muro de delante del golem
	GameController:wakeUpGolem(name_golem) -- despierta al golem
end

function script_ice_1_player()
	
	execDelayedAction("changeScene(\"congelados_scene\")",0)	
	GameController:resetCamera(); 
	execDelayedAction("on_cinematic(true)",0);
	execDelayedAction("on_cinematic_golem(\"golem1\",true)",0);
	execDelayedAction("on_lock_camera3(false)",0); -- esto bloquea la camara normal o activa la camara de la cinameatica?
	execDelayedAction("on_blending_camera(\"CameraGolem1\", 5,\"linear\")",0); --poner en mapa la cmara correspondiente a donde estaran ubicados los termoestatos
	
    execDelayedAction("destroy_and_wake_up(\"golem1\",\"Box007\", 20)",8); -- el tiempo luego se ajustara

	execDelayedAction("on_blending_camera(\"PlayerCamera\", 5,\"linear\")",12);
	execDelayedAction("on_lock_camera3(true)",16);
	execDelayedAction("on_cinematic_golem(\"golem1\",false)",15);
	execDelayedAction("on_cinematic(false)",15);

    -- tocara meter en funciones separadas muchas de estas cosas: (para hacer el delayed action)

	-- cinematica que apunte al golem con el muro -- COMENTAR ESTO CUANDO SE DESCOMENTE LO DE ARRIBA
	--GameController:destroyWallByName("Box007","golem1", 20) -- rompe el muro de delante del golem
	--GameController:wakeUpGolem("golem1") -- despierta al golem
	-- animacion golem golpeando (?)
	

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
	GameController:wakeUpGolem("golem3")
	GameController:sleepGolem("golem2")
	execDelayedAction("on_delete_handle(\"trigger003\")",0);
	-- despierta al ultimo golem y rompe el ultimo muro
	-- funcion para despertar al golem y que fije al player pah siempre
	-- funcion que rompa el muro en varios trozos
end



function cinematic_panel_player()
	GameController:resetCamera();
	execDelayedAction("on_cinematic(true)",0);
	execDelayedAction("on_lock_camera3(false)",0.1);
	execDelayedAction("on_blending_camera(\"CameraPanel001\", 4,\"linear\")",0.3);
	execDelayedAction("on_blending_camera(\"PlayerCamera\", 4,\"linear\")",6);
	execDelayedAction("on_lock_camera3(true)",9);
	execDelayedAction("on_cinematic(false)",9);
	execDelayedAction("on_delete_handle(\"trigger007\")",0);
end

function in_trap_tube_enemies_player()
	
	--GameController:resetCamera();
	execDelayedAction("on_cinematic(true)",3);
	execDelayedAction("on_lock_camera3(false)",3);
	execDelayedAction("on_blending_camera(\"CameraEnemiesTube\", 5,\"linear\")",3); --poner en mapa la cmara correspondiente a donde estaran ubicados los termoestatos
	execDelayedAction("on_blending_camera(\"PlayerCamera\", 5,\"linear\")",7);
	execDelayedAction("on_lock_camera3(true)",7);
	execDelayedAction("on_cinematic(false)",7);

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
	h_suishi1 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(102,-0.193,100), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica1",h_suishi1);
	GameController:setPauseEnemyByHandle(h_suishi1,false);
	
	h_suishi2 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(86,-0.193,85), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica2",h_suishi2);
	GameController:setPauseEnemyByHandle(h_suishi2,false);

	h_suishi3 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(93,-0.193,68), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica3",h_suishi3);	
	GameController:setPauseEnemyByHandle(h_suishi3,false);

	h_suishi4 = GameController:spawnPrefab("data/prefabs/enemies/bt_ranged_sushi.json", VEC3(126,-0.193, 78), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica4",h_suishi4);
	GameController:setPauseEnemyByHandle(h_suishi4,false);

	h_suishi5 = GameController:spawnPrefab("data/prefabs/enemies/bt_ranged_sushi.json", VEC3(61,-0.193,81), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica5",h_suishi5);
	GameController:setPauseEnemyByHandle(h_suishi5,false);

	h_suishi6 = GameController:spawnPrefab("data/prefabs/enemies/bt_ranged_sushi.json", VEC3(128,-0.193, 100), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica6",h_suishi6);
	GameController:setPauseEnemyByHandle(h_suishi6,false);

	h_suishi7 = GameController:spawnPrefab("data/prefabs/enemies/bt_ranged_sushi.json", VEC3(116,-0.193,63), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica7",h_suishi7);
	GameController:setPauseEnemyByHandle(h_suishi7,false);

	h_suishi8 = GameController:spawnPrefab("data/prefabs/enemies/bt_ranged_sushi.json", VEC3(104,-0.193,88), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica8",h_suishi8);
	GameController:setPauseEnemyByHandle(h_suishi8,false);

	--suishis piso superior
	h_suishi9 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(78,7.632,121), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica9",h_suishi9);
	GameController:setPauseEnemyByHandle(h_suishi9,true);

	h_suishi10 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(60,7.632,139), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica10",h_suishi10);
	GameController:setPauseEnemyByHandle(h_suishi10,true);

	h_suishi11 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(58,7.632,110), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica11",h_suishi11);
	GameController:setPauseEnemyByHandle(h_suishi11,true);

	h_suishi12 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(80,7.632,137), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("curvaSuihiAsiatica12",h_suishi12);
	GameController:setPauseEnemyByHandle(h_suishi12,true);

	--suishis que no te atacan hasta el derretir el 2do termoestato

	h_suishi13 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(57,-0.193,67), QUAT(0, 0, 0, 1),1);
	GameController:setPauseEnemyByHandle(h_suishi13,true);
	h_suishi14 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(61,-0.193,65), QUAT(0, 0, 0, 1),1);
	GameController:setPauseEnemyByHandle(h_suishi14,true);
	h_suishi15 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(71,-0.193,61), QUAT(0, 0, 0, 1),1);
	GameController:setPauseEnemyByHandle(h_suishi15,true);
	execDelayedAction("changeScene(\"asiatic_scene\")",0);
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
	execDelayedAction("changeCurvePlatform(\"curvaTrampaPinchos001\",\"trampa_pinchos001\")",0);
	execDelayedAction("changeCurvePlatform(\"curvaTrampaPinchos002\",\"trampa_pinchos002\")",0);
	execDelayedAction("changeCurvePlatform(\"curvaTrampaPinchos003\",\"trampa_pinchos003\")",0);
	execDelayedAction("changeCurvePlatform(\"curvaTrampaPinchos004\",\"trampa_pinchos004\")",0);
	execDelayedAction("activePlatformByName(\"trampa_pinchos001\")",0.1);
	execDelayedAction("activePlatformByName(\"trampa_pinchos002\")",0.3);
	execDelayedAction("activePlatformByName(\"trampa_pinchos003\")",0.5);
	execDelayedAction("activePlatformByName(\"trampa_pinchos004\")",0.7);
	execDelayedAction("activePlatformByName(\"trap001\")",0);
	execDelayedAction("activePlatformByName(\"trap002\")",0.5);
	execDelayedAction("activePlatformByName(\"trap003\")",0.3);
	execDelayedAction("on_delete_handle(\"triggerActivarPlataformas\")",0);
	--suishis PB
	h_suishi1 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(10,0.000,-35), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("CurvaSuishiCarn1",h_suishi1);
	GameController:setPauseEnemyByHandle(h_suishi1,false);
	
	h_suishi2 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(28,0.000,-65), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("CurvaSuishiCarn2",h_suishi2);
	GameController:setPauseEnemyByHandle(h_suishi2,false);

	h_suishi3 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(30,0.000,-33), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("CurvaSuishiCarn3",h_suishi3);
	GameController:setPauseEnemyByHandle(h_suishi3,false);
	--suishis P1
	h_suishi4 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-1,6.157,-67), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("CurvaSuishiCarn4",h_suishi4);
	GameController:setPauseEnemyByHandle(h_suishi4,false);
	--suishis P2
	h_suishi5 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(2.5,12.182,-30), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("CurvaSuishiCarn5",h_suishi5);
	GameController:setPauseEnemyByHandle(h_suishi5,false);
	--suishis P3
	h_suishi6 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(34,14.962,-29), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("CurvaSuishiCarn6",h_suishi6);
	GameController:setPauseEnemyByHandle(h_suishi6,false);
	--suishis P4
	h_suishi7 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(33,22.063,-65), QUAT(0, 0, 0, 1),1);
	GameController:updateEnemyCurveByHandle("CurvaSuishiCarn7",h_suishi7);
	GameController:setPauseEnemyByHandle(h_suishi7,false);
	--Audio
	GameController:updateSoundtrackID(5);
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


--Prueba carga de escena desde trigger, de momento carga NAVMESH
function changeScene(name)
	GameController:loadScene(name);
end