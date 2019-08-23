//
//  Geometry.hpp
//  CSE 167 Example Code 2
//
//  Created by Petre on 10/30/18.
//  Copyright © 2018 Peter Enescu. All rights reserved.
//

#ifndef Geometry_hpp
#define Geometry_hpp

#include "Transform.hpp"

#include <vector>

class Geometry : public Transform {
private:
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
    
    // These variables are needed for the shader program
    GLuint VBO, VAO, EBO;
    GLuint uProjection, uModelView;

	GLuint texture[1];

    
public:
    void draw(Camera* camera, GLuint shader);
    
    void init(const char*, float);
    void init2(const char*, float);
    
    Geometry(const char*);
    Geometry(const char*, float);
    Geometry(const char*, float, bool);
    ~Geometry();
    
    void loadTexture();
    unsigned char* loadPPM(const char*, int&, int&);
};

#endif /* Geometry_hpp */
