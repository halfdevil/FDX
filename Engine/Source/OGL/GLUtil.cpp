
#include "OGL/GLUtil.h"

namespace FDX
{
	GLenum CGLUtil::GetSurfaceFormat(ESurfaceFormat pFormat)
	{
		switch (pFormat)
		{
		case ESurfaceFormat::BGR8:
			return GL_RGB;

		case ESurfaceFormat::BGRA8:
			return GL_RGBA;

		case ESurfaceFormat::D24:
			return GL_DEPTH_COMPONENT24;

		case ESurfaceFormat::D24S8:
			return GL_DEPTH24_STENCIL8;

		case ESurfaceFormat::R32F:
			return GL_R32F;
		}

		return 0;
	}

	GLenum CGLUtil::GetImageFormat(ESurfaceFormat pFormat)
	{
		switch (pFormat)
		{
		case ESurfaceFormat::BGR8:
			return GL_BGR;

		case ESurfaceFormat::BGRA8:
			return GL_BGRA;

		case ESurfaceFormat::D24:
			return GL_DEPTH_COMPONENT;

		case ESurfaceFormat::D24S8:
			return GL_DEPTH_STENCIL;
		}

		return 0;
	}

	GLenum CGLUtil::GetImageDataType(ESurfaceFormat pFormat)
	{
		switch (pFormat)
		{
		case ESurfaceFormat::BGR8:
		case ESurfaceFormat::BGRA8:
			return GL_UNSIGNED_BYTE;

		case ESurfaceFormat::D24:
		case ESurfaceFormat::D24S8:
			return GL_FLOAT;
		}

		return 0;
	}

	GLenum CGLUtil::GetTextureWrap(ETextureWrap pWrap)
	{
		switch (pWrap)
		{
		case ETextureWrap::Clamp:
			return GL_CLAMP_TO_EDGE;

		case ETextureWrap::ClampToEdge:
			return GL_CLAMP_TO_EDGE;

		case ETextureWrap::ClamToBorder:
			return GL_CLAMP_TO_BORDER;

		case ETextureWrap::Repeat:
			return GL_REPEAT;
		}

		return 0;
	}

	GLenum CGLUtil::GetTextureCompareMode(ETextureCompareMode pCompareMode)
	{
		switch (pCompareMode)
		{
		case ETextureCompareMode::None:
			return GL_NONE;

		case ETextureCompareMode::RefToTexture:
			return GL_COMPARE_REF_TO_TEXTURE;
		}

		return 0;
	}

	GLenum CGLUtil::GetShaderType(EShaderType pType)
	{
		switch (pType)
		{
		case EShaderType::Vertex:
			return GL_VERTEX_SHADER;

		case EShaderType::TessellationEvaluation:
			return GL_TESS_EVALUATION_SHADER;

		case EShaderType::TessellationControl:
			return GL_TESS_CONTROL_SHADER;

		case EShaderType::Geometry:
			return GL_GEOMETRY_SHADER;

		case EShaderType::Fragment:
			return GL_FRAGMENT_SHADER;
		}

		return 0;
	}

	GLenum CGLUtil::GetBufferUsage(EBufferUsage pUsage)
	{
		switch (pUsage)
		{
		case EBufferUsage::Static:
			return GL_STATIC_DRAW;

		case EBufferUsage::Dynamic:
			return GL_DYNAMIC_DRAW;
		}

		return 0;
	}

	GLenum CGLUtil::GetVertexElementFormat(EVertexElementFormat pFormat)
	{
		switch (pFormat)
		{
		case EVertexElementFormat::Float1:
		case EVertexElementFormat::Float2:
		case EVertexElementFormat::Float3:
		case EVertexElementFormat::Float4:
		case EVertexElementFormat::HalfFloat2:
		case EVertexElementFormat::HalfFloat4:
			return GL_FLOAT;

		case EVertexElementFormat::Rgba64:
		case EVertexElementFormat::Rgba32:
		case EVertexElementFormat::Short2:
		case EVertexElementFormat::Short4:
			return GL_SHORT;

		case EVertexElementFormat::Color:
		case EVertexElementFormat::Byte4:
			return GL_UNSIGNED_BYTE;
		}

		return 0;
	}

	GLbitfield CGLUtil::GetMapFlags(EMapFlags pFlags)
	{
		switch (pFlags)
		{
		case EMapFlags::Read:
			return GL_READ_ONLY;

		case EMapFlags::Write:
			return GL_WRITE_ONLY;

		case EMapFlags::ReadWrite:
			return GL_READ_WRITE;
		}

		return 0;
	}

	GLenum CGLUtil::GetIndexType(EIndexType pType)
	{
		switch (pType)
		{
		case EIndexType::SixteenBit:
			return GL_UNSIGNED_SHORT;

		case EIndexType::ThirtyTwoBit:
			return GL_UNSIGNED_INT;
		}

		return 0;
	}

	GLenum CGLUtil::GetPrimitiveType(EPrimitiveType pType)
	{
		switch (pType)
		{
		case EPrimitiveType::PointList:
			return GL_POINTS;

		case EPrimitiveType::LineList:
			return GL_LINES;

		case EPrimitiveType::LineStrip:
			return GL_LINE_STRIP;

		case EPrimitiveType::TriangleList:
			return GL_TRIANGLES;

		case EPrimitiveType::TriangleStrip:
			return GL_TRIANGLE_STRIP;

		case EPrimitiveType::TriangleFan:
			return GL_TRIANGLE_FAN;

		case EPrimitiveType::LineListAdjacency:
			return GL_LINES_ADJACENCY;

		case EPrimitiveType::LineStripAdjacency:
			return GL_LINE_STRIP_ADJACENCY;

		case EPrimitiveType::TriangleListAdjacency:
			return GL_TRIANGLES_ADJACENCY;

		case EPrimitiveType::TriangleStripAdjacency:
			return GL_TRIANGLE_STRIP_ADJACENCY;

		case EPrimitiveType::Patches:
			return GL_PATCHES;
		}

		return 0;
	}

	GLenum CGLUtil::GetBlendOption(EBlendOption pOption)
	{
		switch (pOption)
		{
		case EBlendOption::Zero:
			return GL_ZERO;

		case EBlendOption::One:
			return GL_ONE;

		case EBlendOption::SrcColor:
			return GL_SRC_COLOR;

		case EBlendOption::OneMinusSrcColor:
			return GL_ONE_MINUS_SRC_COLOR;

		case EBlendOption::DestColor:
			return GL_DST_COLOR;

		case EBlendOption::OneMinusDestColor:
			return GL_ONE_MINUS_DST_COLOR;

		case EBlendOption::SrcAlpha:
			return GL_SRC_ALPHA;

		case EBlendOption::OneMinusSrcAlpha:
			return GL_ONE_MINUS_SRC_ALPHA;

		case EBlendOption::DestAlpha:
			return GL_DST_ALPHA;

		case EBlendOption::OneMinusDestAlpha:
			return GL_ONE_MINUS_DST_ALPHA;

		case EBlendOption::SrcAlphaSaturate:
			return GL_SRC_ALPHA_SATURATE;
		}

		return 0;
	}

	GLenum CGLUtil::GetBlendEquation(EBlendEquation pEquation)
	{
		switch (pEquation)
		{
		case EBlendEquation::Add:
			return GL_FUNC_ADD;

		case EBlendEquation::Subtract:
			return GL_FUNC_SUBTRACT;

		case EBlendEquation::RevSubtract:
			return GL_FUNC_REVERSE_SUBTRACT;
		}

		return 0;
	}

	GLenum CGLUtil::GetCompareFunction(ECompareFunction pFunction)
	{
		switch (pFunction)
		{
		case ECompareFunction::Never:
			return GL_NEVER;

		case ECompareFunction::Less:
			return GL_LESS;

		case ECompareFunction::LessOrEqual:
			return GL_LEQUAL;

		case ECompareFunction::Greater:
			return GL_GREATER;

		case ECompareFunction::GreaterOrEqual:
			return GL_GEQUAL;

		case ECompareFunction::Equal:
			return GL_EQUAL;

		case ECompareFunction::NotEqual:
			return GL_NOTEQUAL;

		case ECompareFunction::Always:
			return GL_ALWAYS;
		}

		return 0;
	}

	GLenum CGLUtil::GetStencilOption(EStencilOption pOption)
	{
		switch (pOption)
		{
		case EStencilOption::Keep:
			return GL_KEEP;

		case EStencilOption::Zero:
			return GL_ZERO;

		case EStencilOption::Replace:
			return GL_REPLACE;

		case EStencilOption::Increment:
			return GL_INCR;

		case EStencilOption::Decrement:
			return GL_DECR;

		case EStencilOption::Invert:
			return GL_INVERT;

		case EStencilOption::IncrementWrap:
			return GL_INCR_WRAP;

		case EStencilOption::DecrementWrap:
			return GL_DECR_WRAP;
		}

		return 0;
	}

	GLenum CGLUtil::GetCullMode(ECullMode pMode)
	{
		switch (pMode)
		{
		case ECullMode::Clockwise:
			return GL_CW;

		case ECullMode::CounterClockwise:
			return GL_CCW;

		case ECullMode::None:
			return GL_NONE;
		}

		return 0;
	}

	GLenum CGLUtil::GetFrontFace(ECullMode pMode)
	{
		switch (pMode)
		{
		case ECullMode::Clockwise:
			return GL_CCW;

		case ECullMode::CounterClockwise:
			return GL_CW;

		case ECullMode::None:
			return GL_NONE;
		}

		return 0;
	}

	GLenum CGLUtil::GetFillMode(EFillMode pMode)
	{
		switch (pMode)
		{
		case EFillMode::Point:
			return GL_POINT;

		case EFillMode::Solid:
			return GL_FILL;

		case EFillMode::Wireframe:
			return GL_LINE;
		}

		return 0;
	}

	GLenum CGLUtil::GetStencilFace(EStencilFace pFace)
	{
		switch (pFace)
		{
		case EStencilFace::Front:
			return GL_FRONT;

		case EStencilFace::Back:
			return GL_BACK;

		case EStencilFace::FrontAndBack:
			return GL_FRONT_AND_BACK;
		}

		return 0;
	}

	GLenum CGLUtil::GetQueryType(EQueryType pQueryType)
	{
		switch (pQueryType)
		{
		case EQueryType::SamplesPassed:
			return GL_SAMPLES_PASSED;
		}

		return 0;
	}

	GLenum CGLUtil::GetDrawBuffer(EDrawBuffer pDrawBuffer)
	{
		switch (pDrawBuffer)
		{
		case EDrawBuffer::None:
			return GL_NONE;

		case EDrawBuffer::Front:
			return GL_FRONT;

		case EDrawBuffer::Back:
			return GL_BACK;

		case EDrawBuffer::Left:
			return GL_LEFT;

		case EDrawBuffer::Right:
			return GL_RIGHT;

		case EDrawBuffer::FrontLeft:
			return GL_FRONT_LEFT;

		case EDrawBuffer::FrontRight:
			return GL_FRONT_RIGHT;

		case EDrawBuffer::BackLeft:
			return GL_BACK_LEFT;

		case EDrawBuffer::BackRight:
			return GL_BACK_RIGHT;

		case EDrawBuffer::FrontAndBack:
			return GL_FRONT_AND_BACK;
		}

		return 0;
	}
}