#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in int aTexId;
layout (location = 3) in mat4 modelPosition;

out VS_OUT{
	vec2 TexCoord;
	flat int TexId;
} vs_out;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	vs_out.TexId = aTexId;
	vs_out.TexCoord = aTexCoord;
	gl_Position = projection * view * modelPosition * vec4(aPos, 1.0);
	
}