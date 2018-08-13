#pragma once

#include "Math/Color4F.h"
#include "Graphics/Texture2D.h"
#include "Graphics/TextureCube.h"

namespace FDX
{
	struct SViewport;

	enum class ECubeMapFace : uint32_t
	{
		PositiveX,
		NegativeX,
		PositiveY,
		NegativeY,
		PositiveZ,
		NegativeZ
	};

	class FDX_EXPORT IRenderTarget
	{
	public:

		virtual ~IRenderTarget()
		{
		}

		virtual const String& GetName() const = 0;

		virtual uint32_t GetNumColorTargets() const = 0;
		virtual ITexture* GetColorTarget(uint32_t puiTarget) = 0;

		virtual EResult AttachTexture2D(uint32_t puiTarget, CTexture2D *pTexture) = 0;
		virtual EResult AttachTextureCube(uint32_t puiTarget, 
			ECubeMapFace pCubeMapFace, CTextureCube *pTexture) = 0;

		virtual EResult AttachDepthTexture(CTexture2D *pTexture) = 0;

		virtual void Activate() = 0;
		virtual void Deactivate() = 0;
		virtual void DisableDrawBuffer() = 0;

		virtual void SetViewport(const SViewport &pViewport) = 0;
		virtual void Clear(uint32_t puiClearOptions,
			const CColor4F &pColor = CColor4F(0.0f, 0.0f, 0.0f),
			float pfDepth = 1.0f,
			int32_t piStencil = 0) = 0;
	};

	using IRenderTargetPtr = SmartPtr<IRenderTarget>;
}