
#include "OGL/GLRenderTarget.h"
#include "OGL/GLUtil.h"
#include "OS/GameLogger.h"
#include "Graphics/GraphicsDevice.h"
#include "OGL/GLTexture2D.h"
#include "OGL/GLTextureCube.h"

namespace FDX
{
	CGLRenderTarget::CGLRenderTarget(const String & pstrName)
		: mstrName(pstrName),
		muiHandle(0),
		mDepthTarget(nullptr)
	{
	}

	CGLRenderTarget::~CGLRenderTarget()
	{
		if (muiHandle)
			glDeleteFramebuffers(1, &muiHandle);
	}

	EResult CGLRenderTarget::Create(uint32_t puiNumColorTargets)
	{
		EResult lResult = EResult::Success;

		glCreateFramebuffers(1, &muiHandle);
		if (!muiHandle)
		{
			LOG_DEBUG("glCreateFramebuffers() failed for %s", mstrName.c_str());
			lResult = EResult::CreateFailed;
		}
		else
		{
			mColorTargets.resize(puiNumColorTargets);
		}

		return lResult;
	}

	uint32_t CGLRenderTarget::GetNumColorTargets() const
	{
		return mColorTargets.size();
	}

	ITexture * CGLRenderTarget::GetColorTarget(uint32_t puiTarget)
	{
		if (puiTarget < mColorTargets.size())
			return mColorTargets[puiTarget];

		return nullptr;
	}

	EResult CGLRenderTarget::AttachTexture2D(uint32_t puiTarget, CTexture2D * pTexture)
	{
		EResult lResult = EResult::Success;

		if (pTexture)
		{			
			if (pTexture->GetType() != ETextureType::TwoD)
			{
				LOG_DEBUG("Passed texture is not a 2D texture for RenderTarget: %s", 
					mstrName.c_str());
			}
			else
			{
				CGLTexture2D *lGLTexture = dynamic_cast<CGLTexture2D*>(pTexture);

				glBindFramebuffer(GL_FRAMEBUFFER, muiHandle);
				glBindTexture(GL_TEXTURE_2D, lGLTexture->GetHandle());
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + puiTarget,
					GL_TEXTURE_2D, lGLTexture->GetHandle(), 0);

				lResult = CheckBufferStatus();
				if (lResult != EResult::Success)
				{
					LOG_DEBUG("CheckBufferStatus() failed for %s", mstrName.c_str());
				}
				else
				{
					mColorTargets[puiTarget] = lGLTexture;
				}

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}
		else
		{
			mColorTargets[puiTarget] = nullptr;
		}

		return lResult;
	}

	EResult CGLRenderTarget::AttachTextureCube(uint32_t puiTarget, ECubeMapFace pCubeMapFace, CTextureCube * pTexture)
	{
		EResult lResult = EResult::Success;

		if (pTexture)
		{			
			if (pTexture->GetType() != ETextureType::Cube)
			{
				LOG_DEBUG("Passed texture is not a cube texture for RenderTarget: %s",
					mstrName.c_str());
			}
			else
			{
				CGLTextureCube *lGLTexture = dynamic_cast<CGLTextureCube*>(pTexture);

				glBindFramebuffer(GL_FRAMEBUFFER, muiHandle);
				glBindTexture(GL_TEXTURE_2D, lGLTexture->GetHandle());
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + puiTarget,
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + (uint32_t)pCubeMapFace, 
					lGLTexture->GetHandle(), 0);

				lResult = CheckBufferStatus();
				if (lResult != EResult::Success)
				{
					LOG_DEBUG("CheckBufferStatus() failed for %s", mstrName.c_str());
				}
				else
				{
					mColorTargets[puiTarget] = lGLTexture;
				}

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}
		else
		{
			mColorTargets[puiTarget] = nullptr;
		}

		return lResult;
	}

	EResult CGLRenderTarget::AttachDepthTexture(CTexture2D * pTexture)
	{
		EResult lResult = EResult::Success;

		if (pTexture)
		{		
			if (pTexture->GetType() != ETextureType::TwoD)
			{
				LOG_DEBUG("Passed texture is not a 2D texture for RenderTarget: %s",
					mstrName.c_str());
			}
			else
			{
				CGLTexture2D *lGLTexture = dynamic_cast<CGLTexture2D*>(pTexture);

				glBindFramebuffer(GL_FRAMEBUFFER, muiHandle);
				glBindTexture(GL_TEXTURE_2D, lGLTexture->GetHandle());
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
					GL_TEXTURE_2D, lGLTexture->GetHandle(), 0);

				lResult = CheckBufferStatus();
				if (lResult != EResult::Success)
				{
					LOG_DEBUG("CheckBufferStatus() failed for %s", mstrName.c_str());
				}
				else
				{
					mDepthTarget = lGLTexture;
				}

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}
		else
		{
			mDepthTarget = nullptr;
		}

		return lResult;
	}

	void CGLRenderTarget::Activate()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, muiHandle);
	}

	void CGLRenderTarget::Deactivate()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void CGLRenderTarget::DisableDrawBuffer()
	{
		glDrawBuffer(GL_NONE);
	}

	void CGLRenderTarget::SetViewport(const SViewport & pViewport)
	{
		glViewport(pViewport.X, pViewport.Y,
			pViewport.Width, pViewport.Height);
	}

	void CGLRenderTarget::Clear(uint32_t puiClearOptions, const CColor4F & pColor,
		float pfDepth, int32_t piStencil)
	{
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
	}

	EResult CGLRenderTarget::CheckBufferStatus()
	{
		EResult lResult = EResult::Success;

		GLenum lStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (lStatus != GL_FRAMEBUFFER_COMPLETE)
		{
			switch (lStatus)
			{
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				{
					LOG_DEBUG("CheckBufferStatus() - GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
					lResult = EResult::Error;
				}
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				{
					LOG_DEBUG("CheckBufferStatus() - GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
					lResult = EResult::Error;
				}
				break;

			case GL_FRAMEBUFFER_UNSUPPORTED:
				{
					LOG_DEBUG("CheckBufferStatus() - GL_FRAMEBUFFER_UNSUPPORTED");
					lResult = EResult::Error;
				}
				break;

			default:
				{
					LOG_DEBUG("CheckBufferStatus() - UNKNOWN");
					lResult = EResult::Error;
				}
				break;
			}
		}

		return lResult;
	}
}