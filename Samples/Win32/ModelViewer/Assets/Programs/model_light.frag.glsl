#version 410 core

const int MAX_LIGHTS = 10;
const int DIRECTIONAL_LIGHT = 0;
const int POINT_LIGHT = 1;

layout (location = 0) out vec4 vFragColor;

in VS_OUT
{
	vec3 EyeCoord;
	vec3 NormalCoord;
	vec2 TexCoord;
	vec4 ShadowCoord;
} fragIn;

struct Material
{
	vec4 AmbientColor;
	vec4 DiffuseColor;
	vec4 SpecularColor;
	float SpecPower;
};

struct Light
{	
	vec3 Position;	
	float Type;
	vec4 Color;	
	float Intensity;
	float ConstantAtt;
	float LinearAtt;
	float QuadraticAtt;
};

layout(std140) uniform PerFrameUniformBlock
{
	mat4 uWorldMatrix;
	mat4 uWorldViewMatrix;
	mat4 uWorldViewProjMatrix;
	mat4 uViewMatrix;
	mat4 uProjMatrix;
};

layout(std140) uniform LightUniformBlock
{
	Light uLights[MAX_LIGHTS];
};

uniform int uPassType;
uniform int uNumLights;
uniform Material uMaterial;
uniform sampler2D uDiffuseMap;
uniform sampler2DShadow uShadowMap;

vec4 DirectionalLight(int piIdx)
{
	vec3 lLightPos = vec3(uViewMatrix * vec4(uLights[piIdx].Position, 1.0));
	vec3 lLightDir = normalize(lLightPos - fragIn.EyeCoord);
	vec3 lNormal = normalize(fragIn.NormalCoord);
	vec3 lEyeCoord = normalize(-fragIn.EyeCoord);
	vec3 lHvec = normalize(lLightDir + lEyeCoord);

	vec4 lDiffuse = uLights[piIdx].Color * max(0, dot(lNormal, lLightDir)) *
		uLights[piIdx].Intensity;

	float lfSpecular = max(0, pow(dot(lNormal, lHvec), 
		uMaterial.SpecPower));

	return lDiffuse * (texture(uDiffuseMap, fragIn.TexCoord)) +
		lfSpecular * uMaterial.SpecularColor;
}

vec4 PointLight(int piIdx)
{
	vec3 lLightPos = vec3(uViewMatrix * vec4(uLights[piIdx].Position, 1.0));
	vec3 lLightDir = normalize(lLightPos - fragIn.EyeCoord);
	vec3 lNormal = normalize(fragIn.NormalCoord);
	vec3 lEyeCoord = normalize(-fragIn.EyeCoord);
	vec3 lHvec = normalize(lLightDir + lEyeCoord);

	float lfLightLen = length(lLightDir);
	float lfAttenuation = 1.0 / (uLights[piIdx].ConstantAtt +
		uLights[piIdx].LinearAtt * lfLightLen +
		uLights[piIdx].QuadraticAtt * lfLightLen * lfLightLen);

	float lfDiffuse = max(0, dot(lNormal, lLightDir)) * 
		lfAttenuation;

	float lfSpecular = max(0, pow(dot(lNormal, lHvec), 
		uMaterial.SpecPower) * lfAttenuation);

	if (fragIn.ShadowCoord.w > 1.0f)
	{
		float lfShadow = textureProj(uShadowMap,
			fragIn.ShadowCoord);

		lfDiffuse = mix(lfDiffuse, lfDiffuse * lfShadow, 0.5f);
	}

	return (lfDiffuse * uLights[piIdx].Color * 
		texture(uDiffuseMap, fragIn.TexCoord)) +
		lfSpecular * uMaterial.SpecularColor;
}

void main()
{
	vec4 lFragColor = vec4(0);

	for (int lIdx = 0; lIdx < uNumLights; lIdx++)
	{
		int lightType = int(uLights[lIdx].Type);

		if (lightType == DIRECTIONAL_LIGHT)
			lFragColor += DirectionalLight(lIdx);
		else if (lightType == POINT_LIGHT)
			lFragColor += PointLight(lIdx);
	}

	vFragColor = lFragColor;
}