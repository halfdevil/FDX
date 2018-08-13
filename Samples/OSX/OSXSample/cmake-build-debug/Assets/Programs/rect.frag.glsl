#version 410 core

layout (location = 0) out vec4 vFragColor;

in VS_OUT
{
    vec2 TexCoord;
} fragIn;

subroutine vec4 DiffuseType();

uniform vec4 uColor;
uniform sampler2D uTexture;
subroutine uniform DiffuseType uDiffuseType;

subroutine(DiffuseType)
vec4 Colored()
{
	return uColor;
}

subroutine(DiffuseType)
vec4 Textured()
{
	return texture(uTexture, fragIn.TexCoord);
}

void main()
{
	vFragColor = uDiffuseType();
}