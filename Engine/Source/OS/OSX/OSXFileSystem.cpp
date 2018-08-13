
#include "OS/OSX/OSXFileSystem.h"
#include "OS/OSX/OSXDirectory.h"
#include "OS/OSX/OSXSystem.h"
#include "OS/GameLogger.h"

namespace FDX
{
    EResult COSXFileSystem::OpenDirectory(EOpenMode pOpenMode, const String &pstrName,
                                          const String &pstrPath, CDirectory **pDirectory)
    {
        FDX_ASSERT(pDirectory != nullptr, _T("Passed parameter pDirectory cannot be null"));

        EResult lResult = EResult::Success;

        auto lDirectory = new COSXDirectory(pstrName, pstrPath);
        lResult = lDirectory->Create(pOpenMode);

        if (lResult != EResult::Success)
        {
            LOG_DEBUG(_T("COSXDirectory::Create() failed for: %s"), pstrPath.c_str());
            SAFE_DELETE(lDirectory);
        }
        else
        {
            *pDirectory = lDirectory;
        }

        return lResult;
    }

    EResult COSXFileSystem::OpenArchive(const String &pstrName, const String &pstrPath,
                                        EArchiveType pArchiveType, CArchive **pArchive)
    {
        return EResult::Success;
    }

    EResult COSXFileSystem::AddMountPoint(const String &pstrAlias, IMountPoint *pMountPoint)
    {
        FDX_ASSERT(pMountPoint != nullptr, _T("Passed pointer pMountPoint cannot be null"));

        EResult lResult = EResult::Success;
        auto lIt = mMountPoints.find(pstrAlias);

        if (lIt != mMountPoints.end())
            lResult = EResult::AddFailed;
        else
        {
            mMountPoints[pstrAlias] = IMountPointPtr(pMountPoint);
        }

        return lResult;
    }

    EResult COSXFileSystem::RemoveMountPoint(const String &pstrAlias)
    {
        EResult lResult = EResult::Success;
        auto lIt = mMountPoints.find(pstrAlias);

        if (lIt == mMountPoints.end())
        {
            lResult = EResult::RemoveFailed;
        }
        else
        {
            mMountPoints.erase(lIt);
        }

        return lResult;
    }

    EResult COSXFileSystem::GetMountPoint(const String &pstrAlias, IMountPoint **pMountPoint)
    {
        EResult lResult = EResult::Success;
        auto lIt = mMountPoints.find(pstrAlias);

        if (lIt == mMountPoints.end())
            lResult = EResult::Error;
        else
        {
            *pMountPoint = ((*lIt).second).get();
        }

        return lResult;
    }

    EResult COSXFileSystem::GetStream(const String &pstrFileName, IFileStream **pStream)
    {
        EResult lResult = EResult::Success;
        IMountPoint *lMountPoint = nullptr;
        IFileStream *lStream = nullptr;
        String lstrActualPath;

        auto lIt = mMountPoints.begin();
        while (lIt != mMountPoints.end())
        {
            String lstrAlias = (*lIt).first;
            if (pstrFileName.compare(0, lstrAlias.length(), lstrAlias) == 0)
            {
                lMountPoint = (*lIt).second.get();
                lstrActualPath = pstrFileName.substr(lstrAlias.size());

                break;
            }

            lIt++;
        }

        if (lMountPoint == nullptr)
        {
            LOG_ERROR(_T("No mount point found for the path: %s"), pstrFileName.c_str());
            lResult = EResult::NotFound;
        }
        else
        {
            lResult = lMountPoint->GetStream(lstrActualPath, &lStream);
            if (lResult != EResult::Success)
            {
                LOG_ERROR(_T("Unable to get stream %s from mountpoint: %s"),
                          pstrFileName.c_str(),
                          lMountPoint->GetName().c_str());

                lResult = EResult::NotFound;
            }
            else
            {
                *pStream = lStream;
            }
        }

        return lResult;
    }
}