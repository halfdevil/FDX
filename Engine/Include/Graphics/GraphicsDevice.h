#pragma once

#include "Math/Color4F.h"
#include "Graphics/GraphicsContext.h"
#include "Graphics/Texture2D.h"
#include "Graphics/Texture2DArray.h"
#include "Graphics/TextureCube.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/UniformBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/RenderData.h"
#include "Graphics/DrawOperation.h"
#include "Graphics/Program.h"
#include "Graphics/RenderState.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/OcclusionQuery.h"

namespace FDX
{
	enum EClearOptions
	{
		Color = 0x1,
		Depth = 0x2,
		Stencil = 0x4
	};

	enum class EDrawBuffer
	{
		None,
		Front,
		Back,
		Left,
		Right,
		FrontLeft,
		FrontRight,
		BackLeft,
		BackRight,
		FrontAndBack
	};

	struct FDX_EXPORT SViewport
	{
	public:

		SViewport()
			: X(0), Y(0), Width(0), Height(0)
		{
		}
		
		SViewport(uint32_t puiX, uint32_t puiY, 
			uint32_t puiWidth,
			uint32_t puiHeight)
			: X(puiX),
			Y(puiY),
			Width(puiWidth),
			Height(puiHeight)
		{
		}
		
	public:

		uint32_t X;
		uint32_t Y;
		uint32_t Width;
		uint32_t Height;
	};

	class FDX_EXPORT IGraphicsDevice
	{
	public:

		virtual ~IGraphicsDevice() = default;

		virtual IGraphicsContext* GetGraphicsContext() const = 0;
		virtual EResult Create(IGameWindow *pWindow, IGraphicsContextPtr Context) = 0;

		virtual EResult Clear(uint32_t puiClearOptions, 
			const CColor4F &pColor, 
			float pfDepth,
			int32_t piStencil) = 0;

		virtual void SetViewport(const SViewport &pViewport) = 0;
		virtual void SetDrawBuffer(EDrawBuffer pDrawBuffer) = 0;

		virtual EResult CreateTexture2D(const String &pstrName, 
			ETextureUsage pUsage, 
			CTexture2D **pTexture) = 0;

		virtual EResult CreateTexture2D(const String &pstrName,
			ETextureUsage pUsage,
			uint32_t puiWidth,
			uint32_t puiHeight,
			ESurfaceFormat pSurfaceFormat,
			CTexture2D **pTexture) = 0;

		virtual EResult CreateTextureCube(const String &pstrName,
			ETextureUsage pUsage,
			CTextureCube **pTexture) = 0;

		virtual EResult CreateTextureCube(const String &pstrName,
			ETextureUsage pUsage,
			uint32_t puiSize,
			ESurfaceFormat pSurfaceFormat,
			CTextureCube **pTexture) = 0;

		virtual EResult CreateTexture2DArray(const String &pstrName,
			ETextureUsage pUsage,
			CTexture2DArray **pTexture) = 0;

		virtual EResult CreateProgram(const String &pstrName, 
			CVertexFormat *pFormat, IProgram **pProgram) = 0;

		virtual EResult CreateVertexBuffer(const String &pstrName, EBufferUsage pUsage,
			CVertexFormat *pFormat, uint32_t puiNumVertices, CVertexBuffer **pBuffer) = 0;

		virtual EResult CreateIndexBuffer(const String &pstrName, EBufferUsage pUsage,
			EIndexType pIndexType, uint32_t puiNumIndices, CIndexBuffer **pBuffer) = 0;

		virtual EResult CreateUniformBuffer(const String &pstrName, EBufferUsage pUsage,
			uint32_t puiSize, uint32_t puiBindingIndex, CUniformBuffer **pBuffer) = 0;

		virtual EResult CreateRenderData(const String &pstrName, IRenderData **pRenderData) = 0;
		virtual EResult CreateDrawOperation(IRenderData *pRenderData, IDrawOperation **pDrawOperation) = 0;

		virtual EResult CreateOcclusionQuery(const String &pstrName, 
			IOcclusionQuery **pOcclusionQuery) = 0;

		virtual EResult CreateRenderTarget(const String &pstrName, uint32_t puiNumColorTargets,
			IRenderTarget **pRenderTarget) = 0;

		virtual EResult CreateBlendState(const SBlendStateProperties &pProperties, 
			IBlendState **pState) = 0;

		virtual EResult CreateRasterizerState(const SRasterizerStateProperties &pProperties,
			IRasterizerState **pState) = 0;

		virtual EResult CreateDepthState(const SDepthStateProperties &pProperties,
			IDepthState **pState) = 0;

		virtual EResult CreateStencilState(const SStencilStateProperties &pProperties,
			IStencilState **pState) = 0;
	};

	using IGraphicsDevicePtr = SmartPtr<IGraphicsDevice>;
}