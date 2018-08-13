#version 410 core

layout (quads, fractional_odd_spacing) in;

in TCS_OUT
{
	vec2 TexCoord;
} tesIn[];

out TES_OUT
{
	vec2 TexCoord;
	vec3 Normal;
} tesOut;

uniform sampler2D uHeightMap;
uniform mat4 uMVP;
uniform float uHeightScale;
uniform float uTileScale;
uniform float uHeightOffset;
uniform float uSize;
uniform float uPatchSize;

vec4 MixVec4(in vec4 pV0, in vec4 pV1, in vec4 pV2, in vec4 pV3)
{
	vec4 lP1 = mix(pV0, pV1, gl_TessCoord.x);
	vec4 lP2 = mix(pV2, pV3, gl_TessCoord.x);

	return mix(lP2, lP1, gl_TessCoord.y);
}

vec2 MixVec2(in vec2 pV0, in vec2 pV1, in vec2 pV2, in vec2 pV3)
{
	vec2 lP1 = mix(pV0, pV1, gl_TessCoord.x);
	vec2 lP2 = mix(pV2, pV3, gl_TessCoord.x);

	return mix(lP2, lP1, gl_TessCoord.y);
}

void main()
{
	vec4 lPosition = MixVec4(gl_in[0].gl_Position, gl_in[1].gl_Position,
		gl_in[2].gl_Position, gl_in[3].gl_Position);

	vec2 lTexCoord = MixVec2(tesIn[0].TexCoord, tesIn[1].TexCoord,
		tesIn[2].TexCoord, tesIn[3].TexCoord);

	lPosition.xz *= uPatchSize * uTileScale;
	lPosition.y += texture(uHeightMap, lTexCoord).r * 
		uHeightScale + uHeightOffset;

	tesOut.TexCoord = lTexCoord;
	gl_Position = uMVP * lPosition;
}