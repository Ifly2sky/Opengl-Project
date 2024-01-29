#pragma once
#include <GLEW/glew.h>
#include <objects/VertexData.h>
#include <vector>
#include <stdio.h>

class VertexBufferObject
{
public:
	unsigned int Handle;
	VertexBufferObject(float* vertices, int SizeInBytes, std::vector<int> attributeSizes, unsigned long long stride, std::vector<unsigned long long> startPositions);
	VertexBufferObject(Vertex* vertices);
	void Use();
};

