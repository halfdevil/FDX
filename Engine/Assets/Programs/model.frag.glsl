#version 410 core

layout (location = 0) out vec4 vFragColor;

in vec2 oTexCoord;
in vec3 oPosition;
in vec3 oNormal;

struct Material
{
	vec4 AmbientColor;
	vec4 DiffuseColor;
	vec4 SpecularColor;
	float SpecPower;
};

struct Light
{
	int Type;
	vec3 Position;
	vec4 Color;
};

uniform mat4 uMV;
uniform Light uLight;
uniform Material uMaterial;
uniform sampler2D uDiffuseMap;

void main()
{
	vec3 lightPos = vec3(uMV * vec4(uLight.Position, 1.0));
	vec3 lightDir = normalize(lightPos - oPosition);
	float diffuse = max(0.0, dot(oNormal, lightDir));

	vec4 scatteredLight = uLight.Color * diffuse;
	vFragColor = min((uMaterial.AmbientColor +
		texture(uDiffuseMap, oTexCoord)) * scatteredLight, vec4(1.0));
}