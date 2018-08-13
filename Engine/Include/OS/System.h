#pragma once

#include "FS/FileSystem.h"
#include "OS/GameWindow.h"
#include "OS/GameTimer.h"
#include "Core/Singleton.h"
#include "OS/Library.h"
#include "Graphics/GraphicsContext.h"

#if FDX_DEBUG
	#define FDX_ASSERT(pbCondition, pstrFormat, ...) \
		FDX::ISystem::Get().Assert((pbCondition), __FILE__, __LINE__, pstrFormat, ##__VA_ARGS__)
#else
	#define FDX_ASSERT(pbCondition, pstrFormat, ...) (pbCondition);
#endif

namespace FDX
{
	class FDX_EXPORT ISystem : public CSingleton<ISystem>
	{
	public:

		virtual ~ISystem()
		{
		}

		virtual EResult CombinePath(const String &pstrPathA,
			const String &pstrPathB, String &pstrOutPath) = 0;

		virtual EResult ConvertPath(String &pstrPath) = 0;
		virtual EResult GetWorkingDirectory(String &pstrDirectory) = 0;
		virtual EResult GetExecutableDirectory(String &pstrDirectory) = 0;

		virtual EResult GetDirectory(const String &pstrPath, String &pstrDirectory) = 0;
		virtual EResult GetFileName(const String &pstrPath, String &pstrFileName) = 0;
		virtual EResult GetExtension(const String &pstrPath, String &pstrExtension) = 0;

		virtual EResult CreateGameWindow(const CWindowParameters &pWindowParams,
										 const CContextProperties &pContextProps,
										 IGameWindow **pWindow) = 0;

		virtual EResult CreateGameTimer(IGameTimer **pTimer) = 0;
		virtual EResult GetFileSystem(IFileSystem **pSystem) = 0;

		virtual EResult CreateFileStream(EOpenMode pOpenMode,
			EAccessRights pAccessRights,
			const String &pstrFileName,
			IFileStream **pStream) = 0;

		virtual EResult Assert(bool pbCondition,
			const char *pstrFileName,
			uint32_t puiLineNo,
			const char *pstrFormat, ...) = 0;

		virtual EResult GetLibrary(const String &pstrFileName,
			ILibrary **pLibrary) = 0;

		virtual void Sleep(uint32_t puiMilliSeconds) = 0;
		virtual void PollEvents() = 0;
	};

	using ISystemPtr = SmartPtr<ISystem>;
}
