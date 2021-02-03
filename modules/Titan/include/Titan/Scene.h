//Titan Engine, by Atlas X Games 
// Scene.h - header for the class that handles ECS, render calls, etc. 
#pragma once

//precompile header, this file uses entt.hpp
#include "ttn_pch.h"
//include all the component class definitions we need
#include "Transform.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Camera.h"
#include "Light.h"
#include "Tag.h"
#include "Physics.h"
#include "MAnimator.h"
#include "Particle.h"
//include all the graphics features we need
#include "Shader.h"
namespace Titan {
	typedef entt::basic_group<entt::entity, entt::exclude_t<>, entt::get_t<>, TTN_Transform, TTN_Renderer> RenderGroupType;

	//scene class, handles the ECS, render class, etc. 
	class TTN_Scene
	{
	public:
		//default constructor 
		TTN_Scene();

		//constructor with data, using basic scene level lighting information
		TTN_Scene(glm::vec3 AmbientLightingColor, float AmbientLightingStrength);

		//copy, move, and assingment operators
		TTN_Scene(const TTN_Scene& oldScene) = default;
		TTN_Scene(TTN_Scene&&) = default;
		TTN_Scene& operator=(TTN_Scene&) = default;

		//destrutor 
		~TTN_Scene();

#pragma region ECS_functions_dec
		//creates a new entity 
		entt::entity CreateEntity();

		//deletes an entity
		void DeleteEntity(entt::entity entity);

		//attaches a compontent to an entity 
		template<typename T>
		void Attach(entt::entity entity);

		//attaches a component to an entity, and makes that component a copy of whatever object is passed in 
		template<typename T>
		void AttachCopy(entt::entity entity, const T& copy);

		//returns a reference to the component of the relevant entity 
		template<typename T>
		T& Get(entt::entity entity);

		//returns true if the entity has a given component and false otherwise
		template<typename T>
		bool Has(entt::entity entity);

		//removes a component from an object 
		template<typename T>
		void Remove(entt::entity entity);

		//sets the registry
		void SetScene(entt::registry* reg);
		//gets the registry
		entt::registry* GetScene() { return m_Registry; }

		//deletes and clears the scene
		void Unload();

#pragma endregion ECS_functions_dec

		//virtual functions for polymorphism 
		//updates all the entities in the scene
		virtual void Update(float deltaTime);
		virtual void PostRender();

		//input checks
		//keyboard
		virtual void KeyDownChecks() {}
		virtual void KeyChecks() {}
		virtual void KeyUpChecks() {}
		//mouse
		virtual void MouseButtonDownChecks() {}
		virtual void MouseButtonChecks() {}
		virtual void MouseButtonUpChecks() {}

		//init
		virtual void InitScene() {} 

#pragma region Graphics_functions_dec
		//renders all the entities with meshes and transforms in the scene 
		void Render(); 

		//sets wheter or not the scene should be rendered 
		void SetShouldRender(bool _shouldRender);
		//sets the ambient color of the lighting in the scene
		void SetSceneAmbientColor(glm::vec3 color);
		//sets the strenght of the abmient lighting in the scene
		void SetSceneAmbientLightStrength(float str);

		//gets wheter or not the scene should be rendered 
		bool GetShouldRender();
		//gets the ambient color of the lighting in the scene
		glm::vec3 GetSceneAmbientColor();
		//gets the strength of the ambient lighting in the scene
		float GetSceneAmbientLightStrength();

#pragma endregion Graphics_functions_dec

		//sets the camera entity reference
		void SetCamEntity(const entt::entity& cam) { m_Cam = cam; }
		//gets the camera entity
		const entt::entity& GetCamEntity() { return m_Cam; }

		//bullet physics stuff
		//set the gravity
		void SetGravity(glm::vec3 gravity);
		//gets the gravity
		glm::vec3 GetGravity();

		//gets all the collisions for the frame
		std::vector<TTN_Collision::scolptr> GetCollisions() { return collisions; }

		//set wheter or not the scene is paused
		void SetPaused(bool paused) { m_Paused = paused; }

		//get wheter or not the scene is paused
		bool GetPaused() { return m_Paused; }

		//variable to store the entities of the lights
		std::vector<entt::entity> m_Lights;

	private:
		//context that contains all our entities, their ids, and components 
		entt::registry* m_Registry = nullptr;

		//entt group that has all the entities with renderer and transform components so we can edit and render them live
		std::unique_ptr<RenderGroupType> m_RenderGroup;

		//boolean to store wheter or not this scene should currently be rendered
		bool m_ShouldRender; 

		//boolean to store wheter or not this scene should currently be updating 
		bool m_Paused;

		//variable to store the entity for the camera
		entt::entity m_Cam;

		//the colour of the scene's ambient lighting
		glm::vec3 m_AmbientColor;
		//the strenght of that ambient color
		float m_AmbientStrength;

		//physics world properties
		btDefaultCollisionConfiguration* collisionConfig;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btSequentialImpulseConstraintSolver* solver;
		//physics world
		btDiscreteDynamicsWorld* m_physicsWorld;

		//vector of titan collision objects, containing pointers to the rigid bodies (from which you can get entity numbers) and glm vec3s for collision normals
		std::vector<TTN_Collision::scolptr> collisions;

		//constructs the TTN_Collision objects
		void ConstructCollisions();

		//reconstructs the scenegraph, use every time entt reshuffles
		void ReconstructScenegraph();

#pragma region Sorts
		//functions to perform a merge sort on a vector of entities based on their z positions 
		//slightly modified from code from GeeksForGeeks: https://www.geeksforgeeks.org/merge-sort/

		//merge function
		void mergeEntitiesZ(std::vector<entt::entity>& list, int left, int middle, int right) {
			//calculate the lenghts of 2 temprorary vectors
			int lenght1 = middle - left + 1;
			int lenght2 = right - middle;

			//create the temporary vectors
			std::vector<entt::entity> tempLeftList = std::vector<entt::entity>(lenght1);
			std::vector<entt::entity> tempRightList = std::vector<entt::entity>(lenght2);

			//copy data over to the temp vectors
			for (int i = 0; i < lenght1; i++)
				tempLeftList[i] = list[left + i];
			for (int i = 0; i < lenght2; i++)
				tempRightList[i] = list[middle + 1 + i];

			//set inital indexes for each vector
			int leftIndex = 0; //left temp
			int rightIndex = 0; //right temp
			int mergedIndex = left; //merged subvector

			//merge the temp vectors back into list

			//loop through comparing both sides
			while (leftIndex < lenght1 && rightIndex < lenght2) {
				//if the left has a smaller or equal z value, add it back to the merged vector 
				if (Get<TTN_Transform>(tempLeftList[leftIndex]).GetGlobalPos().z <= Get<TTN_Transform>(tempRightList[rightIndex]).GetGlobalPos().z) {
					list[mergedIndex] = tempLeftList[leftIndex];
					leftIndex++;
				}
				//otherwise add the right back to the merged vector
				else {
					list[mergedIndex] = tempRightList[rightIndex];
					rightIndex++;
				}
				//either way increment the merged index
				mergedIndex++;
			}

			//when one side has completed, add any remaining elements from either side
			while (leftIndex < lenght1) {
				list[mergedIndex] = tempLeftList[leftIndex];
				leftIndex++;
				mergedIndex++;
			}
			while (rightIndex < lenght2) {
				list[mergedIndex] = tempRightList[rightIndex];
				rightIndex++;
				mergedIndex++;
			}
		}

		//merge sort function
		void mergeSortEntitiesZ(std::vector<entt::entity>& list, int left, int right) {
			//base case, returns recursively 
			if (left >= right) {
				return;
			}
			//calculate a middle value
			int middle = (left + right - 1) / 2;
			//call mergesort on both sides
			mergeSortEntitiesZ(list, left, middle);
			mergeSortEntitiesZ(list, middle + 1, right);
			//merge the two sides
			mergeEntitiesZ(list, left, middle, right);
		}

#pragma endregion Sorts
	};

#pragma region ECS_functions_def
	//function to attach a default object to an entity as a component
	template<typename T>
	inline void TTN_Scene::Attach(entt::entity entity)
	{
		//assign the component to the entity
		m_Registry->emplace<T>(entity);
		//reconstruct scenegraph as entt was shuffled
		ReconstructScenegraph();
	}

	//function to attach a copy of an object to an entity as a component
	template<typename T>
	inline void TTN_Scene::AttachCopy(entt::entity entity, const T& copy)
	{
		//assign the component to the entity 
		m_Registry->emplace_or_replace<T>(entity, copy);
		//reconstruct scenegraph as entt was shuffled
		ReconstructScenegraph();
	}

	//function to get a reference to a given component from an entity
	template<typename T>
	inline T& TTN_Scene::Get(entt::entity entity)
	{
		//return a reference to the component 
		return m_Registry->get<T>(entity);
	}

	//function to check if an entity has a given component
	template<typename T>
	inline bool TTN_Scene::Has(entt::entity entity)
	{
		return m_Registry->has<T>(entity);
	}

	//function to remove a component from an entity
	template<typename T>
	inline void TTN_Scene::Remove(entt::entity entity)
	{
		//remove the component from the entity
		m_Registry->remove<T>(entity);
		//reconstruct scenegraph as entt was shuffled
		ReconstructScenegraph();
	}
#pragma endregion ECS_functions_def
}