//Obj Loading Assingment By Ame Gilham #100741352 

//include all the required external libraries
#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "GLM/gtx/transform.hpp"
#include "GLM/gtx/quaternion.hpp"
//include the internal files we need
#include "ObjLoader.h" 
#include "Mesh.h" 
#include "Shader.h" 


GLFWwindow* window;

//function to initliaze glfw
bool initGLFW() {
	if (glfwInit() == GLFW_FALSE) {
		LOG_ERROR("Failed to initialize GLFW");
		return false;
	}

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

	//Create a new GLFW window
	window = glfwCreateWindow(800, 800, "OBJ Loader Assignment - Ame Gilham - 100741352", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	return true;
}

//function to initlize glad
bool initGLAD() {
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		LOG_ERROR("Failed to initialize Glad");
		return false;
	}
	return true;
}

//main function, runs the program
int main() {
	//initlize glfw, exiting with an error if the init fails
	if (!initGLFW()) return -1;

	//initlize glad, exiting with an error if the init fails
	if (!initGLAD()) return -1;

	//load our mesh 
	Mesh* model = ObjLoader::LoadFromFile("monkey.obj");
	//call our meshes setup vao function so we can acutally render it 
	model->SetUpVao();

	//create a pointer to a shader program
	Shader::sshptr shaderProgram = Shader::Create();
	//load the shaders into the program
	shaderProgram->LoadShaderStageFromFile("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
	shaderProgram->LoadShaderStageFromFile("shaders/frag_shader.glsl", GL_FRAGMENT_SHADER);
	shaderProgram->Link();
	
	//create variables for the rotations for both models to be display
	glm::vec3 rot1 = glm::vec3(0.0f, 0.0f, 0.f);
	glm::vec3 rot2 = glm::vec3(0.f, 360.f, 0.f);
	glm::vec3 rot1radians = glm::vec3(0.0f, 0.0f, glm::radians(0.f));
	glm::vec3 rot2radians = glm::vec3(0.0f, glm::radians(360.0f), glm::radians(0.f));

	//variable to control wheter the camera is currently an orthographic or a perspective projection
	bool currentModeOrtho = false;
	//variable to help it from continually going back and forth between the two projections
	bool spaceDown = false;

	//view and projection matrices for the camera
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(glm::radians(90.f), 1.0f, 0.01f, 1000.0f);

	//model matrices for each time the mesh is displayed
	glm::mat4 model1matrix;
	glm::mat4 model2matrix;

	//time controls for deltatime
	double timeLastFrame = glfwGetTime();
	double timeThisFrame = timeLastFrame;
	float deltaTime = (float)(timeLastFrame - timeThisFrame);

	//game loop
	while (!glfwWindowShouldClose(window)) {
		//update deltatime
		timeThisFrame = glfwGetTime();
		deltaTime = (float)(timeLastFrame - timeThisFrame);

		//check for events from glfw (this lets us do things like close the window) 
		glfwPollEvents();

		//Clear the window so the draws from the last frame are no longer there
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//rotate both the model instances on their y axises, in opposite directions, forcing it to stay between 0 and 360 degrees
		rot1.y += 10.0f * deltaTime;
		while (rot1.y > 360.0f) rot1.y -= 360.0f;
		rot1radians = glm::vec3(glm::radians(rot1.x), glm::radians(rot1.y), glm::radians(rot1.z));
		rot2.y -= 10.0f * deltaTime;
		while (rot2.y < 0.0f) rot2.y += 360.0f;
		rot2radians = glm::vec3(glm::radians(rot2.x), glm::radians(rot2.y), glm::radians(rot2.z));


		//calculate the model matrix for each instance of the model, 
		//assuming one is too the left and up a bit, and the other is further back to the right and down a bit
		model1matrix = glm::translate(glm::vec3(1.0f, 1.f, 3.f)) * glm::toMat4(glm::quat(rot1radians)) * glm::scale(glm::vec3(1.0f));
		model2matrix = glm::translate(glm::vec3(-1.0f, -1.f, 5.5f)) * glm::toMat4(glm::quat(rot2radians)) * glm::scale(glm::vec3(1.0f));

		//check if the user has changed camera view by pressing space
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			//only run this first frame they press the button
			if (!spaceDown) {
				//if they have then change the mode
				currentModeOrtho = !currentModeOrtho;

				//and recalculate the projection matrix 
				projection = (currentModeOrtho) ? glm::ortho(-800.0f / 200.f, 800.0f / 200.f, -800.0f / 200.f, 800.0f / 200.f, 0.01f, 1000.0f) :
					glm::perspective(glm::radians(90.f), 1.0f, 0.01f, 1000.0f);

				//make that the space button has been pressed and not yet released
				spaceDown = true;
			}
		}
		//when they stop pressing the space button, mark the button as no longer down
		else
			spaceDown = false;

		//bind the shaderprogram
		shaderProgram->Bind();
		//set the mvp for the first model instance
		shaderProgram->SetUniformMatrix("MVP", projection * view * model1matrix);
		//render the first model instance 
		model->GetVAOPointer()->Render();

		//set the mvp for the second model instance
		shaderProgram->SetUniformMatrix("MVP", projection * view * model2matrix);
		//render the secpmd model instance 
		model->GetVAOPointer()->Render();

		//swap the buffers so we can acutally see the results of the render
		glfwSwapBuffers(window);
		//and finally update the time of the last frame to this frame before we move to the next frame
		timeLastFrame = timeThisFrame;
	}

	//exit with no errors
	return 0;
}