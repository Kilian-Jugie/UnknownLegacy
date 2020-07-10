#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2DArray texture1;

uniform int TexId;

void main() {
	//vec2 ftex = vec2(TexCoord.x, TexCoord.y);
	//-1.1 = texture quality (LOD; mipmap utilization distance)
	FragColor = texture(texture1, vec3(TexCoord, TexId));
}