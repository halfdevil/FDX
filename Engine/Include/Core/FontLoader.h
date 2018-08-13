#pragma once

#include "Core/Font.h"

namespace FDX
{
	enum class EFontLoaderType
	{
		FreeType
	};

	class FDX_EXPORT IFontLoader
	{
	public:

		virtual ~IFontLoader()
		{
		}

		virtual EFontLoaderType GetFontLoaderType() const = 0;

		virtual bool CanLoadFontType(EFontType pType) = 0;
		virtual bool CanLoadFont(const String &pstrFilePath) = 0;

		virtual EResult Load(const String &pstrFilePath,
			IFont **pFont) = 0;
	};

	using IFontLoaderPtr = SmartPtr<IFontLoader>;
}