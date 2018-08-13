#pragma once

#include "OS/System.h"

namespace FDX
{
    class FDX_EXPORT COSXSystem : public ISystem
    {
    public:

        COSXSystem() = default;
        ~COSXSystem() override = default;

        EResult CombinePath(const String &pstrPathA,
                            const String &pstrPathB, String &pstrOutPath) override;

        EResult GetDirectory(const String &pstrPath, String &pstrDirectory) override;
        EResult GetFileName(const String &pstrPath, String &pstrFileName) override;

        EResult ConvertPath(String &pstrPath) override;
        EResult GetWorkingDirectory(String &pstrDirectory) override;
        EResult GetExecutableDirectory(String &pstrDirectory) override;

        EResult ToString(const char *pstrText, String &pstrString) override;
        EResult ToString(const wchar_t *pstrText, String &pstrString) override;

        EResult CreateGameWindow(const CWindowParameters &pWindowParams,
                                 const CContextProperties &pContextProps,
                                 IGameWindow **pWindow) override;

        EResult CreateGameTimer(IGameTimer **pTimer) override;
        EResult GetFileSystem(IFileSystem **pSystem) override;

        EResult CreateFileStream(EOpenMode pOpenMode,
                                 EAccessRights pAccessRights,
                                 const String &pstrFileName,
                                 IFileStream **pStream) override;

        EResult Assert(bool pbCondition,
                       const TCHAR *pstrFileName,
                       uint32_t puiLineNo,
                       const TCHAR *pstrFormat, ...) override;

        EResult GetLibrary(const String &pstrFileName,
                           ILibrary **pLibrary) override;

        void Sleep(uint32_t puiMilliSeconds) override;
        void PollEvents() override;
    };
}