clearListener()
gc()

fn isCollider obj = (
	return obj != undefined and (findString obj.name "collider" != undefined ) and obj.parent != null
)

fn isProductLine obj = (
	return obj != undefined and (findString obj.name "product" != undefined ) and obj.parent != null and isLine obj
)

fn isPointLight obj = (
	return obj != undefined and classof obj == Omnilight and obj.parent == null
)

fn isCamera obj = (
		return obj != undefined and  (classOf obj == targetCamera) 
)

fn isLine obj = (
		return obj != undefined and  (classOf obj == line) 
)

fn isDirLight obj = (
	return obj != undefined and (classof obj == Directionallight or classof obj == freeSpot or classof obj == targetSpot) and obj.parent == null
)

fn isTrigger obj = (
	return obj != undefined and obj.layer.name == "triggers"
)

fn isLine obj = (
	return obj != undefined and classof obj == line
)

fn isTargetLight obj = (
	return classof obj == Targetobject
)

fn isMaxShit obj = (
	return classof obj == Point and obj.isFrozen
)

----------------------------------------------------------
struct TSceneExporter (
	fs = TJsonFormatter(), 
	--project_path = "C:/Users/MajoGC/Desktop/mcv_sm/bin/",
	--project_path = "C:/Users/victo/mcv_supermarket/MCV_Supermarket/bin/",
	project_path = "C:/Users/Daniel/Desktop/MCV_Supermarket/bin/",
	--project_path = "D:/Joaquin/Proyecto_43/mcv_sm/bin/",
	--project_path = "C:/Users/mcv17_alumne/Desktop/mcv_sm/bin/",
	base_path = "data/",
	current_scene_name,
	scenes_path = base_path + "scenes/",
	mesh_path = base_path + "meshes/",
	mats_path = base_path + "materials/",
	curve_path = base_path + "curves/",
	productsSize = 0.18,
	numberOfProducts = 12,
	
	fn exportCompName obj = (
		fs.writeKeyValue "name" obj.name
	),
	
	fn isValidName aname = (
		return findString aname " " == undefined 
	),

	----------------------------------------------------------
	fn exportTransform obj = (
		fs.writeComma()
		fs.writeKey "transform" 
		
		local max2mcv = rotateXMatrix -90
		local mcv2max = rotateXMatrix 90
		local mcv_position = obj.position * max2mcv
		
		-- From mcv, we will go to max, apply the max transform and go back to mcv coord system
		local mcv_transform = mcv2max * obj.transform * max2mcv

		-- Take just the rotation as quaterion
		local mcv_quat = mcv_transform.rotationPart as quat
		
	
		fs.beginObj()
			fs.writeKeyValue "pos" mcv_position
			fs.writeComma()
			fs.writeKeyValue "rotation" mcv_quat
			--anadir lookAt si el objeto de la escena es una camera
			if classOf obj == targetCamera then (
				fs.writeComma()
				local mcv_look_at = obj.Target.pos * max2mcv
				fs.writeKeyValue "lookat" mcv_look_at
			)
			if isDirLight obj then (
				fs.writeComma()
				local target = getnodebyname (obj.name + ".Target")
				local mcv_look_at = target.position * max2mcv
				fs.writeKeyvalue "lookat" mcv_look_at
			)
			fs.endObj()
	),
	
	-----------------------------------------------------------
	fn exportTags obj = (
		local tags = getUserProp obj "tags"
		if tags != undefined then (
			local tag_array = filterString tags ","
			fs.writeKeyValue "tags" tag_array
		)
	),
	
	fn exportID obj = (
		local id = getUserProp obj "id"
		if id != undefined then (
			fs.writeComma()
			fs.writeKeyValue "id" id
		)
	),
	fn exportPanel obj =(
		local isPanel =  getUserProp obj "isPanel"
		if isPanel == true then (
			local param1 = getUserProp obj "nombre"
			local param2 = getUserProp obj "elementoAmover"
			
			if param1 == undefined  then (
				throw "A un Panel le faltan datos"
			)
			
			fs.writeComma()
			fs.writeKey "comp_interruptor"
			fs.beginObj()
			if param1 != undefined  then (
				fs.writeKeyValue "nombre" param1
				fs.writeComma()
			)
			if param2 != undefined  then (
				fs.writeKeyValue "elementoAmover" param2 
			)
			fs.endObj()	
		)
	),
	
	fn exportPlatform obj = (
		local isPlatform = getUserProp obj "isPlatform"
		if isPlatform == true then (
			
			local param1 = getUserProp obj "travelTime"
			local param2 = getUserProp obj "platformType"
			local param3 = getUserProp obj "curve"
			local param4 = getUserProp obj "rotationSpeed"
			local param5 = getUserProp obj "rotationTime"
			local param6 = getUserProp obj "angleTurn"
			local param7 = getUserProp obj "direction"
			local param8 = getUserProp obj "axis" 
			local param9 = getUserProp obj "positions" --puntos
			local param10 = getUserProp obj "active"
			
			if param2 == undefined  then (
				throw "A una plataforma le faltan datos"
			)
			
			fs.writeComma()
			fs.writeKey "ai_platform_mobile"
			fs.beginObj()
			if param1 != undefined  then (
				fs.writeKeyValue "mTravelTime_" param1
				fs.writeComma()
			)
			if param4 != undefined  then (
				fs.writeKeyValue "mRotationSpeed_" param4  
				fs.writeComma()
			)
			if param5 != undefined  then (
				fs.writeKeyValue "mRotationTime_" param5 
				fs.writeComma()
			)
			
			if param6 != undefined  then (
				fs.writeKeyValue "angleTurn_" param6 
				fs.writeComma()
			)
			if param7 != undefined  then (
				fs.writeKeyValue "direction" param7 
				fs.writeComma()
			)
			if param8 != undefined  then (
				fs.writeKeyValue "axis" param8 
				fs.writeComma()
			)
			if param3 != undefined  then (
				fs.writeKeyValue "curve" param3 
				fs.writeComma()
			)
			if param9 != undefined  then (
				fs.writeKey "positions" 
				
					fs.arrayOfStrings param9
				
				--fs.writeKeyValue "positions" param9
				fs.writeComma()
			)
			if param10 != undefined  then (
				fs.writeKeyValue "active" param10
				
				--fs.writeKeyValue "positions" param9
				fs.writeComma()
			)
			fs.writeKeyValue "platformType_" param2	
				
			fs.endObj()	
			--if param1 == undefined or param2 == undefined or param3 == undefined then (
			--	throw "A una plataforma le faltan datos"
			--)
			--format "param1 % param2 % param3 %" param1 param2 param3
			--fs.writeKeyPlatform param1 param2 param3
			
		)
	),
	--export camera in scene
	fn exportCamera obj = (
		if classOf obj == targetCamera then (
			local znear = obj.nearclip
			local zfar = obj.farclip
			local fov = obj.fov
			--local pos =  obj.position
			--local rot = obj.transform.rotation
			local targetDistance = obj.targetDistance
			fs.writeComma()
			fs.writeKey "camera"
			fs.beginObj()
			fs.writeKeyValue "near" znear
			fs.writeComma()
			fs.writeKeyValue "far" zfar
			fs.writeComma()
			fs.writeKeyValue "fov" fov
			fs.writeComma()
			fs.writeKeyValue "distanceTarget" targetDistance
			fs.endObj()
		)
	),
	
	-----------------------------------------------------------
	
	fn exportAABB obj = (
		local aabb = getUserProp obj "AABB"
		if aabb != undefined then (
			if aabb == "static" then (
				fs.writeComma()
				fs.writeComponent "abs_aabb"
			)
			if aabb == "dynamic" then (
				fs.writeComma()
				fs.writeComponent "abs_aabb"
				fs.writeComma()
				fs.writeComponent "local_aabb"
			)
		)
	),

	----------------------------------------------------------
	fn exportEditableMesh obj = (
		
		fs.writeComma()
		fs.writeKey "render" 
		fs.beginObj()
		
		
		local mesh_name = mesh_path + current_scene_name +"/" + current_scene_name + "_" + obj.name + ".mesh"
		
		
		--local texture_name = mats_path + obj.name
		fs.writeKeyValue "mesh" mesh_name
		fs.writeComma()
		fs.writeKeyValue "color" obj.wireColor
		
		-- Export the real mesh
		local full_mesh_filename = project_path + mesh_name
		format "full_mesh_filename is %\n" full_mesh_filename
		
		exportMesh obj full_mesh_filename undefined
		
		-- Export material(s)
		local mat = obj.material
		if mat == undefined then (
			throw ("Obj " + obj.name + " does NOT have a material")
		)	
		local me = TMaterialExporter project_path:project_path base_path:base_path
		local custom_mats_path = mats_path + current_scene_name + "/"
		local exported_materials = me.exportMaterial mat custom_mats_path obj
		
		fs.writeComma()
		fs.writeKey "materials"
		fs.arrayOfStrings exported_materials
			
		fs.endObj()
	),
	
	-----------------------------------------------------------
	fn exportCompTrigger obj =(
		if isTrigger obj then (
			fs.writeComma()
			fs.writeKey "trigger" 
			fs.beginObj()
			local onEnter = getuserProp obj "onEnter"
			if onEnter != undefined then(
				fs.writeKeyValue "onEnter" onEnter
			)
			local onExit = getuserProp obj "onExit"
			if onExit != undefined then(
				fs.writeComma()
				fs.writeKeyValue "onExit" onExit
			)
			local delay = getuserProp obj "delay"
			if delay != undefined then(
				fs.writeComma()
				fs.writeKeyValue "delay" delay
			)
			fs.endObj()
			--modification
			local extra_comp = getuserProp obj "extra_comp"
			if extra_comp != undefined then(
				fs.writeComma()
				fs.writeKey extra_comp
				fs.beginObj()
				fs.endObj()
			)
		)
	
	),
	
	
	----------------------------------------------------------
	fn exportPointLight obj = (
		
		fs.writeComma()
		fs.writeKey "light_point" 
		fs.beginObj()

		fs.writeKeyValue "intensity" (obj.multiplier * 5.0)
		fs.writeComma()
		fs.writeKeyValue "color" obj.color
		fs.writeComma()
		fs.writeKeyValue "radius" (obj.DecayRadius * 5.0)
			
		fs.endObj()
	),
	
	----------------------------------------------------------
	fn exportDirLight obj = (
		
		fs.writeComma()
		fs.writeKey "light_dir" 
		fs.beginObj()
		
		fs.writeKeyValue "near" 5
		fs.writeComma()
		
		local target = getnodebyname (obj.name + ".Target")

		fs.writeKeyValue "far" (distance target.pos obj.pos)
		fs.writeComma()
		
		fs.writeKeyValue "fov" 60
		fs.writeComma()
		
		fs.writeKeyValue "color" obj.color
		fs.writeComma()
		
		fs.writeKeyValue "intensity" (obj.multiplier * 5.0)
		fs.writeComma()
		
		fs.writeKeyValue "casts_shadows" obj.castShadows
		fs.writeComma()
		
		fs.writeKeyValue "shadows_resolution" 512
		fs.writeComma()
		
		fs.writeKeyValue "shadows_fmt" "R32_TYPELESS"
			
		fs.endObj()
	),
	
	-- ----------------------------------------------------------
	fn exportCompShape obj = (
		
		fs.beginObj()
		
		if classof obj == Sphere or classof obj == GeoSphere then (
			fs.writeKeyValue "shape" "sphere"
			fs.writeComma()
			fs.writeKeyValue "radius" obj.radius
			
		) else if classof obj == Box then (
			fs.writeKeyValue "shape" "box"
			fs.writeComma()
			local half_size =  ( [abs obj.width, abs obj.height, abs obj.length] * 0.5)
			-- Warning, order might be incorrect!!!
			fs.writeKeyValue "half_size" half_size
			fs.writeComma()
			fs.writeKey "offset"
			fs.beginObj()
				fs.writeKeyValue "pos" [0, half_size.y, 0]
			fs.endObj()
		
			convexmes
			
		) else if classof obj == Editable_Mesh then (
			
			
			local is_platform = getUserProp obj "platform"
			if is_platform != undefined then (
				fs.writeKeyValue "shape" "convex"
				
			)else (
				fs.writeKeyValue "shape" "trimesh"
			)
			--fs.writeKeyValue "shape" "trimesh"
			fs.writeComma()
			-- Warning, order might be incorrect!!!
			local mesh_name = mesh_path + current_scene_name + "_" + obj.parent.name + ".col_mesh"
			local full_mesh_filename = project_path + mesh_name
			exportMesh obj full_mesh_filename "Pos"
			
			fs.writeKeyValue "collision_mesh" mesh_name
		)
		local px_group = getUserProp obj "group"
		if px_group != undefined then (
			fs.writeComma()
			fs.writeKeyValue "group" px_group
		)
		
		local px_mask = getUserProp obj "mask"
		if px_mask != undefined then (
			fs.writeComma()
			fs.writeKeyValue "mask" px_mask
		)
		
		if obj.layer.name == "triggers" then (
			fs.writeComma()
			fs.writeKeyValue "trigger" true
		)
		
		fs.endObj()
	
	),
	
	
	fn exportCompCollider obj candidates = (
		
		fs.writeComma()
		fs.writeKey "collider" 
		fs.beginObj()
		
		fs.writeKey "shapes"
		fs.beginArray()
			local n = 0
			for child in candidates do (
				if n > 0 then fs.writeComma()
				exportCompShape child
				
				
				n = n + 1
			)
		fs.endArray()
		
				
		-- Add it in the user properties panel of max of object(parent), not collider:     density = 10
		local density = getUserProp obj "density"
		--format "export density %\n" density
		if density != undefined then (
			fs.writeComma()
			fs.writeKeyValue "density" density
		)
		
		local is_dynamic = getUserProp obj "dynamic"
		--format "export dynamic %\n" is_dynamic
		if is_dynamic != undefined then (
			fs.writeComma()
			fs.writeKeyValue "dynamic" true
		)
		
		local is_kinematic = getUserProp obj "kinematic"
		--format "export kinematic %\n" is_kinematic
		if is_kinematic != undefined then (
			fs.writeComma()
			fs.writeKeyValue "kinematic" true
		)
		local is_platform = getUserProp obj "platform"
		--format "export platform %\n" is_platform
		if is_platform != undefined then (
			fs.writeComma()
			fs.writeKeyValue "platform" true
		)
		
		fs.endObj()
	),
	
	
	fn exportChildrenColliders obj = (
		
		local candidates = #()
		for child in obj.children do (
			if isCollider child or isTrigger child then append candidates child
		)
		
		if isTrigger obj then candidates = #(obj)
		
		if candidates.count == 0 then return undefined
		
		format "Candidates are %\n" candidates
		--return true
		exportCompCollider obj candidates
		
	),
	-----------------------------------------------------------
	fn exportCompCurveController obj=(
		if classOf obj == line then (
			fs.writeComma()
			fs.writeKey "trans_curve_controller" 
			fs.beginObj()
			local curve_path = getUserProp obj "curve_path"
			fs.writeKeyValue "curve" curve_path
			fs.writeComma()
			local loop = getUserProp obj "loop"
			fs.writeKeyValue "loop" loop
			fs.writeComma()
			local speed_ = getUserProp obj "speed"
			fs.writeKeyValue "speed" speed_
			fs.endObj()
		)
	),
	----------------------------------------------------------
	
	fn exportWalls obj = (
		local typeWall = getUserProp obj "typeWall"
		if typeWall != undefined then (
			fs.writeComma()
			fs.writeKey "comp_destroyable_wall"
			fs.beginObj()
			fs.writeKeyValue "type_wall" typeWall
			fs.endObj()
		)
	),
	
	fn exportMorph obj = (
		
		local modmorph = obj.modifiers[#Morpher]
		if modmorph != undefined then (
			fs.writeComma()
			fs.writeKey "buffers"
			fs.beginObj()
				fs.writeKey "TCtesMorph"
				fs.beginObj()
					fs.writeKeyValue "total_bytes" 16
					fs.writeComma()
					fs.writeKeyValue "slot" 13
				fs.endObj()
			fs.endObj()
			
			
			fs.writeComma()
			fs.writeKey "morph_animation"
			fs.beginObj()
			fs.endObj()
			

				

			
		)
	),
	
	fn exportHierarch obj = (
		local isHierarchy = getUserProp obj "isSon"
		if isHierarchy != undefined then (
			local pos = getUserProp obj "pos"
			local rot = getUserProp obj "rotation"
			local parent = getUserProp obj "parent"
			fs.writeComma()
			fs.writeKey "hierarchy"
			fs.beginObj()
			fs.writeKeyValue "pos" pos
			fs.writeComma()
			if rot != undefined then (
				fs.writeKeyValue "rotation" rot
				fs.writeComma()
			)
			fs.writeKeyValue "parent" parent
			fs.endObj()
			
		)
	),
	
	
	fn exportEntity obj = (
		fs.beginObj()
		fs.writeKey "entity"
			fs.beginObj()
			exportCompName obj
			exportTransform obj
			exportTags obj
			exportID obj
			exportAABB obj
			exportPlatform obj
			exportPanel obj
			exportCamera obj
			exportWalls obj
			exportMorph obj
			exportHierarch obj
			--exportCompCurveController obj
			
			local nCA = custAttributes.count obj
			for idx = 1 to nCA do (
				local ca_data = custAttributes.get obj idx
				if ca_data != undefined then(
					ca_data.exportAsComponent fs
				)
			)
			
			exportChildrenColliders obj
			
			exportCompTrigger obj
			
			if not isTrigger obj  then (
			--if classof obj == Camera then exportCamera obj
				if  not isLine obj then(
				if canConvertTo obj Editable_mesh then exportEditableMesh obj
				)
				if isPointLight obj then exportPointLight obj
				if isDirLight obj then exportDirLight obj
			--else (
			--	format "Warning. Don't kwow how to export obj % of class %\n" obj.name ((classof obj) as string)
			)
			
			fs.endObj()
		fs.endObj()
	),
	
	fn exportProductLine obj = (
		--create a for loop to create the number of entities
		local firstPoint = (interpCurve3D obj 1 0.0)
		local lastPoint = (interpCurve3D obj 1 1.0)
		local dist = distance firstPoint lastPoint
		local step = productsSize / dist
				
		local i = 0.0
		while i <= 1.0 do (
			if i > 0.0 then  fs.writeComma()
			fs.beginObj()
			fs.writeKey "entity"
				fs.beginObj()
				exportCompName obj
				fs.writeComma()
			
				--custom export transform
				fs.writeKey "transform" 
				
				local max2mcv = rotateXMatrix -90
				local mcv2max = rotateXMatrix 90
				local pos = (interpCurve3D obj 1 i)--obj position must be a calculated position from the curve

				local mcv_position =  pos * max2mcv 
				
				local mcv_transform = mcv2max * obj.transform * max2mcv
				local mcv_quat = mcv_transform.rotationPart as quat
				fs.beginObj()
					fs.writeKeyValue "pos" mcv_position
					fs.writeComma()
					fs.writeKeyValue "rotation" mcv_quat
				fs.endObj()
							
				fs.writeComma()
				local numb_indx = random 1 numberOfProducts
				local product_str = "data/prefabs/products/product"+numb_indx as string+".json"
				fs.writeKeyValue "prefab"  product_str
				
				fs.endObj()
			fs.endObj()
			
			i += step
		)
	),
	
	fn exportAll = (
		
		-- Decide output filename based on .max filename
		current_scene_name = getFilenameFile maxFileName
		local full_path = project_path + scenes_path + current_scene_name + ".json"
		format "Exporting to % %\n" full_path  current_scene_name
		
		--local mesh_path_folder = project_path + mesh_path + current_scene_name
		local mesh_path_folder =  project_path + mesh_path + current_scene_name
		local material_path_folder =  project_path + mats_path + current_scene_name
		local texture_path_folder =  project_path +base_path+ "textures/" + current_scene_name
		format "Creating folder %\n"  mesh_path_folder
		format "Creating folder %\n"  material_path_folder
		format "Creating folder %\n"  texture_path_folder
		makeDir mesh_path_folder all:false
		makeDir material_path_folder all:false
		makeDir texture_path_folder all:false
		
		fs.begin full_path
		fs.beginArray()
		
		local nitems = 0
		for obj in $* do (
			if isCollider obj then continue
			if isTargetLight obj then continue
			if isMaxShit obj then continue
			if nitems > 0 then  fs.writeComma()
			if isProductLine obj then (
				exportProductLine obj 
				continue
			)
			exportEntity obj
			nitems = nitems + 1
		)
		
		fs.endArray()
		fs.end()
	)
	
	
)

--gc()
--exporter = TSceneExporter()
--exporter.exportAll()
