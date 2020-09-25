/*
Week 3 Tutorial Starter - Created for INFR 2310 at Ontario Tech.
(c) Atiya Nova and Samantha Stahlke 2020
*/

#include "NOU/App.h"
#include "NOU/Input.h"
#include "NOU/Entity.h"
#include "NOU/CCamera.h"
#include "Sprites/CSpriteRenderer.h"
#include "Sprites/CSpriteAnimator.h"
#include "CKnightFSM.h"
#include "CBlueFSM.h"
#include "imgui.h"
#include <memory>

using namespace nou;

int main() 
{
	App::Init("Week 3 Tutorial - Sprites", 800, 800);
	App::SetClearColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

	App::InitImgui();

	//Load in some shaders.
	auto v_sprite = std::make_unique<Shader>("shaders/texturedunlit.vert", GL_VERTEX_SHADER);
	auto f_sprite = std::make_unique<Shader>("shaders/texturedunlit.frag", GL_FRAGMENT_SHADER);

	auto prog_sprite = ShaderProgram({ v_sprite.get(), f_sprite.get() });

	//Load in sprites.
	Texture2D boomTex = Texture2D("explosion.png", true);
	Texture2D knightTex = Texture2D("knight.png", true);
	Texture2D blueTex = Texture2D("blue.png", true);

	Material boomMat(prog_sprite);
	boomMat.AddTexture("albedo", boomTex);

	Material knightMat(prog_sprite);
	knightMat.AddTexture("albedo", knightTex);

	Material blueMat(prog_sprite);
	blueMat.AddTexture("albedo", blueTex);
	
	//TODO: Load in explosion spritesheet.
	auto boomSheet = std::make_unique<Spritesheet>(boomTex, glm::vec2(222.0f, 222.0f));
	boomSheet->AddAnimation("boom", 0, 27, 30.0f);
	boomSheet->SetDefaultFrame(27);

	//Load in knight spritesheet, add animations.
	auto knightSheet = std::make_unique<Spritesheet>(knightTex, glm::vec2(64.0f, 64.0f));
	knightSheet->AddAnimation(CKnightFSM::idleClip, 0, 4, 12.0f);
	knightSheet->AddAnimation(CKnightFSM::runClip, 5, 12, 12.0f);
	knightSheet->AddAnimation(CKnightFSM::attackClip, 19, 21, 12.0f);

	//Load in blue spritesheet, add animations
	auto blueSheet = std::make_unique<Spritesheet>(blueTex, glm::vec2(250.0f, 250.0f));
	blueSheet->AddAnimation(CBlueFSM::idleClip, 0, 5, 12.0f);
	blueSheet->AddAnimation(CBlueFSM::runClip, 6, 11, 12.0f);
	blueSheet->AddAnimation(CBlueFSM::slideClip, 36, 39, 12.0f);

	//Set up our camera.
	Entity camEntity = Entity::Create();
	auto& cam = camEntity.Add<CCamera>(camEntity);
	cam.Ortho(-400.0f, 400.0f, -400.0f, 400.0f, 0.1f, 100.0f);
	camEntity.transform.m_pos = glm::vec3(0.0f, 0.0f, 2.0f);

	//TODO: Create explosion entity.
	Entity okBoomer = Entity::Create();
	okBoomer.Add<CSpriteRenderer>(okBoomer, *boomSheet, boomMat);
	auto& boomAnim = okBoomer.Add<CSpriteAnimator>(okBoomer, *boomSheet);

	//Create the knight entity.
	Entity knightEntity = Entity::Create();
	knightEntity.transform.m_scale = glm::vec3(2.0f, 2.0f, 2.0f);
	knightEntity.Add<CSpriteRenderer>(knightEntity, *knightSheet, knightMat);
	auto& knightAnim = knightEntity.Add<CSpriteAnimator>(knightEntity, *knightSheet);
	knightEntity.Add<CKnightFSM>(knightEntity);

	//Create blue's entity.
	Entity blueEntity = Entity::Create();
	blueEntity.transform.m_scale = glm::vec3(1.5f, 1.5f, 1.5f);
	blueEntity.Add<CSpriteRenderer>(blueEntity, *blueSheet, blueMat);
	auto& blueAnim = blueEntity.Add<CSpriteAnimator>(blueEntity, *blueSheet);
	blueEntity.Add<CBlueFSM>(blueEntity);

	App::Tick();

	//Disabling the depth buffer.
	//(The reason we do this is so we can naively draw sprites on top
	//of each other in the order we call Draw().)
	glDisable(GL_DEPTH_TEST);

	//Disable backface culling.
	//(The reason we disable culling is to make it simpler for us to 
	//flip sprites without making OpenGL think we're drawing polygons
	//that shouldn't be visible.)
	glDisable(GL_CULL_FACE);
	
	//This is our main loop.
	while (!App::IsClosing() && !Input::GetKey(GLFW_KEY_ESCAPE))
	{
		//Start of the frame.
		App::FrameStart();
		float deltaTime = App::GetDeltaTime();

		//TODO: Control our knight.
		bool moving = Input::GetKey(GLFW_KEY_RIGHT) || Input::GetKey(GLFW_KEY_LEFT);
		knightEntity.Get<CKnightFSM>().SetVariable("moving", moving);

		if (moving)
		{
			bool flip = Input::GetKey(GLFW_KEY_LEFT);

			knightEntity.transform.m_scale.x = (flip) ? -2.0f : 2.0f;
			knightEntity.transform.m_pos.x += (flip) ? -100.f * deltaTime : 100.f * deltaTime;
		}

		//control blue use A and D keys
		bool bMoving = Input::GetKey(GLFW_KEY_D) || Input::GetKey(GLFW_KEY_A);
		bool bFlip = Input::GetKey(GLFW_KEY_D);
		blueEntity.Get<CBlueFSM>().SetVariable("moving", bMoving);

		if (bMoving)
		{
			blueEntity.transform.m_scale.x = (bFlip) ? -1.5f : 1.5f;
			blueEntity.transform.m_pos.x += (bFlip) ? 250.f * deltaTime : -250.f * deltaTime;
		}

		//Updates all the entities.
		camEntity.Get<CCamera>().Update();
		//TODO: Update explosion entity.
		okBoomer.Get<CSpriteAnimator>().Update(deltaTime);
		knightEntity.Get<CKnightFSM>().Update();
		knightEntity.Get<CSpriteAnimator>().Update(deltaTime);
		blueEntity.Get<CSpriteAnimator>().Update(deltaTime);
		blueEntity.Get<CBlueFSM>().Update();
		
		//Recomputes global matrices.
		//TODO: Update explosion entity.
		knightEntity.transform.RecomputeGlobal();
		blueEntity.transform.RecomputeGlobal();
		okBoomer.transform.RecomputeGlobal();

		//Draws the sprites.
		//TODO: Draw explosion entity.
		okBoomer.Get<CSpriteRenderer>().Draw();
		knightEntity.Get<CSpriteRenderer>().Draw();	
		blueEntity.Get<CSpriteRenderer>().Draw();

		//For Imgui stuff...
		App::StartImgui();

		//TODO: Create Imgui panel.
		//TODO: Create Imgui button and play explosion on press.
		static bool panelOpen = true;
		ImGui::Begin("name it properlly", &panelOpen, ImVec2(300, 100));

		if (ImGui::Button("Boom!"))
			okBoomer.Get<CSpriteAnimator>().PlayOnce("boom");

		if (ImGui::Button("Attack!"))
			//tell the knight to attack;
			knightEntity.Get<CKnightFSM>().SetTrigger("attack");

		if (ImGui::Button("Slide!")) {
			//tell blue to slide 
			blueEntity.Get<CBlueFSM>().SetTrigger("slide");
			blueEntity.transform.m_pos.x += (bFlip) ? 5000.f * deltaTime : -5000.f * deltaTime;
		}

		ImGui::End();

		App::EndImgui();

		//This sticks all the drawing we just did on the screen.
		App::SwapBuffers();
	}

	App::Cleanup();

	return 0; 
} 
