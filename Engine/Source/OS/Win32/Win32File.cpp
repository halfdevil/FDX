
#include "OS/Win32/Win32File.h"
#include "OS/Win32/Win32UnicodeAdapter.h"
#include "OS/GameLogger.h"
#include "OS/System.h"

namespace FDX
{
	CWin32File::CWin32File(const String & pstrFileName)
		: mstrFileName(pstrFileName),
		mHandle(INVALID_HANDLE_VALUE)
	{
	}

	CWin32File::~CWin32File()
	{
		if (mHandle != INVALID_HANDLE_VALUE)
			CloseHandle(mHandle);

		mHandle = INVALID_HANDLE_VALUE;
	}

	EResult CWin32File::Create(EOpenMode pOpenMode, EAccessRights pAccessRights)
	{
		EResult lResult = EResult::Success;
		DWORD ldwDesiredAccess = 0;
		DWORD ldwCreationDisposition = 0;

		if (pAccessRights == EAccessRights::Read)
			ldwDesiredAccess = GENERIC_READ;
		if (pAccessRights == EAccessRights::Write)
			ldwDesiredAccess = GENERIC_WRITE;
		else
			ldwDesiredAccess = GENERIC_READ | GENERIC_WRITE;

		if (pOpenMode == EOpenMode::Create)
			ldwCreationDisposition = CREATE_ALWAYS;
		else if (pOpenMode == EOpenMode::Open)
			ldwCreationDisposition = OPEN_EXISTING;
		else
		{
			ldwDesiredAccess = FILE_APPEND_DATA;
			ldwCreationDisposition = OPEN_ALWAYS;
		}

		String lstrFileName(mstrFileName);
		ISystem::Get().ConvertPath(lstrFileName);

		mHandle = CreateFile(CWin32UTF16Adapter(lstrFileName).
			Get().c_str(),
			ldwDesiredAccess,
			FILE_SHARE_READ,
			nullptr,
			ldwCreationDisposition,
			FILE_ATTRIBUTE_NORMAL,
			nullptr);

		if (mHandle == INVALID_HANDLE_VALUE)
		{
			LOG_DEBUG("Win32 CreateFile failed: %d", GetLastError());
			lResult = EResult::CreateFailed;			

			return lResult;
		}
		else
		{
			mOpenMode = pOpenMode;
			mAccessRights = pAccessRights;
		}

		return lResult;
	}

	uint32_t CWin32File::GetPosition() const
	{		
		FDX_ASSERT(mHandle != INVALID_HANDLE_VALUE, "File handle is invalid");
				
		LARGE_INTEGER lCurrPos = { 0 };
		SetFilePointerEx(mHandle, lCurrPos, 
			&lCurrPos, FILE_CURRENT);

		return static_cast<uint32_t>(lCurrPos.QuadPart);
	}

	uint32_t CWin32File::GetSize() const
	{
		FDX_ASSERT(mHandle != INVALID_HANDLE_VALUE, "File handle is invalid");

		LARGE_INTEGER lFileSize = { 0 };
		GetFileSizeEx(mHandle, &lFileSize);

		return static_cast<uint32_t>(lFileSize.QuadPart);
	}

	int32_t CWin32File::Seek(ESeekOrigin pSeekOrigin, int32_t offset)
	{
		FDX_ASSERT(mHandle != INVALID_HANDLE_VALUE, "File handle is invalid");

		DWORD ldwMoveMethod = 0;

		if (pSeekOrigin == ESeekOrigin::Begining)
			ldwMoveMethod = FILE_BEGIN;
		else if (pSeekOrigin == ESeekOrigin::Current)
			ldwMoveMethod = FILE_CURRENT;
		else
			ldwMoveMethod = FILE_END;

		LARGE_INTEGER lCurrPos = { 0 };
		lCurrPos.QuadPart = static_cast<DWORD>(offset);

		SetFilePointerEx(mHandle, lCurrPos,
			&lCurrPos, ldwMoveMethod);

		return static_cast<int32_t>(lCurrPos.QuadPart);
	}

	EResult CWin32File::Read(uint32_t puiSize, void * pBuffer)
	{
		FDX_ASSERT(mHandle != INVALID_HANDLE_VALUE, "File handle is invalid");

		EResult lResult = EResult::Success;
		DWORD ldwBytesRead = 0;

		if (!ReadFile(mHandle, pBuffer, static_cast<DWORD>(puiSize),
			&ldwBytesRead, nullptr))
		{
			LOG_DEBUG("Win32 ReadFile failed: %d", GetLastError());
			lResult = EResult::ReadFailed;
		}
		else if (ldwBytesRead < puiSize)
		{
			LOG_DEBUG("Win32 ReadFile read less bytes: %d", GetLastError());
			lResult = EResult::ReadFailed;
		}

		return lResult;
	}

	EResult CWin32File::Write(uint32_t puiSize, const void * pBuffer)
	{
		FDX_ASSERT(mHandle != INVALID_HANDLE_VALUE, "File handle is invalid");

		EResult lResult = EResult::Success;
		DWORD ldwBytesWritten = 0;

		if (!WriteFile(mHandle, pBuffer, static_cast<DWORD>(puiSize), 
			&ldwBytesWritten, nullptr))
		{
			LOG_DEBUG("Win32 WriteFile failed: %d", GetLastError());
			lResult = EResult::WriteFailed;

			return lResult;
		}

		return lResult;
	}

	EResult CWin32File::WriteString(const char * pstrFormat, ...)
	{
		FDX_ASSERT(mHandle != INVALID_HANDLE_VALUE, "File handle is invalid");

		EResult lResult = EResult::Success;
		va_list pArgs;

		va_start(pArgs, pstrFormat);
		lResult = WriteString(pstrFormat, pArgs);
		va_end(pArgs);

		return lResult;
	}

	EResult CWin32File::WriteString(const char * pstrFormat, va_list pArgs)
	{
		FDX_ASSERT(mHandle != INVALID_HANDLE_VALUE, "File handle is invalid");

		EResult lResult = EResult::Success;
		char lstrBuffer[FDX_MAX_STRING_LEN + 1];

		vsprintf_s(lstrBuffer, pstrFormat, pArgs);
		lResult = Write((strlen(lstrBuffer)) * sizeof(char), lstrBuffer);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("Write failed inside WriteString: %s", 
				CResultHelper::Get().GetErrorString(lResult));
		}	

		return lResult;
	}

	EResult CWin32File::Flush()
	{
		FDX_ASSERT(mHandle != INVALID_HANDLE_VALUE, "Handle is invalid");

		return FlushFileBuffers(mHandle) ? 
			EResult::Success : EResult::Error;
	}
}
