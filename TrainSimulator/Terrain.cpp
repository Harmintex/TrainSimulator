#include "Terrain.h"
#include <GL/glew.h>
#include <stb_image.h>

Terrain::Terrain()
{
	float vertices[] =
	{
		-20.0f, -0.0f,  1000.0f,//        7--------6
		 20.0f, -0.0f,  1000.0f,//       /|       /|
		 20.0f, -0.0f, -1000.0f,//      4--------5 |
		-20.0f, -0.0f, -1000.0f,//      | |      | |
		-20.0f,  0.0f,  1000.0f,//      | 3------|-2
		 20.0f,  0.0f,  1000.0f,//      |/       |/
		 20.0f,  0.0f, -1000.0f,//      0--------1
		-20.0f,  0.0f, -1000.0f
	};

	unsigned int indices[] =
	{
		//Right
		1, 2, 6,
		6, 5, 1,
		// Left
		0, 4, 7,
		7, 3, 0,
		// Top
		4, 5, 6,
		6, 7, 4,
		// Bottom
		0, 3, 2,
		2, 1, 0,
		// Back
		0, 1, 5,
		5, 4, 0,
		// Front
		3, 7, 6,
		6, 2, 3
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

Terrain::~Terrain()
{
}

void Terrain::Draw(Texture groundTexture)
{
	glEnable(GL_TEXTURE_2D);
	glClearColor(0, 0, 0, 1);
	glClearDepth(1.0f);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	groundTexture.Bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
