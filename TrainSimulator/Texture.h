#pragma once

#include "Renderer.h"

class Texture
{
	unsigned int rendererID;
	std::string filePath;
	unsigned char* localBuffer;
	int width;
	int height;
	int BPP;

public:
	Texture();
	Texture(unsigned int textureID);
	Texture(const std::string& filePath);
	Texture(const Texture& texture);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
};