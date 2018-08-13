#version 410 core

layout (location = 0) in vec3 vPosition;

uniform float uSize;
uniform float uPatchSize;
uniform int uNumPatches;
uniform int uPatchIndex;

out VS_OUT
{
	vec2 TexCoord;
} vertOut;

vec3 CalcPosition()
{
	int lX = uPatchIndex % uNumPatches;
	int lZ = uPatchIndex / uNumPatches;

	return (vPosition + vec3(float(lX - uNumPatches / 2) + 0.5, 0.0, 
		float(lZ - uNumPatches / 2) + 0.5));
}

vec2 CalcTexCoord()
{
	int lX = uPatchIndex % uNumPatches;
	int lZ = uPatchIndex / uNumPatches;

	return (vPosition.xz + vec2(lX, lZ) + vec2(0.5)) / float(uNumPatches);
}

void main()
{
	vec3 lPosition = CalcPosition();
	vec2 lTexCoord = CalcTexCoord();

	vertOut.TexCoord = lTexCoord;
	gl_Position = vec4(lPosition, 1.0);
}