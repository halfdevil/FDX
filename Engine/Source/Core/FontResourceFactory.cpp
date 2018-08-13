
#include "Core/FontResourceFactory.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CFontResourceFactory::CFontResourceFactory()
	{
	}

	CFontResourceFactory::~CFontResourceFactory()
	{
		mFontLoaders.clear();
	}

	EResourceType CFontResourceFactory::GetResourceType() const
	{
		return EResourceType::Font;
	}

	bool CFontResourceFactory::IsSupported(const String & pstrFilePath)
	{
		bool lbRet = false;

		auto lIt = mFontLoaders.begin();
		while (lIt != mFontLoaders.end())
		{
			if ((*lIt)->CanLoadFont(pstrFilePath))
			{
				lbRet = true;
				break;
			}

			lIt++;
		}

		return lbRet;
	}

	EResult CFontResourceFactory::Build(const String & pstrFilePath, IResource ** pResource)
	{
		EResult lResult = EResult::LoadFailed;

		auto lIt = mFontLoaders.begin();
		while (lIt != mFontLoaders.end())
		{
			if ((*lIt)->CanLoadFont(pstrFilePath))
			{
				*pResource = new CFontResource(pstrFilePath, (*lIt).get());
				lResult = EResult::Success;
				break;
			}

			lIt++;
		}

		return lResult;
	}
}