#version 410 core

const int NUM_BONES = 100;
const int MAX_WEIGHTS = 4;

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord1;
layout (location = 3) in vec2 vTexCoord2;
layout (location = 4) in vec2 vTexCoord3;
layout (location = 5) in vec4 vBlendIndices;
layout (location = 6) in vec4 vBlendWeights;

uniform mat4 uMVP;
uniform mat4 uMV;
uniform mat3 uN;
uniform mat4 uBones[NUM_BONES];
uniform int uAnimate;

out vec2 oTexCoord;
out vec3 oPosition;
out vec3 oNormal;

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
		lBlendPosition = vec4(vPosition, 1.0f);
		lBlendNormal = vNormal;			
	}

	oPosition = vec3(uMV * lBlendPosition);
	oNormal = normalize(uN * lBlendNormal);
	oTexCoord = vTexCoord1;

	gl_Position = uMVP * lBlendPosition;
}