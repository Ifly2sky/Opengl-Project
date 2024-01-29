#include "VertexBufferObject.h"
VertexBufferObject::VertexBufferObject(float* vertices, int SizeInBytes, std::vector<int> attributeSizes, unsigned long long stride, std::vector<unsigned long long> startPositions)
{
	glGenBuffers(1, &Handle);
	glBindBuffer(GL_ARRAY_BUFFER, Handle);
	glBufferData(GL_ARRAY_BUFFER, SizeInBytes, vertices, GL_STATIC_DRAW);

	int attribute = 0;
	for (int i = 0; i < (int)std::size(attributeSizes); i++) {
		glVertexAttribPointer(attribute, attributeSizes[i], GL_FLOAT, GL_FALSE, stride, (void*)startPositions[i]);
		glEnableVertexAttribArray(attribute);
		printf("vertex attribute data: %d, %d, %d, %d \n", attribute, attributeSizes[i], stride, startPositions[i]);
		attribute++;
	}

	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
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