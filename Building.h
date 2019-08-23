/*
	File: Building.h
	Author: Thomas Hildebrand
	Project: UCSD CSE 167 - Computer Graphics
			 Final Project - Flight Simulator
	Description: Header file for Building.cpp
*/

#ifndef Building_h
#define Building_h

#include "Transform.hpp"
#include "Window.h"
#include "Camera.h"

#include <vector>
#include <math.h>


class Building : public Transform {
private:
	
	/* Costants */
	const float PI = 3.1415927;

	/* Raw data from obj */
	std::vector<glm::vec3> rawVertexInputs;
	std::vector<glm::vec3> rawNormalInputs;
	std::vector<glm::vec3> rawTextureInputs;
	
	/* Parsed f values v/vt/vn */
	std::vector<GLuint> vertexIndices;
	std::vector<GLuint> normalIndices;
	std::vector<GLuint> textureIndices;
	
	/* Aligned Data */
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> textures;
	std::vector<unsigned int> indices;

	/* Shader Variables */
	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelView;
	GLuint texture[1];

	/* Textures */
	std::vector<const char *> skyscraper_textures = { 
		"skyscraperGlass.ppm", 
		"bwglass.ppm",
		"highRise.ppm",
		"1.ppm",
		"32.ppm",
		"33.ppm",
		"34.ppm",
		"59.ppm",
		"69.ppm",
		"71.ppm",
		"72.ppm",
		"76.ppm",
		"81.ppm",
	};

	/* Private Functions */
	glm::vec3 reshapeVertex(float , float , float , float , glm::vec3*);
	void loadBuffers();
	void parse(const char * , glm::vec3 * , int );
	void loadTexture();
	unsigned char* loadPPM(const char*, int&, int&);

public:

	/* Public Variables */
	const char * texfile;

	/* Public Functions */
	Building(const char*, glm::vec3*, int);
	~Building();
	void loadTexture(const char *);
	void draw(Camera* camera, GLuint shader);

};

#endif /* Geometry_hpp */
