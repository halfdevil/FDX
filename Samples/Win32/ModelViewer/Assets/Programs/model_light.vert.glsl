#version 410 core

const int NUM_BONES = 128;
const int MAX_WEIGHTS = 4;

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord1;
layout (location = 3) in vec2 vTexCoord2;
layout (location = 4) in vec2 vTexCoord3;
layout (location = 5) in vec4 vBlendIndices;
layout (location = 6) in vec4 vBlendWeights;

layout(std140) uniform PerFrameUniformBlock
{
	mat4 uWorldMatrix;
	mat4 uWorldViewMatrix;
	mat4 uWorldViewProjMatrix;
	mat4 uViewMatrix;
	mat4 uProjMatrix;	
};

uniform mat3 uNormalMatrix;
uniform mat4 uShadowMatrix;
uniform mat4 uBones[NUM_BONES];
uniform int uAnimate;

out VS_OUT
{
	vec3 EyeCoord;
	vec3 NormalCoord;
	vec2 TexCoord;
	vec4 ShadowCoord;
} vertOut;

void main()
{	
	vec4 lBlendPosition = vec4(0.0f);
	vec3 lBlendNormal = vec3(0.0f);
	vec4 lPosition4 = vec4(vPosition, 1.0); 
	vec4 lNormal4 = vec4(vNormal, 0.0);
	mat4 lBoneTransform = mat4(0.0);

	if (uAnimate == 1)
	{
		for (int i = 0; i < MAX_WEIGHTS; i++)		
			lBoneTransform += uBones[int(vBlendIndices[i])] * vBlendWeights[i];

		lBlendPosition = lBoneTransform * lPosition4;
		lBlendNormal = vec3(lBoneTransform * lNormal4);
	}
	else 
	{
		lBlendPosition = lPosition4;
		lBlendNormal = vNormal;			
	}

	vec4 lWorldPos = uWorldMatrix * lBlendPosition;
	vec4 lEyePos = uViewMatrix * lWorldPos;
	vec4 lClipPos = uProjMatrix * lEyePos;

	vertOut.EyeCoord = vec3(lEyePos);
	vertOut.NormalCoord = normalize(uNormalMatrix * lBlendNormal);
	vertOut.ShadowCoord = uShadowMatrix * lWorldPos;
	vertOut.TexCoord = vTexCoord1;

	gl_Position = lClipPos;
}