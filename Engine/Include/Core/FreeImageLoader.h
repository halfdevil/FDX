#pragma once

#include "Core/ImageLoader.h"

namespace FDX
{
	class CFreeImage : public IImage
	{
	public:

		CFreeImage();
		~CFreeImage() override;

		EImageType GetImageType() const override
		{
			return mImageType;
		}

		ESurfaceFormat GetImageFormat() const override
		{
			return mImageFormat;
		}

		uint32_t GetWidth() const override
		{
			return muiWidth;
		}

		uint32_t GetHeight() const override
		{
			return muiHeight;
		}

		uint32_t GetBitsPerPixel() const override
		{
			return muiBitsPerPixel;
		}

		uint32_t GetDataSize() const override
		{
			return muiDataSize;
		}

		const void* GetData() const override
		{
			return mData.get();
		}

		EResult Create(const String &pstrFilePath, bool pbFlipVertical);

	private:

		EImageType mImageType;
		ESurfaceFormat mImageFormat;
		uint32_t muiWidth;
		uint32_t muiHeight;
		uint32_t muiBitsPerPixel;
		uint32_t muiDataSize;
		SmartPtr<uint8_t[]> mData;
	};

	class CFreeImageLoader : public IImageLoader
	{
	public:

		CFreeImageLoader();
		~CFreeImageLoader();

		EImageLoaderType GetImageLoaderType() const;

		bool CanLoadImageType(EImageType pImageType);
		bool CanLoadImage(const String &pstrFilePath);

		EResult Load(const String &pstrFilePath,
			IImage **pImage);
	};
}