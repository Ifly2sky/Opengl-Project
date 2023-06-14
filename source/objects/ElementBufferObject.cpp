#include "ElementBufferObject.h"

ElementBufferObject::ElementBufferObject(unsigned int* indices, unsigned long long SizeinBytes)
{
	glGenBuffers(1, &Handle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, SizeinBytes, indices, GL_STATIC_DRAW);
}
void ElementBufferObject::Use() 
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Handle);
}