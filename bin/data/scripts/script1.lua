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

--MILESTONE 3 Zona Suishy script parte Cinematica + asignacion de curvas + despertar suishis

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



function cinematic_scene_termoestatos_player()
	--evento cinematica
	GameController:resetCamera();
	execDelayedAction("on_cinematic(true)",0);
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
	execDelayedAction("on_delete_handle(\"Box049\")",0);
	--execDelayedAction("spawn(\"data/prefabs/enemies/sushi.json\", VEC3(-10, 0, -43.5), QUAT(0, 0, 0, 1),1)",0);
	
	--Suishis piso inferior
	

end

function createEnemies_player()
	--h_suishi1 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(7.144,28.00,-84.962), QUAT(0, 0, 0, 1),1);
	--GameController:updateEnemyCurveByHandle("Curve003",h_suishi1);
	--GameController:setPauseEnemyByHandle(h_suishi1,false);
	
	--h_suishi2 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-1.837,28.00,-95.127), QUAT(0, 0, 0, 1),1);
	--GameController:updateEnemyCurveByHandle("Curve003",h_suishi2);
	--GameController:setPauseEnemyByHandle(h_suishi2,false);

	--h_suishi3 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(20.943,28.000,-75.646), QUAT(0, 0, 0, 1),1);
	--GameController:updateEnemyCurveByHandle("Curve003",h_suishi3);
	--GameController:setPauseEnemyByHandle(h_suishi3,false);

	--h_suishi4 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(14.719,28.000, -70.336), QUAT(0, 0, 0, 1),1);
	--GameController:updateEnemyCurveByHandle("Curve003",h_suishi4);
	--GameController:setPauseEnemyByHandle(h_suishi4,false);

	--h_suishi5 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-24.869,28.000,-95.127), QUAT(0, 0, 0, 1),1);
--	GameController:updateEnemyCurveByHandle("Curve003",h_suishi5);
--	GameController:setPauseEnemyByHandle(h_suishi5,false);

	--h_suishi6 = GameController:spawnPrefab("data/prefabs/enemies/bt_ranged_sushi.json", VEC3(7.144,28.000, -74.094), QUAT(0, 0, 0, 1),1);
	--GameController:updateEnemyCurveByHandle("Curve003",h_suishi6);
	--GameController:setPauseEnemyByHandle(h_suishi6,false);

	--h_suishi7 = GameController:spawnPrefab("data/prefabs/enemies/bt_ranged_sushi.json", VEC3(2.513,28.000,-67.569), QUAT(0, 0, 0, 1),1);
	--GameController:updateEnemyCurveByHandle("Curve003",h_suishi7);
	--GameController:setPauseEnemyByHandle(h_suishi7,false);

	--h_suishi8 = GameController:spawnPrefab("data/prefabs/enemies/bt_ranged_sushi.json", VEC3(-8.633,28.000,-83.607), QUAT(0, 0, 0, 1),1);
	--GameController:updateEnemyCurveByHandle("Curve003",h_suishi8);
	--GameController:setPauseEnemyByHandle(h_suishi8,false);

	--suishis piso superior
	--h_suishi9 = GameController:spawnPrefab("data/prefabs/enemies/bt_ranged_sushi.json", VEC3(-26.688,35.652,-48.338), QUAT(0, 0, 0, 1),1);
	

	--h_suishi10 = GameController:spawnPrefab("data/prefabs/enemies/bt_ranged_sushi.json", VEC3(-17.853,35.652,-37.984), QUAT(0, 0, 0, 1),1);
	

	--h_suishi11 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-26.688,35.652,-55.408), QUAT(0, 0, 0, 1),1);
	

	--h_suishi12 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-34.754,35.652,-48.338), QUAT(0, 0, 0, 1),1);
	

	--suishis que no te atacan hasta el derretir el 2do termoestato

	--h_suishi13 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-38,28,-102), QUAT(0, 0, 0, 1),1);

	--h_suishi14 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-33.754,28,-104), QUAT(0, 0, 0, 1),1);

	--h_suishi15 = GameController:spawnPrefab("data/prefabs/enemies/bt_sushi.json", VEC3(-29,28,-105), QUAT(0, 0, 0, 1),1);

	execDelayedAction("changeScene(\"asiatic_scene\")",0);--Prueba de cambio de escena
	--execDelayedAction("on_delete_handle(\"Box050\")",0);


	
end

function setTransformObject_(name,pos,yaw,pith,roll)
	GameController:setTransformObject(name,pos,yaw,pith,roll);
	--GameController:destroyCHandleByName(name)
end

function wakeUpSuishisSecondFloor()
	if h_suishi9 ~= nil then --con uno vale, ya que se instancian a la vez
		GameController:updateEnemyCurveByHandle("Curve003",h_suishi9);
		GameController:setPauseEnemyByHandle(h_suishi9,false);
		GameController:updateEnemyCurveByHandle("Curve003",h_suishi10);
		GameController:setPauseEnemyByHandle(h_suishi10,false);
		GameController:updateEnemyCurveByHandle("Curve003",h_suishi11);
		GameController:setPauseEnemyByHandle(h_suishi11,false);
		GameController:updateEnemyCurveByHandle("Curve003",h_suishi12);
		GameController:setPauseEnemyByHandle(h_suishi12,false);
	end
end

function defrost2()
	
	execDelayedAction("changeLookAt(\"cubosHielo_033\",0.5)",0);
	execDelayedAction("on_delete_handle(\"cubosHielo_033\")",5);--simulamos tiempo de animacion morph por ahora
	execDelayedAction("changeLookAt(\"cubosHielo_033\",0)",4.9);
	--GameController:destroyCHandleByName(name)

end

function changeLookAt(name,amount)
	handle = GameController:getPlayerHandle();
	t_compskellook = toCompSkelLookAt(toEntity(handle):getCompByName("skel_lookat"));
	t_compskellook.target_entity_name = name
	t_compskellook.amount= amount
	t_compskellook.flagFirst = true--nuevo, esto es para el giro inicial de cabeza
end


function defrost1()
	
	
	execDelayedAction("changeLookAt(\"cubosHielo_034\",0.5)",0);
	execDelayedAction("on_delete_handle(\"cubosHielo_034\")",5);--simulamos tiempo de animacion morph por ahora
	execDelayedAction("changeLookAt(\"cubosHielo_034\",0)",4.9);


end

function on_spawnPrefab(prefab,pos,quat,scale)
	handle = GameController:spawnPrefab(name, pos, quat,scale);
	GameController:setPauseEnemyByHandle(handle,false)
end

function on_ambush()
	--suishis que se despiertan al romper ultimo termoestato
	if h_suishi13 ~= nil then
		GameController:updateEnemyCurveByHandle("Curve003",h_suishi13);
		GameController:setPauseEnemyByHandle(h_suishi13,false);
		GameController:updateEnemyCurveByHandle("Curve003",h_suishi14);
		GameController:setPauseEnemyByHandle(h_suishi14,false);
		GameController:updateEnemyCurveByHandle("Curve003",h_suishi15);
		GameController:setPauseEnemyByHandle(h_suishi15,false);
	end
end


--MILESTONE 3  zona madalenas Plataforma activada

function activePlataform_player()
	GameController:activatePlatformByName("plat1346467");
end

--Prueba carga de escena desde trigger
function changeScene(name)
	GameController:loadScene(name);
end