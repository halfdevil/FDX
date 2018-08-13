
#include "Core/Logger.h"
#include <stdarg.h>

namespace FDX
{
	CLogger::CLogger(uint32_t puiLogLevel)
		: muiLogLevel(puiLogLevel)
	{
	}

	CLogger::~CLogger()
	{
	}

	EResult CLogger::WriteDebug(const char * pstrFileName, uint32_t puiLogLevel, const char * pstrFormat, ...)
	{
		EResult lResult = EResult::Success;
		va_list lArgs;

		va_start(lArgs, pstrFormat);

		lResult = Write(ELogLevel::Debug,
			pstrFileName,
			puiLogLevel,
			pstrFormat,
			lArgs);

		va_end(lArgs);

		return lResult;
	}

	EResult CLogger::WriteInfo(const char * pstrFileName, uint32_t puiLogLevel, const char * pstrFormat, ...)
	{
		EResult lResult = EResult::Success;
		va_list lArgs;

		va_start(lArgs, pstrFormat);

		lResult = Write(ELogLevel::Info,
			pstrFileName,
			puiLogLevel,
			pstrFormat,
			lArgs);

		va_end(lArgs);

		return lResult;
	}

	EResult CLogger::WriteWarn(const char * pstrFileName, uint32_t puiLogLevel, const char * pstrFormat, ...)
	{
		EResult lResult = EResult::Success;
		va_list lArgs;

		va_start(lArgs, pstrFormat);

		lResult = Write(ELogLevel::Warn,
			pstrFileName,
			puiLogLevel,
			pstrFormat,
			lArgs);

		va_end(lArgs);

		return lResult;
	}

	EResult CLogger::WriteError(const char * pstrFileName, uint32_t puiLogLevel, const char * pstrFormat, ...)
	{
		EResult lResult = EResult::Success;
		va_list lArgs;

		va_start(lArgs, pstrFormat);

		lResult = Write(ELogLevel::Error,
			pstrFileName,
			puiLogLevel,
			pstrFormat,
			lArgs);

		va_end(lArgs);

		return lResult;
	}
}
