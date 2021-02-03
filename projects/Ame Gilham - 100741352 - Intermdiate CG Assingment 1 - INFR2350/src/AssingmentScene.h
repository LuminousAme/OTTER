//Intermidate Computer Graphics (INFR-2350) Assingment 1 by Ame Gilham (100741352)
//AssingmentScene.h, the header file that represents the class for the assignment's scene
#pragma once

//include required features from titan
#include "Titan/Application.h"
#include "Titan/Interpolation.h"

using namespace Titan;

class AssingmentScene : public TTN_Scene {
public:
	//constructor
	AssingmentScene();

	//destructor
	~AssingmentScene();

	//sets up the scene
	void InitScene();

	//update the scene
	void Update(float deltaTime);

	//keyboard input
	void KeyChecks();

	//mouse input
	void MouseButtonChecks();

protected:
	//assets
	//shaders: 0-no lighting, 1-ambient only, 2-specular only, 3-ambient and specular, 4-ambient, specular, and rim
	TTN_Shader::sshptr m_waterShaders[5];
	TTN_Shader::sshptr m_terrainShaders[5];
	TTN_Shader::sshptr m_animatedShaders[5];
	TTN_Shader::sshptr m_nonAnimatedShaders[5];
	TTN_Shader::sshptr m_skyboxShader;

	//models
	TTN_Mesh::smptr BirdMesh;
	TTN_Mesh::smptr BoatMeshes[3];
	TTN_Mesh::smptr SkyboxMesh;
	TTN_Mesh::smptr TerrainMesh;
	TTN_Mesh::smptr WaterMesh;

	//textures
	TTN_Texture2D::st2dptr BirdTexture;
	TTN_Texture2D::st2dptr BoatTextures[3];
	TTN_Texture2D::st2dptr GrassTexture;
	TTN_Texture2D::st2dptr RockTexture;
	TTN_Texture2D::st2dptr SandTexture;
	TTN_Texture2D::st2dptr WaterTexture;
	TTN_TextureCubeMap::stcmptr SkyboxTexture;

protected:
	//data
	//lighting uniform vec3s
	glm::vec3 m_lightPos, m_ambientColor, m_rimColor;
	//lightning uniform floats
	float m_ambientStrenght, m_specularStrenght, m_rimSize, m_attenuationConstant, m_attenuationLinear, m_attenuationQuadric;
	int m_bands;

	//current shader (ie what lighting is currently being used)
	int shaderIndex;
	bool shaderBool[5];

	//water vert shader data
	float time, speed, baseHeight, heightMultiplier, waveLenghtMultiplier;

	//variables for camera control
	glm::vec2 rotAmmount;
	glm::vec2 mousePos;
	glm::vec3 facing;
	float m_deltaTime;

	//variables for birds
	glm::vec3 birdBase = glm::vec3(50.0f, 8.0f, 50.0f);
	glm::vec3 birdTarget = glm::vec3(-50.0f, 8.0f, -50.0f);
	float birdTimer = 0.0f;

protected:
	//entities
	entt::entity m_camera, m_terrain, m_water;
	entt::entity boats[3];
	entt::entity birds[2];
};