#pragma once
#include <GLEW/glew.h>
#include <objects/VertexData.h>

class VertexBufferObject
{
public:
	unsigned int Handle;
	VertexBufferObject(float* vertices, int SizeInBytes);
	VertexBufferObject(Vertex* vertices);
	void Use();
};

