
#include "OGL/GLTexture2D.h"
#include "OS/GameLogger.h"
#include "OGL/GLUtil.h"
#include "OS/System.h"

namespace FDX
{
	CGLTexture2D::CGLTexture2D(const String & pstrName)
		: mstrName(pstrName),
		mbHasMipMaps(false),
		muiWidth(0),
		muiHeight(0),
		muiHandle(0)
	{
	}

	CGLTexture2D::~CGLTexture2D()
	{
		if (muiHandle != 0)
			glDeleteTextures(1, &muiHandle);
	}

	ETextureType CGLTexture2D::GetType() const
	{
		return ETextureType::TwoD;
	}

	const String & CGLTexture2D::GetName() const
	{
		return mstrName;
	}

	ETextureUsage CGLTexture2D::GetUsage() const
	{
		return mUsage;
	}

	bool CGLTexture2D::HasMipMaps() const
	{
		return mbHasMipMaps;
	}

	const CTextureProperties & CGLTexture2D::GetProperties() const
	{
		return mProperties;
	}

	void CGLTexture2D::SetProperties(const CTextureProperties & pProperties)
	{
		mProperties = pProperties;
		glBindTexture(GL_TEXTURE_2D, muiHandle);

		if (pProperties.GetGenerateMipMaps())
			GenerateMipMaps();

		if (mbHasMipMaps)
		{
			switch (pProperties.GetFilter())
			{
			case ETextureFilter::Nearest:
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				}
				break;

			case ETextureFilter::Linear:
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				}
				break;

			case ETextureFilter::Bilinear:
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}
				break;

			case ETextureFilter::Trilinear:
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}
				break;
			}
		}		
		else
		{
			switch (pProperties.GetFilter())
			{
			case ETextureFilter::Nearest:
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				}
				break;

			default:
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}
				break;
			}
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, CGLUtil::GetTextureWrap(pProperties.GetWrapS()));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, CGLUtil::GetTextureWrap(pProperties.GetWrapT()));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, CGLUtil::GetTextureCompareMode(pProperties.GetCompareMode()));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, CGLUtil::GetCompareFunction(pProperties.GetCompareFunction()));

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	uint32_t CGLTexture2D::GetWidth() const
	{
		return muiWidth;
	}

	uint32_t CGLTexture2D::GetHeight() const
	{
		return muiHeight;
	}

	EResult CGLTexture2D::Load(IImage * pImage)
	{
		FDX_ASSERT(pImage != nullptr, "Passed parameter pImage cannot be null");

		EResult lResult = EResult::Success;
		EMPTY_GL_ERROR();

		glBindTexture(GL_TEXTURE_2D, muiHandle);
		glTexImage2D(GL_TEXTURE_2D, 
			0, 
			CGLUtil::GetSurfaceFormat(pImage->GetImageFormat()),
			pImage->GetWidth(), 
			pImage->GetHeight(), 
			0,
			CGLUtil::GetImageFormat(pImage->GetImageFormat()),
			CGLUtil::GetImageDataType(pImage->GetImageFormat()),
			pImage->GetData());

		GLenum lError = glGetError();
		if (lError != GL_NO_ERROR)
		{
			LOG_DEBUG("glTexImage2D failed: %d", lError);
			lResult = EResult::LoadFailed;
		}
		else
		{
			mFormat = pImage->GetImageFormat();
			muiWidth = pImage->GetWidth();
			muiHeight = pImage->GetHeight();
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		return lResult;
	}

	EResult CGLTexture2D::Create(ETextureUsage pUsage)
	{
		EResult lResult = EResult::Success;

		glGenTextures(1, &muiHandle);
		if (!muiHandle)
		{
			LOG_DEBUG("glGenTextures failed: %d", glGetError());
			lResult = EResult::CreateFailed;
		}
		else
		{
			mUsage = pUsage;
		}

		return lResult;
	}

	EResult CGLTexture2D::Initialize(ESurfaceFormat pFormat, uint32_t puiWidth, uint32_t puiHeight)
	{
		EResult lResult = EResult::Success;

		glBindTexture(GL_TEXTURE_2D, muiHandle);
		glTexImage2D(GL_TEXTURE_2D,
			0,
			CGLUtil::GetSurfaceFormat(pFormat),
			puiWidth,
			puiHeight,
			0,
			CGLUtil::GetImageFormat(pFormat),
			CGLUtil::GetImageDataType(pFormat),
			nullptr);

		GLenum lError = glGetError();
		if (lError != GL_NO_ERROR)
		{
			LOG_DEBUG("glTexImage2D failed: %d", lError);
			lResult = EResult::LoadFailed;
		}
		else
		{
			mFormat = pFormat;
			muiWidth = puiWidth;
			muiHeight = puiHeight;
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		return lResult;
	}

	void CGLTexture2D::Activate(uint32_t puiUnit)
	{
		glActiveTexture(GL_TEXTURE0 + puiUnit);
		glBindTexture(GL_TEXTURE_2D, muiHandle);
	}

	void CGLTexture2D::GenerateMipMaps()
	{
		glGenerateMipmap(GL_TEXTURE_2D);

		GLenum lError = glGetError();
		if (lError != GL_NO_ERROR)
		{
			LOG_DEBUG("glGenerateMipmap failed: %d", lError);
		}
		else
		{
			mbHasMipMaps = true;
		}
	}
}