#pragma once

#include "FS/FileSystem.h"

namespace FDX
{
    class FDX_EXPORT COSXFileSystem : public IFileSystem
    {
    public:

        COSXFileSystem() = default;
        ~COSXFileSystem() override = default;

        EResult OpenDirectory(EOpenMode pOpenMode,
                              const String &pstrName,
                              const String &pstrPath,
                              CDirectory **pDirectory) override;

        EResult OpenArchive(const String &pstrName,
                            const String &pstrPath,
                            EArchiveType pArchiveType,
                            CArchive **pArchive) override;

        EResult AddMountPoint(const String &pstrAlias,
                              IMountPoint *pMountPoint) override;

        EResult RemoveMountPoint(const String &pstrAlias) override;

        EResult GetMountPoint(const String &pstrAlias,
                              IMountPoint **pMountPoint) override;

        EResult GetStream(const String &pstrFileName,
                          IFileStream **pStream) override;

    private:

        UnorderedMap<String, IMountPointPtr> mMountPoints;
    };
}