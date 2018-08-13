#pragma once

#include "FS/FileStream.h"
#include <Windows.h>

namespace FDX
{
	class FDX_EXPORT CWin32File : public IFileStream
	{
	public:

		CWin32File(const String &pstrFileName);
		virtual ~CWin32File();

		virtual EResult Create(EOpenMode pOpenMode,
			EAccessRights pAccessRights);		

		virtual EOpenMode GetOpenMode() const override
		{
			return mOpenMode;
		}

		virtual EAccessRights GetAccessRights() const override
		{
			return mAccessRights;
		}

		virtual bool CanRead() const override
		{
			return (mAccessRights == EAccessRights::Read ||
				mAccessRights == EAccessRights::ReadWrite);
		}

		virtual bool CanWrite() const override
		{
			return (mAccessRights == EAccessRights::Write ||
				mAccessRights == EAccessRights::ReadWrite);
		}

		virtual uint32_t GetPosition() const override;
		virtual uint32_t GetSize() const override;

		virtual int32_t Seek(ESeekOrigin pSeekOrigin, int32_t offset) override;

		virtual EResult Read(uint32_t puiSize, void *pBuffer) override;
		virtual EResult Write(uint32_t puiSize, const void *pBuffer) override;

		virtual EResult WriteString(const char *pstrFormat, ...) override;
		virtual EResult WriteString(const char *pstrFormat, va_list pArgs) override;

		virtual EResult Flush() override;

	protected:

		EOpenMode mOpenMode;
		EAccessRights mAccessRights;
		String mstrFileName;
		HANDLE mHandle;
	};
}