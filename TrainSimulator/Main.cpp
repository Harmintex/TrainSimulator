#include <iostream>
#include <fstream>
#include <sstream>
#include<GL/glew.h>
#include<glfw3.h>
#include<GL/glut.h>
#include<glm.hpp>

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
Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 0.0f));

enum EStation
{
	NONE,
	BRASOV,
	SINAIA,
	PLOIESTI,
	BUCURESTI
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
	 -2.0f,  -0.5f, 0.0f,		0.0f, 0.0f, 	0.0f,  0.0f,  1.0f,//0
	  2.0f,  -0.5f, 0.0f,		1.0f, 0.0f, 	0.0f,  0.0f,  1.0f,//1
	  2.0f,   0.5f, 0.0f,		1.0f, 1.0f, 	0.0f,  0.0f,  1.0f,//2
	 -2.0f,   0.5f, 0.0f,		0.0f, 1.0f,  	0.0f,  0.0f,  1.0f,//3
	};

	std::vector<unsigned int> indices = {
		0,1,2,
		2,3,0
	};

	return Model(vertices, indices, texture);
}

void RenderTrain(Shader& shader, Camera& camera, Renderer& renderer, Model& train, GLFWwindow* window)
Model TerrainModel(Texture& texture)
{
	std::vector<float> vertices = {
		 150.0f,  -0.5f, -90.0f,		0.0f, 0.0f, 	0.0f,  0.0f,  1.0f,
		 180.0f,  -0.5f, -90.0f,		1.0f, 0.0f, 	0.0f,  0.0f,  1.0f,
		 180.0f,  -0.5f, -60.0f,		1.0f, 1.0f, 	0.0f,  0.0f,  1.0f,
		 150.0f,  -0.5f, -60.0f,		0.0f, 1.0f,  	0.0f,  0.0f,  1.0f
	};

	std::vector<unsigned int> indices = {
		0,1,2,
		2,3,0
	};

	return Model(vertices, indices, texture);
}

void RenderTrain(Shader& objectShader, Camera& camera, Renderer& renderer, Model& train, GLFWwindow* window, glm::vec3& lightPos)
{
	glm::mat4 model = glm::mat4(0.7f);

	model = glm::translate(model, glm::vec3(0.0f, -8.0f, -30.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));

	shader.Bind();
	shader.SetUniformMat4f("model", model);

	train.Draw(camera, shader, renderer);
}

void RenderStation(Shader& shader, Camera& camera, Renderer& renderer, Model& station, Model& stationSign, GLFWwindow* window, EStation stationName)
{

	switch (stationName)
	{
	case NONE:
		break;
	case BRASOV:
	{
		if (std::abs(camera.GetPosition().x - 0.0f) < 50.0f)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -5.0f, -40.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			shader.Bind();
			shader.SetUniformMat4f("model", model);
			station.Draw(camera, shader, renderer);

			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::translate(model2, glm::vec3(0.5f, -1.26f, -36.6f));
			model2 = glm::scale(model2, glm::vec3(0.7f, 0.7f, 0.7f));
			shader.Bind();
			shader.SetUniformMat4f("model", model2);
			stationSign.Draw(camera, shader, renderer);
		}

		break;
	}
	case SINAIA:
	{
		if (std::abs(camera.GetPosition().x - (-240.0f)) < 50.0f)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-240.0f, -5.0f, -40.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			shader.Bind();
			shader.SetUniformMat4f("model", model);
			station.Draw(camera, shader, renderer);


			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::translate(model2, glm::vec3(-239.5f, -1.26f, -36.6f));
			model2 = glm::scale(model2, glm::vec3(0.7f, 0.7f, 0.7f));
			shader.Bind();
			shader.SetUniformMat4f("model", model2);
			stationSign.Draw(camera, shader, renderer);
		}

		break;
	}
	case PLOIESTI:
	{
		if (std::abs(camera.GetPosition().x - (-480.0f)) < 50.0f)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-480.0f, -5.0f, -40.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			shader.Bind();
			shader.SetUniformMat4f("model", model);
			station.Draw(camera, shader, renderer);

			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::translate(model2, glm::vec3(-479.5f, -1.26f, -36.6f));
			model2 = glm::scale(model2, glm::vec3(0.7f, 0.7f, 0.7f));
			shader.Bind();
			shader.SetUniformMat4f("model", model2);
			stationSign.Draw(camera, shader, renderer);
		}

		break;
	}
	case BUCURESTI:
	{
		if (std::abs(camera.GetPosition().x - (-720.0f)) < 50.0f)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-720.0f, -5.0f, -40.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			shader.Bind();
			shader.SetUniformMat4f("model", model);
			station.Draw(camera, shader, renderer);

			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::translate(model2, glm::vec3(-719.5f, -1.26f, -36.6f));
			model2 = glm::scale(model2, glm::vec3(0.7f, 0.7f, 0.7f));
			shader.Bind();
			shader.SetUniformMat4f("model", model2);
			stationSign.Draw(camera, shader, renderer);
		}

		break;
	}
	default:
		break;
	}
}

void RenderTerrain(Shader& objectShader, Camera& camera, Renderer& renderer, Model& terrain, GLFWwindow* window, glm::vec3& lightPos)
{
	glm::mat4 model = glm::mat4(0.7f);
	glm::vec3 position(0, 0, 0);

	for (int i = 0; i < 30; i++)
	{
		if (camera.GetPosition().x < 150.0f - (i-5)*30.0f && camera.GetPosition().x > -120.0f - (i-5)*30.0f )
		{
			for (int j = 0; j < 4; j++)
			{
				position = { 0, 0, 30 };
				model = glm::translate(model, position);

				objectShader.Bind();
				objectShader.SetUniformMat4f("model", model);

				terrain.Draw(camera, objectShader, renderer);
			}
			position = { -30, 0, -120 };
		}
		else
		{
			position = { -30, 0, 0 };

		}
		model = glm::translate(model, position);
	}
}

int main(void)
{
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

	Texture trainTexture("Resources/Textures/train_texture.png");
	Texture stationTexture("Resources/Textures/station_texture.png");
	Texture brasovSignTexture("Resources/Textures/brasov_sign_texture.png");
	Texture sinaiaSignTexture("Resources/Textures/sinaia_sign_texture.png");
	Texture ploiestiSignTexture("Resources/Textures/ploiesti_sign_texture.png");
	Texture bucurestiSignTexture("Resources/Textures/bucuresti_sign_texture.png");

	Model terrain = TerrainModel(groundTexture);

	SkyBox skybox_scene;
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetCursorPos(window, (SCR_WIDTH / 2), (SCR_HEIGHT / 2));

	Renderer renderer;

	glEnable(GL_DEPTH_TEST);

	SkyBox skybox;
	Terrain terrain;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::vector<float> trainVertices;
	std::vector<unsigned int> trainIndices;
	ObjectLoader::LoadObject("Resources/Models/train.obj", trainVertices, trainIndices);

	std::vector<float> stationVertices;
	std::vector<unsigned int> stationIndices;
	ObjectLoader::LoadObject("Resources/Models/station.obj", stationVertices, stationIndices);

	Model train(trainVertices, trainIndices, trainTexture);
	Model station(stationVertices, stationIndices, stationTexture);
	Model brasovSign = StationSignModel(brasovSignTexture);
	Model sinaiaSign = StationSignModel(sinaiaSignTexture);
	Model ploiestiSign = StationSignModel(ploiestiSignTexture);
	Model bucurestiSign = StationSignModel(bucurestiSignTexture);

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

	glm::vec3 lightPos(-8.0f, 30.f, 15.0f);

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
		RenderTrain(shadowMapDepthShader, camera, renderer, station, window);

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

		/* Render here */

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glDisable(GL_CULL_FACE);

		RenderTrain(shadowMapShader, camera, renderer, train, window);
		RenderStation(shadowMapShader, camera, renderer, station, brasovSign, window, EStation::BRASOV);
		RenderStation(shadowMapShader, camera, renderer, station, sinaiaSign, window, EStation::SINAIA);
		RenderStation(shadowMapShader, camera, renderer, station, ploiestiSign, window, EStation::PLOIESTI);
		RenderStation(shadowMapShader, camera, renderer, station, bucurestiSign, window, EStation::BUCURESTI);
		RenderTrain(shadowMap_shader, camera, renderer, train, window, lightPos);
		RenderTerrain(shadowMap_shader, camera, renderer, terrain, window, lightPos);
		//terrain.Draw(groundTexture); //TODO: reduce frame issues

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
