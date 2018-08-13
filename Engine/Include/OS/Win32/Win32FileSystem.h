#pragma once

#include "FS/FileSystem.h"

namespace FDX
{
	class FDX_EXPORT CWin32FileSystem : public IFileSystem
	{
	public:

		CWin32FileSystem();
		~CWin32FileSystem() = default;

		CWin32FileSystem(const CWin32FileSystem&) = delete;
		CWin32FileSystem& operator=(const CWin32FileSystem&) = delete;

		EResult OpenDirectory(EOpenMode pOpenMode, 
			const String &pstrName,
			const String &pstrPath,
			CDirectory **pDirectory);

		EResult OpenArchive(const String &pstrName,
			const String &pstrPath,
			EArchiveType pArchiveType,
			CArchive **pArchive);

		EResult AddMountPoint(const String &pstrAlias,
			IMountPoint *pMountPoint);

		EResult RemoveMountPoint(const String &pstrAlias);

		EResult GetMountPoint(const String &pstrAlias,
			IMountPoint **pMountPoint);

		EResult GetStream(const String &pstrFileName,
			IFileStream **pStream);

		bool IsExist(const String &pstrFileName) override;
		char GetPathSeperator() const override;

	private:

		UnorderedMap<String, IMountPointPtr> mMountPoints;
	};
}