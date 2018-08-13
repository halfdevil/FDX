#pragma once

#include "Graphics/GraphicsDevice.h"

namespace FDX
{
	class CGLGraphicsDevice : public IGraphicsDevice
	{
	public:

		CGLGraphicsDevice();
		~CGLGraphicsDevice() override = default;

		IGraphicsContext* GetGraphicsContext() const override
		{
			return mContext.get();
		}

		EResult Create(IGameWindow *pWindow, IGraphicsContextPtr pContext) override;

		EResult Clear(uint32_t puiClearOptions, 
			const CColor4F &pColor, 
			float pfDepth,
			int32_t piStencil) override;

		void SetViewport(const SViewport &pViewport) override;
		void SetDrawBuffer(EDrawBuffer pDrawBuffer) override;

		EResult CreateTexture2D(const String &pstrName,
			ETextureUsage pUsage, 
			CTexture2D **pTexture) override;

		EResult CreateTexture2D(const String &pstrName,
			ETextureUsage pUsage,
			uint32_t puiWidth,
			uint32_t puiHeight,
			ESurfaceFormat pSurfaceFormat,
			CTexture2D **pTexture) override;

		EResult CreateTextureCube(const String &pstrName,
			ETextureUsage pUsage, 
			CTextureCube **pTexture) override;

		EResult CreateTextureCube(const String &pstrName,
			ETextureUsage pUsage,
			uint32_t puiSize,
			ESurfaceFormat pSurfaceFormat,
			CTextureCube **pTexture) override;

		EResult CreateTexture2DArray(const String &pstrName,
			ETextureUsage pUsage,
			CTexture2DArray **pTexture) override;

		EResult CreateProgram(const String &pstrName, 
			CVertexFormat *pFormat, IProgram **pProgram) override;

		EResult CreateVertexBuffer(const String &pstrName, EBufferUsage pUsage,
			CVertexFormat *pFormat, uint32_t puiNumVertices, CVertexBuffer **pBuffer) override;

		EResult CreateIndexBuffer(const String &pstrName, EBufferUsage pUsage,
			EIndexType pIndexType, uint32_t puiNumIndices, CIndexBuffer **pBuffer) override;

		EResult CreateUniformBuffer(const String &pstrName, EBufferUsage pUsage,
			uint32_t puiSize, uint32_t puiBindingIndex, CUniformBuffer **pBuffer) override;

		EResult CreateRenderData(const String &pstrName, IRenderData **pRenderData) override;
		EResult CreateDrawOperation(IRenderData *pRenderData, IDrawOperation **pDrawOperation) override;

		EResult CreateOcclusionQuery(const String &pstrName,
			IOcclusionQuery **pOcclusionQuery) override;

		EResult CreateRenderTarget(const String &pstrName, uint32_t puiNumColorTargets,
			IRenderTarget **pRenderTarget) override;

		EResult CreateBlendState(const SBlendStateProperties &pProperties,
			IBlendState **pState) override;

		EResult CreateRasterizerState(const SRasterizerStateProperties &pProperties,
			IRasterizerState **pState) override;

		EResult CreateDepthState(const SDepthStateProperties &pProperties,
			IDepthState **pState) override;

		EResult CreateStencilState(const SStencilStateProperties &pProperties,
			IStencilState **pState) override;

	private:

		void SetDefaults();
		void EnableDebugging();

	private:

		IGraphicsContextPtr mContext;
	};		
}