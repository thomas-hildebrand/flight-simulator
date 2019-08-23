#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include "Camera.h"

//#include <stdlib.h>
//#include <math.h>   // include math functions, such as sin, cos, M_PI

using namespace std;
class Skybox {

private:

	GLuint VBO, VAO, EBO;

public:
	Skybox();
	~Skybox();
	glm::mat4 toWorld;
	GLuint cubeTexture;
	void draw(Camera*, GLuint);
	unsigned int loadCubemap(vector<const GLchar*> faces);
	unsigned char* loadPPM(const char* filename, int& width, int& height);
};

const GLfloat skyvertices[8][3] = {
	// "Front" vertices
	{-512.0, -0.0,  512.0}, {512.0, -0.0,  512.0}, {512.0,  512.0,  512.0}, {-512.0,  512.0,  512.0},
	// "Back" vertices
	{-512.0, -0.0, -512.0}, {512.0, -0.0, -512.0}, {512.0,  512.0, -512.0}, {-512.0,  512.0, -512.0}

};


const GLuint skyindices[] = {
	// Front face
	0, 1, 2, 2, 3, 0,
	// Top face
	1, 5, 6, 6, 2, 1,
	// Back face
	7, 6, 5, 5, 4, 7,
	// Bottom face
	4, 0, 3, 3, 7, 4,
	// Left face
	4, 5, 1, 1, 0, 4,
	// Right face
	3, 2, 6, 6, 7, 3
};


#endif