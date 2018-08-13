#version 410 core

layout(location = 0) out vec4 vFragColor;

in VS_OUT
{
	vec2 TexCoord;
} fragIn;

struct Material
{
	vec4 AmbientColor;
	vec4 DiffuseColor;
	vec4 SpecularColor;
	float SpecPower;
};

uniform Material uMaterial;
uniform sampler2D uDiffuseMap;

void main()
{	
	vFragColor = texture(uDiffuseMap, fragIn.TexCoord) + 
		uMaterial.DiffuseColor;
}