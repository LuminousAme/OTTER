//Intermidate Computer Graphics (INFR-2350) Assingment 1 by Ame Gilham (100741352)
//main.cpp, the source file that runs the application

//import required titan features
#include "Titan/Application.h"
//import the scene class
#include "AssingmentScene.h"

using namespace Titan;

//asset setup function
void PrepareAssetLoading();

int main() { 
	Logger::Init(); //initliaze otter's base logging system
	TTN_Application::Init("INFR-2350, Assingment 1, Ame Gilham (100741352)", 1920, 1080); //initliaze titan's application

	//lock the cursor while focused in the application window
	TTN_Application::TTN_Input::SetCursorLocked(false);

	//prepare the assets
	PrepareAssetLoading();

	//load set 0 assets
	TTN_AssetSystem::LoadSetNow(0);

	//create and initliaze the scene
	AssingmentScene* scene = new AssingmentScene;
	scene->InitScene();

	//add the scene to the application
	TTN_Application::scenes.push_back(scene);

	//while the application is running
	while (!TTN_Application::GetIsClosing()) {
		//check if the user is trying to close the application with the esc key
		if (TTN_Application::TTN_Input::GetKeyDown(TTN_KeyCode::Esc)) {
			//if they are, exit the application loop and end the program
			break;
		}

		//update the scenes and render the screen
		TTN_Application::Update();
	}

	return 0; 
} 

void PrepareAssetLoading() {
	//shaders
	TTN_AssetSystem::AddShaderToBeLoaded("skybox", "shaders/skybox_vert.glsl", "shaders/skybox_frag.glsl");

	TTN_AssetSystem::AddShaderToBeLoaded("no-lighting", "shaders/regular_vert.glsl", "shaders/no_lighting.glsl");
	TTN_AssetSystem::AddShaderToBeLoaded("ambient-only", "shaders/regular_vert.glsl", "shaders/ambient_only.glsl");
	TTN_AssetSystem::AddShaderToBeLoaded("specular-only", "shaders/regular_vert.glsl", "shaders/specular_only.glsl");
	TTN_AssetSystem::AddShaderToBeLoaded("ambient-and-specular", "shaders/regular_vert.glsl", "shaders/ambient_and_specular.glsl");
	TTN_AssetSystem::AddShaderToBeLoaded("all-plus-rim", "shaders/regular_vert.glsl", "shaders/all_plus_toon.glsl");

	TTN_AssetSystem::AddShaderToBeLoaded("animated-no-lighting", "shaders/animated_vert.glsl", "shaders/no_lighting.glsl");
	TTN_AssetSystem::AddShaderToBeLoaded("animated-ambient-only", "shaders/animated_vert.glsl", "shaders/ambient_only.glsl");
	TTN_AssetSystem::AddShaderToBeLoaded("animated-specular-only", "shaders/animated_vert.glsl", "shaders/specular_only.glsl");
	TTN_AssetSystem::AddShaderToBeLoaded("animated-ambient-and-specular", "shaders/animated_vert.glsl", "shaders/ambient_and_specular.glsl");
	TTN_AssetSystem::AddShaderToBeLoaded("animated-all-plus-rim", "shaders/animated_vert.glsl", "shaders/all_plus_toon.glsl");

	TTN_AssetSystem::AddShaderToBeLoaded("water-no-lighting", "shaders/water_vert.glsl", "shaders/no_lighting.glsl");
	TTN_AssetSystem::AddShaderToBeLoaded("water-ambient-only", "shaders/water_vert.glsl", "shaders/ambient_only.glsl");
	TTN_AssetSystem::AddShaderToBeLoaded("water-specular-only", "shaders/water_vert.glsl", "shaders/specular_only.glsl");
	TTN_AssetSystem::AddShaderToBeLoaded("water-ambient-and-specular", "shaders/water_vert.glsl", "shaders/ambient_and_specular.glsl");
	TTN_AssetSystem::AddShaderToBeLoaded("water-all-plus-rim", "shaders/water_vert.glsl", "shaders/all_plus_toon.glsl");

	TTN_AssetSystem::AddShaderToBeLoaded("terrain-no-lighting", "shaders/terrain_vert.glsl", "shaders/terrain_frag_no_lighting.glsl");
	TTN_AssetSystem::AddShaderToBeLoaded("terrain-ambient-only", "shaders/terrain_vert.glsl", "shaders/terrain_frag_ambient_only.glsl");
	TTN_AssetSystem::AddShaderToBeLoaded("terrain-specular-only", "shaders/terrain_vert.glsl", "shaders/terrain_frag_specular_only.glsl");
	TTN_AssetSystem::AddShaderToBeLoaded("terrain-ambient-and-specular", "shaders/terrain_vert.glsl", "shaders/terrain_frag_ambient_and_specular.glsl");
	TTN_AssetSystem::AddShaderToBeLoaded("terrain-all-plus-rim", "shaders/terrain_vert.glsl", "shaders/terrain_frag_all_plus_toon.glsl");
	
	//models
	TTN_AssetSystem::AddMorphAnimationMeshesToBeLoaded("Bird mesh", "models/bird", 2);
	TTN_AssetSystem::AddMeshToBeLoaded("Boat 1 mesh", "models/Boat 1.obj");
	TTN_AssetSystem::AddMeshToBeLoaded("Boat 2 mesh", "models/Boat 2.obj");
	TTN_AssetSystem::AddMeshToBeLoaded("Boat 3 mesh", "models/Boat 3.obj");
	TTN_AssetSystem::AddMeshToBeLoaded("Skybox mesh", "models/SkyboxMesh.obj");
	TTN_AssetSystem::AddMeshToBeLoaded("Terrain mesh", "models/terrain.obj");
	TTN_AssetSystem::AddMeshToBeLoaded("Water mesh", "models/water.obj");

	//textures
	TTN_AssetSystem::AddTexture2DToBeLoaded("Bird texture", "textures/BirdTexture.png");
	TTN_AssetSystem::AddTexture2DToBeLoaded("Boat 1 texture", "textures/Boat 1 Texture.png");
	TTN_AssetSystem::AddTexture2DToBeLoaded("Boat 2 texture", "textures/Boat 2 Texture.png");
	TTN_AssetSystem::AddTexture2DToBeLoaded("Boat 3 texture", "textures/Boat 3 Texture.png");
	TTN_AssetSystem::AddTexture2DToBeLoaded("Grass texture", "textures/GrassTexture.jpg");
	TTN_AssetSystem::AddTexture2DToBeLoaded("Rock texture", "textures/RockTexture.jpg");
	TTN_AssetSystem::AddTexture2DToBeLoaded("Sand texture", "textures/SandTexture.jpg");
	TTN_AssetSystem::AddTexture2DToBeLoaded("Water texture", "textures/water.png");
	TTN_AssetSystem::AddSkyboxToBeLoaded("Skybox texture", "textures/skybox/sky.png");
}