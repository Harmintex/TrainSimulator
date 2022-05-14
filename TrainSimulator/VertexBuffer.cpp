#include "VertexBuffer.h"

#include"Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GlCall(glGenBuffers(1, &rendererID));
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
	GlCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GlCall(glDeleteBuffers(1, &rendererID));
}
	
void VertexBuffer::Bind()const
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
}

void VertexBuffer::Unbind()const
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
