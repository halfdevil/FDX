
#include "Core/ImageResource.h"
#include "OS/GameLogger.h"
#include "OS/System.h"

namespace FDX
{
	CImageResource::CImageResource(const String &pstrFilePath, IImageLoader * pImageLoader)
		: mstrFilePath(pstrFilePath), 
		mImageLoader(pImageLoader), 
		mImage(nullptr),
		mbLoaded(false)
	{
	}

	CImageResource::~CImageResource()
	{
		Unload();
	}

	EResult CImageResource::Load()
	{
		FDX_ASSERT(mImageLoader != nullptr, "ImageLoader cannot be null");
		EResult lResult = EResult::Success;

		if (mbLoaded)
			return lResult;
		
		if (!mImageLoader->CanLoadImage(mstrFilePath))
		{
			LOG_ERROR("ImageLoader cannot load image: %s", mstrFilePath.c_str());
			lResult = EResult::LoadFailed;
		}
		else
		{
			IImage *lImage;
			lResult = mImageLoader->Load(mstrFilePath, &lImage);

			if (lResult != EResult::Success)
			{
				LOG_ERROR("ImageLoader cannot load image: %s", mstrFilePath.c_str());
				lResult = EResult::LoadFailed;
			}
			else
			{
				mImage = IImagePtr(lImage);
				mbLoaded = true;
			}
		}

		return lResult;
	}

	EResult CImageResource::Unload()
	{
		EResult lResult = EResult::Success;

		if (!mbLoaded)
			return lResult;

		mImage = nullptr;
		mbLoaded = false;

		return lResult;
	}

	EResult CImageResource::Reload()
	{
		if (mbLoaded)
			Unload();

		return Load();
	}
}