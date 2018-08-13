
#include "Core/FontResource.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CFontResource::CFontResource(const String & pstrFilePath, IFontLoader * pLoader)
		: mstrFilePath(pstrFilePath),
		mFontLoader(pLoader),
		mFont(nullptr),
		mbLoaded(false)
	{
	}

	CFontResource::~CFontResource()
	{
	}

	EResult CFontResource::Load()
	{
		EResult lResult = EResult::Success;

		if (mbLoaded)
			return lResult;

		if (!mFontLoader->CanLoadFont(mstrFilePath))
		{
			LOG_DEBUG("FontLoader cannot load font: %s", mstrFilePath.c_str());
			lResult = EResult::LoadFailed;
		}
		else
		{
			IFont *lFont;
			lResult = mFontLoader->Load(mstrFilePath, &lFont);

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("FontLoader Load() failed: %s", mstrFilePath.c_str());
			}
			else
			{
				mFont = IFontPtr(lFont);
				mbLoaded = true;
			}
		}

		return lResult;
	}

	EResult CFontResource::Unload()
	{
		EResult lResult = EResult::Success;

		if (!mbLoaded)
			return lResult;

		mFont = nullptr;
		mbLoaded = false;

		return lResult;
	}

	EResult CFontResource::Reload()
	{
		if (mbLoaded)
			Unload();

		return Load();
	}
}