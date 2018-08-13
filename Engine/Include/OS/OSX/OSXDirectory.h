#pragma once

#include "FS/Directory.h"
#include <dirent.h>

namespace FDX
{
    class FDX_EXPORT COSXDirectory : public CDirectory
    {
    public:

        COSXDirectory(const String &pstrName, const String &pstrPath);
        ~COSXDirectory() override = default;

        EResult Create(EOpenMode pOpenMode);
        EResult IsFileExist(const String &pstrFileName);

        EResult CreateStreamInternal(const String &pstrFileName,
                                     EOpenMode pOpenMode,
                                     EAccessRights pAccessRights,
                                     IFileStream **pStream) override;

    private:

        String mstrPath;
    };
}
