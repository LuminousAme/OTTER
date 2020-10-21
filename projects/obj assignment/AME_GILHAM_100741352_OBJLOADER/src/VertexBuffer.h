// VertexBuffer.h - header for the class that stores the buffer for vertices that we can render
#pragma once

//import the buffer base class
#include "IBuffer.h"
//import the other features we need
#include <memory>


//class for the buffer that will store vertices for rendering
class VertexBuffer : public IBuffer {
public:
	//defines a special easier to use name for shared(smart) pointers to the class 
	typedef std::shared_ptr<VertexBuffer> svbptr;

	//creates and returns a shared(smart) pointer to the class 
	static inline svbptr Create(GLenum usage = GL_STATIC_DRAW) {
		return std::make_shared<VertexBuffer>(usage);
	}

public:
	//constructor, creates a new vertex buffer with the given usage, data will be still need be loaded before it can be used though
	VertexBuffer(GLenum usage = GL_STATIC_DRAW) : IBuffer(GL_ARRAY_BUFFER, usage)
	{ }

	//unbinds the current vertex buffer
	static void UnBind() {
		IBuffer::UnBind(GL_ARRAY_BUFFER);
	}

};