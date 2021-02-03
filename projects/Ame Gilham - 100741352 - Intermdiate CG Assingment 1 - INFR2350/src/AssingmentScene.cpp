#include "AssingmentScene.h"
#include "imgui.h"

AssingmentScene::AssingmentScene()
	: TTN_Scene()
{
}

AssingmentScene::~AssingmentScene()
{
}

void AssingmentScene::InitScene()
{
	//set up data
	//light data
	m_lightPos = glm::vec3(0.0f, 10.0f, 0.0f);
	m_ambientColor = glm::vec3(1.0f);
	m_rimColor = glm::vec3(1.0f);
	m_ambientStrenght = 5.0f;
	m_specularStrenght = 1.0f;
	m_bands = 6.0f;
	m_rimSize = 0.2f;
	m_attenuationConstant = 0.1f;
	m_attenuationLinear = 0.1f;
	m_attenuationQuadric = 0.1f;
	//shader data
	shaderIndex = 0;
	shaderBool[0] = true;
	for (int i = 1; i < 5; i++) shaderBool[i] = false;
	//water data
	time = 0.0f;
	speed = -2.5f;
	baseHeight = 0.0f;
	heightMultiplier = 0.005f;
	waveLenghtMultiplier = -10.0f;
	//camera control
	rotAmmount = glm::vec2(0.0f, 0.0f);
	mousePos = glm::vec2(0.0f, 0.0f);
	facing = glm::vec3(0.0f, 0.0f, 1.0f);

	//set up assets
	//shaders
	m_skyboxShader = TTN_AssetSystem::GetShader("skybox");
	//non animated
	m_nonAnimatedShaders[0] = TTN_AssetSystem::GetShader("no-lighting");
	m_nonAnimatedShaders[1] = TTN_AssetSystem::GetShader("ambient-only");
	m_nonAnimatedShaders[2] = TTN_AssetSystem::GetShader("specular-only");
	m_nonAnimatedShaders[3] = TTN_AssetSystem::GetShader("ambient-and-specular");
	m_nonAnimatedShaders[4] = TTN_AssetSystem::GetShader("all-plus-rim");
	//animated
	m_animatedShaders[0] = TTN_AssetSystem::GetShader("animated-no-lighting");
	m_animatedShaders[1] = TTN_AssetSystem::GetShader("animated-ambient-only");
	m_animatedShaders[2] = TTN_AssetSystem::GetShader("animated-specular-only");
	m_animatedShaders[3] = TTN_AssetSystem::GetShader("animated-ambient-and-specular");
	m_animatedShaders[4] = TTN_AssetSystem::GetShader("animated-all-plus-rim");
	//terrain
	m_terrainShaders[0] = TTN_AssetSystem::GetShader("terrain-no-lighting");
	m_terrainShaders[1] = TTN_AssetSystem::GetShader("terrain-ambient-only");
	m_terrainShaders[2] = TTN_AssetSystem::GetShader("terrain-specular-only");
	m_terrainShaders[3] = TTN_AssetSystem::GetShader("terrain-ambient-and-specular");
	m_terrainShaders[4] = TTN_AssetSystem::GetShader("terrain-all-plus-rim");
	//water
	m_waterShaders[0] = TTN_AssetSystem::GetShader("water-no-lighting");
	m_waterShaders[1] = TTN_AssetSystem::GetShader("water-ambient-only");
	m_waterShaders[2] = TTN_AssetSystem::GetShader("water-specular-only");
	m_waterShaders[3] = TTN_AssetSystem::GetShader("water-ambient-and-specular");
	m_waterShaders[4] = TTN_AssetSystem::GetShader("water-all-plus-rim");
	//models
	BirdMesh = TTN_AssetSystem::GetMesh("Bird mesh");
	BoatMeshes[0] = TTN_AssetSystem::GetMesh("Boat 1 mesh");
	BoatMeshes[1] = TTN_AssetSystem::GetMesh("Boat 2 mesh");
	BoatMeshes[2] = TTN_AssetSystem::GetMesh("Boat 3 mesh");
	SkyboxMesh = TTN_AssetSystem::GetMesh("Skybox mesh");
	TerrainMesh = TTN_AssetSystem::GetMesh("Terrain mesh");
	WaterMesh = TTN_AssetSystem::GetMesh("Water mesh");
	//textures
	BirdTexture = TTN_AssetSystem::GetTexture2D("Bird texture");
	BoatTextures[0] = TTN_AssetSystem::GetTexture2D("Boat 1 texture");
	BoatTextures[1] = TTN_AssetSystem::GetTexture2D("Boat 2 texture");
	BoatTextures[2] = TTN_AssetSystem::GetTexture2D("Boat 3 texture");
	GrassTexture = TTN_AssetSystem::GetTexture2D("Grass texture");
	RockTexture = TTN_AssetSystem::GetTexture2D("Rock texture");
	SandTexture = TTN_AssetSystem::GetTexture2D("Sand texture");
	WaterTexture = TTN_AssetSystem::GetTexture2D("Water texture");
	SkyboxTexture = TTN_AssetSystem::GetSkybox("Skybox texture");

	//set up entities
	//entity for the camera
	{
		//create an entity in the scene for the camera
		m_camera = CreateEntity();
		SetCamEntity(m_camera);
		Attach<TTN_Transform>(m_camera);
		Attach<TTN_Camera>(m_camera);
		auto& camTrans = Get<TTN_Transform>(m_camera);
		camTrans.SetPos(glm::vec3(0.0f, 5.0f, 0.0f));
		camTrans.SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
		camTrans.LookAlong(glm::vec3(0.0, 0.0, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		Get<TTN_Camera>(m_camera).CalcPerspective(60.0f, 1.78f, 0.01f, 1000.f);
		Get<TTN_Camera>(m_camera).View();
	}

	//terrain entity
	{
		m_terrain = CreateEntity();

		//setup a transform for the terrain
		TTN_Transform terrainTrans = TTN_Transform(glm::vec3(0.0f, -10.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(50.0f));
		//attach that transform to the entity
		AttachCopy(m_terrain, terrainTrans);
	}

	//water
	{
		m_water = CreateEntity();

		//setup a transform for the water
		TTN_Transform waterTrans = TTN_Transform(glm::vec3(0.0f, -6.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(50.0f));
		//attach that transform to the entity
		AttachCopy(m_water, waterTrans);
	}

	//boats trees and birds

	//birds
	for (int i = 0; i < 2; i++) {
		birds[i] = CreateEntity();

		//create an animator
		TTN_MorphAnimator birdAnimator = TTN_MorphAnimator();

		//create an animation for the bird flying
		TTN_MorphAnimation flyingAnim = TTN_MorphAnimation({ 0, 1 }, { 10.0f / 24.0f, 10.0f / 24.0f }, true); //anim 0
		birdAnimator.AddAnim(flyingAnim);
		birdAnimator.SetActiveAnim(0);

		//attach that animator to the entity
		AttachCopy(birds[i], birdAnimator);

		//create a transform
		TTN_Transform birdTrans = TTN_Transform(birdBase, glm::vec3(0.0f), glm::vec3(1.0f));
		if (i == 1) birdTrans.SetPos(birdBase + glm::vec3(3.0f, -3.0f, 3.0f));
		birdTrans.RotateFixed(glm::vec3(0.0f, -45.0f + 180.0f, 0.0f));

		//attach that transform to the entity
		AttachCopy(birds[i], birdTrans);
	}

	//boats
	for (int i = 0; i < 3; i++) {
		boats[i] = CreateEntity();

		//create a transform
		TTN_Transform boatTrans = TTN_Transform(glm::vec3(21.0f, 10.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		if (i == 0) {
			boatTrans.SetPos({ -10.0f, -6.5f, -30.0f });
			boatTrans.SetScale(glm::vec3(0.25f, 0.25f, 0.25f));
		}
		else if (i == 1) {
			boatTrans.SetPos({ -20.0f, -6.0f, 15.0f });
			boatTrans.SetScale(glm::vec3(0.05f, 0.05f, 0.05f));
		}
		else if (i == 2) {
			boatTrans.SetPos({ 30.0f, -5.0f, 5.0f });
			boatTrans.SetScale(glm::vec3(0.15f, 0.15f, 0.15f));
		}

		//attach that transform to the entity
		AttachCopy(boats[i], boatTrans);
	}
}

void AssingmentScene::Update(float deltaTime)
{
	//add to the time
	time += deltaTime;
	m_deltaTime = deltaTime;

	//birds movement
	//move the birds
	birdTimer += deltaTime;

	birdTimer = fmod(birdTimer, 20);

	float t = TTN_Interpolation::InverseLerp(0.0f, 20.0f, birdTimer);

	for (int i = 0; i < 2; i++) {
		if (i == 0) Get<TTN_Transform>(birds[i]).SetPos(TTN_Interpolation::Lerp(birdBase, birdTarget, t));

		if (i == 1) Get<TTN_Transform>(birds[i]).SetPos(TTN_Interpolation::Lerp(birdBase + glm::vec3(3.0f, -3.0f, 3.0f), birdTarget + glm::vec3(3.0f, -3.0f, 3.0f), t));
	}

	//allow the user to control settings via ImGui
#pragma region ImGUI
	//Lighting ImGUI controller
	ImGui::Begin("Light Settings");

	//light position
	float lightPos[3] = { m_lightPos.x, m_lightPos.y, m_lightPos.z };
	if (ImGui::SliderFloat3("light position", lightPos, -50.0f, 50.0f)) {
		m_lightPos = { lightPos[0], lightPos[1], lightPos[2] };
	}

	//ambient light strenght
	ImGui::SliderFloat("ambient strenght", &m_ambientStrenght, 0.0f, 1000.0f);

	//ambient light colour
	float amColor[3] = { m_ambientColor.x, m_ambientColor.y, m_ambientColor.z};
	if (ImGui::ColorPicker3("ambient color", amColor)) {
		m_ambientColor = { amColor[0], amColor[1], amColor[2] };
	}

	//specular light strenght
	ImGui::SliderFloat("specular strenght", &m_specularStrenght, 0.0f, 5.0f);

	//toon outline size
	ImGui::SliderFloat("outline size", &m_rimSize, 0.0f, 1.0f);

	//toon bands count
	ImGui::SliderInt("number of bands", &m_bands, 1, 12);

	//rim light colour
	float rimColor[3] = { m_rimColor.x, m_rimColor.y, m_rimColor.z };
	if (ImGui::ColorPicker3("rim color", rimColor)) {
		m_rimColor = { rimColor[0], rimColor[1], rimColor[2] };
	}

	//attenution
	ImGui::SliderFloat("attenution constant", &m_attenuationConstant, 0.0f, 10.0f);
	ImGui::SliderFloat("attenution linear", &m_attenuationLinear, 0.0f, 10.0f);
	ImGui::SliderFloat("attenution quadric", &m_attenuationQuadric, 0.0f, 10.0f);

	ImGui::End();

	ImGui::Begin("Water");

	ImGui::SliderFloat("speed", &speed, -10.0f, 10.0f);
	ImGui::SliderFloat("base height", &baseHeight, -10.0f, 10.0f);
	float multiplier = heightMultiplier * 100;
	if (ImGui::SliderFloat("height change multiplier", &multiplier, 0.0f, 1.0f)) {
		heightMultiplier = multiplier / 100.0f;
	}
	ImGui::SliderFloat("wave lenght multiplier", &waveLenghtMultiplier, -20.0f, 20.0f);

	ImGui::End();

	ImGui::Begin("Shaders");

	//shader
	if (ImGui::Checkbox("no lighting", &shaderBool[0])) {
		for (int i = 0; i < 5; i++) shaderBool[i] = false;
		shaderBool[0] = true;
		shaderIndex = 0;
	}
	if (ImGui::Checkbox("ambient only", &shaderBool[1])) {
		for (int i = 0; i < 5; i++) shaderBool[i] = false;
		shaderBool[1] = true;
		shaderIndex = 1;
	}
	if (ImGui::Checkbox("specular only", &shaderBool[2])) {
		for (int i = 0; i < 5; i++) shaderBool[i] = false;
		shaderBool[2] = true;
		shaderIndex = 2;
	}
	if (ImGui::Checkbox("ambient and specular", &shaderBool[3])) {
		for (int i = 0; i < 5; i++) shaderBool[i] = false;
		shaderBool[3] = true;
		shaderIndex = 3;
	}
	if (ImGui::Checkbox("ambient, specular, and toon shading", &shaderBool[4])) {
		for (int i = 0; i < 5; i++) shaderBool[i] = false;
		shaderBool[4] = true;
		shaderIndex = 4;
	}

	ImGui::End();

#pragma endregion

	//because we're using custom shaders and not Titan's default shaders, we need to manually call the renders so we'll do that here
#pragma region Rendering
	//get the view matrix
	glm::mat4 viewMat = glm::inverse(Get<TTN_Transform>(m_camera).GetGlobal());
	//get the vp matrix
	glm::mat4 vp = Get<TTN_Camera>(m_camera).GetProj() * viewMat;
	
	//sets consistent uniforms
	//water uniforms
	m_waterShaders[shaderIndex]->SetUniform("time", time);
	m_waterShaders[shaderIndex]->SetUniform("speed", speed);
	m_waterShaders[shaderIndex]->SetUniform("baseHeight", baseHeight);
	m_waterShaders[shaderIndex]->SetUniform("heightMultiplier", heightMultiplier);
	m_waterShaders[shaderIndex]->SetUniform("waveLenghtMultiplier", waveLenghtMultiplier);
	//skybox uniforms
	m_skyboxShader->SetUniformMatrix("u_EnvironmentRotation", glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1, 0, 0))));
	m_skyboxShader->SetUniformMatrix("u_SkyboxMatrix", Get<TTN_Camera>(m_camera).GetProj() * glm::mat4(glm::mat3(viewMat)));
	//all shaders (minus skybox)
	m_nonAnimatedShaders[shaderIndex]->SetUniform("u_CamPos", Get<TTN_Transform>(m_camera).GetGlobalPos());
	m_animatedShaders[shaderIndex]->SetUniform("u_CamPos", Get<TTN_Transform>(m_camera).GetGlobalPos());
	m_terrainShaders[shaderIndex]->SetUniform("u_CamPos", Get<TTN_Transform>(m_camera).GetGlobalPos());
	m_waterShaders[shaderIndex]->SetUniform("u_CamPos", Get<TTN_Transform>(m_camera).GetGlobalPos());
	//shaders with any lighting
	if (shaderIndex > 0) {
		m_nonAnimatedShaders[shaderIndex]->SetUniform("u_lightPos", m_lightPos);
		m_animatedShaders[shaderIndex]->SetUniform("u_lightPos", m_lightPos);
		m_terrainShaders[shaderIndex]->SetUniform("u_lightPos", m_lightPos);
		m_waterShaders[shaderIndex]->SetUniform("u_lightPos", m_lightPos);

		m_nonAnimatedShaders[shaderIndex]->SetUniform("u_attenConst", m_attenuationConstant);
		m_animatedShaders[shaderIndex]->SetUniform("u_attenConst", m_attenuationConstant);
		m_terrainShaders[shaderIndex]->SetUniform("u_attenConst", m_attenuationConstant);
		m_waterShaders[shaderIndex]->SetUniform("u_attenConst", m_attenuationConstant);

		m_nonAnimatedShaders[shaderIndex]->SetUniform("u_attenLin", m_attenuationLinear);
		m_animatedShaders[shaderIndex]->SetUniform("u_attenLin", m_attenuationLinear);
		m_terrainShaders[shaderIndex]->SetUniform("u_attenLin", m_attenuationLinear);
		m_waterShaders[shaderIndex]->SetUniform("u_attenLin", m_attenuationLinear);

		m_nonAnimatedShaders[shaderIndex]->SetUniform("u_attenQuad", m_attenuationQuadric);
		m_animatedShaders[shaderIndex]->SetUniform("u_attenQuad", m_attenuationQuadric);
		m_terrainShaders[shaderIndex]->SetUniform("u_attenQuad", m_attenuationQuadric);
		m_waterShaders[shaderIndex]->SetUniform("u_attenQuad", m_attenuationQuadric);
	}
	//ambient shaders
	if (shaderBool[1] || shaderBool[3] || shaderBool[4]) {
		m_nonAnimatedShaders[shaderIndex]->SetUniform("u_ambStr", m_ambientStrenght);
		m_animatedShaders[shaderIndex]->SetUniform("u_ambStr", m_ambientStrenght);
		m_terrainShaders[shaderIndex]->SetUniform("u_ambStr", m_ambientStrenght);
		m_waterShaders[shaderIndex]->SetUniform("u_ambStr", m_ambientStrenght);

		m_nonAnimatedShaders[shaderIndex]->SetUniform("u_ambCol", m_ambientColor);
		m_animatedShaders[shaderIndex]->SetUniform("u_ambCol", m_ambientColor);
		m_terrainShaders[shaderIndex]->SetUniform("u_ambCol", m_ambientColor);
		m_waterShaders[shaderIndex]->SetUniform("u_ambCol", m_ambientColor);
	}
	//specular shaders
	if (shaderBool[2] || shaderBool[3] || shaderBool[4]) {
		m_nonAnimatedShaders[shaderIndex]->SetUniform("u_specStr", m_specularStrenght);
		m_animatedShaders[shaderIndex]->SetUniform("u_specStr", m_specularStrenght);
		m_terrainShaders[shaderIndex]->SetUniform("u_specStr", m_specularStrenght);
		m_waterShaders[shaderIndex]->SetUniform("u_specStr", m_specularStrenght);
	}
	//rim shaders
	if (shaderBool[4]) {
		m_nonAnimatedShaders[shaderIndex]->SetUniform("u_bands", m_bands);
		m_animatedShaders[shaderIndex]->SetUniform("u_bands", m_bands);
		m_terrainShaders[shaderIndex]->SetUniform("u_bands", m_bands);
		m_waterShaders[shaderIndex]->SetUniform("u_bands", m_bands);

		m_nonAnimatedShaders[shaderIndex]->SetUniform("u_outlineSize", m_rimSize);
		m_animatedShaders[shaderIndex]->SetUniform("u_outlineSize", m_rimSize);
		m_terrainShaders[shaderIndex]->SetUniform("u_outlineSize", m_rimSize);
		m_waterShaders[shaderIndex]->SetUniform("u_outlineSize", m_rimSize);
	}

	//now accutally render the entities
	//skybox
	{
		//bind the shader
		m_skyboxShader->Bind();

		//bind the texture to slot 0
		SkyboxTexture->Bind(0);

		//set up the vao on the mesh
		SkyboxMesh->SetUpVao();

		//and render it
		SkyboxMesh->GetVAOPointer()->Render();

		//unbind the shader
		m_skyboxShader->UnBind();
	}

	//boats, birds, and trees
	//birds
	for (int i = 0; i < 2; i++) {
		//bind the shader
		m_animatedShaders[shaderIndex]->Bind();

		//vert shader
		//pass the mvp uniform
		glm::mat4 mvp = vp;
		mvp *= Get<TTN_Transform>(birds[i]).GetGlobal();
		m_animatedShaders[shaderIndex]->SetUniformMatrix("MVP", mvp);
		//pass the model uniform
		m_animatedShaders[shaderIndex]->SetUniformMatrix("Model", Get<TTN_Transform>(birds[i]).GetGlobal());
		//and pass the normal matrix uniform
		m_animatedShaders[shaderIndex]->SetUniformMatrix("NormalMat",
			glm::mat3(glm::inverse(glm::transpose(Get<TTN_Transform>(birds[i]).GetGlobal()))));
		//and pass the t value from interpolation
		m_animatedShaders[shaderIndex]->SetUniform("t", Get<TTN_MorphAnimator>(birds[i]).getActiveAnimRef().getInterpolationParameter());

		//frag shader
		//bind the texture
		BirdTexture->Bind(0);

		//set up vao
		BirdMesh->SetUpVao(Get<TTN_MorphAnimator>(birds[i]).getActiveAnimRef().getCurrentMeshIndex(), Get<TTN_MorphAnimator>(birds[i]).getActiveAnimRef().getNextMeshIndex());
		//render the bird
		BirdMesh->GetVAOPointer()->Render();
	}

	//boats
	for (int i = 0; i < 3; i++) {
		//bind the shader
		m_nonAnimatedShaders[shaderIndex]->Bind();

		//vert shader
		//pass the mvp uniform
		glm::mat4 mvp = vp;
		mvp *= Get<TTN_Transform>(boats[i]).GetGlobal();
		m_nonAnimatedShaders[shaderIndex]->SetUniformMatrix("MVP", mvp);
		//pass the model uniform
		m_nonAnimatedShaders[shaderIndex]->SetUniformMatrix("Model", Get<TTN_Transform>(boats[i]).GetGlobal());
		//and pass the normal matrix uniform
		m_nonAnimatedShaders[shaderIndex]->SetUniformMatrix("NormalMat",
			glm::mat3(glm::inverse(glm::transpose(Get<TTN_Transform>(boats[i]).GetGlobal()))));

		//frag shader
		//bind the texture
		BoatTextures[i]->Bind(0);

		//set up vao
		BoatMeshes[i]->SetUpVao();
		//render the bird
		BoatMeshes[i]->GetVAOPointer()->Render();
	}

	//terrain
	{
		//bind the shader
		m_terrainShaders[shaderIndex]->Bind();

		//vert shader
		//pass the mvp uniform
		glm::mat4 mvp = vp;
		mvp *= Get<TTN_Transform>(m_terrain).GetGlobal();
		m_terrainShaders[shaderIndex]->SetUniformMatrix("MVP", mvp);
		//pass the model uniform
		m_terrainShaders[shaderIndex]->SetUniformMatrix("Model", Get<TTN_Transform>(m_terrain).GetGlobal());
		//and pass the normal matrix uniform
		m_terrainShaders[shaderIndex]->SetUniformMatrix("NormalMat",
			glm::mat3(glm::inverse(glm::transpose(Get<TTN_Transform>(m_terrain).GetGlobal()))));

		//frag shader
		//bind the textures
		SandTexture->Bind(0);
		RockTexture->Bind(1);
		GrassTexture->Bind(2);

		//render the terrain
		TerrainMesh->GetVAOPointer()->Render();

		//and unbind the shader
		m_terrainShaders[shaderIndex]->UnBind();
	}

	//water
	{
		//bind the shader
		m_waterShaders[0]->Bind();

		//vert shader
		//pass the mvp uniform
		glm::mat4 mvp = vp;
		mvp *= Get<TTN_Transform>(m_water).GetGlobal();
		m_waterShaders[0]->SetUniformMatrix("MVP", mvp);
		//pass the model uniform
		m_waterShaders[0]->SetUniformMatrix("Model", Get<TTN_Transform>(m_water).GetGlobal());
		//and pass the normal matrix uniform
		m_waterShaders[0]->SetUniformMatrix("NormalMat",
			glm::mat3(glm::inverse(glm::transpose(Get<TTN_Transform>(m_water).GetGlobal()))));

		//frag shader
		//bind the textures
		WaterTexture->Bind(0);

		//render the water
		WaterMesh->GetVAOPointer()->Render();
	}

#pragma endregion

	//get the mouse position
	glm::vec2 tempMousePos = TTN_Application::TTN_Input::GetMousePosition();

	if (TTN_Application::TTN_Input::GetMouseButton(TTN_MouseButton::Right)) {
	

		//figure out how much the cannon and camera should be rotated
		rotAmmount += (tempMousePos - mousePos) * 25.0f * m_deltaTime;

		//this is needed to make the rotating by mouse work
		if (rotAmmount.x > 360.0f) rotAmmount.x = 360.0f;
		else if (rotAmmount.x < 0.0f) rotAmmount.x = 0.0f;
		if (rotAmmount.y > 85.0f) rotAmmount.y = 85.0f;
		else if (rotAmmount.y < -85.0f) rotAmmount.y = -85.0f;

		//reset the rotation
		Get<TTN_Transform>(m_camera).LookAlong(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//and rotate it by the ammount it should be rotated
		Get<TTN_Transform>(m_camera).RotateFixed(glm::vec3(rotAmmount.y, -rotAmmount.x, 0.0f));
		//clear the direction the player is facing, and rotate it to face the same along
		facing = glm::vec3(0.0f, 0.0f, 1.0f);
		facing = glm::vec3(glm::toMat4(glm::quat(glm::radians(glm::vec3(rotAmmount.y, -rotAmmount.x, 0.0f))))
			* glm::vec4(facing, 1.0f));
		facing = glm::normalize(facing);
	}

	mousePos = tempMousePos;

	//don't forget to call the base scene's update
	TTN_Scene::Update(deltaTime);
}

void AssingmentScene::KeyChecks()
{
	if (TTN_Application::TTN_Input::GetKey(TTN_KeyCode::W)) {
		Get<TTN_Transform>(m_camera).SetPos(Get<TTN_Transform>(m_camera).GetPos() + facing * 20.0f * m_deltaTime);
	}

	if (TTN_Application::TTN_Input::GetKey(TTN_KeyCode::S)) {
		Get<TTN_Transform>(m_camera).SetPos(Get<TTN_Transform>(m_camera).GetPos() + facing * -20.0f * m_deltaTime);
	}

	if (TTN_Application::TTN_Input::GetKey(TTN_KeyCode::A)) {
		glm::vec3 right = glm::vec3(glm::toMat4(glm::quat(glm::radians(glm::vec3(90.0f, 90.0f, 90.0f))))
			* glm::vec4(facing, 1.0f));;
		right.y = 0.0f;
		Get<TTN_Transform>(m_camera).SetPos(Get<TTN_Transform>(m_camera).GetPos() + right * 20.0f * m_deltaTime);
	}

	if (TTN_Application::TTN_Input::GetKey(TTN_KeyCode::D)) {
		glm::vec3 right = glm::vec3(glm::toMat4(glm::quat(glm::radians(glm::vec3(90.0f, 90.0f, 90.0f))))
			* glm::vec4(facing, 1.0f));;
		right.y = 0.0f;
		Get<TTN_Transform>(m_camera).SetPos(Get<TTN_Transform>(m_camera).GetPos() + right * -20.0f * m_deltaTime);
	}
}

void AssingmentScene::MouseButtonChecks()
{

}