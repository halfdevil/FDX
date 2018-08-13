#version 410 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec2 vTexCoord;

uniform mat4 uMVP;

out VS_OUT
{
	vec4 Color;
	vec2 TexCoord;
} vertOut;

void main()
{
   vertOut.Color = vColor;
   vertOut.TexCoord = vTexCoord;
   
   gl_Position = uMVP * vec4(vPosition, 1.0);
}