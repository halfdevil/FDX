
#include "Core/Result.h"
#include <stdarg.h>

namespace FDX
{
	void CResultHelper::Initialize()
	{
		mErrorStringMap[EResult::Success] = "Success";
		mErrorStringMap[EResult::Error] = "Error";
		mErrorStringMap[EResult::LoadFailed] = "LoadFailed";
		mErrorStringMap[EResult::OpenFailed] = "OpenFailed";
		mErrorStringMap[EResult::ReadFailed] = "ReadFailed";
		mErrorStringMap[EResult::WriteFailed] = "WriteFailed";
		mErrorStringMap[EResult::AccessDenied] = "AccessDenied";
		mErrorStringMap[EResult::Unknown] = "Unknown";

		mErrorDescriptionMap[EResult::Success] = "Success";
		mErrorDescriptionMap[EResult::Error] = "Error";
		mErrorDescriptionMap[EResult::LoadFailed] = "LoadFailed";
		mErrorDescriptionMap[EResult::OpenFailed] = "OpenFailed";
		mErrorDescriptionMap[EResult::ReadFailed] = "ReadFailed";
		mErrorDescriptionMap[EResult::WriteFailed] = "WriteFailed";
		mErrorDescriptionMap[EResult::AccessDenied] = "AccessDenied";
		mErrorDescriptionMap[EResult::Unknown] = "Unknown";
	}

	const char* CResultHelper::GetErrorString(EResult pResult)
	{
		return "";
	}

	const char* CResultHelper::GetErrorDescription(EResult pResult)
	{
		return "";
	}
}