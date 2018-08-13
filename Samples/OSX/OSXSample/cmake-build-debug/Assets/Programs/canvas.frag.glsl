#version 410 core

layout (location = 0) out vec4 vFragColor;

in VS_OUT
{
	vec4 Color;
    vec2 TexCoord;
} fragIn;

uniform sampler2D uTexture;

void main()
{
	vFragColor = fragIn.Color + texture(uTexture, fragIn.TexCoord);
}