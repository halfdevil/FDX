
#include "OS/OSX/OSXFile.h"
#include "OS/GameLogger.h"
#include "OS/System.h"

namespace FDX
{
    COSXFile::COSXFile(const String &pstrFileName)
        : mstrFileName(pstrFileName),
          mHandle(nullptr),
          mOpenMode(EOpenMode::Open),
          mAccessRights(EAccessRights::Read)
    {
    }

    COSXFile::~COSXFile()
    {
        if (mHandle != nullptr)
        {
            fclose(mHandle);
            mHandle = nullptr;
        }
    }

    EResult COSXFile::Create(EOpenMode pOpenMode, EAccessRights pAccessRights)
    {
        EResult lResult = EResult::Success;
        String lstrMode;

        if (pOpenMode == EOpenMode::Open)
        {
            if (pAccessRights == EAccessRights::Read)
                lstrMode = _T("rb");
            else if (pAccessRights == EAccessRights::Write)
                lstrMode = _T("rb+");
            else if (pAccessRights == EAccessRights::ReadWrite)
                lstrMode = _T("rb+");
        }
        else if (pOpenMode == EOpenMode::Create)
        {
            if (pAccessRights == EAccessRights::Read)
                lstrMode = _T("wb+");
            else if (pAccessRights == EAccessRights::Write)
                lstrMode = _T("wb");
            else if (pAccessRights == EAccessRights::ReadWrite)
                lstrMode = _T("wb+");
        }
        else if (pOpenMode == EOpenMode::Append)
        {
            if (pAccessRights == EAccessRights::Read)
                lstrMode = _T("ab+");
            else if (pAccessRights == EAccessRights::Write)
                lstrMode = _T("ab");
            else if (pAccessRights == EAccessRights::ReadWrite)
                lstrMode = _T("ab+");
        }

        mHandle = fopen(mstrFileName.c_str(), lstrMode.c_str());
        if (mHandle == nullptr)
        {
            LOG_DEBUG(_T("fopen() failed for %s"), mstrFileName.c_str());
            lResult = EResult::CreateFailed;
        }

        return lResult;
    }

    uint32_t COSXFile::GetPosition() const
    {
        FDX_ASSERT(mHandle != nullptr, _T("File handle is invalid"));
        return (uint32_t)ftell(mHandle);
    }

    uint32_t COSXFile::GetSize() const
    {
        FDX_ASSERT(mHandle != nullptr, _T("File handle is invalid"));

        auto luiCurrPosition = (uint32_t)ftell(mHandle);
        fseek(mHandle, 0, SEEK_END);

        auto luiSize = (uint32_t)ftell(mHandle);
        fseek(mHandle, luiCurrPosition, SEEK_SET);

        return luiSize;
    }

    int32_t COSXFile::Seek(ESeekOrigin pSeekOrigin, int32_t offset)
    {
        FDX_ASSERT(mHandle != nullptr, _T("File handle is invalid"));
        int32_t liSeekType = 0;

        if (pSeekOrigin == ESeekOrigin::Begining)
            liSeekType = SEEK_SET;
        else if (pSeekOrigin == ESeekOrigin::Current)
            liSeekType = SEEK_CUR;
        else if (pSeekOrigin == ESeekOrigin::End)
            liSeekType = SEEK_END;

        return (int32_t)fseek(mHandle, offset, liSeekType);
    }

    EResult COSXFile::Read(uint32_t puiSize, void *pBuffer)
    {
        FDX_ASSERT(mHandle != nullptr, _T("File handle is invalid"));

        EResult lResult = EResult::Success;
        auto lBytesRead = fread(pBuffer, 1, puiSize, mHandle);

        if (lBytesRead != puiSize)
        {
            LOG_DEBUG(_T("fread() failed for %s"), mstrFileName.c_str());
            lResult = EResult::ReadFailed;
        }

        return lResult;
    }

    EResult COSXFile::Write(uint32_t puiSize, const void *pBuffer)
    {
        FDX_ASSERT(mHandle != nullptr, _T("File handle is invalid"));

        EResult lResult = EResult::Success;
        auto lBytesWritten = fwrite(pBuffer, 1, puiSize, mHandle);

        if (lBytesWritten != puiSize)
        {
            LOG_DEBUG(_T("fwrite() failed for %s"), mstrFileName.c_str());
            lResult = EResult::WriteFailed;
        }

        return lResult;
    }

    EResult COSXFile::WriteString(const FDX::TCHAR *pstrFormat, ...)
    {
        FDX_ASSERT(mHandle != nullptr, _T("File handle is invalid"));

        EResult lResult = EResult::Success;
        va_list pArgs;

        va_start(pArgs, pstrFormat);
        lResult = WriteString(pstrFormat, pArgs);
        va_end(pArgs);

        return lResult;
    }

    EResult COSXFile::WriteString(const FDX::TCHAR *pstrFormat, va_list pArgs)
    {
        FDX_ASSERT(mHandle != nullptr, _T("File handle is invalid"));

        EResult lResult = EResult::Success;
        TCHAR lstrBuffer[FDX_MAX_STRING_LEN + 1];

        vsprintf(lstrBuffer, pstrFormat, pArgs);
        lResult = Write(((uint32_t)strlen(lstrBuffer) + 1) * sizeof(TCHAR), lstrBuffer);

        if (lResult != EResult::Success)
        {
            LOG_DEBUG(_T("Write failed inside WriteString: %s"),
                      CResultHelper::Get().GetErrorString(lResult));
        }

        return lResult;
    }

    EResult COSXFile::Flush()
    {
        FDX_ASSERT(mHandle != nullptr, _T("File handle is invalid"));

        return fflush(mHandle) != 0 ? EResult::Error :
               EResult::Success;
    }
}

