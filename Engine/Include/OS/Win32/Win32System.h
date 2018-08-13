#pragma once

#include "OS/System.h"
#include <Windows.h>

namespace FDX
{
	class FDX_EXPORT CWin32System : public ISystem
	{
	public:

		CWin32System(HINSTANCE pInstance);
		~CWin32System() override;

		HINSTANCE GetInstance() const
		{
			return mInstance;
		}

		EResult Initialize();
		EResult Terminate();

		EResult CombinePath(const String &pstrPathA,
			const String &pstrPathB, String &pstrOutPath) override;

		EResult GetDirectory(const String &pstrPath, String &pstrDirectory) override;
		EResult GetFileName(const String &pstrPath, String &pstrFileName) override;
		EResult GetExtension(const String &pstrPath, String &pstrExtension) override;

		EResult ConvertPath(String &pstrPath) override;
		EResult GetWorkingDirectory(String &pstrDirectory) override;
		EResult GetExecutableDirectory(String &pstrDirectory) override;

		EResult CreateGameWindow(const CWindowParameters &pParameters,
			const CContextProperties &pContextProps,
			IGameWindow **pWindow) override;

		EResult CreateGameTimer(IGameTimer **pTimer) override;
		EResult GetFileSystem(IFileSystem **pSystem) override;

		EResult CreateFileStream(EOpenMode pOpenMode,
			EAccessRights pAccessRights,
			const String &pstrFileName,
			IFileStream **pStream) override;

		EResult Assert(bool pbCondition,
			const char *pstrFileName,
			uint32_t puiLineNo,
			const char *pstrFormat, ...) override;

		EResult GetLibrary(const String &pstrFileName,
			ILibrary **pLibrary) override;

		void Sleep(uint32_t puiMilliSeconds) override;
		void PollEvents() override;

	private:

		HINSTANCE mInstance;
	};
}