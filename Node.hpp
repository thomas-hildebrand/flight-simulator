//
//  Node.hpp
//  CSE 167 Example Code 2
//
//  Created by Petre on 10/30/18.
//  Copyright © 2018 Peter Enescu. All rights reserved.
//

#ifndef Node_hpp
#define Node_hpp

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif



#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <float.h>
#include <math.h>   // include math functions, such as sin, cos, M_PI
#include "shader.h"

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "shader.vert"
#define FRAGMENT_SHADER_PATH "shader.frag"

class Node {
public:
    virtual void draw(glm::mat4) = 0;
    virtual bool getDraw() = 0;
    virtual void update() = 0;
    void doNothing();
    virtual void toggleVisibility(bool) = 0;
    
protected:
    GLint shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
};

#endif
