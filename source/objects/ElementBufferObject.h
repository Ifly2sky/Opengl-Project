#pragma once
#include <iostream>
#include <GLEW/glew.h>

class ElementBufferObject
{
public:
	unsigned int Handle;
	ElementBufferObject(unsigned int* indices, unsigned long long SizeinBytes);
	void Use();
};

