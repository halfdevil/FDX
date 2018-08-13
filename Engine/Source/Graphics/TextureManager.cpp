
#include "Graphics/TextureManager.h"
#include "OS/GameLogger.h"
#include "Core/ImageResource.h"
#include "Core/ResourceManager.h"
#include "OS/System.h"

namespace FDX
{
	CTextureManager::CTextureManager(IGraphicsDevice * pGraphicsDevice)
		: mGraphicsDevice(pGraphicsDevice)
	{
	}

	CTextureManager::~CTextureManager()
	{
		RemoveAll();
	}

	EResult CTextureManager::GetTexture(const String & pstrName, ITexture ** pTexture)
	{
		EResult lResult = EResult::Success;

		auto lIt = mTextures.find(pstrName);
		if (lIt != mTextures.end())
		{
			*pTexture = (*lIt).second.get();
		}
		else
		{
			lResult = EResult::NotFound;
		}

		return lResult;
	}

	EResult CTextureManager::AddTexture2D(const String & pstrName, const String & pstrFilePath,
		const CTextureProperties &pProperties, CTexture2D ** pTexture)
	{
		EResult lResult = EResult::Success;

		auto lIt = mTextures.find(pstrName);
		if (lIt != mTextures.end())
		{
			*pTexture = dynamic_cast<CTexture2D*>((*lIt).second.get());
		}
		else
		{
			CTexture2D *lTexture = nullptr;
			lResult = mGraphicsDevice->CreateTexture2D(pstrName, ETextureUsage::Normal, &lTexture);

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("CreateTexture2D() failed: %s", pstrFilePath.c_str());
			}
			else
			{
				CImageResource *lResource = nullptr;
				lResult = CResourceManager::Get().LoadResource(EResourceType::Image, pstrFilePath, &lResource);

				if (lResult != EResult::Success)
				{
					LOG_DEBUG("GetResource() failed: %s", pstrFilePath.c_str());
				}
				else
				{
					lResult = lTexture->Load(lResource->GetImage());
					if (lResult != EResult::Success)
					{
						LOG_DEBUG("Texture Load() failed: %s", pstrFilePath.c_str());
					}
					else
					{
						if (pTexture != nullptr)
						{
							*pTexture = lTexture;
						}

						lTexture->SetProperties(pProperties);
						mTextures[pstrName] = ITexturePtr(lTexture);
					}
				}
			}

			if (lResult != EResult::Success)
				SAFE_DELETE(lTexture);
		}

		return lResult;
	}

	EResult CTextureManager::AddTextureCube(const String & pstrName, const Array<String>& pFilePaths, 
		const CTextureProperties &pProperties, CTextureCube ** pTexture)
	{
		FDX_ASSERT(pFilePaths.size() == 6, "Number of images must be 6");

		EResult lResult = EResult::Success;

		auto lIt = mTextures.find(pstrName);
		if (lIt != mTextures.end())
		{
			*pTexture = dynamic_cast<CTextureCube*>((*lIt).second.get());
		}
		else
		{
			CTextureCube *lTexture = nullptr;
			lResult = mGraphicsDevice->CreateTextureCube(pstrName, ETextureUsage::Normal, &lTexture);

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("CreateTextureCube() failed: %s", pstrName.c_str());
			}
			else
			{
				Array<IImage*> lImages;

				for (uint32_t lIdx = 0; lIdx < 6; lIdx++)
				{
					CImageResource *lResource = nullptr;
					lResult = CResourceManager::Get().LoadResource(EResourceType::Image, pFilePaths[lIdx], &lResource);

					if (lResult != EResult::Success)
					{
						LOG_DEBUG("GetResource() failed: %s", pFilePaths[lIdx].c_str());
						break;
					}
					else
					{
						lImages.push_back(lResource->GetImage());
					}
				}

				if (lResult != EResult::Success)
				{
					LOG_DEBUG("GetResource() failed for loading images: %s", pstrName.c_str());
				}				
				else
				{
					lResult = lTexture->Load(lImages);
					if (lResult != EResult::Success)
					{
						LOG_DEBUG("Texture Load() failed: %s", pstrName.c_str());
					}
					else
					{
						if (pTexture != nullptr)
						{
							*pTexture = lTexture;
						}

						lTexture->SetProperties(pProperties);
						mTextures[pstrName] = ITexturePtr(lTexture);
					}
				}
			}

			if (lResult != EResult::Success)
				SAFE_DELETE(lTexture);
		}

		return lResult;
	}

	EResult CTextureManager::AddTexture2DArray(const String & pstrName, const Array<String>& pstrFilePaths, 
		const CTextureProperties & pProperties, CTexture2DArray ** pTexture)
	{
		EResult lResult = EResult::Success;

		auto lIt = mTextures.find(pstrName);
		if (lIt != mTextures.end())
		{
			*pTexture = dynamic_cast<CTexture2DArray*>((*lIt).second.get());
		}
		else
		{
			CTexture2DArray *lTexture = nullptr;
			lResult = mGraphicsDevice->CreateTexture2DArray(pstrName, ETextureUsage::Normal, &lTexture);

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("CreateTexture2DArray() failed: %s", pstrName.c_str());
			}
			else
			{
				Array<IImage*> lImages;

				for (uint32_t lIdx = 0; lIdx < pstrFilePaths.size(); lIdx++)
				{
					CImageResource *lResource = nullptr;
					lResult = CResourceManager::Get().LoadResource(EResourceType::Image, pstrFilePaths[lIdx], &lResource);

					if (lResult != EResult::Success)
					{
						LOG_DEBUG("GetResource() failed: %s", pstrFilePaths[lIdx].c_str());
						break;
					}
					else
					{
						lImages.push_back(lResource->GetImage());
					}
				}

				if (lResult != EResult::Success)
				{
					LOG_DEBUG("GetResource() failed for loading images: %s", pstrName.c_str());
				}
				else
				{
					lResult = lTexture->Load(lImages);
					if (lResult != EResult::Success)
					{
						LOG_DEBUG("Texture Load() failed: %s", pstrName.c_str());
					}
					else
					{
						*pTexture = lTexture;
					}
				}
			}

			if (lResult != EResult::Success)
				SAFE_DELETE(lTexture);
		}

		return lResult;
	}

	EResult CTextureManager::RemoveTexture(const String & pstrName)
	{
		EResult lResult = EResult::Success;

		auto lIt = mTextures.find(pstrName);
		if (lIt != mTextures.end())
		{
			mTextures.erase(lIt);
		}
		else
		{
			lResult = EResult::NotFound;
		}

		return lResult;
	}

	void CTextureManager::RemoveAll()
	{
		mTextures.clear();
	}
}