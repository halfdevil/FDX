#pragma once

#include "Graphics/RenderTarget.h"
#include "Graphics/GraphicsDevice.h"
#include "Graphics/Program.h"
#include "Scene/Camera.h"

namespace FDX
{
	class ILight;

	class FDX_EXPORT CShadowMap
	{
	public:

		CShadowMap(ILight *pLight);
		~CShadowMap();

		ILight* GetLight() const
		{
			return mLight;
		}

		IRenderTarget* GetRenderTarget() const
		{
			return mRenderTarget.get();
		}

		CTexture2D* GetTexture() const
		{
			return mDepthTexture.get();
		}

		uint32_t GetTextureUnit() const
		{
			return muiTextureUnit;
		}

		EResult Create(IGraphicsDevice *pGraphicsDevice, 
			uint32_t puiWidth, uint32_t puiHeight);

		void Activate();
		void Deactivate();

		void SetTextureUnit(uint32_t puiTextureUnit);
		void Apply(IProgram *pProgram);

	private:

		ILight *mLight;
		CTexture2DPtr mDepthTexture;
		IRenderTargetPtr mRenderTarget;
		SViewport mViewport;
		uint32_t muiTextureUnit;
	};

	using CShadowMapPtr = SmartPtr<CShadowMap>;
}