#include "Terrain.h"
#include "Window.h"
#define WIDTH 257
#define RANGE 100.0f

Terrain::Terrain() {
	
	toWorld = glm::mat4(1.0f);
	
	for (int i = 0; i < WIDTH; i++) {
		
		for (int j = 0; j < WIDTH; j++) {
			int offset = 0;
			if(j < WIDTH/2){
				offset = 4.0f;
			}
			vertices.push_back(glm::vec3((float)j*10-(WIDTH*5), offset, (float)i*10 - (WIDTH*5)));
			normals.push_back(glm::vec3(0.0, 1.0, 0.0));
			textures.push_back(glm::vec2(j, i));

		}
		
	}
	for (int i = 0; i < WIDTH - 1; i++) {
		for (int j = 0; j < WIDTH - 1; j++) {
			int tLeft = i * WIDTH + j;
			int bLeft = (i + 1)* WIDTH + j;
			int tRight = tLeft + 1;
			int bRight = bLeft + 1;
			indices.push_back(tLeft);
			indices.push_back(bLeft);
			indices.push_back(tRight);
			indices.push_back(tRight);
			indices.push_back(bLeft);
			indices.push_back(bRight);
		}
	}

	diamondSquare(RANGE);

	setUpTerrain();
	
}


Terrain::~Terrain() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO);
}

void Terrain::setUpTerrain() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO2);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(glm::vec2), &textures[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

	texture1 = loadTerrain("../terrain1.ppm");
	texture2 = loadTerrain("../terrain2.ppm");
	texture3 = loadTerrain("../terrain3.ppm");
	texture4 = loadTerrain("../terrain4.ppm");


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);
}

unsigned char* Terrain::loadPPM(const char* filename, int& width, int& height) {
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



void Terrain::diamondSquare(float ran) {
	int steps = WIDTH-1;
	float range = ran;

	int count = 1;

	while (steps > 1) {
		int halfstep = steps / 2;
		//diamond
		for (int i = 0; i < WIDTH-1; i+= steps) {
			for (int j = 0; j < WIDTH-1; j+= steps) {
				float topL = vertices[i*WIDTH + j].y;
				float topR = vertices[i*WIDTH + (j + steps)].y;
				float botL = vertices[(i + steps)*WIDTH + j].y;
				float botR = vertices[(i + steps)*WIDTH + (j+steps)].y;
				float random = fRand(-range, range);
				float average = (topL + topR + botL + botR) / 4.0;
				vertices[(i + halfstep)*WIDTH + (j + halfstep)].y = average + random;

			}
		}

		//square
		bool evenstep = true;
		for (int i = 0; i < WIDTH; i += halfstep) {
			int start = 0;
			if (evenstep) {
				start = halfstep;
			}
			for (int j = start; j < WIDTH; j += halfstep) {
				float left = 0;
				if (j - halfstep >= 0) {
					left = vertices[(j - halfstep) + (i*WIDTH)].y;

				}
				float right = 0;
				if (j + halfstep < WIDTH) {
					right = vertices[(j + halfstep) + (i*WIDTH)].y;

				}
				float up = 0;
				float down = 0;
				if (i + halfstep < WIDTH) {
					down = vertices[j + ((i + halfstep) * WIDTH)].y;

				}
				if (i - halfstep >= 0) {
					up = vertices[j + ((i - halfstep) * WIDTH)].y;

				}
				float random = fRand(-range, range);
				float average = (up+down+left+right) / 4;
				vertices[i*WIDTH+j].y = average + random;

			}
			evenstep = !evenstep;
		}
		steps = steps / 2;
		range = range / 1.8f;

	}
}


void Terrain::draw(Camera* C, GLuint shaderProgram) {
	glm::mat4 modelview = C->GetView() * toWorld;
	glm::mat4 projection = C->GetProjection();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelview"), 1, GL_FALSE, &modelview[0][0]);

	// Now send these values to the shader program
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &toWorld[0][0]);


	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture1);
	glUniform1i(glGetUniformLocation(shaderProgram, "terrain1"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->texture2);
	glUniform1i(glGetUniformLocation(shaderProgram, "terrain2"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->texture3);
	glUniform1i(glGetUniformLocation(shaderProgram, "terrain3"), 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, this->texture4);
	glUniform1i(glGetUniformLocation(shaderProgram, "terrain4"), 3);

	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, (GLsizei) indices.size(), GL_UNSIGNED_INT, 0);
	
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}


GLuint Terrain::loadTerrain(const char* filename) {
	GLuint textureID;
	int width, height;
	unsigned char * image;
	glGenTextures(1, &textureID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	image = loadPPM(filename, width, height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}

float Terrain::fRand(float min, float max)
{
	float f = (float)rand() / RAND_MAX;
	return min + f * (max - min);
}