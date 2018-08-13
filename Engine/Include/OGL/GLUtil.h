#pragma once

#include "Graphics/Texture.h"
#include "Graphics/Program.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/RenderData.h"
#include "Graphics/RenderState.h"
#include "Graphics/DrawOperation.h"
#include "Graphics/OcclusionQuery.h"
#include "Graphics/GraphicsDevice.h"
#include <GL/gl3w.h>

#define EMPTY_GL_ERROR() { while (glGetError() != GL_NO_ERROR); }

namespace FDX
{
	class CGLUtil
	{
	public:

		static GLenum GetSurfaceFormat(ESurfaceFormat pFormat);
		static GLenum GetImageFormat(ESurfaceFormat pFormat);
		static GLenum GetImageDataType(ESurfaceFormat pFormat);
		static GLenum GetTextureWrap(ETextureWrap pWrap);
		static GLenum GetTextureCompareMode(ETextureCompareMode pCompareMode);
		static GLenum GetShaderType(EShaderType pType);
		static GLenum GetBufferUsage(EBufferUsage pUsage);
		static GLenum GetVertexElementFormat(EVertexElementFormat pFormat);
		static GLbitfield GetMapFlags(EMapFlags pFlags);
		static GLenum GetIndexType(EIndexType pType);
		static GLenum GetPrimitiveType(EPrimitiveType pType);
		static GLenum GetBlendOption(EBlendOption pOption);
		static GLenum GetBlendEquation(EBlendEquation pEquation);
		static GLenum GetCompareFunction(ECompareFunction pFunction);
		static GLenum GetStencilOption(EStencilOption pOption);
		static GLenum GetCullMode(ECullMode pMode);
		static GLenum GetFrontFace(ECullMode pMode);
		static GLenum GetFillMode(EFillMode pMode);
		static GLenum GetStencilFace(EStencilFace pFace);
		static GLenum GetQueryType(EQueryType pQueryType);
		static GLenum GetDrawBuffer(EDrawBuffer pDrawBuffer);
	};
}