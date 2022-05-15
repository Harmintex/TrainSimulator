#pragma once
#include<vector>
#include<GL/glew.h>
#include<glfw3.h>
#include"Renderer.h"
#include"VertexBufferLayout.h"
#include"VertexBuffer.h"
#include"IndexBuffer.h"
#include"VertexArray.h"
#include"Shader.h"
#include"Texture.h"
#include "Camera.h"
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>

class Model
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	Texture texture;

	VertexBuffer vertexBuffer;
	VertexArray vertexArray;
	IndexBuffer indexBuffer;

public:
	Model(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const Texture& texture = Texture());
	void Draw(const Camera& camera, Shader& shader, const Renderer& renderer);
};