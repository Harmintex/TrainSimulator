#include "Texture.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture()
{
	this->rendererID = 0;
	this->filePath = "";
	this->localBuffer = nullptr;
	this->width = 0;
	this->height = 0;
	this->BPP = 0;
}

Texture::Texture(unsigned int textureID)
{
	this->rendererID = textureID;
	this->filePath = "";
	this->localBuffer = nullptr;
	this->width = 0;
	this->height = 0;
	this->BPP = 0;
}

Texture::Texture(const std::string& filePath)
{
	this->rendererID = 0;
	this->filePath = filePath;
	this->localBuffer = nullptr;
	this->width = 0;
	this->height = 0;
	this->BPP = 0;

	stbi_set_flip_vertically_on_load(1);
	localBuffer = stbi_load(filePath.c_str(), &width, &height, &BPP, 4);

	glGenTextures(1, &rendererID);
	glBindTexture(GL_TEXTURE_2D, rendererID);

	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
	GlCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (localBuffer)
		stbi_image_free(localBuffer);
}

Texture::Texture(const Texture& texture): Texture(texture.filePath)
{

}

Texture::~Texture()
{
	glDeleteTextures(1, &rendererID);
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	GlCall(glBindTexture(GL_TEXTURE_2D, rendererID));
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}