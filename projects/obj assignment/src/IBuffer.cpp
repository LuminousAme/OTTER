// IBuffer.cpp - source file for the abstract base class for all the OpenGL buffer types

#include "IBuffer.h"

	//constructor, creates a buffer with the given type and usage
IBuffer::IBuffer(GLenum type, GLenum usage)
	: _elementCount(0), _elementSize(0), _handle(0),
	_type(type), _usage(usage)
{
	glCreateBuffers(1, &_handle);
}

//virtual destructor, destroys the buffer
IBuffer::~IBuffer()
{
	//if the buffer exists
	if (_handle != 0) {
		//delete it and set the handle to 0
		glDeleteBuffers(1, &_handle);
		_handle = 0;
	}
}

//loads the data into the buffer using bindless state access 
void IBuffer::LoadData(const void* data, size_t elementSize, size_t elementCount)
{
	//load the data into the buffer
	glNamedBufferData(_handle, elementSize * elementCount, data, _usage);
	//save the size and number of elements in the data
	_elementCount = elementCount;
	_elementSize = elementSize;
}

//bind the buffer so it can be used
void IBuffer::Bind()
{
	glBindBuffer(_type, _handle);
}

//unbind the buffer bound to the given type
void IBuffer::UnBind(GLenum type)
{
	glBindBuffer(type, 0);
}
