
#include "OGL/GLTexture2DArray.h"
#include "OGL/GLUtil.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CGLTexture2DArray::CGLTexture2DArray(const String & pstrName)
		: mstrName(pstrName),
		mbHasMipMaps(false),
		muiWidth(0),
		muiHeight(0),
		muiNumLayers(0),
		muiHandle(0)
	{
	}

	CGLTexture2DArray::~CGLTexture2DArray()
	{
		if (muiHandle != 0)
			glDeleteTextures(1, &muiHandle);
	}

	ETextureType CGLTexture2DArray::GetType() const
	{
		return ETextureType::TwoDArray;
	}

	const String & CGLTexture2DArray::GetName() const
	{
		return mstrName;
	}

	ETextureUsage CGLTexture2DArray::GetUsage() const
	{
		return mUsage;
	}

	bool CGLTexture2DArray::HasMipMaps() const
	{
		return mbHasMipMaps;
	}

	const CTextureProperties & CGLTexture2DArray::GetProperties() const
	{
		return mProperties;
	}

	void CGLTexture2DArray::SetProperties(const CTextureProperties & pProperties)
	{
		mProperties = pProperties;
		glBindTexture(GL_TEXTURE_2D_ARRAY, muiHandle);

		if (mbHasMipMaps)
		{
			switch (pProperties.GetFilter())
			{
			case ETextureFilter::Nearest:
			{
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			break;

			case ETextureFilter::Linear:
			{
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			break;

			case ETextureFilter::Bilinear:
			{
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			break;

			case ETextureFilter::Trilinear:
			{
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			break;

			default:
			{
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			break;
			}
		}

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, CGLUtil::GetTextureWrap(pProperties.GetWrapS()));
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, CGLUtil::GetTextureWrap(pProperties.GetWrapT()));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, CGLUtil::GetTextureCompareMode(pProperties.GetCompareMode()));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, CGLUtil::GetCompareFunction(pProperties.GetCompareFunction()));
	}

	uint32_t CGLTexture2DArray::GetWidth() const
	{
		return muiWidth;
	}

	uint32_t CGLTexture2DArray::GetHeight() const
	{
		return muiHeight;
	}

	uint32_t CGLTexture2DArray::GetNumLayers() const
	{
		return muiNumLayers;
	}

	EResult CGLTexture2DArray::Load(const Array<IImage*>& pImages)
	{
		EResult lResult = EResult::Success;
		glBindTexture(GL_TEXTURE_2D_ARRAY, muiHandle);

		GLenum lError = GL_NO_ERROR;

		for (uint32_t lIdx = 0; lIdx < pImages.size(); lIdx++)
		{
			glTexImage3D(GL_TEXTURE_2D_ARRAY,
				0,
				CGLUtil::GetSurfaceFormat(pImages[lIdx]->GetImageFormat()),
				pImages[lIdx]->GetWidth(),
				pImages[lIdx]->GetHeight(),
				lIdx,
				0,
				CGLUtil::GetImageFormat(pImages[lIdx]->GetImageFormat()),
				GL_UNSIGNED_BYTE,
				pImages[lIdx]->GetData());

			lError = glGetError();
			if (lError != GL_NO_ERROR)
			{
				LOG_DEBUG("glTexImage3D failed: %d", lError);
				lResult = EResult::LoadFailed;

				break;
			}
			else
			{
				mFormat = pImages[lIdx]->GetImageFormat();
				muiWidth = pImages[lIdx]->GetWidth();
				muiHeight = pImages[lIdx]->GetHeight();
				muiNumLayers = pImages.size();
			}
		}

		if (lResult == EResult::Success)
		{
			glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

			lError = glGetError();
			if (lError != GL_NO_ERROR)
			{
				LOG_DEBUG("glGenerateMipmap failed: %d", lError);
			}
			else
			{
				mbHasMipMaps = true;
			}
		}

		return lResult;
	}

	EResult CGLTexture2DArray::Create(ETextureUsage pUsage)
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

	void CGLTexture2DArray::Activate(uint32_t puiUnit)
	{
		glActiveTexture(GL_TEXTURE0 + puiUnit);
		glBindTexture(GL_TEXTURE_2D_ARRAY, muiHandle);
	}
}