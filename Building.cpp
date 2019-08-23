/*
File: Building.cpp
Author: Thomas Hildebrand
Project: UCSD CSE 167 - Computer Graphics
		 Final Project - Flight Simulator
Description: Subclass of transform class. Handles functionality for construction of building object.
*/

#include "Building.h"

/*
Function: Building::Building(const char * filename, glm::vec3* scaleSize, int height) : Transform()
Description: Constructor for Building object.  
Parameters:
	filename - string representing filename for building's shape
	scaleSize - scale factors for the x, y, z directions
	height - height of the building
Return: NONE
*/
Building::Building(const char * filename, glm::vec3* scaleSize, int height) : Transform() {

	parse(filename, scaleSize, height);
	loadBuffers();
	loadTexture();
}

/*
Function: Building::~Building()
Description: Destructor for Building object.
Parameters: NONE
Return: NONE
*/
Building::~Building() {
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

/*
Function: void Building::draw(Camera* C, GLuint shader)
Description: Sends updated view, projection, and model matrix information to the corresponding shader for rendering.
Parameters:
	C - Camera object, use's camera's view and projection matrices for rendering.
	shader - Id of corresponding shader program used to render current building object.
Return: NONE
*/
void Building::draw(Camera* C, GLuint shader) {
	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	uProjection = glGetUniformLocation(shader, "projection");
	uModelView = glGetUniformLocation(shader, "modelview");

	glm::mat4 view = C->GetView();
	glm::mat4 projection = C->GetProjection();

	// Now send these values to the shader program
	glm::mat4 mv = view * GetToWorld();
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(uModelView, 1, GL_FALSE, &mv[0][0]);

	glUniform3f(glGetUniformLocation(shader, "camPos"), C->GetPosition().x, C->GetPosition().y, C->GetPosition().z);
	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

/*
Function: unsigned char* Building::loadPPM(const char* filename, int& width, int& height)
Description: Load a ppm file from disk.
Parameters:
	filename - PPM file.  If the file is not found, an error message will be printed and this function will return 0.
	width - This will be modified to contain the width of the loaded image, or 0 if file not found
	height - This will be modified to contain the height of the loaded image, or 0 if file not found
Return: Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
*/
unsigned char* Building::loadPPM(const char* filename, int& width, int& height)
{
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	if ((fp = fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}

	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	// Read width and height:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	return rawData;
}

/*
Function: void Building::loadTexture()
Description: Load image file into texture object
Parameters: NONE
Return: NONE
*/
void Building::loadTexture()
{
	int twidth, theight;   // texture width/height [pixels]
	unsigned char* tdata;  // texture pixel data

	int index = rand() % skyscraper_textures.size();
	texfile = skyscraper_textures[index];
	// Load image file
	tdata = loadPPM(texfile, twidth, theight);
	if (tdata == NULL) {
		std::cout << "tdata is NULL" << std::endl;
		return;
	}
	
	// Create ID for texture1
	glGenTextures(1, &texture[0]);
	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

/*
Function: void Building::loadTexture(const char * tex)
Description: Load specific image file into texture object
Parameters: 
	tex - texture to use when loading image into texture object
Return: NONE
*/
void Building::loadTexture(const char * tex)
{
	int twidth, theight;   // texture width/height [pixels]
	unsigned char* tdata;  // texture pixel data


	// Load image file
	tdata = loadPPM(tex, twidth, theight);
	if (tdata == NULL) {
		std::cout << "tdata is NULL" << std::endl;
		return;
	}

	// Create ID for texture1
	glGenTextures(1, &texture[0]);
	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

/*
Function: glm::vec3 Building::reshapeVertex( float x, float y, float z, float height, glm::vec3* scaleSize )
Description: Scales individual vertex based on supplied scaleSize and height of building.
Parameters:
	x - x coordinate of vertex
	y - y coordinate of vertex
	z - z coordinate of vertex
	height - Desired height of the building
	scaleSize - Vector containing x, y, z, scale values.
Return: newVertex - The reshaped/scaled vertex.
*/
glm::vec3 Building::reshapeVertex( float x, float y, float z, float height, glm::vec3* scaleSize )
{

	// Apply changes
	glm::vec3 newVertex = (*scaleSize) * glm::vec3(x, y, z);

	if (y > 0)
	{
		newVertex.y += height;
	}
	
	// Return changed vertex
	return newVertex;
}

/*
Function: void Building::loadBuffers()
Description: Loads Buffers with vertice and indice information for the shader programs to use.
Parameters: NONE
Return: NONE
*/
void Building::loadBuffers()
{
	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Vertices
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);

	// Indices
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const GLvoid*)12);
	
	// Unbind 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

/*
Function: void Building::parse(const char* filename, glm::vec3* scaleSize, int height)
Description: Reads in vertice/indice information for shapes used in building construction
Parameters:
	filename - string representing filename for building's shape
	scaleSize - scale factors for the x, y, z directions
	height - height of the building
Return: NONE
*/
void Building::parse(const char* filename, glm::vec3* scaleSize, int height)
{
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	FILE* fp;     // file pointer
	float x, y, z;  // vertex coordinates
	int c1, c2;    // characters read from file

	fp = fopen(filename, "rb");  // make the file name configurable so you can load other files
	if (fp == NULL) { std::cerr << "error loading file " << filename << std::endl; exit(-1); }  // just in case the file can't be found or is corrupt

	c1 = fgetc(fp);
	c2 = fgetc(fp);

	for (int i = 0; c1 != EOF; i++) {
		c1 = fgetc(fp);
		if (c1 != 'v' && c1 != 'f')
			continue;
		c2 = fgetc(fp);

		// read from file
		if ((c1 == 'v') && (c2 == ' ')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);

			glm::vec3 newVertex = reshapeVertex(x, y, z, height, scaleSize);
			rawVertexInputs.push_back(newVertex);

		}
		else if ((c1 == 'v') && (c2 == 'n')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			rawNormalInputs.push_back(glm::vec3(x, y, z));
		}
		else if ((c1 == 'v') && (c2 == 't')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			rawTextureInputs.push_back(glm::vec3(x, y*0.2f, z));
		}
		else if ((c1 == 'f') && (c2 == ' ')) {
			unsigned int v1, v2, v3, vt1, vt2, vt3; // indices for face values
			unsigned int vn1, vn2, vn3; // throwaway values
			fscanf(fp, "%u/%u/%u %u/%u/%u %u/%u/%u", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);

			vertexIndices.push_back(v1 - 1);
			vertexIndices.push_back(v2 - 1);
			vertexIndices.push_back(v3 - 1);

			textureIndices.push_back(vt1 - 1);
			textureIndices.push_back(vt2 - 1);
			textureIndices.push_back(vt3 - 1);
		}
	}
	int size = (int)vertexIndices.size();
	for (int i = 0; i < size; i++) {
		vertices.push_back(rawVertexInputs[vertexIndices[i]]);
		vertices.push_back(rawTextureInputs[textureIndices[i]]);
		indices.push_back(i);

	}
}
