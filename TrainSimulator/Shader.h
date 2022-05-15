#pragma once

#include <glm.hpp>
#include<unordered_map>

struct shaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
private:
	std::string filePath;
	unsigned int rendererID;
	std::unordered_map<std::string, int> uniformLocationCache;

public:

	Shader(const std::string& filePath);
	~Shader();

	void Bind()const;
	void Unbind()const;

	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	shaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& setUniform);

	int GetUniformLocation(const std::string& name);

};
