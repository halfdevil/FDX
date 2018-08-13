#pragma once

#include "FS/FileStream.h"

namespace FDX
{
	class FDX_EXPORT CStreamReader
	{
	public:

		CStreamReader(IFileStream *pStream);
		~CStreamReader();

		inline EAccessRights GetAccessRights() const;
		inline uint32_t GetPosition() const;
		inline uint32_t GetSize() const;

		int32_t Seek(ESeekOrigin pSeekOrigin, int32_t offset);

		EResult ReadInt8(int8_t *pData);
		EResult ReadUInt8(uint8_t *pData);
		EResult ReadInt16(int16_t *pData);
		EResult ReadUInt16(uint16_t *pData);
		EResult ReadInt32(int32_t *pData);
		EResult ReadUInt32(uint32_t *pData);
		EResult ReadInt64(int64_t *pData);
		EResult ReadUInt64(uint64_t *pData); 

		EResult ReadFloat(float *pData);
		EResult ReadDouble(double *pData);
		EResult ReadBuffer(uint32_t puiSize, void *pData);

	private:

		IFileStream	*mStream;
	};

	using CStreamReaderPtr = SmartPtr<CStreamReader>;	
}
