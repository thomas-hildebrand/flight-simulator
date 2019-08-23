//
//  Transform.cpp
//  CSE 167 Example Code 2
//
//  Created by Petre on 10/30/18.
//  Copyright © 2018 Peter Enescu. All rights reserved.
//

#include "Transform.hpp"
#include "Camera.h"

Transform::Transform() {}

void Transform::addChild(Transform* node) {
    if (node->parent) node->parent->removeChild(node);
    children.push_back(node);
    node->parent = this;
    node->MarkDirty();
}
void Transform::removeChild(Transform* node) {
    node->parent = nullptr;
    for (int i = 0; i < children.size(); i++)
        if (children[i] == node)
            children.erase(children.begin() + i);
    node->MarkDirty();
}
Transform* Transform::GetParent(){
    return parent;
}
glm::vec3 Transform::GetPosition() {
    return position;
}
glm::quat Transform::GetRotation() {
    return rotation;
}
glm::vec3 Transform::GetScale(){
    return scale;
}
void Transform::SetPosition(glm::vec3 p) {
    position = p;
    MarkDirty();
}
void Transform::SetRotation(glm::quat r) {
    rotation = r;
    MarkDirty();
}
void Transform::SetScale(glm::vec3 s){
    scale = s;
    MarkDirty();
}
void Transform::MarkDirty(){
    transformsDirty = true;
    for (auto const& object : children)
        object->MarkDirty();
}
void Transform::UpdateTransform(){
    transformsDirty = false;
    
    glm::mat4 i(1.0f);
    obj2world = glm::translate(i, position) * glm::toMat4(rotation) * glm::scale(i, scale);
    
    if (parent) {
        obj2world = parent->GetToWorld() * obj2world;
        worldRotation = parent->GetWorldRotation() * rotation;
        worldPosition = parent->GetToWorld() * glm::vec4(position, 1.0);
    }else{
        worldPosition = position;
        worldRotation = rotation;
    }
    
    world2obj = glm::inverse(obj2world);
}

glm::mat4 Transform::GetToWorld(){
    if (transformsDirty) UpdateTransform();
    return obj2world;
}
glm::quat Transform::GetWorldRotation(){
    if (transformsDirty) UpdateTransform();
    return worldRotation;
}
glm::vec3 Transform::GetWorldPosition(){
    if (transformsDirty) UpdateTransform();
    return worldPosition;
}
glm::mat4 Transform::GetToObject(){
    if (transformsDirty) UpdateTransform();
    return world2obj;
}

void Transform::draw(Camera* camera, GLuint shader) {
    for (auto const& object : children)
        object->draw(camera, shader);
}
