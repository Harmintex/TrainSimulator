#pragma once
#include <vector>
#include <string>
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

class SkyBox
{
public:
	SkyBox();
	~SkyBox();
	unsigned int CreateCubemapTexture();
	void Draw(const glm::mat4& view, const glm::mat4& projection);
private:
	unsigned int VBO, EBO, VAO;
	unsigned int textureID;
	Shader skyBoxShader;
	std::vector<std::string> facePaths;
};

