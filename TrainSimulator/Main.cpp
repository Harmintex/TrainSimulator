#include <iostream>
#include <GL/glew.h>
#include <glfw3.h>
#include <fstream>
#include <sstream>

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
#include "SkyBox.h"
#include "ObjectLoader.h"
#include "Model.h"

//Variables
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(9.0f, 10.0f, 33.0f));
double deltaTime = 0.0f;
double lastFrame = 0.0f;


//Functions
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yOffset);

void RenderTrain(Shader& shadowMappingShader, Camera& camera, Renderer& renderer, Model& train, GLFWwindow* window, glm::vec3& lightPos)
{
	glm::mat4 model = glm::mat4(0.7f);

	//train movement values
	double fIncrement = 0.001;
	static double fMovementValue = 0.0;
	float current_x = glm::sin(fMovementValue) * 460.0f;
	lightPos.x = current_x;

	model = glm::translate(model, glm::vec3(0.0f, -0.09f, 5.1f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));

	shadowMappingShader.Bind();
	shadowMappingShader.SetUniformMat4f("model", model);

	train.Draw(camera, shadowMappingShader, renderer);
}

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit()) {
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Train Simulator", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	//glfwSwapInterval(1);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		std::cout << "Error!" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	/*float positions[] = {
		-0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	unsigned int vao;
	GlCall(glGenVertexArrays(1, &vao));
	GlCall(glBindVertexArray(vao));

	VertexArray vertexArray;
	VertexBuffer vertexBuffer(positions, 4 * 4 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	vertexArray.AddBuffer(vertexBuffer, layout);

	IndexBuffer indexBuffer(indices, 6);*/

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader shadowMappingShader("Resources/Shaders/ShadowMapping.shader");

	Shader shadowMappingDepthShader("Resources/Shaders/ShadowMappingDepth.shader");

	Texture trainTexture("/Resources/Textures/trainTexture.png");

	Renderer renderer;

	std::vector<float> trainVertices;
	std::vector<unsigned int> trainIndices;
	ObjectLoader::LoadObject("Resources/Models/train.obj", trainVertices, trainIndices);

	Model train(trainVertices, trainIndices, trainTexture);

	/*Texture texture("Train_MainTex.psd");
	texture.Bind();
	shader.SetUniform1i("u_Texture", 0);*/

	/*vertexArray.Unbind();
	vertexBuffer.Unbind();
	indexBuffer.Unbind();
	shader.Unbind();*/

	SkyBox skyBox;
	float last_frame = 0.0f, delta_time;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetCursorPos(window, (SCR_WIDTH / 2), (SCR_HEIGHT / 2));

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

	shadowMappingShader.Bind();
	shadowMappingShader.SetUniform1i("diffuseTexture", 0);
	shadowMappingShader.SetUniform1i("shadowMap", 1);


	glm::vec3 lightPos(-10.f, 25.f, 10.0f);

	glEnable(GL_DEPTH_TEST);
	//glDepthRange(1, 1);
	//// Enables Cull Facing
	glEnable(GL_CULL_FACE);
	//// Keeps front faces
	glCullFace(GL_FRONT);
	//// Uses counter clock-wise standard
	glFrontFace(GL_CCW);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		renderer.Clear();

		glDepthFunc(GL_LEQUAL);

		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		/* Render here */

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skyBox.Draw(camera.GetViewMatrix(), camera.GetProjectionMatrix());

		//Render depth of scene to texture (from light's perspective)
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = -100.0f, far_plane = 200.5f;
		lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos + 20.f, lightPos - 20.f, glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;

		shadowMappingDepthShader.Bind();
		shadowMappingDepthShader.SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		RenderTrain(shadowMappingDepthShader, camera, renderer, train, window, lightPos);

		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		shadowMappingShader.Bind();
		shadowMappingShader.SetUniformMat4f("projection", camera.GetProjectionMatrix());
		shadowMappingShader.SetUniformMat4f("view", camera.GetViewMatrix());

		shadowMappingShader.SetUniform3f("viewPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		shadowMappingShader.SetUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
		shadowMappingShader.SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		/* Render here */
		
		RenderTrain(shadowMappingShader, camera, renderer, train, window, lightPos);

		glDisable(GL_CULL_FACE);
		

		glDepthFunc(GL_LESS);

		/* Swap front and back buffers */
		GlCall(glfwSwapBuffers(window));

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
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
