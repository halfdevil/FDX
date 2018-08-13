#pragma once

#include "Core/Core.h"
#include "Core/Result.h"

namespace FDX
{
	enum class EOpenMode
	{
		Open,
		Create,
		Append
	};

	enum class EAccessRights
	{
		Read,
		Write,
		ReadWrite
	};

	enum class ESeekOrigin
	{
		Current,
		Begining,
		End
	};

	class FDX_EXPORT IFileStream
	{
	public:

		virtual ~IFileStream() = default;

		virtual EOpenMode GetOpenMode() const = 0;
		virtual EAccessRights GetAccessRights() const = 0;

		virtual bool CanRead() const = 0;
		virtual bool CanWrite() const = 0;
		
		virtual uint32_t GetPosition() const = 0;	
		virtual uint32_t GetSize() const = 0;

		virtual int32_t Seek(ESeekOrigin pSeekOrigin, int32_t offset) = 0;

		virtual EResult Read(uint32_t puiSize, void *pBuffer) = 0;
		virtual EResult Write(uint32_t puiSize, const void *pBuffer) = 0;

		virtual EResult WriteString(const char *pstrFormat, ...) = 0;
		virtual EResult WriteString(const char *pstrFormat, va_list pArgs) = 0;

		virtual EResult Flush() = 0;
	};

	using IFileStreamPtr = SmartPtr<IFileStream>;
}