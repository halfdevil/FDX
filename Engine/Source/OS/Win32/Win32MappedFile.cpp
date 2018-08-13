
#include "OS/Win32/Win32MappedFile.h"
#include "OS/System.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CWin32MappedFile::CWin32MappedFile(const String & pstrFileName)
		: CWin32File(pstrFileName),
		mMapHandle(INVALID_HANDLE_VALUE),
		muiSize(0),
		muiPosition(0),
		mucData(nullptr)
	{
	}

	CWin32MappedFile::~CWin32MappedFile()
	{
		if (mucData)
			UnmapViewOfFile(mucData);

		if (mMapHandle)
			CloseHandle(mMapHandle);

		mucData = nullptr;
		mMapHandle = nullptr;
	}

	EResult CWin32MappedFile::Create(EOpenMode pOpenMode, EAccessRights pAccessRights)
	{
		FDX_ASSERT(pOpenMode == EOpenMode::Open, "Open mode is not open");
		FDX_ASSERT(pAccessRights == EAccessRights::Read, "Access rights is not read");

		EResult lResult = CWin32File::Create(pOpenMode,
			pAccessRights);

		if (lResult != EResult::Success)
			return lResult;

		mMapHandle = CreateFileMapping(mHandle, nullptr,
			PAGE_READONLY, 0, 0, nullptr);

		if (mMapHandle == INVALID_HANDLE_VALUE)
		{
			LOG_ERROR("Unable to create win32 file mapping for file: %s", mstrFileName.c_str());
			lResult = EResult::CreateFailed;
		}
		else
		{
			mucData = (uint8_t*)MapViewOfFile(mMapHandle, FILE_MAP_READ, 0, 0, 0);
			if (!mucData)
			{
				LOG_ERROR("Unable to map win32 view of file for: %s", mstrFileName.c_str());
				lResult = EResult::CreateFailed;
			}
			else
			{
				LARGE_INTEGER lFileSize = { 0 };
				GetFileSizeEx(mHandle, &lFileSize);

				muiSize = lFileSize.LowPart;
			}
		}

		return lResult;
	}

	uint32_t CWin32MappedFile::GetPosition() const
	{
		return muiPosition;
	}

	uint32_t CWin32MappedFile::GetSize() const
	{
		return muiSize;
	}

	int32_t CWin32MappedFile::Seek(ESeekOrigin pSeekOrigin, int32_t offset)
	{
		int32_t liPosition = static_cast<int32_t>(muiPosition);

		if (pSeekOrigin == ESeekOrigin::Begining)
			liPosition = offset;
		else if (pSeekOrigin == ESeekOrigin::Current)
			liPosition += offset;
		else
			liPosition = static_cast<int32_t>(muiSize) + offset;

		muiPosition = static_cast<uint32_t>(liPosition);

		return liPosition;
	}

	EResult CWin32MappedFile::Read(uint32_t puiSize, void * pBuffer)
	{
		EResult lResult = EResult::Success;

		if (muiPosition + puiSize > muiSize)
			lResult = EResult::ReadFailed;
		else
		{
			memcpy(pBuffer, mucData + muiPosition, puiSize);
			muiPosition += puiSize;
		}

		return lResult;
	}

	EResult CWin32MappedFile::Write(uint32_t puiSize, const void * pBuffer)
	{
		FDX_ASSERT(false, "Win32 mapped file: cannot write to read only file");
		return EResult::WriteFailed;
	}

	EResult CWin32MappedFile::WriteString(const char * pstrFormat, ...)
	{
		FDX_ASSERT(false, "Win32 mapped file: cannot write to read only file");
		return EResult::WriteFailed;
	}

	EResult CWin32MappedFile::WriteString(const char * pstrFormat, va_list pArgs)
	{
		FDX_ASSERT(false, "Win32 mapped file: cannot write to read only file");
		return EResult::WriteFailed;
	}

	EResult CWin32MappedFile::Flush()
	{
		FDX_ASSERT(false, "Win32 mapped file: cannot flush read only file");
		return EResult::WriteFailed;
	}
}