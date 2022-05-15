#include "Model.h"

Model::Model(const std::vector<float>& verticesParam, const std::vector<unsigned int>& indicesParam, const Texture& textureParam) :
	vertices{ verticesParam }, indices{ indicesParam }, texture{ textureParam },
	vertexBuffer{ VertexBuffer(vertices.data(), vertices.size() * sizeof(float)) },
	indexBuffer{ IndexBuffer(indices.data(), indices.size()) }
{
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	layout.Push<float>(3);

	vertexArray.AddBuffer(vertexBuffer, layout);
}

void Model::Draw(const Camera& camera, Shader& shader, const Renderer& renderer)
{
	texture.Bind();
	shader.Bind();
	shader.SetUniformMat4f("projection", camera.GetProjectionMatrix());
	shader.SetUniformMat4f("view", camera.GetViewMatrix());

	renderer.Draw(vertexArray, indexBuffer, shader);
}
