#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 texture;

uniform mat4 projection;
uniform mat4 modelview;

out vec2 TexCoord;

void main()
{
    gl_Position = projection * (modelview * vec4(position, 1.0f));
    vec2 tempTex = vec2(texture.x, 1-texture.y);
    TexCoord = vec2(tempTex);
}
