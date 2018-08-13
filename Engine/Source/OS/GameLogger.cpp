
#include "OS/GameLogger.h"
#include "OS/System.h"
#include "Core/DateTime.h"

namespace FDX
{
	CGameLogger::CGameLogger(uint32_t puiLogLevel)
		: CLogger(puiLogLevel),
		mLogStream(nullptr)
	{
	}

	CGameLogger::~CGameLogger()
	{
		SAFE_DELETE(mLogStream);
	}

	EResult CGameLogger::Create(const String & pstrLogDir, const String & pstrLogFileName)
	{
		IFileStream *lStream = nullptr;
		EResult lResult = EResult::Success;

		lResult = ISystem::Get().CombinePath(pstrLogDir,
			pstrLogFileName, mstrLogFileName);
		
		lResult = ISystem::Get().CreateFileStream(EOpenMode::Create, 
			EAccessRights::Write,
			mstrLogFileName, &lStream);

		if (lResult != EResult::Success)
			return lResult;

		mLogStream = lStream;
		return lResult;
	}

	EResult CGameLogger::Write(ELogLevel pLogLevel, const char * pstrFileName, 
		uint32_t puiLineNo, const char * pstrFormat, va_list pArgs)
	{
		EResult lResult = EResult::Success;

		if (mLogStream == nullptr)
			return EResult::Error;

		if (static_cast<uint32_t>(pLogLevel) < muiLogLevel)
			return lResult;

		CDateTime lNow = CDateTime::GetNow();
		std::lock_guard<std::mutex> lLock(mLoggerMutex);

		switch (pLogLevel)
		{
		case ELogLevel::Debug:
			lResult = mLogStream->WriteString("[%02d:%02d:%02d][%s, %d][DEBUG]: ",
				lNow.GetHour(),
				lNow.GetMinute(),
				lNow.GetSecond(),
				pstrFileName,
				puiLineNo);
			break;

		case ELogLevel::Info:
			lResult = mLogStream->WriteString("[%02d:%02d:%02d][%s, %d][INFO]: ",
				lNow.GetHour(),
				lNow.GetMinute(),
				lNow.GetSecond(),
				pstrFileName,
				puiLineNo);
			break;

		case ELogLevel::Warn:
			lResult = mLogStream->WriteString("[%02d:%02d:%02d][%s, %d][WARN]: ",
				lNow.GetHour(),
				lNow.GetMinute(),
				lNow.GetSecond(),
				pstrFileName,
				puiLineNo);
			break;

		case ELogLevel::Error:
			lResult = mLogStream->WriteString("[%02d:%02d:%02d][%s, %d][ERROR]: ",
				lNow.GetHour(),
				lNow.GetMinute(),
				lNow.GetSecond(),
				pstrFileName,
				puiLineNo);
			break;
		}

		if (lResult != EResult::Success)
			return lResult;

		lResult = mLogStream->WriteString(pstrFormat, pArgs);

		if (lResult == EResult::Success)
		{
			mLogStream->WriteString("\r\n");
			mLogStream->Flush();
		}

		return lResult;
	}
}