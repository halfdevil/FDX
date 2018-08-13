
#include "OGL/GLTextureCube.h"
#include "OS/GameLogger.h"
#include "OGL/GLUtil.h"
#include "OS/System.h"

namespace FDX
{
	CGLTextureCube::CGLTextureCube(const String & pstrName)
		: mstrName(pstrName),
		mbHasMipMaps(false),
		muiSize(0),
		muiHandle(0)
	{
	}

	CGLTextureCube::~CGLTextureCube()
	{
		if (muiHandle != 0)
			glDeleteTextures(1, &muiHandle);
	}

	ETextureType CGLTextureCube::GetType() const
	{
		return ETextureType::Cube;
	}

	const String & CGLTextureCube::GetName() const
	{
		return mstrName;
	}

	ETextureUsage CGLTextureCube::GetUsage() const
	{
		return mUsage;
	}

	bool CGLTextureCube::HasMipMaps() const
	{
		return mbHasMipMaps;
	}

	const CTextureProperties & CGLTextureCube::GetProperties() const
	{
		return mProperties;
	}

	void CGLTextureCube::SetProperties(const CTextureProperties & pProperties)
	{
		mProperties = pProperties;
		glBindTexture(GL_TEXTURE_CUBE_MAP, muiHandle);

		if (pProperties.GetGenerateMipMaps())
			GenerateMipMaps();

		if (mbHasMipMaps)
		{
			switch (pProperties.GetFilter())
			{
			case ETextureFilter::Nearest:
			{
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			break;

			case ETextureFilter::Linear:
			{
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			break;

			case ETextureFilter::Bilinear:
			{
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			break;

			case ETextureFilter::Trilinear:
			{
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				}
				break;

				default:
				{
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}
				break;
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, CGLUtil::GetTextureWrap(pProperties.GetWrapS()));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, CGLUtil::GetTextureWrap(pProperties.GetWrapT()));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, CGLUtil::GetTextureWrap(pProperties.GetWrapR()));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, CGLUtil::GetTextureCompareMode(pProperties.GetCompareMode()));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, CGLUtil::GetCompareFunction(pProperties.GetCompareFunction()));

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	uint32_t CGLTextureCube::GetSize() const
	{
		return muiSize;
	}

	EResult CGLTextureCube::Load(const Array<IImage*>& pImages)
	{
		FDX_ASSERT(pImages.size() == 6, "Number of images is not equalt to 6");

		EResult lResult = EResult::Success;
		glBindTexture(GL_TEXTURE_CUBE_MAP, muiHandle);

		GLenum lError = GL_NO_ERROR;

		for (uint32_t lIdx = 0; lIdx < 6; lIdx++)
		{
			GLuint luiTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X + lIdx;

			glTexImage2D(luiTarget,
				0,
				CGLUtil::GetSurfaceFormat(pImages[lIdx]->GetImageFormat()),
				pImages[lIdx]->GetWidth(),
				pImages[lIdx]->GetHeight(),
				0,
				CGLUtil::GetImageFormat(pImages[lIdx]->GetImageFormat()),
				GL_UNSIGNED_BYTE,
				pImages[lIdx]->GetData());

			lError = glGetError();
			if (lError != GL_NO_ERROR)
			{
				LOG_DEBUG("glTexImage2D failed: %d", lError);
				lResult = EResult::LoadFailed;

				break;
			}
			else
			{
				mFormat = pImages[lIdx]->GetImageFormat();
				muiSize = pImages[lIdx]->GetWidth();
			}
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return lResult;
	}

	EResult CGLTextureCube::Create(ETextureUsage pUsage)
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

	EResult CGLTextureCube::Initialize(ESurfaceFormat pFormat, uint32_t puiSize)
	{
		EResult lResult = EResult::Success;
		glBindTexture(GL_TEXTURE_CUBE_MAP, muiHandle);

		for (uint32_t lIdx = 0; lIdx < 6; lIdx++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP + lIdx,
				0,
				CGLUtil::GetSurfaceFormat(pFormat),
				puiSize,
				puiSize,
				0,
				CGLUtil::GetSurfaceFormat(pFormat),
				GL_UNSIGNED_BYTE,
				nullptr);

			GLenum lError = glGetError();
			if (lError != GL_NO_ERROR)
			{
				LOG_DEBUG("glTexImage2D failed: %d", lError);
				lResult = EResult::LoadFailed;
				break;
			}
			else
			{
				mFormat = pFormat;
				muiSize = puiSize;
			}
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return lResult;
	}

	void CGLTextureCube::Activate(uint32_t puiUnit)
	{
		glActiveTexture(GL_TEXTURE0 + puiUnit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, muiHandle);
	}

	void CGLTextureCube::GenerateMipMaps()
	{
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

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