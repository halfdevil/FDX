#pragma once

#include "FS/Directory.h"
#include <Windows.h>

namespace FDX
{
	class FDX_EXPORT CWin32Directory : public CDirectory
	{
	public:

		CWin32Directory(const String &pstrName, const String &pstrPath);
		~CWin32Directory();

		EResult Create(EOpenMode pOpenMode);
		bool IsExist(const String &pstrFileName) override;

		EResult CreateStreamInternal(const String &pstrFileName,
			EOpenMode pOpenMode,
			EAccessRights pAccessRights,
			IFileStream **pStream) override; 
	};
}


