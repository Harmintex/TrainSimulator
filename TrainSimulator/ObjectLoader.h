#pragma once
#include <fstream>
#include <string>
#include <vector>
#include"glm.hpp"

class ObjectLoader
{
public:
	static void LoadObject(std::string filePath, std::vector<float>& vertices, std::vector<unsigned int>& indices);
};