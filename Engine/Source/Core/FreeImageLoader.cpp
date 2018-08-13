
#include "Core/FreeImageLoader.h"
#include "FreeImage.h"
#include "OS/GameLogger.h"
#include "OS/System.h"
#include "FS/FileSystem.h"

namespace FDX
{
	CFreeImage::CFreeImage()
		: muiWidth(0),
		muiHeight(0),
		muiBitsPerPixel(0),
		muiDataSize(0)
	{
	}

	CFreeImage::~CFreeImage()
	{
	}

	EResult CFreeImage::Create(const String & pstrFilePath, bool pbFlipVertical)
	{
		EResult lResult = EResult::Success;
		IFileStream *lStream = nullptr;

		lResult = IFileSystem::Get().GetStream(pstrFilePath, &lStream);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("Image GetStream failed: %s", pstrFilePath.c_str());			
		}
		else
		{
			auto lImageData = SmartPtr<uint8_t[]>(new uint8_t[lStream->GetSize()]);
			lResult = lStream->Read(lStream->GetSize(), lImageData.get());

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("Image data read from stream failed: %s", pstrFilePath.c_str());
			}
			else
			{
				FIMEMORY *lMemory = FreeImage_OpenMemory(lImageData.get(), lStream->GetSize());
				if (!lMemory)
				{
					LOG_DEBUG("FreeImage_OpenMemory() failed: %s", pstrFilePath.c_str());
					lResult = EResult::CreateFailed;
				}
				else
				{
					FREE_IMAGE_FORMAT lFif = FreeImage_GetFileTypeFromMemory(lMemory);

					switch (lFif)
					{
					case FIF_BMP:
						mImageType = EImageType::Bmp;
						break;

					case FIF_PNG:
						mImageType = EImageType::Png;
						break;

					case FIF_JPEG:
						mImageType = EImageType::Jpeg;
						break;

					case FIF_TARGA:
						mImageType = EImageType::Tga;
						break;

					case FIF_DDS:
						mImageType = EImageType::Dds;
						break;

					default:
						{
							LOG_DEBUG("FreeImage image type not supported: %s", pstrFilePath.c_str());
							lResult = EResult::CreateFailed;
						}
						break;
					}

					if (lResult == EResult::Success)
					{
						FIBITMAP *lBitmap = FreeImage_LoadFromMemory(lFif, lMemory);
						if (!lBitmap)
						{
							LOG_DEBUG("FreeImage image type not supported: %s", pstrFilePath.c_str());
							FreeImage_CloseMemory(lMemory);

							lResult = EResult::CreateFailed;
						}
						else
						{
							FreeImage_CloseMemory(lMemory);

							FIBITMAP *lConvBitmap = FreeImage_IsTransparent(lBitmap) ?
								FreeImage_ConvertTo32Bits(lBitmap) : FreeImage_ConvertTo24Bits(lBitmap);

							FreeImage_Unload(lBitmap);
							lBitmap = lConvBitmap;

							if (pbFlipVertical)
								FreeImage_FlipVertical(lBitmap);

							muiWidth = FreeImage_GetWidth(lBitmap);
							muiHeight = FreeImage_GetHeight(lBitmap);
							muiBitsPerPixel = FreeImage_GetBPP(lBitmap);
							muiDataSize = muiWidth * muiHeight * (muiBitsPerPixel / 8);

							mImageFormat = FreeImage_IsTransparent(lBitmap) ?
								ESurfaceFormat::BGRA8 : ESurfaceFormat::BGR8;
							
							mData = std::make_unique<uint8_t[]>(muiDataSize);
							memcpy(mData.get(), FreeImage_GetBits(lBitmap), muiDataSize);

							FreeImage_Unload(lBitmap);
						}
					}
				}
			}		
		}

		return lResult;
	}

	CFreeImageLoader::CFreeImageLoader()
	{
	}

	CFreeImageLoader::~CFreeImageLoader()
	{
	}

	EImageLoaderType CFreeImageLoader::GetImageLoaderType() const
	{
		return EImageLoaderType::FreeImage;
	}

	bool CFreeImageLoader::CanLoadImageType(EImageType pImageType)
	{
		switch (pImageType)
		{
		case EImageType::Bmp:
		case EImageType::Jpeg:
		case EImageType::Png:
		case EImageType::Tga:
		case EImageType::Dds:
			return true;
		}

		return false;
	}

	bool CFreeImageLoader::CanLoadImage(const String & pstrFilePath)
	{
		bool lbCanLoad = false;
		uint32_t luiDot = pstrFilePath.find_last_of(".");

		if (luiDot != (uint32_t)String::npos)
		{
			String lstrExt = pstrFilePath.substr(luiDot);
			std::transform(lstrExt.begin(), lstrExt.end(), lstrExt.begin(), ::tolower);

			if (lstrExt == ".jpg" || lstrExt == ".png" || 
				lstrExt == ".bmp" || lstrExt == ".tga" ||
				lstrExt == ".dds")
			{
				lbCanLoad = true;
			}
		}

		return lbCanLoad;
	}

	EResult CFreeImageLoader::Load(const String & pstrFilePath, IImage ** pImage)
	{
		EResult lResult = EResult::Success;

		CFreeImage *lImage = new CFreeImage();
		lResult = lImage->Create(pstrFilePath, true);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CFreeImage Create failed: %s", pstrFilePath.c_str());
		}
		else
		{
			*pImage = lImage;
		}

		return lResult;
	}
}