#pragma once

#include "Core/Core.h"
#include "Core/Result.h"
#include "Core/Singleton.h"

namespace FDX
{
	enum class ELogLevel
	{
		Debug = 0,
		Info,
		Warn,
		Error
	};

	class FDX_EXPORT ILogger
	{
	public:

		virtual uint32_t GetLogLevel() const = 0;

		virtual EResult Write(ELogLevel pLogLevel,	
			const char *pstrFileName,
			uint32_t puiLogLevel,
			const char *pstrFormat, 
			va_list pArgs) = 0;
	};

	class FDX_EXPORT CLogger : public ILogger
	{
	public:

		CLogger(uint32_t puiLogLevel);
		virtual ~CLogger();

		uint32_t GetLogLevel() const
		{
			return muiLogLevel;
		}

		EResult WriteDebug(const char *pstrFileName,
			uint32_t puiLogLevel,
			const char *pstrFormat,
			...);

		EResult WriteInfo(const char *pstrFileName,
			uint32_t puiLogLevel,
			const char *pstrFormat,
			...);

		EResult WriteWarn(const char *pstrFileName,
			uint32_t puiLogLevel,
			const char *pstrFormat,
			...);

		EResult WriteError(const char *pstrFileName,
			uint32_t puiLogLevel,
			const char *pstrFormat,
			...);

	protected:

		uint32_t muiLogLevel;
	};
}
