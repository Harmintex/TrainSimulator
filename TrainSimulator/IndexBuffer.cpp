#include "IndexBuffer.h"
#include"Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
{
	this->count = count;
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GlCall(glGenBuffers(1, &rendererID));
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GlCall(glDeleteBuffers(1, &rendererID));
}

void IndexBuffer::Bind()const
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
}

void IndexBuffer::Unbind()const
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}