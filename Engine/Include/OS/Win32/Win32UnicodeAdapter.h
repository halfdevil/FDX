#pragma once

#include "Core/Core.h"
#include <Windows.h>

namespace FDX
{
	class CWin32UTF16Adapter
	{
	public:

		explicit CWin32UTF16Adapter(const String &pstrText)
		{
			int32_t liNumWchars = MultiByteToWideChar(CP_UTF8, 0, 
				pstrText.c_str(), -1, nullptr, 0);

			wchar_t *lstrWchars = new wchar_t[liNumWchars];
			MultiByteToWideChar(CP_UTF8, 0, pstrText.c_str(), 
				-1, lstrWchars, liNumWchars);

			mstrText = UTF16String(lstrWchars);
		}

		UTF16String&& Get()
		{
			return std::move(mstrText);
		}

	private:

		UTF16String mstrText;
	};

	class CWin32UTF8Adapter
	{
	public:

		explicit CWin32UTF8Adapter(const UTF16String &pstrText)
		{
			int32_t liNumChars = WideCharToMultiByte(CP_UTF8, 0,
				pstrText.c_str(), -1, nullptr, 0, nullptr, nullptr);

			char *lstrChars = new char[liNumChars];
			WideCharToMultiByte(CP_UTF8, 0, pstrText.c_str(),
				-1, lstrChars, liNumChars, nullptr, nullptr);

			mstrText = UTF8String(lstrChars);
		}

		UTF8String&& Get()
		{
			return std::move(mstrText);
		}

	private:

		UTF8String mstrText;
	};
}