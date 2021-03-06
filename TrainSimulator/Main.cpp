#include <iostream>
#include <fstream>
#include <sstream>
#include<GL/glew.h>
#include<glfw3.h>
#include<GL/glut.h>
#include<glm.hpp>
#include <time.h> 

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Camera.h"
#include "ObjectLoader.h"
#include "Model.h"
#include "SkyBox.h"

// Global avriables
const int SCR_HEIGHT = 1080;
const int SCR_WIDTH = 1920;
float lastFrame = 0.0f, deltaTime;
float trainSpeed = 1.0f;
glm::vec3 trainPosition = glm::vec3(0.0f, -5.7f, -21.6f);
Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 0.0f));
glm::vec3 lightPos(-8.0f, 30.f, 15.0f);

enum EStation
{
	NONE,
	BRASOV,
	SINAIA,
	PLOIESTI,
	BUCURESTI
};

enum TypeOfNatureObject {
	FIRTREE,
	PINETREE,
	BUSH,
	HAZELNUTTREE,
	STONE,
	Count
};

// Functions used for proccesing input from user
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yOffset);

Model StationSignModel(const Texture& texture)
{
	std::vector<float> vertices = {

		/*Vertex coords	       Texture coords        Normals*/
	 -2.0f,  -0.5f, 0.0f,		0.0f, 0.0f, 	0.0f,  0.0f,  1.0f,
	  2.0f,  -0.5f, 0.0f,		1.0f, 0.0f, 	0.0f,  0.0f,  1.0f,
	  2.0f,   0.5f, 0.0f,		1.0f, 1.0f, 	0.0f,  0.0f,  1.0f,
	 -2.0f,   0.5f, 0.0f,		0.0f, 1.0f,  	0.0f,  0.0f,  1.0f,
	};

	std::vector<unsigned int> indices = {
		0,1,2,
		2,3,0
	};

	return Model(vertices, indices, texture);
}

Model StationPlatformModel(const Texture& texture)
{
	std::vector<float> vertices = {
		/*Vertex coords	            Texture coords        Normals*/

		//front
		-23.0f, -0.25f, 0.0f,       0.0f, 0.0f,         0.0f, 0.0f, 1.0f,
		23.0f, -0.25f, 0.0f,        1.0f, 0.0f,         0.0f, 0.0f, 1.0f,
		23.0f, 0.25f, 0.0f,         1.0f, 1.0f,         0.0f, 0.0f, 1.0f,
		-23.0f, 0.25f, 0.0f,        0.0f, 1.0f,         0.0f, 0.0f, 1.0f,

		//back
		-23.0f, -0.25f, -45.0f,     1.0f, 0.0f,         0.0f, 0.0f, -1.0f,
		-23.0f, 0.25f, -45.0f,      1.0f, 1.0f,         0.0f, 0.0f, -1.0f,
		23.0f, 0.25f, -45.0f,       0.0f, 1.0f,         0.0f, 0.0f, -1.0f,
		23.0f, -0.25f, -45.0f,      0.0f, 0.0f,         0.0f, 0.0f, -1.0f,

		//left	   	  
		-23.0f, -0.25f, -45.0,      0.0f, 0.0f,         -1.0f, 0.0f, 0.0f,
		-23.0f, 0.25f, -45.0,       0.0f, 1.0f,         -1.0f, 0.0f, 0.0f,
		-23.0f, 0.25f, 0.0,         1.0f, 1.0f,         -1.0f, 0.0f, 0.0f,
		-23.0f, -0.25f, 0.0,        1.0f, 0.0f,         -1.0f, 0.0f, 0.0f,

		//right
		23.0, -0.25f, 0.0f,         0.0f, 0.0f,          1.0f, 0.0f, 0.0f,
		23.0, -0.25f, -45.0f,       1.0f, 0.0f,          1.0f, 0.0f, 0.0f,
		23.0, 0.25f, -45.0f,        1.0f, 1.0f,          1.0f, 0.0f, 0.0f,
		23.0, 0.25f, 0.0f,          0.0f, 1.0f,          1.0f, 0.0f, 0.0f,

		//top
		-23.0f, 0.25f, 0.0f,        0.0f, 0.0f,          0.0f, 1.0f, 0.0f,
		23.0f, 0.25f, 0.0f,         1.0f, 0.0f,          0.0f, 1.0f, 0.0f,
		23.0f, 0.25f, -45.0f,       1.0f, 1.0f,          0.0f, 1.0f, 0.0f,
		-23.0f, 0.25f, -45.0f,      0.0f, 1.0f,          0.0f, 1.0f, 0.0f,

		//bottom
		-23.0f, -0.25f, -45.0f,     0.0f, 0.0f,          0.0f, -1.0f, 0.0f,
		23.0f, -0.25f, -45.0f,      1.0f, 0.0f,          0.0f, -1.0f, 0.0f,
		23.0f, -0.25f, 0.0f,        1.0f, 1.0f,          0.0f, -1.0f, 0.0f,
		-23.0f, -0.25f, 0.0f,       0.0f, 1.0f,          0.0f, -1.0f, 0.0f
	};

	std::vector<unsigned int> indices = {
		//front
		0,1,2,
		2,3,0,

		//back
		5,4,7,
		7,6,5,

		//left
		11,8,9,
		9,10,11,

		//right
		12,13,14,
		14,15,12,

		//top
		16,17,18,
		18,19,16,

		//bottom
		23,22,21,
		21,20,23
	};

	return Model(vertices, indices, texture);
}

Model RailwayModel(const Texture& texture)
{
	std::vector<float> vertices = {

		/*Vertex coords	       Texture coords        Normals*/
	  0.0f,   -5.6f,  -25.2f,	    0.0f, 0.0f, 	0.0f,  0.0f,  1.0f,
	  30.0f,  -5.6f,  -25.2f,	    1.0f, 0.0f, 	0.0f,  0.0f,  1.0f,
	  30.0f,  -5.6f,  -18.0f,		1.0f, 1.0f, 	0.0f,  0.0f,  1.0f,
	  0.0f,   -5.6f,  -18.0f,		0.0f, 1.0f,  	0.0f,  0.0f,  1.0f,
	};

	std::vector<unsigned int> indices = {
		0,1,2,
		2,3,0
	};

	return Model(vertices, indices, texture);
}

Model TerrainModel(Texture& texture)
{
	std::vector<float> vertices = {
		 150.0f,  -5.63f, -270.0f,		0.0f, 0.0f, 	0.0f,  0.0f,  1.0f,
		 180.0f,  -5.63f, -270.0f,		1.0f, 0.0f, 	0.0f,  0.0f,  1.0f,
		 180.0f,  -5.63f, -240.0f,		1.0f, 1.0f, 	0.0f,  0.0f,  1.0f,
		 150.0f,  -5.63f, -240.0f,		0.0f, 1.0f,  	0.0f,  0.0f,  1.0f
	};

	std::vector<unsigned int> indices = {
		0,1,2,
		2,3,0
	};

	return Model(vertices, indices, texture);
}

std::pair<float, float> GenerateRandomXAndZ() 
{
	float minx = camera.GetPosition().x - 850.0f;
	float minz = camera.GetPosition().z - 200.0f;
	float maxx = camera.GetPosition().x + 170.0f;
	float maxz = camera.GetPosition().z + 100.0f;
	float x = minx + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxx - minx)));
	float z = minz + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxz - minz)));
	while (((x > -23.0f && x < 23.0f) || (x > -263.0f && x < -217.0f) || (x > -503.0f && x < -457.0f) || (x > -743.0f && x < -697.0f)) && z < -10.5f && z > -58.5f) {
		x = minx + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxx - minx)));
		z = minz + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxz - minz)));
	}
	while (z < -10.5f && z > -30.5f)
	{
		z = minz + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxz - minz)));
	}

	return std::make_pair(x, z);
}

TypeOfNatureObject GenerateRandomNatureObject() 
{
	int corespondingNumber = rand() % static_cast<int>(TypeOfNatureObject::Count);
	return static_cast<TypeOfNatureObject>(corespondingNumber);
}

std::vector<std::pair<std::pair<float, float>, TypeOfNatureObject>> GenerateObjectsWithRandomPositions()
{
	std::vector<std::pair<std::pair<float, float>, TypeOfNatureObject>> objectsWithRandomPositons;
	for (int i = 0; i < 2500; i++) {
		objectsWithRandomPositons.push_back(std::make_pair(GenerateRandomXAndZ(), GenerateRandomNatureObject()));
	}
	return objectsWithRandomPositons;
}

void RenderTrain(Shader& objectShader, Camera& camera, Renderer& renderer, Model& train, GLFWwindow* window)
{
	glm::mat4 model = glm::mat4(0.7f);

	model = glm::translate(model, trainPosition);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.5f, 0.5f));

	objectShader.Bind();
	objectShader.SetUniformMat4f("model", model);

	train.Draw(camera, objectShader, renderer);
}

void RenderStation(Shader& shader, Camera& camera, Renderer& renderer, Model& station, Model& stationSign, Model& stationPlatform, GLFWwindow* window, EStation stationName)
{

	switch (stationName)
	{
	case NONE:
		break;
	case BRASOV:
	{
		if (std::abs(camera.GetPosition().x - 0.0f) < 130.0f)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -5.0f, -40.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 0.7f, 1.5f));
			shader.Bind();
			shader.SetUniformMat4f("model", model);
			station.Draw(camera, shader, renderer);

			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::translate(model2, glm::vec3(0.5f, -1.26f, -32.9f));
			model2 = glm::scale(model2, glm::vec3(0.7f, 0.7f, 0.7f));
			shader.Bind();
			shader.SetUniformMat4f("model", model2);
			stationSign.Draw(camera, shader, renderer);

			glm::mat4 model3 = glm::mat4(0.7f);
			model3 = glm::translate(model3, glm::vec3(0.8f, -5.45f, -24.5f));
			model3 = glm::scale(model3, glm::vec3(0.6f, 0.6f, 0.6f));
			shader.Bind();
			shader.SetUniformMat4f("model", model3);
			stationPlatform.Draw(camera, shader, renderer);
		}

		break;
	}
	case SINAIA:
	{
		if (std::abs(camera.GetPosition().x - (-240.0f)) < 130.0f)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-240.0f, -5.0f, -40.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 0.7f, 1.5f));
			shader.Bind();
			shader.SetUniformMat4f("model", model);
			station.Draw(camera, shader, renderer);


			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::translate(model2, glm::vec3(-239.5f, -1.26f, -32.9f));
			model2 = glm::scale(model2, glm::vec3(0.7f, 0.7f, 0.7f));
			shader.Bind();
			shader.SetUniformMat4f("model", model2);
			stationSign.Draw(camera, shader, renderer);

			glm::mat4 model3 = glm::mat4(0.7f);
			model3 = glm::translate(model3, glm::vec3(-239.2f, -5.45f, -24.5f));
			model3 = glm::scale(model3, glm::vec3(0.6f, 0.6f, 0.6f));
			shader.Bind();
			shader.SetUniformMat4f("model", model3);
			stationPlatform.Draw(camera, shader, renderer);
		}

		break;
	}
	case PLOIESTI:
	{
		if (std::abs(camera.GetPosition().x - (-480.0f)) < 130.0f)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-480.0f, -5.0f, -40.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 0.7f, 1.5f));
			shader.Bind();
			shader.SetUniformMat4f("model", model);
			station.Draw(camera, shader, renderer);

			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::translate(model2, glm::vec3(-479.5f, -1.26f, -32.9f));
			model2 = glm::scale(model2, glm::vec3(0.7f, 0.7f, 0.7f));
			shader.Bind();
			shader.SetUniformMat4f("model", model2);
			stationSign.Draw(camera, shader, renderer);

			glm::mat4 model3 = glm::mat4(0.7f);
			model3 = glm::translate(model3, glm::vec3(-479.2f, -5.45f, -24.5f));
			model3 = glm::scale(model3, glm::vec3(0.6f, 0.6f, 0.6f));
			shader.Bind();
			shader.SetUniformMat4f("model", model3);
			stationPlatform.Draw(camera, shader, renderer);
		}

		break;
	}
	case BUCURESTI:
	{
		if (std::abs(camera.GetPosition().x - (-720.0f)) < 130.0f)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-720.0f, -5.0f, -40.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 0.7f, 1.5f));
			shader.Bind();
			shader.SetUniformMat4f("model", model);
			station.Draw(camera, shader, renderer);

			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::translate(model2, glm::vec3(-719.5f, -1.26f, -32.9f));
			model2 = glm::scale(model2, glm::vec3(0.7f, 0.7f, 0.7f));
			shader.Bind();
			shader.SetUniformMat4f("model", model2);
			stationSign.Draw(camera, shader, renderer);

			glm::mat4 model3 = glm::mat4(0.7f);
			model3 = glm::translate(model3, glm::vec3(-719.2f, -5.45f, -24.5f));
			model3 = glm::scale(model3, glm::vec3(0.6f, 0.6f, 0.6f));
			shader.Bind();
			shader.SetUniformMat4f("model", model3);
			stationPlatform.Draw(camera, shader, renderer);
		}

		break;
	}
	default:
		break;
	}
}

void RenderRailway(Shader& shader, Camera& camera, Renderer& renderer, Model& railway, GLFWwindow* window)
{
	glm::mat4 model = glm::mat4(1.0f);

	glm::vec3 position(0.0f, 0.0f, 0.0f);
	model = glm::translate(model, position);

	for (int i = 0; i < 37; i++)
	{
		if (camera.GetPosition().x < 30.0f - (i - 5) * 30.0f && camera.GetPosition().x > -280.0f - (i - 5) * 30.0f)
		{
				shader.Bind();
				shader.SetUniformMat4f("model", model);

				railway.Draw(camera,shader, renderer);
		}
		position = { -30.0f, 0.0f, 0.0f };
		model = glm::translate(model, position);
	}
	railway.Draw(camera,shader, renderer);
}

void RenderTerrain(Shader& shader, Camera& camera, Renderer& renderer, Model& terrain, GLFWwindow* window)
{
	glm::mat4 model = glm::mat4(0.7f);
	glm::vec3 position(0.0f, 0.0f, 0.0f);


	for (int i = 0; i < 35; i++)
	{
		if (camera.GetPosition().x < 200.0f - (i - 5) * 30.0f && camera.GetPosition().x > -120.0f - (i - 5) * 30.0f)
		{
			for (int j = 0; j < 12; j++)
			{
				position = { 0.0f, 0.0f, 30.0f };
				model = glm::translate(model, position);

				shader.Bind();
				shader.SetUniformMat4f("model", model);

				terrain.Draw(camera, shader, renderer);
			}
			position = { -30.0f, 0.0f, -360.0f };
		}
		else
		{
			position = { -30.0f, 0.0f, 0.0f };

		}
		model = glm::translate(model, position);
	}
	terrain.Draw(camera, shader, renderer);
}

void RenderNatureObjects(Shader& shader, Camera& camera, Renderer& renderer, Model& firTree, Model& pineTree, Model& bush, Model& hazelnutTree, Model& stone, 
	std::vector<std::pair<std::pair<float, float>, TypeOfNatureObject>> objectsWithRandomPositons, GLFWwindow* window)
{
	for (auto& pair : objectsWithRandomPositons) {
		if (camera.GetPosition().x - 120.0f < pair.first.first && camera.GetPosition().x + 120.0f > pair.first.first)
		{
			glm::mat4 model = glm::mat4(0.7f);
			if (pair.second == TypeOfNatureObject::FIRTREE) {
				//firtree
				model = glm::translate(model, glm::vec3(pair.first.first, -5.45f, pair.first.second));
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));

				shader.Bind();
				shader.SetUniformMat4f("model", model);

				firTree.Draw(camera, shader, renderer);
			}
			else if (pair.second == TypeOfNatureObject::PINETREE) {
				//pinetree
				model = glm::mat4(0.7f);
				model = glm::translate(model, glm::vec3(pair.first.first, -5.45f, pair.first.second));
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));

				shader.Bind();
				shader.SetUniformMat4f("model", model);

				pineTree.Draw(camera, shader, renderer);
			}
			else if (pair.second == TypeOfNatureObject::BUSH) {
				//bush
				model = glm::mat4(0.7f);
				model = glm::translate(model, glm::vec3(pair.first.first, -5.45f, pair.first.second));
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));

				shader.Bind();
				shader.SetUniformMat4f("model", model);

				bush.Draw(camera, shader, renderer);
			} 
			else if (pair.second == TypeOfNatureObject::HAZELNUTTREE) {
				//hazelnut
				model = glm::mat4(0.7f);
				model = glm::translate(model, glm::vec3(pair.first.first, -5.45f, pair.first.second));
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

				shader.Bind();
				shader.SetUniformMat4f("model", model);

				hazelnutTree.Draw(camera, shader, renderer);
			}
			else if (pair.second == TypeOfNatureObject::STONE) {
				//stone
				model = glm::mat4(0.7f);
				model = glm::translate(model, glm::vec3(pair.first.first, -6.45f, pair.first.second));
				model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));

				shader.Bind();
				shader.SetUniformMat4f("model", model);

				stone.Draw(camera, shader, renderer);
			}
		}
	}
}

int main(void)
{
	srand(time(NULL));

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Train simulator", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create window!\n";
		glfwTerminate();
		return -1;
	}


	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	glewInit();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	Shader shadowMapShader("Resources/Shaders/ShadowMapping.shader"); //shaders used for shadows
	Shader shadowMapDepthShader("Resources/Shaders/ShadowMappingDepth.shader");
	Shader basicShader("Resources/Shaders/Basic.shader");

	Texture trainTexture("Resources/Textures/train_texture.png");
	Texture stationTexture("Resources/Textures/station_texture.png");
	Texture brasovSignTexture("Resources/Textures/brasov_sign_texture.png");
	Texture sinaiaSignTexture("Resources/Textures/sinaia_sign_texture.png");
	Texture ploiestiSignTexture("Resources/Textures/ploiesti_sign_texture.png");
	Texture bucurestiSignTexture("Resources/Textures/bucuresti_sign_texture.png");
	Texture stationPlatformTexture("Resources/Textures/platform_texture.png");
	Texture railwayTexture("Resources/Textures/railway_texture.png");
	Texture firTreeLeafsTexture("Resources/Textures/firBranch.png");
	//Texture firTreeBarkTexture("Resources/Textures/fir.jpg");
	Texture pineTreeLeafsTexture("Resources/Textures/test4.png");
	Texture bushLeafsTexture("Resources/Textures/test.png");
	Texture hazleNutLeaftsTexture("Resources/Textures/test2.png");
	Texture stoneTexture("Resources/Textures/stone.png");
	Texture groundTexture("Resources/Textures/ground2.jpg");
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, (SCR_WIDTH / 2), (SCR_HEIGHT / 2));

	Renderer renderer;

	glEnable(GL_DEPTH_TEST);

	SkyBox skybox;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::vector<float> trainVertices;
	std::vector<unsigned int> trainIndices;
	ObjectLoader::LoadObject("Resources/Models/train.obj", trainVertices, trainIndices);

	std::vector<float> stationVertices;
	std::vector<unsigned int> stationIndices;
	ObjectLoader::LoadObject("Resources/Models/station.obj", stationVertices, stationIndices);

	std::vector<float> firVertices;
	std::vector<unsigned int> firIndices;
	ObjectLoader::LoadObject("Resources/Models/fir.obj", firVertices, firIndices);

	std::vector<float> pineVertices;
	std::vector<unsigned int> pineIndices;
	ObjectLoader::LoadObject("Resources/Models/pineTree.obj", pineVertices, pineIndices);

	std::vector<float> bushVertices;
	std::vector<unsigned int> bushIndices;
	ObjectLoader::LoadObject("Resources/Models/bush.obj", bushVertices, bushIndices);

	std::vector<float> hazelnutVertices;
	std::vector<unsigned int> hazelnutIndices;
	ObjectLoader::LoadObject("Resources/Models/hazelnut.obj", hazelnutVertices, hazelnutIndices);

	std::vector<float> stoneVertices;
	std::vector<unsigned int> stoneIndices;
	ObjectLoader::LoadObject("Resources/Models/stone.obj", stoneVertices, stoneIndices);


	//Models
	Model train(trainVertices, trainIndices, trainTexture);
	Model station(stationVertices, stationIndices, stationTexture);
	Model brasovSign = StationSignModel(brasovSignTexture);
	Model sinaiaSign = StationSignModel(sinaiaSignTexture);
	Model ploiestiSign = StationSignModel(ploiestiSignTexture);
	Model bucurestiSign = StationSignModel(bucurestiSignTexture);
	Model stationPlatform = StationPlatformModel(stationPlatformTexture);
	Model railway = RailwayModel(railwayTexture);
	Model firTree(firVertices, firIndices, firTreeLeafsTexture);
	Model pineTree(pineVertices, pineIndices, pineTreeLeafsTexture);
	Model bush(bushVertices, bushIndices, bushLeafsTexture);
	Model hazelnutTree(hazelnutVertices, hazelnutIndices, hazleNutLeaftsTexture);
	Model stone(stoneVertices, stoneIndices, stoneTexture);
	Model terrain = TerrainModel(groundTexture);

	//Generate random objects with random positions
	auto objectsWithRandomPositons = GenerateObjectsWithRandomPositions();


	float ambientIntensity = 0.8f;

	const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shadowMapShader.Bind();
	shadowMapShader.SetUniform1i("diffuseTexture", 0);
	shadowMapShader.SetUniform1i("shadowMap", 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);
	glEnable(GL_FRAMEBUFFER_SRGB);

	while (!glfwWindowShouldClose(window))
	{
		renderer.Clear();

		float current_frame = static_cast<float>(glfwGetTime());
		deltaTime = current_frame - lastFrame;
		lastFrame = current_frame;


		shadowMapShader.Bind();
		shadowMapShader.SetUniform1f("u_AmbientIntensity", ambientIntensity);

		processInput(window);

		glDepthFunc(GL_LEQUAL);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = -100.0f, far_plane = 200.5f;
		lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos + 20.f, lightPos - 20.f, glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;


		shadowMapDepthShader.Bind();
		shadowMapDepthShader.SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);

		skybox.Draw(camera.GetViewMatrix(), camera.GetProjectionMatrix());

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		RenderTrain(shadowMapDepthShader, camera, renderer, train, window);
		RenderStation(shadowMapDepthShader, camera, renderer, station, brasovSign, stationPlatform, window, EStation::BRASOV);
		RenderStation(shadowMapDepthShader, camera, renderer, station, sinaiaSign, stationPlatform, window, EStation::SINAIA);
		RenderStation(shadowMapDepthShader, camera, renderer, station, ploiestiSign, stationPlatform, window, EStation::PLOIESTI);
		RenderStation(shadowMapDepthShader, camera, renderer, station, bucurestiSign, stationPlatform, window, EStation::BUCURESTI);
		RenderNatureObjects(shadowMapDepthShader, camera, renderer, firTree, pineTree, bush, hazelnutTree, stone, objectsWithRandomPositons, window);

		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/

		shadowMapShader.Bind();
		shadowMapShader.SetUniformMat4f("projection", camera.GetProjectionMatrix());
		shadowMapShader.SetUniformMat4f("view", camera.GetViewMatrix());

		shadowMapShader.SetUniform3f("viewPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		shadowMapShader.SetUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
		shadowMapShader.SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);

		basicShader.Bind();
		basicShader.SetUniformMat4f("projection", camera.GetProjectionMatrix());
		basicShader.SetUniformMat4f("view", camera.GetViewMatrix());

		/* Render here */

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glDisable(GL_CULL_FACE);

		RenderTrain(shadowMapShader, camera, renderer, train, window);
		RenderTerrain(shadowMapShader, camera, renderer, terrain, window);
		RenderStation(shadowMapShader, camera, renderer, station, brasovSign, stationPlatform, window, EStation::BRASOV);
		RenderStation(shadowMapShader, camera, renderer, station, sinaiaSign, stationPlatform, window, EStation::SINAIA);
		RenderStation(shadowMapShader, camera, renderer, station, ploiestiSign, stationPlatform, window, EStation::PLOIESTI);
		RenderStation(shadowMapShader, camera, renderer, station, bucurestiSign, stationPlatform, window, EStation::BUCURESTI);
		RenderRailway(shadowMapShader, camera, renderer, railway, window);	
		RenderNatureObjects(basicShader, camera, renderer, firTree, pineTree, bush, hazelnutTree, stone, objectsWithRandomPositons, window);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, (float)deltaTime);

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, (float)deltaTime);

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		camera.ChangeXPosition(LEFT, (float)trainSpeed);
		trainPosition.x -= (float)trainSpeed;
		lightPos.x = camera.GetPosition().x + 100.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		camera.ChangeXPosition(RIGHT, (float)trainSpeed);
		trainPosition.x += (float)trainSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		camera.Reset(width, height);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	camera.Reshape(width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.MouseControl((float)xpos, (float)ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yOffset)
{
	camera.ProcessMouseScroll((float)yOffset);
}
