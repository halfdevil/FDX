
#include "OS/OSX/OSXDirectory.h"
#include "OS/OSX/OSXFile.h"
#include "OS/GameLogger.h"
#include <sys/stat.h>

namespace FDX
{
    COSXDirectory::COSXDirectory(const String &pstrName, const String &pstrPath)
        : CDirectory(pstrName),
          mstrPath(pstrPath)
    {
    }

    EResult COSXDirectory::Create(EOpenMode pOpenMode)
    {
        EResult lResult = EResult::Success;
        struct stat lStatBuf = {};

        if (pOpenMode == EOpenMode::Open)
        {
            if (stat(mstrPath.c_str(), &lStatBuf) != 0)
            {
                lResult = EResult::CreateFailed;
            }
            else
            {
                if ((lStatBuf.st_mode & (uint32_t)S_IFDIR) == 0)
                {
                    lResult = EResult::CreateFailed;
                }
            }
        }
        else
        {
            if (mkdir(mstrPath.c_str(), S_IRWXU) != 0)
            {
                LOG_DEBUG(_T("Unable to create directory: %s"), mstrPath.c_str());
                lResult = EResult::CreateFailed;
            }
        }

        return lResult;
    }

    EResult COSXDirectory::IsFileExist(const String &pstrFileName)
    {
        EResult lResult = EResult::Success;
        String lstrFullPath;
        struct stat lStatBuf = {};

        if (pstrFileName[0] == _T('/'))
            lstrFullPath = mstrPath + pstrFileName;
        else
            lstrFullPath = mstrPath + _T('/') + pstrFileName;

        if (stat(lstrFullPath.c_str(), &lStatBuf) != 0)
        {
            LOG_DEBUG(_T("stat failed for file: %s"), lstrFullPath.c_str());
            lResult = EResult::CreateFailed;
        }

        return lResult;
    }

    EResult COSXDirectory::CreateStreamInternal(const String &pstrFileName,
                                                EOpenMode pOpenMode,
                                                EAccessRights pAccessRights,
                                                IFileStream **pStream)
    {
        EResult lResult = EResult::Success;
        String lstrAbsouleFileName;

        if (IsFileExist(pstrFileName) != EResult::Success)
        {
            LOG_DEBUG(_T("File %s does not exist"), lstrAbsouleFileName.c_str());
            lResult = EResult::CreateFailed;
        }
        else
        {
            if (mstrPath[0] == _T('/'))
                lstrAbsouleFileName = mstrPath + pstrFileName;
            else
                lstrAbsouleFileName = mstrPath + _T("/") + pstrFileName;

            auto lFile = new COSXFile(lstrAbsouleFileName);
            lResult = lFile->Create(pOpenMode, pAccessRights);

            if (lResult != EResult::Success)
            {
                LOG_DEBUG(_T("Unable to create Stream: %s"),
                          CResultHelper::Get().GetErrorString(lResult));
                SAFE_DELETE(lFile);

                return lResult;
            }
            else
            {
                *pStream = lFile;
            }
        }

        return lResult;
    }
}