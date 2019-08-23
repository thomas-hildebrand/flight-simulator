#ifndef WATER_H
#define WATER_H

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>   
#include <iostream> 
#include "Camera.h"

using namespace std;


class Water {

private:
	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector<glm::vec2> textures;
	vector<unsigned int> indices;
	GLuint VAO, VBO, EBO, VBO2, VBO3;
	void setUpWater();
	GLuint skytexture;

public:
	glm::mat4 toWorld;
	GLuint dudvmap;
	GLuint sky;
	GLuint ground;
	float speed;

	unsigned char* loadPPM(const char* filename, int& width, int& height);
	unsigned int loadCubemap(vector<const GLchar*> faces);
	Water();
	~Water();
	void draw(Camera*,GLuint);
	GLuint loadTexture(const char*);




};



#endif