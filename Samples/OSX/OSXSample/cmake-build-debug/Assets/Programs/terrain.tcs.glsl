#version 410 core

layout (vertices = 4) out;

in VS_OUT
{
	vec2 TexCoord;
} tcsIn[];

out TCS_OUT
{
	vec2 TexCoord;
} tcsOut[];

uniform mat4 uMV;
uniform vec4 uPatchOuterLevels;

void main()
{
	if (gl_InvocationID == 0)
	{
		gl_TessLevelOuter[0] = uPatchOuterLevels.x;
		gl_TessLevelOuter[1] = uPatchOuterLevels.w;
		gl_TessLevelOuter[2] = uPatchOuterLevels.y;
		gl_TessLevelOuter[3] = uPatchOuterLevels.z;

		gl_TessLevelInner[0] = min(uPatchOuterLevels.z, uPatchOuterLevels.w);
		gl_TessLevelInner[1] = min(uPatchOuterLevels.x, uPatchOuterLevels.y);
	}	

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	tcsOut[gl_InvocationID].TexCoord = tcsIn[gl_InvocationID].TexCoord;
}