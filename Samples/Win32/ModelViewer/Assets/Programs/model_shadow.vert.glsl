#version 410 core

const int NUM_BONES = 128;
const int MAX_WEIGHTS = 4;

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord1;
layout(location = 3) in vec2 vTexCoord2;
layout(location = 4) in vec2 vTexCoord3;
layout(location = 5) in vec4 vBlendIndices;
layout(location = 6) in vec4 vBlendWeights;

layout(std140) uniform PerFrameUniformBlock
{
	mat4 uWorldMatrix;
	mat4 uWorldViewMatrix;
	mat4 uWorldViewProjMatrix;
	mat4 uViewMatrix;
	mat4 uProjMatrix;
};

uniform mat4 uBones[NUM_BONES];
uniform int uAnimate;

void main()
{
	vec4 lBlendPosition = vec4(0.0f);
	vec4 lPosition4 = vec4(vPosition, 1.0);
	mat4 lBoneTransform = mat4(0.0);

	if (uAnimate == 1)
	{
		for (int i = 0; i < MAX_WEIGHTS; i++)
			lBoneTransform += uBones[int(vBlendIndices[i])] * vBlendWeights[i];

		lBlendPosition = lBoneTransform * lPosition4;
	}
	else
	{
		lBlendPosition = lPosition4;
	}

	gl_Position = uWorldViewProjMatrix * lBlendPosition;
}