#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject() 
{
	glGenVertexArrays(1, &Handle);
	glBindVertexArray(Handle);
}
void VertexArrayObject::Use() 
{
	glBindVertexArray(Handle);
}