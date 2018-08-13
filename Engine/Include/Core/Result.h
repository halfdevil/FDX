#pragma once

#include "Core/Core.h"
#include "Core/Singleton.h"

namespace FDX
{
	enum class EResult
	{
		Success = 0,
		Error,
		LoadFailed,
		OpenFailed,
		AccessDenied,
		WriteFailed,
		ReadFailed,
		CreateFailed,
		PathDoesNotExist,
		AddFailed,
		RemoveFailed,
		NotFound,
		ActivateFailed,
		RegisterFailed,
		UploadFailed,
		MapFailed,
		UnmapFailed,
		BindFailed,
		ApplyFailed,
		Unknown
	};

	class FDX_EXPORT CResultHelper : public CSingleton<CResultHelper>
	{
	public:

		void Initialize();

		const char* GetErrorString(EResult pResult);
		const char* GetErrorDescription(EResult pResult);

	private:

		Map<EResult, String> mErrorStringMap;
		Map<EResult, String> mErrorDescriptionMap;
	};
}