//Titan Engine, by Atlas X Games 
// Mesh.h - header for the class that represents 3D meshes
#pragma once

//include the opengl wrap around classes
#include "VertexArrayObject.h"
//include glm features
#include "GLM/glm.hpp"
//import other required features
#include <vector>


//class representing 3D meshes 
class Mesh {
public:
	//constructor
	Mesh();
	//copy constructors
	Mesh(const Mesh&) = default;
	Mesh(Mesh&&) = default;
	Mesh& operator=(Mesh&) = default;

	//constructor with data built in 
	Mesh(std::vector<glm::vec3>& verts, std::vector<glm::vec3>& norms, std::vector<glm::vec2>& uvs);

	//destructor
	~Mesh();

	//sets up the VAO for the mesh so it can acutally be rendered, called by the user (as they may change details of the mesh)
	void SetUpVao();

	//SETTERS 
	//sets the vertices of the mesh and loads them into a position VBO
	void setVertices(std::vector<glm::vec3>& verts);
	//sets the normals of the mesh and loads them into a normal VBO
	void setNormals(std::vector<glm::vec3>& norms);
	//sets the UVs of the mesh and loads them into a uv VBO
	void setUVs(std::vector<glm::vec2>& uvs);

	//GETTERS
	//Gets the pointer to the meshes vao
	VertexArrayObject::svaptr GetVAOPointer();


protected:
	//a vector containing all the vertices on the mesh 
	std::vector<glm::vec3> m_Vertices;
	//a vector containing all the normals of the mesh
	std::vector<glm::vec3> m_Normals;
	//a vector containing all the uvs of a mesh
	std::vector<glm::vec2> m_Uvs;

	//array of VBO smart pointers, 0 for pos, 1 for normals, 2 for uvs. 
	VertexBuffer::svbptr m_vbos[3];
	//smart pointer with the VAO for the mesh 
	VertexArrayObject::svaptr m_vao;

	//sends data into a vbo, creating one if it doesn't exist already, takes the index for which vbo, the num of elements being passed, and the data itself
	template<typename T>
	void SendToVBO(unsigned int index, GLint numOfElements, const std::vector<T>& data)
	{
		//if the vbo doesn't exist, create it 
		if (m_vbos[index] == nullptr)
			m_vbos[index] = VertexBuffer::Create();

		//ensure we acutally have data to send before continuing
		if (data.size() != 0) {
			//if it does have data, then load it into the approriate vbo
			m_vbos[index]->LoadData(data.data(), numOfElements);
		}
	}
};
