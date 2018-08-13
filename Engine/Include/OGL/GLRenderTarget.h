#pragma once

#include "Graphics/RenderTarget.h"
#include "Graphics/GraphicsDevice.h"
#include <GL/gl3w.h>

namespace FDX
{
	class CGLRenderTarget : public IRenderTarget
	{
	public:

		CGLRenderTarget(const String &pstrName);
		~CGLRenderTarget();

		const String& GetName() const
		{
			return mstrName;
		}

		GLuint GetHandle() const
		{
			return muiHandle;
		}

		EResult Create(uint32_t puiNumColorTargets);

		uint32_t GetNumColorTargets() const override;
		ITexture* GetColorTarget(uint32_t puiTarget) override;

		EResult AttachTexture2D(uint32_t puiTarget, CTexture2D *pTexture) override;
		EResult AttachTextureCube(uint32_t puiTarget,
			ECubeMapFace pCubeMapFace, CTextureCube *pTexture) override;

		EResult AttachDepthTexture(CTexture2D *pTexture) override;

		void Activate() override;
		void Deactivate() override;
		void DisableDrawBuffer() override;

		void SetViewport(const SViewport &pViewport) override;
		void Clear(uint32_t puiClearOptions,
			const CColor4F &pColor = CColor4F(0.0f, 0.0f, 0.0f),
			float pfDepth = 1.0f,
			int32_t piStencil = 0) override;

	private:

		EResult CheckBufferStatus();

	private:

		GLuint muiHandle;
		String mstrName;
		Array<ITexture*> mColorTargets;
		ITexture *mDepthTarget;
	};
}