#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;



uniform sampler2D terrain1;
uniform sampler2D terrain2;
uniform sampler2D terrain3;
uniform sampler2D terrain4;


in vec2 FragTexCoords;
in vec3 FragPos;
// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
    //color = vec4(1.0f, 1.0f, 1.0f, sampleExtraOutput);
	
	if(FragPos.y > 2.5f){
		color = texture(terrain4, FragTexCoords) * 0.5f;
		color += texture(terrain3, FragTexCoords) * 0.3f;
	}
	else if(FragPos.y > 1.7f){
		color = texture(terrain4, FragTexCoords) * 0.5f;
		color += texture(terrain3, FragTexCoords) * 0.5f;
	}
	else if(FragPos.y > 1.4f){
		color = texture(terrain4, FragTexCoords)* 0.2f;
		color += texture(terrain3, FragTexCoords)* 0.8f;
	}
	else if(FragPos.y > 1.1f){
		color = texture(terrain3, FragTexCoords);
	}
	else if(FragPos.y > 0.9f){
		color = texture(terrain3, FragTexCoords)* 0.8f;
		color += texture(terrain2, FragTexCoords)* 0.2f;
	}
	else if(FragPos.y > 0.7f){
		color = texture(terrain3, FragTexCoords)* 0.5f;
		color += texture(terrain2, FragTexCoords)* 0.5f;
	}
	else if(FragPos.y > 0.0f){
		color = texture(terrain2, FragTexCoords)* 1.0;
	}
	else if(FragPos.y > -0.5f){
		color = texture(terrain2, FragTexCoords) * 0.8f;
		color += texture(terrain1, FragTexCoords) * 0.2f;
	}
	else if(FragPos.y > -1.0f){
		color = texture(terrain2, FragTexCoords) * 0.7f;
		color += texture(terrain1, FragTexCoords) * 0.3f;
	}
	else if(FragPos.y > -1.5f){
		color = texture(terrain2, FragTexCoords) * 0.5f;
		color += texture(terrain1, FragTexCoords) * 0.5f;
	}
	else{
		color = texture(terrain2, FragTexCoords) * 0.3f;
		color += texture(terrain1, FragTexCoords) * 0.7f;
	}
}
