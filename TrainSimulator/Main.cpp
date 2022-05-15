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

//Variables
Camera* camera = nullptr;
double deltaTime = 0.0f;
double lastFrame = 0.0f;
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

//Functions
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yOffset);

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit()) {
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World", NULL, NULL);
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

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        float positions[] = {
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

        IndexBuffer indexBuffer(indices, 6);

        Shader shader("Resources/Shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        /*Texture texture("Train_MainTex.psd");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);*/

        vertexArray.Unbind();
        vertexBuffer.Unbind();
        indexBuffer.Unbind();
        shader.Unbind();

        Renderer renderer;
        SkyBox skyBox;
        camera = new Camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0, 0.0, 0.0));

        glEnable(GL_DEPTH_TEST);
        glDepthRange(1, 1);
        // Enables Cull Facing
        glEnable(GL_CULL_FACE);
        // Keeps front faces
        glCullFace(GL_FRONT);
        // Uses counter clock-wise standard
        glFrontFace(GL_CCW);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {

            float currentFrame = (float)glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window);

            glm::mat4 projection = camera->GetProjectionMatrix();
            glm::mat4 view = camera->GetViewMatrix();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            /* Render here */
            renderer.Clear();

            //skyBox.Draw(projection, view);
            skyBox.Draw(view, projection);
            
            shader.Bind();

            //renderer.Draw(vertexArray, indexBuffer, shader);

            /* Swap front and back buffers */
            GlCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, (float)deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, (float)deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, (float)deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, (float)deltaTime);
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        camera->ProcessKeyboard(UP, (float)deltaTime);
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        camera->ProcessKeyboard(DOWN, (float)deltaTime);

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        camera->Reset(width, height);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    camera->Reshape(width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera->MouseControl((float)xpos, (float)ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yOffset)
{
    camera->ProcessMouseScroll((float)yOffset);
}
