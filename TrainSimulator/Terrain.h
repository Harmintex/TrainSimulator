#pragma once
#include "Texture.h"

class Terrain
{
private:
	unsigned int length, width, height;
	unsigned int VBO, EBO, VAO;
	Texture* groundTexture;

public:
	Terrain();
	~Terrain();

	void Draw();
};