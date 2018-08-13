#version 410 core

layout(location = 0) out vec4 vFragColor;

in vec3 oTexCoord;
uniform samplerCube uCubeMap;

void main()
{
	vFragColor = texture(uCubeMap, oTexCoord);
}