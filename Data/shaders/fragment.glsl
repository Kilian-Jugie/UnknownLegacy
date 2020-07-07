#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;

void main() {
	//vec2 ftex = vec2(TexCoord.x, 1.0-TexCoord.y);
	//-1 = texture quality (LOD; mipmap utilization distance)
	FragColor = texture(texture1, TexCoord, -1.1);
}