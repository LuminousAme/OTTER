//ObjLoader.h - header file for the class that parses OBJ files into TTN_Models 
#pragma once

//include the mesh class so we write the data to it 
#include "Mesh.h"



	//class to parse ObjFiles into TTN_Model objects
class ObjLoader {
public:
	static Mesh* LoadFromFile(const std::string& fileName);

protected:
	//we don't need objects of this class as the only method is static so we can just have the compiler generate constructors and destrcutors for us
	ObjLoader() = default;
	~ObjLoader() = default;
};
