#pragma once
#include <GLEW/glew.h>

class VertexBufferObject
{
public:
	unsigned int Handle;
	VertexBufferObject(float* vertices, int SizeInBytes);
	void Use();
};

