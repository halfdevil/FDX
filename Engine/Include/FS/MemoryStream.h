#pragma once

#include "FS/FileStream.h"

namespace FDX
{
	class FDX_EXPORT CMemoryStream : public IFileStream
	{
	public:

		CMemoryStream();
		~CMemoryStream();

		EOpenMode GetOpenMode() const;
		EAccessRights GetAccessRights() const;

		bool CanRead() const;
		bool CanWrite() const;

		uint32_t GetPosition() const;
		uint32_t GetSize() const;

		int32_t Seek(ESeekOrigin pSeekOrigin, int32_t offset);

		EResult Read(uint32_t puiSize, void *pBuffer);
		EResult Write(uint32_t puiSize, const void *pBuffer);

		EResult WriteString(const char *pstrFormat, ...);
		EResult WriteString(const char *pstrFormat, va_list pArgs);

		EResult Flush();

	public:

		static EResult Create(EAccessRights pAccessRights, 
			uint32_t puiSize, 
			CMemoryStream **pMemStream);

		static EResult CreateFromStream(EAccessRights pAccessRights, 
			IFileStream *pStream, 
			CMemoryStream **pMemStream);

		static EResult CreateFromData(EAccessRights pAccessRights, 
			uint32_t puiSize,
			uint8_t *pData, 
			bool pbOwnData, 
			CMemoryStream **pMemStream);

	private:

		EOpenMode mOpenMode;
		EAccessRights mAccessRights;
		uint32_t muiPosition;
		uint32_t muiSize;
		uint8_t *mData;
		bool mbOwnData;
	};
}
