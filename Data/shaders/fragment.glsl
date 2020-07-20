#version 330 core
out vec4 FragColor;

in VS_OUT{
	vec2 TexCoord;
	flat int TexId;
} fs_in;

// texture samplers
uniform sampler2DArray texture1;



void main() {
	//vec2 ftex = vec2(TexCoord.x, TexCoord.y);
	//-1.1 = texture quality (LOD; mipmap utilization distance)
	FragColor = texture(texture1, vec3(fs_in.TexCoord, fs_in.TexId));
}