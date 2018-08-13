#version 410 core

layout (location = 0) out vec4 fragOutColor;

in TES_OUT
{
	vec2 TexCoord;
	vec3 Normal;
} fragIn;

uniform sampler2D uColorMap;
uniform sampler2D uDetailMap;
uniform sampler2D uDiffuseMap1;
uniform sampler2D uDiffuseMap2;
uniform sampler2D uDiffuseMap3;
uniform sampler2D uDiffuseMap4;

void main()
{
	vec4 lColorMap = texture(uColorMap, fragIn.TexCoord);
	vec4 lDiffuse1 = texture(uDiffuseMap1, fragIn.TexCoord);
	vec4 lDiffuse2 = texture(uDiffuseMap2, fragIn.TexCoord);
	vec4 lDiffuse3 = texture(uDiffuseMap3, fragIn.TexCoord);
	vec4 lDiffuse4 = texture(uDiffuseMap4, fragIn.TexCoord);

	fragOutColor = lDiffuse1 * lColorMap.r;
	fragOutColor = mix(fragOutColor, lDiffuse2, lColorMap.g);
	fragOutColor = mix(fragOutColor, lDiffuse3, lColorMap.b);
	fragOutColor = mix(fragOutColor, lDiffuse4, lColorMap.a);

	fragOutColor *= texture(uDetailMap, fragIn.TexCoord * 10.0);
}