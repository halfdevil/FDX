#version 410 core

layout(location = 0) out vec4 vFragColor;

in VS_OUT
{
	vec2 TexCoord;
} fragIn;

uniform sampler2D uTexture;

void main()
{
	float lfDepth = texture(uTexture, fragIn.TexCoord).x;
	lfDepth = 1.0f - (1.0f - lfDepth) * 25.0;

	vFragColor = vec4(lfDepth);
}