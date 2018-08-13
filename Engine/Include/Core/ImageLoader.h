#pragma once

#include "Core/Image.h"

namespace FDX
{
	enum EImageLoaderType
	{
		FreeImage
	};

	class FDX_EXPORT IImageLoader
	{
	public:

		virtual ~IImageLoader()
		{
		}

		virtual EImageLoaderType GetImageLoaderType() const = 0;

		virtual bool CanLoadImageType(EImageType pImageType) = 0;
		virtual bool CanLoadImage(const String &pstrFilePath) = 0;

		virtual EResult Load(const String &pstrFilePath,
			IImage **pImage) = 0;
	};

	using IImageLoaderPtr = std::unique_ptr<IImageLoader>;
}
