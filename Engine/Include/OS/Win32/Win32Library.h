#pragma once

#include "OS/Library.h"
#include "Core/Result.h"
#include <Windows.h>

namespace FDX
{
	class FDX_EXPORT CWin32Library : public ILibrary
	{
	public:

		CWin32Library(const String &pstrFileName);
		~CWin32Library();

		const String& GetFileName() const
		{
			return mstrFileName;
		}

		void* GetFunctionAddress(const char *pstrFuncName);

		EResult Load();
		EResult Unload();

	private:

		HMODULE mDLL;
		String mstrFileName;
	};
}
