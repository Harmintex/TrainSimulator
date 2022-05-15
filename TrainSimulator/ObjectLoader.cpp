#include "ObjectLoader.h"
#pragma once

void ObjectLoader::LoadObject(std::string& filePath, std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUvs;
	std::vector<glm::vec3> tempNormals;

	std::ifstream inputFile(filePath);

	std::string line;

	unsigned long long count = 0;

	while (true)
	{
		inputFile >> line;

		if (line == "end")
			break;

		if (line == "v")
		{
			glm::vec3 vertex;
			inputFile >> vertex.x >> vertex.y >> vertex.z;
			tempVertices.push_back(vertex);
			continue;
		}

		if (line == "vt")
		{
			glm::vec2 uv;
			inputFile >> uv.x >> uv.y;
			tempUvs.push_back(uv);
			continue;
		}

		if (line == "vt")
		{
			glm::vec3 normal;
			inputFile >> normal.x >> normal.y >> normal.z;
			tempNormals.push_back(normal);
			continue;
		}

		if (line == "f")
		{
			char separator;

			unsigned int vertexIndex[3];
			unsigned int uvIndex[3];
			unsigned int normalIndex[3];

			inputFile >> vertexIndex[0] >> separator >> uvIndex[0] >> separator >> normalIndex[0];
			inputFile >> vertexIndex[1] >> separator >> uvIndex[1] >> separator >> normalIndex[1];
			inputFile >> vertexIndex[2] >> separator >> uvIndex[2] >> separator >> normalIndex[2];

			vertices.push_back(tempVertices[vertexIndex[0] - 1].x);
			vertices.push_back(tempVertices[vertexIndex[0] - 1].y);
			vertices.push_back(tempVertices[vertexIndex[0] - 1].z);

			vertices.push_back(tempUvs[uvIndex[0] - 1].x);
			vertices.push_back(tempUvs[uvIndex[0] - 1].y);

			vertices.push_back(tempNormals[normalIndex[0] - 1].x);
			vertices.push_back(tempNormals[normalIndex[0] - 1].y);
			vertices.push_back(tempNormals[normalIndex[0] - 1].z);

			indices.push_back(count);
			count++;


			vertices.push_back(tempVertices[vertexIndex[1] - 1].x);
			vertices.push_back(tempVertices[vertexIndex[1] - 1].y);
			vertices.push_back(tempVertices[vertexIndex[1] - 1].z);

			vertices.push_back(tempUvs[uvIndex[1] - 1].x);
			vertices.push_back(tempUvs[uvIndex[1] - 1].y);

			vertices.push_back(tempNormals[normalIndex[1] - 1].x);
			vertices.push_back(tempNormals[normalIndex[1] - 1].y);
			vertices.push_back(tempNormals[normalIndex[1] - 1].z);

			indices.push_back(count);
			count++;


			vertices.push_back(tempVertices[vertexIndex[2] - 1].x);
			vertices.push_back(tempVertices[vertexIndex[2] - 1].y);
			vertices.push_back(tempVertices[vertexIndex[2] - 1].z);

			vertices.push_back(tempUvs[uvIndex[2] - 1].x);
			vertices.push_back(tempUvs[uvIndex[2] - 1].y);

			vertices.push_back(tempNormals[normalIndex[2] - 1].x);
			vertices.push_back(tempNormals[normalIndex[2] - 1].y);
			vertices.push_back(tempNormals[normalIndex[2] - 1].z);

			indices.push_back(count);
			count++;
		}
	}
}
