#version 410 core

layout (location = 0) out vec4 fragOutColor;

uniform vec4 uDebugColor;

void main()
{
	fragOutColor = uDebugColor;
}