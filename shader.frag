#version 330 core

uniform vec3 camPos;
uniform sampler2D texsampler;
in vec2 TexCoord;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
    vec3 col = texture(texsampler, TexCoord).rgb; // or else will complain
    //col.y = 1 - col.y;
    color = vec4(col, 1.0f);
    //color = vec4(TexCoord, 1.0f, 1.0f);
    //color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
