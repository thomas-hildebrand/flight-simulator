//
//  Geometry.cpp
//  CSE 167 Example Code 2
//
//  Created by Petre on 10/30/18.
//  Copyright © 2018 Peter Enescu. All rights reserved.
//

#include "Geometry.hpp"
#include "Window.h"
#include "Camera.h"

Geometry::Geometry(const char * filename) : Transform() {
    init(filename, 1.0f);
}

Geometry::Geometry(const char * filename, float scaleSize) : Transform() {
    init(filename, scaleSize);
}

Geometry::Geometry(const char * filename, float scaleSize, bool isPlane) : Transform() {
    if(isPlane) init(filename, scaleSize);
    else init2(filename, scaleSize);
}

Geometry::~Geometry() {
    // Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Geometry::init(const char* filename, float scaleSize) {
    //TODO parse the OBJ file
    // Populate the face indices, vertices, and normals vectors with the OBJ Object data
    FILE* fp;     // file pointer
    float x,y,z;  // vertex coordinates
    int c1,c2;    // characters read from file
    
    fp = fopen(filename,"rb");  // make the file name configurable so you can load other files
    if (fp==NULL) { std::cerr << "error loading file " << filename << std::endl; exit(-1); }  // just in case the file can't be found or is corrupt
    
    int count = 0;
    int count2 = 0;
    
	c1 = fgetc(fp);
	c2 = fgetc(fp);

    glm::vec3 center;
    int vcount = 0;
    
    for (int i = 0; c1 != EOF; i++) {
        c1 = fgetc(fp);
        if(c1 != 'v' && c1 != 'f')
            continue;
        c2 = fgetc(fp);
        
        // read from file
        if ((c1=='v') && (c2==' ')) {
            fscanf(fp, "%f %f %f", &x, &y, &z);
            rawVertexInputs.push_back(scaleSize * glm::vec3(x, y, z));
        }
        else if ((c1=='v') && (c2=='n')) {
            fscanf(fp, "%f %f %f", &x, &y, &z);
            rawNormalInputs.push_back(glm::vec3(x, y, z));
        }
        else if ((c1=='v') && (c2=='t')) {
            fscanf(fp, "%f %f %f", &x, &y, &z);
            rawTextureInputs.push_back(glm::vec3(x, y, z));
        }
        else if((c1 == 'f') && (c2==' ')) {
            count2++;
            unsigned int v1,v2,v3,vt1,vt2,vt3; // indices for face values
            int v4 = -1, vt4 = -1;
            fscanf(fp, "%u/%u %u/%u %u/%u %u/%u", &v1,&vt1,&v2,&vt2,&v3,&vt3,&v4,&vt4);
            //std::cout << v1 << "/" << vt1 << std::endl;
            // Due to openGL starting at index 0
            
            // TRIANGLE
            if(v4 == -1 && vt4 == -1) {
                count++;
                vertexIndices.push_back(v1-1);
                vertexIndices.push_back(v2-1);
                vertexIndices.push_back(v3-1);
                
                textureIndices.push_back(vt1-1);
                textureIndices.push_back(vt2-1);
                textureIndices.push_back(vt3-1);
                
            }

            // QUAD
            else {
                count++;

                vertexIndices.push_back(v1-1);
                vertexIndices.push_back(v2-1);
                vertexIndices.push_back(v3-1);
                
                textureIndices.push_back(vt1-1);
                textureIndices.push_back(vt2-1);
                textureIndices.push_back(vt3-1);
                
                vertexIndices.push_back(v1-1);
                vertexIndices.push_back(v3-1);
                vertexIndices.push_back(v4-1);
                
                textureIndices.push_back(vt1-1);
                textureIndices.push_back(vt3-1);
                textureIndices.push_back(vt4-1);
            }
        }
    }
    std::cout << "Count: " << count << "\nCount2: " << count2 << std::endl;
    std::cout << "vertexIndices size: " << vertexIndices.size() << std::endl;
    int size = (int)vertexIndices.size();

    for (int i = 0; i < size; i++) {
        
        vertices.push_back(rawVertexInputs[vertexIndices[i]]);
        center += vertices[vertices.size() - 1];
        vcount++;
  
        vertices.push_back(rawTextureInputs[textureIndices[i]]);
        indices.push_back(i);
    }
    
    center /= vcount;
    for (int i = 0; i < vertices.size(); i += 2)
        vertices[i] -= center;
    
    fclose(fp);
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
                          3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
                          GL_FLOAT, // What type these components are
                          GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
                          6 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
                          (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
    // TEXTURES
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const GLvoid*)12);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    // Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind the VAO now so we don't accidentally tamper with it.
    // NOTE: You must NEVER unbind the element array buffer associated with a VAO!
    glBindVertexArray(0);
    
    loadTexture();
}

void Geometry::init2(const char* filename, float scaleSize) {
    // Populate the face indices, vertices, and normals vectors with the OBJ Object data
    FILE* fp;     // file pointer
    float x,y,z;  // vertex coordinates
    int c1,c2;    // characters read from file
    
    fp = fopen(filename,"rb");  // make the file name configurable so you can load other files
    if (fp==NULL) { std::cerr << "error loading file " << filename << std::endl; exit(-1); }  // just in case the file can't be found or is corrupt
    
    c1 = fgetc(fp);
    c2 = fgetc(fp);
    
    for (int i = 0; c1 != EOF; i++) {
        c1 = fgetc(fp);
        if(c1 != 'v' && c1 != 'f')
            continue;
        c2 = fgetc(fp);
        
        // read from file
        if ((c1=='v') && (c2==' ')) {
            fscanf(fp, "%f %f %f", &x, &y, &z);
            rawVertexInputs.push_back(scaleSize * glm::vec3(x, y, z));
        }
        else if ((c1=='v') && (c2=='n')) {
            fscanf(fp, "%f %f %f", &x, &y, &z);
            rawNormalInputs.push_back(glm::vec3(x, y, z));
        }
        else if ((c1=='v') && (c2=='t')) {
            fscanf(fp, "%f %f %f", &x, &y, &z);
            rawTextureInputs.push_back(glm::vec3(x, y, z));
        }
        else if((c1 == 'f') && (c2==' ')) {
            unsigned int v1,v2,v3,vt1,vt2,vt3; // indices for face values
            unsigned int vn1,vn2,vn3; // throwaway values
            fscanf(fp, "%u/%u/%u %u/%u/%u %u/%u/%u", &v1,&vt1,&vn1,&v2,&vt2,&vn2,&v3,&vt3,&vn3);
            // Due to openGL starting at index 0
            
            vertexIndices.push_back(v1-1);
            vertexIndices.push_back(v2-1);
            vertexIndices.push_back(v3-1);
            
            textureIndices.push_back(vt1-1);
            textureIndices.push_back(vt2-1);
            textureIndices.push_back(vt3-1);
        }
    }
    int size = (int)vertexIndices.size();
    for (int i = 0; i < size; i++) {
        vertices.push_back(rawVertexInputs[vertexIndices[i]]);
        vertices.push_back(rawTextureInputs[textureIndices[i]]);
        indices.push_back(i);
    }
    fclose(fp);
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);

    // TEXTURES
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const GLvoid*)12);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    loadTexture();
}

void Geometry::draw(Camera* C, GLuint shader) {

	// Set this texture to be the one we are working with
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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

    // Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
    glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, 0);
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray(0);
}

/** Load a ppm file from disk.
 @input filename The location of the PPM file.  If the file is not found, an error message
 will be printed and this function will return 0
 @input width This will be modified to contain the width of the loaded image, or 0 if file not found
 @input height This will be modified to contain the height of the loaded image, or 0 if file not found
 @return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
 **/
unsigned char* Geometry::loadPPM(const char* filename, int& width, int& height)
{
    const int BUFSIZE = 128;
    FILE* fp;
    unsigned int read;
    unsigned char* rawData;
    char buf[3][BUFSIZE];
    char* retval_fgets;
    size_t retval_sscanf;
    
    if ( (fp=fopen(filename, "rb")) == NULL)
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
        retval_fgets=fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');
    retval_sscanf=sscanf(buf[0], "%s %s", buf[1], buf[2]);
    width  = atoi(buf[1]);
    height = atoi(buf[2]);
    
    // Read maxval:
    do
    {
        retval_fgets=fgets(buf[0], BUFSIZE, fp);
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

// load image file into texture object
void Geometry::loadTexture()
{
    int twidth, theight;   // texture width/height [pixels]
    unsigned char* tdata;  // texture pixel data
    
    // Load image file
    tdata = loadPPM("../plane.PPM", twidth, theight);
    if (tdata==NULL) {
        std::cout << "tdata is NULL" << std::endl;
        return;
    }
    
    // Create ID for texture
    glGenTextures(1, &texture[0]);
    
    // Set this texture to be the one we are working with
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    
    // Generate the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
    
    // Set bi-linear filtering for both minification and magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
