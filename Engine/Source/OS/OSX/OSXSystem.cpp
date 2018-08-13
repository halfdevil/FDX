
#include "OS/OSX/OSXSystem.h"
#include "OS/OSX/OSXFile.h"
#include "OS/OSX/OSXFileSystem.h"
#include "OS/OSX/OSXGameTimer.h"
#include "OS/OSX/OSXLibrary.h"
#include "GLFW/GLFWGameWindow.h"
#include "OS/GameLogger.h"
#include <sys/stat.h>
#include <unistd.h>

namespace FDX
{
    EResult COSXSystem::CombinePath(const String &pstrPathA,
                                    const String &pstrPathB,
                                    String &pstrOutPath)
    {
        EResult lResult = EResult::Success;

        if (pstrPathA.find(_T('/')) != String::npos)
        {
            if (pstrPathA[pstrPathA.size() - 1] == _T('/'))
                pstrOutPath = pstrPathA + pstrPathB;
            else
                pstrOutPath = pstrPathA + _T('/') + pstrPathB;
        }

        struct stat lStatBuf = {};

        if (stat(pstrOutPath.c_str(), &lStatBuf) != 0)
            lResult = EResult::PathDoesNotExist;
        else
            ConvertPath(pstrOutPath);

        return lResult;
    }

    EResult COSXSystem::GetDirectory(const FDX::String &pstrPath, FDX::String &pstrDirectory)
    {
        EResult lResult = EResult::Success;

        if (pstrPath.find_last_of(_T('/')) != String::npos)
            pstrDirectory = pstrPath.substr(0, pstrPath.find_last_of(_T('/')));
        else
            lResult = EResult::NotFound;

        return lResult;
    }

    EResult COSXSystem::GetFileName(const FDX::String &pstrPath, FDX::String &pstrFileName)
    {
        EResult lResult = EResult::Success;

        if (pstrPath.find_last_of(_T('/')) != String::npos)
            pstrFileName = pstrPath.substr(pstrPath.find_last_of(_T('/')) + 1);
        else
            pstrFileName = pstrPath;

        return lResult;
    }

    EResult COSXSystem::ConvertPath(FDX::String &pstrPath)
    {
        for (uint32_t lIdx = 0; lIdx < pstrPath.length(); lIdx++)
        {
            if (pstrPath[lIdx] == _T('\\'))
                pstrPath[lIdx] = _T('/');
        }

        return EResult::Success;
    }

    EResult COSXSystem::GetWorkingDirectory(FDX::String &pstrDirectory)
    {
        EResult lResult = EResult::Success;
        TCHAR lstrPath[FDX_MAX_PATH_LEN];

        if (getcwd(lstrPath, FDX_MAX_PATH_LEN) != nullptr)
            pstrDirectory = String(lstrPath);
        else
            lResult = EResult::Error;

        return lResult;
    }

    EResult COSXSystem::GetExecutableDirectory(FDX::String &pstrDirectory)
    {
        return GetWorkingDirectory(pstrDirectory);
    }

    EResult COSXSystem::ToString(const char *pstrText, String &pstrString)
    {
        EResult lResult = EResult::Success;

        if (pstrText != nullptr)
            pstrString = String(pstrText);
        else
            lResult = EResult::Error;

        return lResult;
    }

    EResult COSXSystem::ToString(const wchar_t *pstrText, String &pstrString)
    {
        EResult lResult = EResult::Success;

        uint32_t luiSize = (uint32_t)wcslen(pstrText) * sizeof(wchar_t);
        auto lChars = (char*)malloc(luiSize);

        if ((int32_t)wcstombs(lChars, pstrText, luiSize) == -1)
            lResult = EResult::Error;
        else
            pstrString = String(lChars);

        return lResult;
    }

    EResult COSXSystem::CreateGameWindow(const CWindowParameters &pWindowParams,
                                         const CContextProperties &pContextProps,
                                         IGameWindow **pWindow)
    {
        FDX_ASSERT(pWindow != nullptr, _T("Passed parameter pWindow cannot null"));

        EResult lResult = EResult::Success;

        auto lWindow = new CGLFWGameWindow(pWindowParams);
        lResult = lWindow->Create(pContextProps);

        if (lResult != EResult::Success)
        {
            LOG_DEBUG(_T("GLFWGameWindow creation failed: %s"),
                      CResultHelper::Get().GetErrorString(lResult));
            SAFE_DELETE(lWindow);
        }
        else
        {
            *pWindow = lWindow;
        }

        return lResult;
    }

    EResult COSXSystem::CreateGameTimer(FDX::IGameTimer **pTimer)
    {
        FDX_ASSERT(pTimer != nullptr, _T("Passed parameter pTimer cannot null"));

        *pTimer = new COSXGameTimer();
        return EResult::Success;
    }

    EResult COSXSystem::GetFileSystem(IFileSystem **pSystem)
    {
        FDX_ASSERT(pSystem != nullptr, _T("Passed parameter pSystem cannot null"));

        *pSystem = new COSXFileSystem();
        return EResult::Success;
    }

    EResult COSXSystem::CreateFileStream(EOpenMode pOpenMode, EAccessRights pAccessRights,
                                         const String &pstrFileName, IFileStream **pStream)
    {
        FDX_ASSERT(pStream != nullptr, _T("Passed parameter pStream cannot null"));

        EResult lResult = EResult::Success;

        auto lStream = new COSXFile(pstrFileName);
        lResult = lStream->Create(pOpenMode, pAccessRights);

        if (lResult != EResult::Success)
        {
            LOG_DEBUG(_T("OSXFile() failed for: %s"), pstrFileName.c_str());
            SAFE_DELETE(lStream);
        }
        else
        {
            *pStream = lStream;
        }

        return lResult;
    }

    EResult COSXSystem::Assert(bool pbCondition, const TCHAR *pstrFileName, uint32_t puiLineNo,
                               const TCHAR *pstrFormat, ...)
    {
        EResult lResult = EResult::Success;

        if (!pbCondition)
        {
            TCHAR lstrBuffer[FDX_MAX_BUFFER_LEN];
            TCHAR lstrString[FDX_MAX_STRING_LEN];

            va_list lArgs;

            va_start(lArgs, pstrFormat);
            vsprintf(lstrBuffer, pstrFormat, lArgs);
            va_end(lArgs);

            sprintf(lstrString, _T("Assert failed.\r\n\r\n"
                                      "at %s(%d)"
                                      "Message: %s\r\n\r\n"),
                       pstrFileName, puiLineNo, lstrBuffer);

            assert(pbCondition && lstrString);
        }

        return lResult;
    }

    EResult COSXSystem::GetLibrary(const String &pstrFileName, ILibrary **pLibrary)
    {
        EResult lResult = EResult::Success;

        auto lLibrary = new COSXLibrary(pstrFileName);
        lResult = lLibrary->Load();

        if (lResult != EResult::Success)
        {
            LOG_DEBUG(_T("COSXLibrary::Load() failed: %s"), pstrFileName.c_str());
        }
        else
        {
            *pLibrary = lLibrary;
        }

        return lResult;
    }

    void COSXSystem::Sleep(uint32_t puiMilliSeconds)
    {
        sleep(puiMilliSeconds);
    }

    void COSXSystem::PollEvents()
    {
        CGLFWGameWindow::PollEvents();
    }
}
