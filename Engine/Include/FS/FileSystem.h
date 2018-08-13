#pragma once

#include "FS/Directory.h"
#include "FS/Archive.h"
#include "Core/Singleton.h"

namespace FDX
{
	class FDX_EXPORT IFileSystem : public CSingleton<IFileSystem>
	{
	public:

		virtual ~IFileSystem()
		{
		}

		virtual EResult OpenDirectory(EOpenMode pOpenMode,
			const String &pstrName,
			const String &pstrPath,
			CDirectory **pDirectory) = 0;

		virtual EResult OpenArchive(const String &pstrName,
			const String &pstrPath,
			EArchiveType pArchiveType,
			CArchive **pArchive) = 0;

		virtual EResult AddMountPoint(const String &pstrAlias,
			IMountPoint *pMountPoint) = 0;

		virtual EResult RemoveMountPoint(const String &pstrAlias) = 0;

		virtual EResult GetMountPoint(const String &pstrAlias,
			IMountPoint **pMountPoint) = 0;

		virtual EResult GetStream(const String &pstrFileName,
			IFileStream **pStream) = 0;		

		virtual bool IsExist(const String &pstrFileName) = 0;
		virtual char GetPathSeperator() const = 0;
	};

	using IFileSystemPtr = SmartPtr<IFileSystem>;
}
