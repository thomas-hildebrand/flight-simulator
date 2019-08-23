#pragma once

// Std. Includes
#include <vector>

// GL Includes
#define GLEW_STATIC
//#include <GL/glew.h>

#include <glm/glm.hpp>

#include "Transform.hpp"

class Camera : public Transform
{
public:
    Camera() : Transform(), aspect(1.0f), fov(70.0f) {}
    
    glm::mat4 GetView(){
        if (transformsDirty) UpdateTransforms();
        return view;
    }
    glm::mat4 GetProjection(){
        if (transformsDirty) UpdateTransforms();
        return projection;
    }
    void SetAspect(float a){
        aspect = a;
        MarkDirty();
    }
    void SetFov(float f){
        fov = f;
        MarkDirty();
    }
    float GetAspect(){
        return aspect;
    }
    float GetFov(){
        return fov;
    }
    
protected:
    void UpdateTransforms(){
        Transform::UpdateTransform();
        
        glm::vec3 p = GetWorldPosition();
        glm::quat r = GetWorldRotation();
        
        glm::vec3 fwd = r * glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up = r * glm::vec3(0.0f, 1.0f, 0.0f);
        
        view = glm::lookAt(p, p + fwd, up);
        projection = glm::perspective(glm::radians(fov), aspect, .1f, 1000.0f);
    }
    
private:
    float aspect;
    float fov;
    glm::mat4 view;
    glm::mat4 projection;
};
