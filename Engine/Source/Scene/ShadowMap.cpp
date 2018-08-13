
#include "Graphics/ShadowMap.h"
#include "Graphics/ProgramManager.h"
#include "Scene/Light.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CShadowMap::CShadowMap(ILight * pLight)
		: mLight(pLight),
		mDepthTexture(nullptr),
		mRenderTarget(nullptr)
	{
	}

	CShadowMap::~CShadowMap()
	{
	}

	EResult CShadowMap::Create(IGraphicsDevice * pGraphicsDevice, 
		uint32_t puiWidth, uint32_t puiHeight)
	{
		EResult lResult = EResult::Success;

		CTexture2D *lTexture = nullptr;
		IRenderTarget *lRenderTarget = nullptr;

		lResult = pGraphicsDevice->CreateTexture2D("ShadowMap", ETextureUsage::DepthStencilTarget,
			puiWidth, puiHeight, ESurfaceFormat::D24, &lTexture);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("IGraphicsDevice::CreateTexture2D() failed");
		}
		else
		{
			CTextureProperties lTextureProps;
			lTextureProps.SetGenerateMipMaps(false);
			lTextureProps.SetWrapS(ETextureWrap::ClampToEdge);
			lTextureProps.SetWrapT(ETextureWrap::ClampToEdge);
			lTextureProps.SetFilter(ETextureFilter::Nearest);
			lTextureProps.SetCompareMode(ETextureCompareMode::RefToTexture);
			lTextureProps.SetCompareFunction(ECompareFunction::LessOrEqual);
			lTexture->SetProperties(lTextureProps);

			lResult = pGraphicsDevice->CreateRenderTarget("ShadowMap_RT", 0, &lRenderTarget);
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("IGraphicsDevice::CreateRenderTarget() failed");
				SAFE_DELETE(lTexture);
			}
			else
			{
				lResult = lRenderTarget->AttachDepthTexture(lTexture);
				if (lResult != EResult::Success)
				{
					LOG_DEBUG("IRenderTarget::AttachDepthTexture() failed");
					SAFE_DELETE(lTexture);
					SAFE_DELETE(lRenderTarget);
				}
				else
				{
					mDepthTexture = CTexture2DPtr(lTexture);
					mRenderTarget = IRenderTargetPtr(lRenderTarget);
					
					mViewport = {
						0, 0,
						puiWidth, puiHeight
					};
				}
			}
		}

		return lResult;
	}

	void CShadowMap::Activate()
	{
		mRenderTarget->Activate();
		mRenderTarget->Clear(EClearOptions::Depth);
		mRenderTarget->SetViewport(mViewport);
		mRenderTarget->DisableDrawBuffer();
	}

	void CShadowMap::Deactivate()
	{
		mRenderTarget->Deactivate();
	}

	void CShadowMap::SetTextureUnit(uint32_t puiTextureUnit)
	{
		muiTextureUnit = puiTextureUnit;
	}

	void CShadowMap::Apply(IProgram * pProgram)
	{
		if (pProgram)
		{
			pProgram->GetConstants()->SetTexture("uShadowMap", 
				muiTextureUnit, mDepthTexture.get());
		}
	}
}