
#include "OS/Win32/Win32Library.h"
#include "OS/Win32/Win32UnicodeAdapter.h"
#include "OS/System.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CWin32Library::CWin32Library(const String & pstrFileName)
		: mstrFileName(pstrFileName), mDLL(nullptr)
	{
	}

	CWin32Library::~CWin32Library()
	{
		Unload();
	}

	void * CWin32Library::GetFunctionAddress(const char * pstrFuncName)
	{
		FDX_ASSERT(mDLL != nullptr, "DLL %s not loaded", mstrFileName.c_str());
		return GetProcAddress(mDLL, pstrFuncName);
	}

	EResult CWin32Library::Load()
	{
		EResult lResult = EResult::Success;

		mDLL = LoadLibrary(CWin32UTF16Adapter(mstrFileName).Get().c_str());
		if (mDLL == nullptr)
		{
			LOG_ERROR("Unable to load library: %s", mstrFileName.c_str());
			lResult = EResult::LoadFailed;
		}

		return lResult;
	}

	EResult CWin32Library::Unload()
	{
		EResult lResult = EResult::Success;

		if (mDLL == nullptr)
			return lResult;

		if (!FreeLibrary(mDLL))
		{
			LOG_ERROR("Unable to unload library: %s", mstrFileName.c_str());
			lResult = EResult::LoadFailed;
		}

		return lResult;
	}
}