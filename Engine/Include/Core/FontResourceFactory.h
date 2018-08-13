#pragma once

#include "Core/ResourceFactory.h"
#include "Core/FontResource.h"

namespace FDX
{
	class FDX_EXPORT CFontResourceFactory : public IResourceFactory
	{
	public:

		CFontResourceFactory();
		~CFontResourceFactory();

		CFontResourceFactory(const CFontResourceFactory&) = delete;
		CFontResourceFactory& operator=(const CFontResourceFactory&) = delete;

		EResourceType GetResourceType() const;

		bool IsSupported(const String &pstrFilePath);
		EResult Build(const String &pstrFilePath,
			IResource **pResource);

		template <typename T>
		EResult RegisterLoader(EFontLoaderType puiFontLoaderType, T **pLoader);

	private:

		Array<IFontLoaderPtr> mFontLoaders;
	};

	template<typename T>
	inline EResult CFontResourceFactory::RegisterLoader(EFontLoaderType puiFontLoaderType, T **pLoader)
	{
		EResult lResult = EResult::Success;

		auto lIt = mFontLoaders.begin();
		while (lIt != mFontLoaders.end())
		{
			if ((*lIt)->GetFontLoaderType() == puiFontLoaderType)
			{
				lResult = EResult::AddFailed;
				break;
			}

			lIt++;
		}

		if (lIt == mFontLoaders.end())
		{
			*pLoader = new T();
			mFontLoaders.push_back(IFontLoaderPtr(*pLoader));
		}

		return lResult;
	}
}