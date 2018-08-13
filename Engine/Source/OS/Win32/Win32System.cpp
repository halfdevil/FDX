#include "OS/Win32/Win32System.h"
#include "OS/Win32/Win32File.h"
#include "OS/Win32/Win32Library.h"
#include "OS/Win32/Win32FileSystem.h"
#include "OS/Win32/Win32GameTimer.h"
#include "OS/Win32/Win32UnicodeAdapter.h"
#include "OS/GameLogger.h"
#include "GLFW/GLFWGameWindow.h"
#include <Shlwapi.h>

namespace FDX
{
	CWin32System::CWin32System(HINSTANCE pInstance)
		: mInstance(pInstance)
	{
	}

	CWin32System::~CWin32System()
	{
	}

	EResult CWin32System::Initialize()
	{
		return EResult::Success;
	}

	EResult CWin32System::Terminate()
	{
		return EResult::Success;
	}

	EResult CWin32System::CombinePath(const String & pstrPathA,
		const String & pstrPathB, String & pstrOutPath)
	{
		EResult lResult = EResult::Success;

		if (pstrPathA.find("/") != String::npos)
		{
			if (pstrPathA[pstrPathA.size() - 1] == '/')
				pstrOutPath = pstrPathA + pstrPathB;
			else
				pstrOutPath = pstrPathA + '/' + pstrPathB;
		}
		else
		{
			if (pstrPathA[pstrPathA.size() - 1] == '\\')
				pstrOutPath = pstrPathA + pstrPathB;
			else
				pstrOutPath = pstrPathA + '\\' + pstrPathB;
		}

		if (!PathFileExists(CWin32UTF16Adapter(pstrOutPath).Get().c_str()))
			lResult = EResult::PathDoesNotExist;

		return lResult;
	}

	EResult CWin32System::GetDirectory(const String & pstrPath, String & pstrDirectory)
	{
		EResult lResult = EResult::Success;

		if (pstrPath.find_last_of("/") != String::npos)
			pstrDirectory = pstrPath.substr(0, pstrPath.find_last_of("/"));
		else if (pstrPath.find_last_of("\\") != String::npos)
			pstrDirectory = pstrPath.substr(0, pstrPath.find_last_of("\\"));
		else
			lResult = EResult::NotFound;

		return lResult;
	}

	EResult CWin32System::GetFileName(const String & pstrPath, String & pstrFileName)
	{
		EResult lResult = EResult::Success;

		if (pstrPath.find_last_of("/") != String::npos)
			pstrFileName = pstrPath.substr(pstrPath.find_last_of("/") + 1);
		else if (pstrPath.find_last_of("\\") != String::npos)
			pstrFileName = pstrPath.substr(pstrPath.find_last_of("\\") + 1);
		else
			pstrFileName = pstrPath;

		return lResult;
	}

	EResult CWin32System::GetExtension(const String & pstrPath, String & pstrExtension)
	{
		EResult lResult = EResult::Success;

		if (pstrPath.find_last_of(".") != String::npos)
			pstrExtension = pstrPath.substr(pstrPath.find_last_of("."));
		else
			pstrExtension = pstrPath;
		
		return lResult;
	}

	EResult CWin32System::ConvertPath(String & pstrPath)
	{
		for (uint32_t lIdx = 0; lIdx < pstrPath.length(); lIdx++)
		{
			if (pstrPath[lIdx] == '/')
				pstrPath[lIdx] = '\\';
		}

		return EResult::Success;
	}

	EResult CWin32System::GetWorkingDirectory(String & pstrDirectory)
	{
		EResult lResult = EResult::Success;
		wchar_t lstrPath[MAX_PATH];

		if (GetCurrentDirectory(MAX_PATH, lstrPath) > 0)
			pstrDirectory = CWin32UTF8Adapter(lstrPath).Get();
		else
			lResult = EResult::Error;

		return lResult;
	}

	EResult CWin32System::GetExecutableDirectory(String & pstrDirectory)
	{
		EResult lResult = EResult::Success;
		wchar_t lstrUnicodePath[MAX_PATH];

		if (GetModuleFileName(GetModuleHandle(nullptr), lstrUnicodePath, MAX_PATH) != 0)
		{			
			GetDirectory(CWin32UTF8Adapter(lstrUnicodePath).Get(), 
				pstrDirectory);
		}
		else
			lResult = EResult::Error;

		return lResult;
	}

	EResult CWin32System::CreateGameWindow(const CWindowParameters & pParameters, 
		const CContextProperties &pContextProps, IGameWindow ** pWindow)
	{
		FDX_ASSERT(pWindow != nullptr, "Passed parameter pWindow cannot null");

		EResult lResult = EResult::Success;
		IGameWindow *lGameWindow = nullptr;

		lGameWindow = new CGLFWGameWindow(pParameters);
		lResult = dynamic_cast<CGLFWGameWindow*>(lGameWindow)->Create(pContextProps);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("Win32 GameWindow creation failed: %s", 
				CResultHelper::Get().GetErrorString(lResult));
		}
		else
		{
			*pWindow = lGameWindow;
		}

		return lResult;
	}

	EResult CWin32System::CreateGameTimer(IGameTimer ** pTimer)
	{
		FDX_ASSERT(pTimer != nullptr, "Passed parameter pTimer cannot null");

		*pTimer = new CWin32GameTimer();
		return EResult::Success;
	}

	EResult CWin32System::GetFileSystem(IFileSystem ** pSystem)
	{
		FDX_ASSERT(pSystem != nullptr, "Passed parameter pSystem cannot null");

		*pSystem = new CWin32FileSystem();
		return EResult::Success;
	}

	EResult CWin32System::CreateFileStream(EOpenMode pOpenMode, EAccessRights pAccessRights,
		const String & pstrFileName, IFileStream ** pStream)
	{
		FDX_ASSERT(pStream != nullptr, "Passed parameter pStream cannot null");

		EResult lResult = EResult::Success;
		CWin32File *lStream = nullptr;

		lStream = new CWin32File(pstrFileName);
		lResult = lStream->Create(pOpenMode, pAccessRights);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("Win32 File creation failed: %s",
				CResultHelper::Get().GetErrorString(lResult));
		}
		else
		{
			*pStream = lStream;
		}

		return lResult;
	}

	EResult CWin32System::Assert(bool pbCondition, const char * pstrFileName, 
		uint32_t puiLineNo, const char * pstrFormat, ...)
	{
		EResult lResult = EResult::Success;

		if (!pbCondition)
		{
			char lstrBuffer[FDX_MAX_BUFFER_LEN];
			char lstrString[FDX_MAX_STRING_LEN];

			va_list lArgs;

			va_start(lArgs, pstrFormat);
			vsprintf_s(lstrBuffer, pstrFormat, lArgs);
			va_end(lArgs);

			sprintf_s(lstrString, "Assert failed.\r\n\r\n"
				"at %s(%ld)"
				"Message: %s\r\n\r\n",
				pstrFileName, puiLineNo, lstrBuffer);

			int32_t liMbRet = MessageBox(nullptr, 
				CWin32UTF16Adapter(lstrString).Get().c_str(), 
				L"FDX Engine", 
				MB_ABORTRETRYIGNORE | MB_ICONERROR);

			if (liMbRet == IDABORT)
			{
				lResult = EResult::Error;
				abort();
			}
			else if (liMbRet == IDIGNORE)
			{
				lResult = EResult::Error;
				DebugBreak();
			}
		}

		return lResult;
	}

	EResult CWin32System::GetLibrary(const String & pstrFileName, ILibrary ** pLibrary)
	{
		EResult lResult = EResult::Success;
		CWin32Library *lLibrary = nullptr;

		lLibrary = new CWin32Library(pstrFileName);
		lResult = lLibrary->Load();

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("Load library failed: %s", pstrFileName.c_str());
		}
		else
		{
			*pLibrary = lLibrary;
		}

		return lResult;
	}

	void CWin32System::Sleep(uint32_t puiMilliSeconds)
	{
		::Sleep(puiMilliSeconds);
	}

	void CWin32System::PollEvents()
	{
		CGLFWGameWindow::PollEvents();
	}
}


