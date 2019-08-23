#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

in vec3 FragNormal;
in vec3 FragPos;
in vec2 textureCoords;

uniform samplerCube skybox;
uniform vec3 viewPos;
uniform sampler2D dudvMap;
uniform sampler2D sky;
uniform sampler2D ground;
uniform float speed;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{

	vec4 distortion = texture(dudvMap, vec2(textureCoords.x + speed, textureCoords.y + speed));
	//distortion = vec4(distortion.x * 2.0 - 1.0, distortion.y * 2.0 - 1.0, distortion.z, 1.0);
	vec3 offset = vec3(distortion.x, distortion.y, distortion.z) * 0.1f;
    vec3 I = normalize(FragPos - viewPos);
    
    vec3 reflection = reflect(I, normalize(FragNormal)) + offset;
    vec3 refraction = refract(I, normalize(FragNormal), 0.75f) + offset;
	//vec4 reflection = texture(sky, textureCoords);
	//vec4 refraction = texture(ground, textureCoords);

    vec4 colorReflection = texture(skybox, reflection);
    vec4 colorRefraction = texture(skybox, refraction);
	color = mix(colorReflection,colorRefraction, 0.5f);
	


}
