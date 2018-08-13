
#include <GL/gl3w.h>
#include "OGL/GLGraphicsDevice.h"
#include "OGL/GLProgram.h"
#include "OGL/GLVertexBuffer.h"
#include "OGL/GLIndexBuffer.h"
#include "OGL/GLUniformBuffer.h"
#include "OGL/GLTexture2D.h"
#include "OGL/GLTextureCube.h"
#include "OGL/GLTexture2DArray.h"
#include "OGL/GLRenderData.h"
#include "OGL/GLDrawOperation.h"
#include "OGL/GLRenderState.h"
#include "OGL/GLRenderTarget.h"
#include "OGL/GLOcclusionQuery.h"
#include "OGL/GLUtil.h"
#include "OS/GameLogger.h"
#include "OS/System.h"
#include "OS/System.h"

namespace FDX
{
	void APIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
		const GLchar* message, const void* userParam)
	{
		LOG_DEBUG("[OpenGL]: %s", message);
	}

	CGLGraphicsDevice::CGLGraphicsDevice()
		: mContext(nullptr)
	{
	}

	EResult CGLGraphicsDevice::Create(IGameWindow *pWindow, IGraphicsContextPtr pContext)
	{
		EResult lResult = EResult::Success;
		mContext = std::move(pContext);

		EnableDebugging();
		SetDefaults();

		return lResult;
	}

	EResult CGLGraphicsDevice::Clear(uint32_t puiClearOptions, const CColor4F & pColor, float pfDepth, int32_t piStencil)
	{
		FDX_ASSERT(mContext != nullptr, "Graphics context cannot be null");

		EResult lResult = EResult::Success;
		GLuint luiClearFlags = 0;
		
		if (puiClearOptions & EClearOptions::Color)
		{
			luiClearFlags |= GL_COLOR_BUFFER_BIT;
			glClearColor(pColor.R, pColor.G, pColor.B, pColor.A);
		}

		if (puiClearOptions & EClearOptions::Depth)
		{
			luiClearFlags |= GL_DEPTH_BUFFER_BIT;
			glClearDepth(pfDepth);
		}

		if (puiClearOptions & EClearOptions::Stencil)
		{
			luiClearFlags |= GL_STENCIL_BUFFER_BIT;
			glClearStencil(piStencil);
		}

		glClear(luiClearFlags);

		return lResult;
	}

	void CGLGraphicsDevice::SetViewport(const SViewport & pViewport)
	{
		FDX_ASSERT(mContext != nullptr, "Graphics context cannot be null");

		glViewport(pViewport.X, pViewport.Y, 
			pViewport.Width, pViewport.Height);
	}

	void CGLGraphicsDevice::SetDrawBuffer(EDrawBuffer pDrawBuffer)
	{
		FDX_ASSERT(mContext != nullptr, "Graphics context cannot be null");
		glDrawBuffer(CGLUtil::GetDrawBuffer(pDrawBuffer));
	}

	EResult CGLGraphicsDevice::CreateTexture2D(const String & pstrName, ETextureUsage pUsage, CTexture2D ** pTexture)
	{
		FDX_ASSERT(mContext != nullptr, "Graphics context cannot be null");

		EResult lResult = EResult::Success;
		auto lTexture = new CGLTexture2D(pstrName);

		lResult = lTexture->Create(pUsage);

		if (lResult != EResult::Success)
		{
			LOG_ERROR("Unable to create Texture2D");
		}
		else
		{
			*pTexture = lTexture;
		}

		return lResult;
	}

	EResult CGLGraphicsDevice::CreateTexture2D(const String & pstrName, ETextureUsage pUsage, uint32_t puiWidth, 
		uint32_t puiHeight, ESurfaceFormat pSurfaceFormat, CTexture2D ** pTexture)
	{
		FDX_ASSERT(mContext != nullptr, "Graphics context cannot be null");

		EResult lResult = EResult::Success;
		auto lTexture = new CGLTexture2D(pstrName);

		lResult = lTexture->Create(pUsage);
		if (lResult != EResult::Success)
		{
			LOG_ERROR("Unable to create Texture2D");
		}
		else
		{
			lResult = lTexture->Initialize(pSurfaceFormat, puiWidth, puiHeight);
			if (lResult != EResult::Success)
			{
				LOG_ERROR("Unable to initialize Texture2D");
			}
			else
			{
				*pTexture = lTexture;
			}
		}

		return lResult;
	}

	EResult CGLGraphicsDevice::CreateTextureCube(const String & pstrName, ETextureUsage pUsage, CTextureCube ** pTexture)
	{
		FDX_ASSERT(mContext != nullptr, "Graphics context cannot be null");
		
		EResult lResult = EResult::Success;
		auto lTexture = new CGLTextureCube(pstrName);

		lResult = lTexture->Create(pUsage);

		if (lResult != EResult::Success)
		{
			LOG_ERROR("Unable to create TextureCube");
		}
		else
		{
			*pTexture = lTexture;
		}

		return lResult;
	}

	EResult CGLGraphicsDevice::CreateTextureCube(const String & pstrName, ETextureUsage pUsage, uint32_t puiSize, 
		ESurfaceFormat pSurfaceFormat, CTextureCube ** pTexture)
	{
		FDX_ASSERT(mContext != nullptr, "Graphics context cannot be null");

		EResult lResult = EResult::Success;
		auto lTexture = new CGLTextureCube(pstrName);

		lResult = lTexture->Create(pUsage);

		if (lResult != EResult::Success)
		{
			LOG_ERROR("Unable to create TextureCube");
		}
		else
		{
			lResult = lTexture->Initialize(pSurfaceFormat, puiSize);
			if (lResult != EResult::Success)
			{
				LOG_ERROR("Unable to initialize TextureCube");
			}
			else
			{
				*pTexture = lTexture;
			}
		}

		return lResult;
	}

	EResult CGLGraphicsDevice::CreateTexture2DArray(const String & pstrName, ETextureUsage pUsage, CTexture2DArray ** pTexture)
	{
		FDX_ASSERT(mContext != nullptr, "Graphics context cannot be null");

		EResult lResult = EResult::Success;
		auto lTexture = new CGLTexture2DArray(pstrName);

		lResult = lTexture->Create(pUsage);

		if (lResult != EResult::Success)
		{
			LOG_ERROR("Unable to create TextureCube");
		}
		else
		{
			*pTexture = lTexture;
		}

		return lResult;
	}

	EResult CGLGraphicsDevice::CreateProgram(const String & pstrName, CVertexFormat *pFormat, IProgram **pProgram)
	{
		FDX_ASSERT(mContext != nullptr, "Graphics context cannot be null");

		EResult lResult = EResult::Success;
		auto lProgram = new CGLProgram(pstrName);

		lResult = lProgram->Create(pFormat);

		if (lResult != EResult::Success)
		{
			LOG_ERROR("Unable to create program");
		}
		else
		{
			*pProgram = lProgram;
		}

		return lResult;
	}

	EResult CGLGraphicsDevice::CreateVertexBuffer(const String & pstrName, EBufferUsage pUsage, 
		CVertexFormat * pFormat, uint32_t puiNumVertices, CVertexBuffer **pBuffer)
	{
		FDX_ASSERT(mContext != nullptr, "Graphics context cannot be null");

		EResult lResult = EResult::Success;
		auto lBuffer = new CGLVertexBuffer(pstrName);

		lResult = lBuffer->Create(pUsage, pFormat, puiNumVertices);

		if (lResult != EResult::Success)
		{
			LOG_ERROR("Unable to create Vertex buffer");
		}
		else
		{
			*pBuffer = lBuffer;
		}

		return lResult;
	}

	EResult CGLGraphicsDevice::CreateIndexBuffer(const String & pstrName, EBufferUsage pUsage, 
		EIndexType pIndexType, uint32_t puiNumIndices, CIndexBuffer **pBuffer)
	{
		FDX_ASSERT(mContext != nullptr, "Graphics context cannot be null");

		EResult lResult = EResult::Success;
		auto lBuffer = new CGLIndexBuffer(pstrName);

		lResult = lBuffer->Create(pUsage, pIndexType, puiNumIndices);

		if (lResult != EResult::Success)
		{
			LOG_ERROR("Unable to create Index buffer");
		}
		else
		{
			*pBuffer = lBuffer;
		}

		return lResult;
	}

	EResult CGLGraphicsDevice::CreateUniformBuffer(const String & pstrName, EBufferUsage pUsage, 
		uint32_t puiSize, uint32_t puiBindingIndex, CUniformBuffer ** pBuffer)
	{
		FDX_ASSERT(mContext != nullptr, "Graphics context cannot be null");

		EResult lResult = EResult::Success;
		auto lBuffer = new CGLUniformBuffer(pstrName);

		lResult = lBuffer->Create(pUsage, puiSize, puiBindingIndex);

		if (lResult != EResult::Success)
		{
			LOG_ERROR("Unable to create Uniform buffer");
		}
		else
		{
			*pBuffer = lBuffer;
		}

		return lResult;
	}

	EResult CGLGraphicsDevice::CreateRenderData(const String & pstrName, IRenderData **pRenderData)
	{
		FDX_ASSERT(mContext != nullptr, "Graphics context cannot be null");

		EResult lResult = EResult::Success;
		auto lRenderData = new CGLRenderData(pstrName, this);

		lResult = lRenderData->Create();

		if (lResult != EResult::Success)
		{
			LOG_ERROR("Unable to create Renderdata");
		}
		else
		{
			*pRenderData = lRenderData;
		}

		return lResult;
	}

	EResult CGLGraphicsDevice::CreateDrawOperation(IRenderData * pRenderData, IDrawOperation **pDrawOperation)
	{
		*pDrawOperation = new CGLDrawOperation(dynamic_cast<CGLRenderData*>(pRenderData));
		return EResult::Success;
	}

	EResult CGLGraphicsDevice::CreateOcclusionQuery(const String & pstrName, IOcclusionQuery ** pOcclusionQuery)
	{
		FDX_ASSERT(mContext != nullptr, "Graphics context cannot be null");

		EResult lResult = EResult::Success;
		auto lOcclusionQuery = new CGLOcclusionQuery(pstrName);

		lResult = lOcclusionQuery->Create();

		if (lResult != EResult::Success)
		{
			LOG_ERROR("Unable to create OcclusionQuery");
		}
		else
		{
			*pOcclusionQuery = lOcclusionQuery;
		}

		return lResult;
	}

	EResult CGLGraphicsDevice::CreateRenderTarget(const String & pstrName, uint32_t puiNumColorTargets, 
		IRenderTarget ** pRenderTarget)
	{
		FDX_ASSERT(mContext != nullptr, "Graphics context cannot be null");

		EResult lResult = EResult::Success;
		auto lRenderTarget = new CGLRenderTarget(pstrName);

		lResult = lRenderTarget->Create(puiNumColorTargets);

		if (lResult != EResult::Success)
		{
			LOG_ERROR("Unable to create CGLRenderTarget");
		}
		else
		{
			*pRenderTarget = lRenderTarget;
		}

		return lResult;
	}

	EResult CGLGraphicsDevice::CreateBlendState(const SBlendStateProperties & pProperties, 
		IBlendState ** pState)
	{
		*pState = new CGLBlendState();
		(*pState)->SetProperties(pProperties);

		return EResult::Success;
	}

	EResult CGLGraphicsDevice::CreateRasterizerState(const SRasterizerStateProperties & pProperties, 
		IRasterizerState ** pState)
	{
		*pState = new CGLRasterizerState();
		(*pState)->SetProperties(pProperties);

		return EResult::Success;
	}

	EResult CGLGraphicsDevice::CreateDepthState(const SDepthStateProperties & pProperties, 
		IDepthState ** pState)
	{
		*pState = new CGLDepthState();
		(*pState)->SetProperties(pProperties);

		return EResult::Success;
	}

	EResult CGLGraphicsDevice::CreateStencilState(const SStencilStateProperties & pProperties, 
		IStencilState ** pState)
	{
		*pState = new CGLStencilState();
		(*pState)->SetProperties(pProperties);

		return EResult::Success;
	}

	void CGLGraphicsDevice::SetDefaults()
	{		
		auto lBlendState = std::make_unique<CGLBlendState>();
		lBlendState->SetProperties({});
		lBlendState->Apply();

		auto lRasterizerState = std::make_unique<CGLRasterizerState>();
		lRasterizerState->SetProperties({});
		lRasterizerState->Apply();

		auto lDepthState = std::make_unique<CGLDepthState>();
		lDepthState->SetProperties({});
		lDepthState->Apply();

		auto lStencilState = std::make_unique<CGLStencilState>();
		lStencilState->SetProperties({});
		lStencilState->Apply();
	}

	void CGLGraphicsDevice::EnableDebugging()
	{
		glDebugMessageCallback(DebugMessageCallback, this);
	}
}