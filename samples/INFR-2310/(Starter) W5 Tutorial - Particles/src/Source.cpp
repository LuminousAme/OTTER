/*
Week 5 Tutorial Starter - Created for INFR 2310 at Ontario Tech.
(c) Atiya Nova and Samantha Stahlke 2020
*/

#include "NOU/App.h"
#include "NOU/Input.h"
#include "NOU/Entity.h"
#include "NOU/CCamera.h"
#include "NOU/CMeshRenderer.h"
#include "CParticleSystem.h"
#include "NOU/GLTFLoader.h"

#include <GLM/gtc/type_ptr.hpp>

#include "imgui.h"

#include <memory>
#include <ctime>

using namespace nou;

//Forward declaring our global resources for this demo.
std::unique_ptr<ShaderProgram> prog_texLit, prog_particles;
std::unique_ptr<Mesh> duckMesh;
std::unique_ptr<Texture2D> duckTex, fireParticleTex, snowParticleTex;
std::unique_ptr<Material> duckMat, fireParticleMat, snowParticleMat;

//This function will load in our global resources.
//(It's only been separated to make main() a bit cleaner to look at.)
void LoadDefaultResources();

int main()
{
	srand(static_cast<unsigned int>(time(0)));

	App::Init("Week 5 Tutorial - Particles", 800, 800);
	App::SetClearColor(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));

	App::InitImgui();

	LoadDefaultResources();

	//Set up our camera.
	Entity camEntity = Entity::Create();
	auto& cam = camEntity.Add<CCamera>(camEntity);
	cam.Perspective(60.0f, 1.0f, 0.1f, 100.0f);
	camEntity.transform.m_pos = glm::vec3(0.0f, 0.0f, 4.0f);

	//Creating the duck entity.
	Entity duckEntity = Entity::Create();
	duckEntity.Add<CMeshRenderer>(duckEntity, *duckMesh, *duckMat);
	duckEntity.transform.m_scale = glm::vec3(0.005f, 0.005f, 0.005f);
	duckEntity.transform.m_pos = glm::vec3(0.0f, -1.0f, 0.0f);
	duckEntity.transform.m_rotation = glm::angleAxis(glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//Set up for flame particle system.
	ParticleParam fireParticleData;
	fireParticleData.lifetime = 1.5f;
	fireParticleData.startColor = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	fireParticleData.endColor = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	fireParticleData.startSize = 0.1f;
	fireParticleData.maxParticles = 2000;
	fireParticleData.emissionRate = 250.0f;
	float fireParticleDegreeAngle = 30.f;
	fireParticleData.tanTheta = glm::tan(glm::radians(fireParticleDegreeAngle));

	Entity fireParticleEntity = Entity::Create();
	fireParticleEntity.transform.m_pos = glm::vec3(0.5f, -0.4f, 0.0f);
	fireParticleEntity.transform.m_rotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	fireParticleEntity.Add<CParticleSystem>(fireParticleEntity, *fireParticleMat, fireParticleData);

	//Set up for snow particle system 
	ParticleParam snowParticleData;
	snowParticleData.lifetime = 20.f;
	snowParticleData.startColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	snowParticleData.endColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	snowParticleData.startSize = 0.2f;
	snowParticleData.maxParticles = 250;
	snowParticleData.emissionRate = 5.0f;
	float snowParticleDegreeAngle = 45.f;
	snowParticleData.tanTheta = glm::tan(glm::radians(snowParticleDegreeAngle));

	Entity snowParticleEntity = Entity::Create();
	snowParticleEntity.transform.m_rotation = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	snowParticleEntity.transform.m_pos = glm::vec3(0.0f, 5.2f, -2.0f);
	snowParticleEntity.Add<CParticleSystem>(snowParticleEntity, *snowParticleMat, snowParticleData);

	App::Tick();

	//This is our main loop.
	while (!App::IsClosing() && !Input::GetKey(GLFW_KEY_ESCAPE))
	{
		//Start of the frame.
		App::FrameStart();
		float deltaTime = App::GetDeltaTime();
		
		//Updates the camera.
		camEntity.Get<CCamera>().Update();
		 
		fireParticleEntity.Get<CParticleSystem>().Update(deltaTime);
		snowParticleEntity.Get<CParticleSystem>().Update(deltaTime);

		duckEntity.transform.RecomputeGlobal();
		fireParticleEntity.transform.RecomputeGlobal();
		snowParticleEntity.transform.RecomputeGlobal();

		//We draw particles with the depth buffer disabled
		//to prevent z-fighting.
		//Custom depth sorting is handled by our particle system.
		glDisable(GL_DEPTH_TEST);
		fireParticleEntity.Get<CParticleSystem>().Draw();
		snowParticleEntity.Get<CParticleSystem>().Draw();
		glEnable(GL_DEPTH_TEST);
		
		duckEntity.Get<CMeshRenderer>().Draw();

		//For Imgui...
		App::StartImgui(); 

		//Put any Imgui controls you plan to use for your
		//particle system here (for the exercise).

		//fire particle controls
		ImGui::Begin("fire particles");
		//control for lifetime
		if (ImGui::SliderFloat("Particle lifetime", &fireParticleData.lifetime, 0.1f, 50.0f))
			fireParticleEntity.Get<CParticleSystem>().SetParticleData(fireParticleData);
		//control for size 
		if (ImGui::SliderFloat("Particle size", &fireParticleData.startSize, 0.1f, 2.0f))
			fireParticleEntity.Get<CParticleSystem>().SetParticleData(fireParticleData);
		//control for maximum number of particles 
		if (ImGui::SliderInt("Particle max", (int*) &fireParticleData.maxParticles, 1, 10000))
			fireParticleEntity.Get<CParticleSystem>().SetParticleData(fireParticleData);
		//control for emission rate 
		if (ImGui::SliderFloat("Emission rate", &fireParticleData.emissionRate, 0.1f, 1000.f))
			fireParticleEntity.Get<CParticleSystem>().SetParticleData(fireParticleData);
		//control for the angle 
		if (ImGui::SliderFloat("Cone angle", &fireParticleDegreeAngle, 0.0f, 90.f))
		{
			fireParticleData.tanTheta = glm::tan(fireParticleDegreeAngle);
			fireParticleEntity.Get<CParticleSystem>().SetParticleData(fireParticleData);
		}
		//control for colour
		if (ImGui::ColorPicker4("Starting colour", glm::value_ptr(fireParticleData.startColor)))
			fireParticleEntity.Get<CParticleSystem>().SetParticleData(fireParticleData);
		if (ImGui::ColorPicker4("Ending colour", glm::value_ptr(fireParticleData.endColor)))
			fireParticleEntity.Get<CParticleSystem>().SetParticleData(fireParticleData);

		ImGui::End(); //fire particle controls over 

		//snow particle controls
		ImGui::Begin("snow particles");
		//control for lifetime
		if (ImGui::SliderFloat("Particle lifetime", &snowParticleData.lifetime, 0.1f, 50.0f))
			snowParticleEntity.Get<CParticleSystem>().SetParticleData(snowParticleData);
		//control for size 
		if (ImGui::SliderFloat("Particle size", &snowParticleData.startSize, 0.1f, 2.0f))
			snowParticleEntity.Get<CParticleSystem>().SetParticleData(snowParticleData);
		//control for maximum number of particles 
		if (ImGui::SliderInt("Particle max", (int*)&snowParticleData.maxParticles, 1, 10000))
			snowParticleEntity.Get<CParticleSystem>().SetParticleData(snowParticleData);
		//control for emission rate 
		if (ImGui::SliderFloat("Emission rate", &snowParticleData.emissionRate, 0.1f, 1000.f))
			snowParticleEntity.Get<CParticleSystem>().SetParticleData(snowParticleData);
		//control for the angle 
		if (ImGui::SliderFloat("Cone angle", &snowParticleDegreeAngle, 0.0f, 90.f))
		{
			snowParticleData.tanTheta = glm::tan(snowParticleDegreeAngle);
			snowParticleEntity.Get<CParticleSystem>().SetParticleData(snowParticleData);
		}
		//control for colour
		if (ImGui::ColorPicker4("Starting colour", glm::value_ptr(snowParticleData.startColor)))
			snowParticleEntity.Get<CParticleSystem>().SetParticleData(snowParticleData);
		if (ImGui::ColorPicker4("Ending colour", glm::value_ptr(snowParticleData.endColor)))
			snowParticleEntity.Get<CParticleSystem>().SetParticleData(snowParticleData);

		ImGui::End(); //end of snow particle controls 

		App::EndImgui();  

		//This sticks all the drawing we just did on the screen.
		App::SwapBuffers();
	}

	App::Cleanup();

	return 0;
}

void LoadDefaultResources()
{
	//Load in some shaders.
	//Smart pointers will automatically deallocate memory when they go out of scope.
	//Lit and textured shader program.
	auto v_texLit = std::make_unique<Shader>("shaders/texturedlit.vert", GL_VERTEX_SHADER);
	auto f_texLit = std::make_unique<Shader>("shaders/texturedlit.frag", GL_FRAGMENT_SHADER);

	std::vector<Shader*> texLit = { v_texLit.get(), f_texLit.get() };
	prog_texLit = std::make_unique<ShaderProgram>(texLit);

	//Billboarded particles shader program.
	auto v_particles = std::make_unique<Shader>("shaders/particles.vert", GL_VERTEX_SHADER);
	auto g_particles = std::make_unique<Shader>("shaders/particles.geom", GL_GEOMETRY_SHADER);
	auto f_particles = std::make_unique<Shader>("shaders/particles.frag", GL_FRAGMENT_SHADER);

	std::vector<Shader*> particles = { v_particles.get(), g_particles.get(), f_particles.get() };
	prog_particles = std::make_unique<ShaderProgram>(particles);

	//Load in the ducky model.
	duckMesh = std::make_unique<Mesh>();
	GLTF::LoadMesh("duck/Duck.gltf", *duckMesh);

	//Load in textures.
	duckTex = std::make_unique<Texture2D>("duck/DuckCM.png");
	fireParticleTex = std::make_unique<Texture2D>("particle.png");
	snowParticleTex = std::make_unique<Texture2D>("snow particle.png");
	 
	//Make materials. 
	duckMat = std::make_unique<Material>(*prog_texLit);
	duckMat->AddTexture("albedo", *duckTex);

	fireParticleMat = std::make_unique<Material>(*prog_particles);
	fireParticleMat->AddTexture("albedo", *fireParticleTex);
	snowParticleMat = std::make_unique<Material>(*prog_particles);
	snowParticleMat->AddTexture("albedo", *snowParticleTex);
}