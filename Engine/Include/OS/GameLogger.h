#pragma once

#include "Core/Logger.h"
#include "Core/Singleton.h"
#include "FS/FileSystem.h"

#define LOG_DEBUG(pFormat, ...) FDX::CGameLogger::Get().WriteDebug(__FILE__, __LINE__, pFormat, ##__VA_ARGS__)
#define LOG_INFO(pFormat, ...) FDX::CGameLogger::Get().WriteInfo(__FILE__, __LINE__, pFormat, ##__VA_ARGS__)
#define LOG_WARN(pFormat, ...) FDX::CGameLogger::Get().WriteWarn(__FILE__, __LINE__, pFormat, ##__VA_ARGS__)
#define LOG_ERROR(pFormat, ...) FDX::CGameLogger::Get().WriteError(__FILE__, __LINE__, pFormat, ##__VA_ARGS__)

namespace FDX
{
	class FDX_EXPORT CGameLogger : public CLogger, public CSingleton<CGameLogger>
	{
	public:

		CGameLogger(uint32_t puiLogLevel);
		~CGameLogger();

		const String& GetLogDir() const
		{
			return mstrLogDir;
		}

		const String& GetLogFileName() const
		{
			return mstrLogFileName;
		}

		void SetLogLevel(uint32_t puiLogLevel)
		{
			muiLogLevel = puiLogLevel;
		}

		EResult Create(const String &pstrLogDir, 
			const String &pstrLogFileName);

		EResult Write(ELogLevel pLogLevel,
			const char *pstrFileName,
			uint32_t puiLogLevel,
			const char *pstrFormat,
			va_list pArgs);

	private:

		String mstrLogDir;
		String mstrLogFileName;
		IFileStream *mLogStream;
		std::mutex mLoggerMutex;
	};

	using CGameLoggerPtr = SmartPtr<CGameLogger>;
}