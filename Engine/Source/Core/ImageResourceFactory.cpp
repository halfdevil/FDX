
#include "Core/ImageResourceFactory.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CImageResourceFactory::CImageResourceFactory()
	{
	}

	CImageResourceFactory::~CImageResourceFactory()
	{
		mImageLoaders.clear();
	}

	EResourceType CImageResourceFactory::GetResourceType() const
	{
		return EResourceType::Image;
	}

	bool CImageResourceFactory::IsSupported(const String & pstrFilePath)
	{
		bool lbRet = false;

		auto lIt = mImageLoaders.begin();
		while (lIt != mImageLoaders.end())
		{
			if ((*lIt)->CanLoadImage(pstrFilePath))
			{
				lbRet = true;
				break;
			}

			lIt++;
		}

		return lbRet;
	}

	EResult CImageResourceFactory::Build(const String & pstrFilePath, IResource ** pResource)
	{
		EResult lResult = EResult::LoadFailed;		

		auto lIt = mImageLoaders.begin();
		while (lIt != mImageLoaders.end())
		{
			if ((*lIt)->CanLoadImage(pstrFilePath))
			{
				*pResource = new CImageResource(pstrFilePath, (*lIt).get());
				lResult = EResult::Success;
				break;
			}

			lIt++;
		}

		return lResult;
	}
}