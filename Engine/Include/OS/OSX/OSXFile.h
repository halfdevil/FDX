#pragma once

#include "FS/FileStream.h"
#include <stdio.h>

namespace FDX
{
    class COSXFile : public IFileStream
    {
    public:

        explicit COSXFile(const String &pstrFileName);
        ~COSXFile() override;

        virtual EResult Create(EOpenMode pOpenMode,
                               EAccessRights pAccessRights);

        EOpenMode GetOpenMode() const override
        {
            return mOpenMode;
        }

        EAccessRights GetAccessRights() const override
        {
            return mAccessRights;
        }

        bool CanRead() const override
        {
            return (mAccessRights == EAccessRights::Read ||
                    mAccessRights == EAccessRights::ReadWrite);
        }

        bool CanWrite() const override
        {
            return (mAccessRights == EAccessRights::Write ||
                    mAccessRights == EAccessRights::ReadWrite);
        }

        uint32_t GetPosition() const override;
        uint32_t GetSize() const override;

        int32_t Seek(ESeekOrigin pSeekOrigin, int32_t offset) override;

        EResult Read(uint32_t puiSize, void *pBuffer) override;
        EResult Write(uint32_t puiSize, const void *pBuffer) override;

        EResult WriteString(const TCHAR *pstrFormat, ...) override;
        EResult WriteString(const TCHAR *pstrFormat, va_list pArgs) override;

        EResult Flush() override;

    protected:

        EOpenMode mOpenMode;
        EAccessRights mAccessRights;
        String mstrFileName;
        FILE *mHandle;
    };
}