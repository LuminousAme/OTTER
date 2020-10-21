// Mesh.cpp - source file for the class that represents 3D meshes

//include the header
#include "Mesh.h"


	//constructor, creates a mesh
Mesh::Mesh()
{
	//initliaze the array of VBO pointers
	for (int i = 0; i < 3; i++)
		m_vbos[i]->Create();

	//initliaze the vao
	m_vao->Create();
}

//Constructor, creates a mesh with the data given
Mesh::Mesh(std::vector<glm::vec3>& verts, std::vector<glm::vec3>& norms, std::vector<glm::vec2>& uvs)
{
	//initliaze the array of VBO pointers
	for (int i = 0; i < 3; i++)
		m_vbos[i]->Create();

	//initliaze the vao
	m_vao->Create();

	//set the vertices
	setVertices(verts);
	//set the normals
	setNormals(norms);
	//set the uvs
	setUVs(uvs);
	
}

//destructor
Mesh::~Mesh()
{
}

//sets up the VAO for the mesh so it can acutally be rendered, needs to be called by the user in case they change the mesh
void Mesh::SetUpVao()
{
	//creates a new vao
	m_vao = VertexArrayObject::Create();

	//load the vbos from the mesh into the vao 
	m_vao->AddVertexBuffer(m_vbos[0], { BufferAttribute(0, 3, GL_FLOAT, false, sizeof(float) * 3, 0, AttribUsage::Position) });
	m_vao->AddVertexBuffer(m_vbos[1], { BufferAttribute(1, 3, GL_FLOAT, false, sizeof(float) * 3, 0, AttribUsage::Normal) });
	m_vao->AddVertexBuffer(m_vbos[2], { BufferAttribute(2, 2, GL_FLOAT, false, sizeof(float) * 2, 0, AttribUsage::Texture) });
}

//sets the vertices and puts them in a position vbo
void Mesh::setVertices(std::vector<glm::vec3>& verts)
{
	//copy the vertices' postion
	m_Vertices = verts;
	//send them to a vbo
	SendToVBO(0, verts.size(), verts);
}

void Mesh::setNormals(std::vector<glm::vec3>& norms)
{
	//copy the normals
	m_Normals = norms;
	//send them to a vbo
	SendToVBO(1, norms.size(), norms);
}

void Mesh::setUVs(std::vector<glm::vec2>& uvs)
{
	//copy the uvs
	m_Uvs = uvs;
	//send them to a vbo
	SendToVBO(2, uvs.size(), uvs);
}

//gets the pointer to the meshes vao 
VertexArrayObject::svaptr Mesh::GetVAOPointer()
{
	return m_vao;
}

