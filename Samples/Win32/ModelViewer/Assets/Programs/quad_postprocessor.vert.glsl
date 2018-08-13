#version 410 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexCoord;

out VS_OUT
{
	vec2 TexCoord;
} vertOut;

void main()
{
	vertOut.TexCoord = vTexCoord;
	gl_Position = vec4(vPosition, 1.0f);
}