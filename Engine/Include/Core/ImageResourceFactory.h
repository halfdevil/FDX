#pragma once

#include "Core/ResourceFactory.h"
#include "Core/ImageResource.h"

namespace FDX
{
	class FDX_EXPORT CImageResourceFactory : public IResourceFactory
	{
	public:

		CImageResourceFactory();
		~CImageResourceFactory();

		CImageResourceFactory(const CImageResourceFactory&) = delete;
		CImageResourceFactory& operator=(const CImageResourceFactory&) = delete;

		EResourceType GetResourceType() const;

		bool IsSupported(const String &pstrFilePath);
		EResult Build(const String &pstrFilePath,
			IResource **pResource);

		template <typename T>
		EResult RegisterLoader(EImageLoaderType puiImageLoaderType, T **pLoader);

	private:

		Array<SmartPtr<IImageLoader>> mImageLoaders;
	};

	template<typename T>
	inline EResult CImageResourceFactory::RegisterLoader(EImageLoaderType puiImageLoaderType, T **pLoader)
	{
		EResult lResult = EResult::Success;

		auto lIt = mImageLoaders.begin();
		while (lIt != mImageLoaders.end())
		{
			if ((*lIt)->GetImageLoaderType() == puiImageLoaderType)
			{
				lResult = EResult::AddFailed;
				break;
			}

			lIt++;
		}

		if (lIt == mImageLoaders.end())
		{
			*pLoader = new T();
			mImageLoaders.push_back(IImageLoaderPtr(*pLoader));
		}

		return lResult;
	}
}
