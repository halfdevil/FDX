
#include <stdarg.h>
#include "FS/MemoryStream.h"
#include "OS/System.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CMemoryStream::CMemoryStream()
		: muiSize(0),
		muiPosition(0),
		mData(nullptr),
		mbOwnData(false),
		mOpenMode(EOpenMode::Create)
	{
	}

	CMemoryStream::~CMemoryStream()
	{
		if (!mbOwnData)
			SAFE_DELETE(mData);

		muiSize = 0;
		muiPosition = 0;
		mData = nullptr;
	}

	EOpenMode CMemoryStream::GetOpenMode() const
	{
		return mOpenMode;
	}

	EAccessRights CMemoryStream::GetAccessRights() const
	{
		return mAccessRights;
	}

	bool CMemoryStream::CanRead() const
	{
		return mAccessRights == EAccessRights::Read ||
			mAccessRights == EAccessRights::ReadWrite;
	}

	bool CMemoryStream::CanWrite() const
	{
		return mAccessRights == EAccessRights::Write ||
			mAccessRights == EAccessRights::ReadWrite;
	}

	uint32_t CMemoryStream::GetPosition() const
	{
		return muiPosition;
	}

	uint32_t CMemoryStream::GetSize() const
	{
		return muiSize;
	}

	int32_t CMemoryStream::Seek(ESeekOrigin pSeekOrigin, int32_t offset)
	{
		int32_t liRet = -1;

		if (muiPosition + offset >= 0 && muiPosition + offset < muiSize)
		{
			muiPosition += offset;
			liRet = static_cast<int32_t>(muiPosition);
		}

		return liRet;
	}

	EResult CMemoryStream::Read(uint32_t puiSize, void * pBuffer)
	{
		FDX_ASSERT(CanRead(), "Memory stream is not readable");
		EResult lResult = EResult::Success;

		if (muiPosition + puiSize >= muiSize)
		{
			lResult = EResult::ReadFailed;
		}
		else
		{
			std::memcpy(pBuffer, mData + muiPosition, puiSize);
			muiPosition += puiSize;
		}

		return lResult;
	}

	EResult CMemoryStream::Write(uint32_t puiSize, const void * pBuffer)
	{
		FDX_ASSERT(CanWrite(), "Memory stream is not writeable");
		EResult lResult = EResult::Success;

		if (muiPosition + puiSize >= muiSize)
		{
			lResult = EResult::WriteFailed;
		}
		else
		{
			std::memcpy(mData + muiPosition, pBuffer, puiSize);
			muiPosition += puiSize;
		}

		return lResult;
	}

	EResult CMemoryStream::WriteString(const char * pstrFormat, ...)
	{
		FDX_ASSERT(CanWrite(), "Memory stream is not writeable");

		EResult lResult = EResult::Success;
		va_list pArgs;

		va_start(pArgs, pstrFormat);
		lResult = WriteString(pstrFormat, pArgs);
		va_end(pArgs);

		return lResult;
	}

	EResult CMemoryStream::WriteString(const char * pstrFormat, va_list pArgs)
	{
		FDX_ASSERT(CanWrite(), "Memory stream is not writeable");

		EResult lResult = EResult::Success;
		char lstrBuffer[FDX_MAX_STRING_LEN + 1];

		vsprintf(lstrBuffer, pstrFormat, pArgs);
		lResult = Write((strlen(lstrBuffer) + 1) * sizeof(char), lstrBuffer);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("Write failed inside WriteString: %s",
				CResultHelper::Get().GetErrorString(lResult));
		}

		return lResult;
	}

	EResult CMemoryStream::Flush()
	{
		return EResult::Success;
	}

	EResult CMemoryStream::Create(EAccessRights pAccessRights, uint32_t puiSize, CMemoryStream **pMemStream)
	{
		EResult lResult = EResult::Success;
		CMemoryStream *lMemStream = nullptr;

		lMemStream = new CMemoryStream();
		lMemStream->mAccessRights = pAccessRights;
		lMemStream->muiSize = puiSize;
		lMemStream->mbOwnData = true;
		lMemStream->mData = new uint8_t[puiSize];

		*pMemStream = lMemStream;
		return lResult;
	}

	EResult CMemoryStream::CreateFromStream(EAccessRights pAccessRights, IFileStream * pStream, CMemoryStream **pMemStream)
	{
		FDX_ASSERT(pStream != nullptr, "Passed parameter pStream cannot be null");

		EResult lResult = EResult::Success;
		CMemoryStream *lMemStream = nullptr;

		lMemStream = new CMemoryStream();
		lMemStream->mAccessRights = pAccessRights;
		lMemStream->mbOwnData = true;
		lMemStream->muiSize = pStream->GetSize();

		pStream->Seek(ESeekOrigin::Begining, 0);		
		lResult = pStream->Read(lMemStream->muiSize, lMemStream->mData);

		if (lResult != EResult::Success)
		{
			lResult = EResult::CreateFailed;
		}
		else
		{
			*pMemStream = lMemStream;
		}

		return lResult;
	}

	EResult CMemoryStream::CreateFromData(EAccessRights pAccessRights, 
		uint32_t puiSize, uint8_t * pData, bool pbOwnData, CMemoryStream **pMemStream)
	{
		EResult lResult = EResult::Success;
		CMemoryStream *lMemStream = nullptr;

		lMemStream = new CMemoryStream();
		lMemStream->mAccessRights = pAccessRights;
		lMemStream->muiSize = puiSize;
		lMemStream->mbOwnData = pbOwnData;

		if (!pbOwnData)
		{
			lMemStream->mData = new uint8_t[puiSize];
			std::memcpy(lMemStream->mData, pData, puiSize);
		}
		else
		{
			lMemStream->mData = pData;
		}

		*pMemStream = lMemStream;
		return lResult;
	}
}