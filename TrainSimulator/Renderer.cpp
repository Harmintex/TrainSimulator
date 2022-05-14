#include "Renderer.h"
#include<iostream>

#pragma once

void GlClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GlLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << " [OpenGl Error] (" << error << ")" << function
			<< " " << file << " : " << line << std::endl;
		return false;
	}

	return true;
}

void Renderer::Clear()const
{
	GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader)const
{
	shader.Bind();
	vertexArray.Bind();
	indexBuffer.Bind();
	GlCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}
