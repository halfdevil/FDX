#version 410 core

layout(location = 0) in vec3 vPosition;

uniform mat4 uMVP;
out vec3 oTexCoord;

void main()
{
	gl_Position = uMVP * vec4(vPosition, 1.0);
	oTexCoord = vPosition;
}
