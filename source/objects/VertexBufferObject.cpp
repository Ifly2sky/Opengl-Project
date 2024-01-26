#include "VertexBufferObject.h"

VertexBufferObject::VertexBufferObject(float* vertices, int SizeInBytes)
{
	glGenBuffers(1, &Handle);
	glBindBuffer(GL_ARRAY_BUFFER, Handle);
	glBufferData(GL_ARRAY_BUFFER, SizeInBytes, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
}
VertexBufferObject::VertexBufferObject(Vertex* vertices)
{
	glGenBuffers(1, &Handle);
	glBindBuffer(GL_ARRAY_BUFFER, Handle);
	glBufferData(GL_ARRAY_BUFFER, 1000 * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}
void VertexBufferObject::Use()
{
	glBindBuffer(GL_ARRAY_BUFFER, Handle);
}