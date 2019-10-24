#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in mat4 modelPosition;

out vec2 TexCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	TexCoord = aTexCoord;
	gl_Position = projection * view * modelPosition * vec4(aPos, 1.0);
	
}