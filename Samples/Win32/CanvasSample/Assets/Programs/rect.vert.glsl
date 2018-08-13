#version 410 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec2 vTexCoord;

uniform vec4 uRectSize;

out VS_OUT
{
	vec2 TexCoord;
} vertOut;

void main()
{
    float x1 = uRectSize.x;
	float y1 = uRectSize.y;
	float x2 = uRectSize.z;
	float y2 = uRectSize.w;
	float width  = x2 - x1;
	float height = y2 - y1;

    vertOut.TexCoord = vTexCoord;
	gl_Position = vec4( x1 + vPosition.x * width, y1 + vPosition.y * 
		height, vPosition.z, vPosition.w ) * 
		vec4( 2.0, -2.0, 1.0, 1.0 ) + 
		vec4( -1.0, 1.0, 0.0, 0.0 );
}