//Titan Engine, by Atlas X Games 
// Scene.h - header for the class that represents openGL shaders
#pragma once

//include required libraries
//open gl interfacing
#include <glad/glad.h>
//memory functions
#include <memory>
//std::string class
#include <string>
//std::unordered_map class 
#include <unordered_map>
//GLM types and basic functions
#include <GLM/glm.hpp>
//GLM pointers 
#include <GLM/gtc/type_ptr.hpp>
//logging 
#include "Logging.h"



	//class to wrap around an opengl shader 
class Shader final {
public:
	//defines a special easier to use name for shared(smart) pointers to the class 
	typedef std::shared_ptr<Shader> sshptr;

	//creates and returns a shared(smart) pointer to the class 
	static inline sshptr Create() {
		return std::make_shared<Shader>();
	}

public:
	//ensuring moving and copying is not allowed so we can manually control destructor calls
	Shader(const Shader& other) = delete;
	Shader(Shader& other) = delete;
	Shader& operator=(const Shader& other) = delete;
	Shader& operator=(Shader&& other) = delete;

public:
	//default constructor that creates a new empty shader
	Shader();

	//destructor
	~Shader();

	//Loads a single stage on the pipeline (vertix or fragment shader, etc.) 
	//and returns true if successful, false if not
	bool LoadShaderStage(const char* sourceCode, GLenum shaderType);

	//Loads a single stage on the pipeline (vertex or fragment shader, etc.) from an external file
	//and returns true if sucessful, false if not 
	bool LoadShaderStageFromFile(const char* filePath, GLenum shaderType);

	//Links the stages together creating the pipeline and making the shader program useable
	//returns true if sucessful, false if not 
	bool Link();

	//Binds the shader program so we can acutally use it 
	void Bind();

	//Unbinds the shader program so we can use another 
	static void UnBind();

	//Gets the OpenGL handle that it's wrapping around 
	GLuint GetHandle() const { return _handle; }

public:
	//Set a uniform for a 3x3 matrix 
	void SetUniformMatrix(int location, const glm::mat3* value, int count = 1, bool transposed = false);
	//Set a unform for  a 4x4 matrix 
	void SetUniformMatrix(int location, const glm::mat4* value, int count = 1, bool transposed = false);
	//Set a uniform for a float scalar 
	void SetUniform(int location, const float* value, int count = 1);
	//Set a uniform for a 2D float vector 
	void SetUniform(int location, const glm::vec2* value, int count = 1);
	//Set a uniform for a 3D float vector
	void SetUniform(int location, const glm::vec3* value, int count = 1);
	//Set a uniform for a 4D float vector
	void SetUniform(int location, const glm::vec4* value, int count = 1);
	//Set a uniform for an integer scalar
	void SetUniform(int location, const int* value, int count = 1);
	//Set a uniform for a 2D integer vector 
	void SetUniform(int location, const glm::ivec2* value, int count = 1);
	//Set a uniform for a 3D integer vector 
	void SetUniform(int location, const glm::ivec3* value, int count = 1);
	//Set a uniform for a 4D integer vector 
	void SetUniform(int location, const glm::ivec4* value, int count = 1);
	//Set a uniform for a boolean 
	void SetUniform(int location, const bool* value, int count = 1);
	//Set a uniform for 2 booleans 
	void SetUniform(int location, const glm::bvec2* value, int count = 1);
	//Set a uniform for 3 booleans
	void SetUniform(int location, const glm::bvec3* value, int count = 1);
	//Set a uniform for 4 booleans
	void SetUniform(int location, const glm::bvec4* value, int count = 1);

	//template function for setting a uniform based on just name and data
	template <typename T>
	void SetUniform(const std::string& name, const T& value) {
		//finds the location that the uniform of that name is stored at 
		int location = __GetUniformLocation(name);
		//check if the location exists
		if (location != -1) {
			//if it does, then set the uniform at that location
			SetUniform(location, &value, 1);
		}
		else {
			//if it doesn't log a warning
			LOG_WARN("Ignoring uniform \"{}\"", name);
		}
	}

	//template function for setting a uniform matrix based on just name and data
	template <typename T>
	void SetUniformMatrix(const std::string& name, const T& value, bool transposed = false) {
		//finds the location that the uniform of that name is stored at 
		int location = __GetUniformLocation(name);
		//check if the location exists
		if (location != -1) {
			//if it does, then set the uniform matrix at that location
			SetUniformMatrix(location, &value, 1, transposed);
		}
		else {
			//if it doesn't log a warning
			LOG_WARN("Ignoring uniform \"{}\"", name);
		}
	}

protected:
	//vertex shader
	GLuint _vs;
	//fragment shader 
	GLuint _fs;

	//handle for the shader program 
	GLuint _handle;

	//unordered map to store the locations of all the uniforms 
	std::unordered_map<std::string, int> _uniformLocations;

	//function to get the locations of all the uniforms
	int __GetUniformLocation(const std::string& name);
};
