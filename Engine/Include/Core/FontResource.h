#pragma once

#include "Core/Resource.h"
#include "Core/FontLoader.h"

namespace FDX
{
	class FDX_EXPORT CFontResource : public IResource
	{
	public:

		CFontResource(const String &pstrFilePath, IFontLoader *pLoader);
		~CFontResource();

		const String& GetFilePath() const
		{
			return mstrFilePath;
		}

		EResourceType GetResourceType() const override
		{
			return EResourceType::Font;
		}

		bool IsLoaded() const override
		{
			return mbLoaded;
		}

		IFontLoader* GetFontLoader() const
		{
			return mFontLoader;
		}

		IFont* GetFont() const
		{
			return mFont.get();
		}

		EResult Load() override;
		EResult Unload() override;
		EResult Reload() override;

	private:

		String mstrFilePath;
		IFontLoader *mFontLoader;
		IFontPtr mFont;
		bool mbLoaded;
	};
}