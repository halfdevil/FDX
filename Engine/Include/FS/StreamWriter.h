#pragma once

#include "FS/FileStream.h"

namespace FDX
{
	class FDX_EXPORT CStreamWriter
	{
	public:

		CStreamWriter(IFileStream *pStream);
		~CStreamWriter();

		inline EAccessRights GetAccessRights() const;
		inline uint32_t GetPosition() const;
		inline uint32_t GetSize() const;

		int32_t Seek(ESeekOrigin pSeekOrigin, int32_t offset);

		EResult WriteInt8(int8_t pData);
		EResult WriteUInt8(uint8_t pData);
		EResult WriteInt16(int16_t pData);
		EResult WriteUInt16(uint16_t pData);
		EResult WriteInt32(int32_t pData);
		EResult WriteUInt32(uint32_t pData);
		EResult WriteInt64(int64_t pData);
		EResult WriteUInt64(uint64_t pData);

		EResult WriteFloat(float pData);
		EResult WriteDouble(double pData);
		EResult WriteBuffer(uint32_t puiSize, const void *pData);

	private:

		IFileStream	*mStream;
	};

	using CStreamWriterPtr = SmartPtr<CStreamWriter>;
}