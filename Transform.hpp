//
//  Transform.hpp
//  CSE 167 Example Code 2
//
//  Created by Petre on 10/30/18.
//  Copyright © 2018 Peter Enescu. All rights reserved.
//

#ifndef Transform_hpp
#define Transform_hpp

#include <glm/mat4x4.hpp>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif
#include <vector>

#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


class Camera;

class Transform {
private:
    glm::mat4 obj2world = glm::mat4(1.0f);
    glm::mat4 world2obj = glm::mat4(1.0f);
    
    std::vector<Transform*> children;
    Transform* parent = nullptr;
    
    glm::vec3 position;
    glm::quat rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::quat worldRotation;
    glm::vec3 worldPosition;
    
protected:
    virtual void UpdateTransform();
    bool transformsDirty = true;
    
public:
    virtual void draw(Camera* camera, GLuint shader);
    
    Transform();
    void addChild(Transform *);
    void removeChild(Transform *);
    
    void MarkDirty();
    
    glm::mat4 getWorld();
    Transform* GetParent();
    void SetPosition(glm::vec3);
    glm::vec3 GetPosition();
    void SetRotation(glm::quat);
    glm::quat GetRotation();
    void SetScale(glm::vec3);
    glm::vec3 GetScale();
    
    glm::mat4 GetToWorld();
    glm::mat4 GetToObject();
    glm::quat GetWorldRotation();
    glm::vec3 GetWorldPosition();
};

#endif
