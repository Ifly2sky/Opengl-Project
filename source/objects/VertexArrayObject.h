#pragma once
#include <iostream>
#include <GLEW/glew.h>

class VertexArrayObject
{
	public:
		unsigned int Handle;
		VertexArrayObject();
		void Use();
};

