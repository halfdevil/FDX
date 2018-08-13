#pragma once

#include "Core/Resource.h"
#include "Core/ImageLoader.h"

namespace FDX
{
	class FDX_EXPORT CImageResource : public IResource
	{
	public:

		CImageResource(const String &pstrFilePath, IImageLoader *pImageLoader);
		~CImageResource();

		const String& GetFilePath() const
		{
			return mstrFilePath;
		}

		EResourceType GetResourceType() const override
		{
			return EResourceType::Image;
		}

		bool IsLoaded() const override
		{
			return mbLoaded;
		}

		IImageLoader* GetImageLoader() const
		{
			return mImageLoader;
		}

		IImage* GetImage() const
		{
			return mImage.get();
		}

		EResult Load() override;
		EResult Unload() override;
		EResult Reload() override;

	private:

		String mstrFilePath;
		IImageLoader *mImageLoader;
		IImagePtr mImage;
		bool mbLoaded;
	};
}
