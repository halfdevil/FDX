
#include "FS/StreamReader.h"

namespace FDX
{
	CStreamReader::CStreamReader(IFileStream *pStream)
		: mStream(pStream)
	{
	}

	CStreamReader::~CStreamReader()
	{
		mStream = nullptr;
	}

	inline EAccessRights CStreamReader::GetAccessRights() const
	{
		return mStream->GetAccessRights();
	}

	inline uint32_t CStreamReader::GetPosition() const
	{
		return mStream->GetPosition();
	}

	inline uint32_t CStreamReader::GetSize() const
	{
		return mStream->GetSize();
	}

	int32_t CStreamReader::Seek(ESeekOrigin pSeekOrigin, int32_t offset)
	{
		return mStream->Seek(pSeekOrigin, offset);
	}

	EResult CStreamReader::ReadInt8(int8_t * pData)
	{
		return mStream->Read(sizeof(int8_t), pData);
	}

	EResult CStreamReader::ReadUInt8(uint8_t * pData)
	{
		return mStream->Read(sizeof(uint8_t), pData);
	}

	EResult CStreamReader::ReadInt16(int16_t * pData)
	{
		return mStream->Read(sizeof(int16_t), pData);
	}

	EResult CStreamReader::ReadUInt16(uint16_t * pData)
	{
		return mStream->Read(sizeof(uint16_t), pData);
	}

	EResult CStreamReader::ReadInt32(int32_t * pData)
	{
		return mStream->Read(sizeof(int32_t), pData);
	}

	EResult CStreamReader::ReadUInt32(uint32_t * pData)
	{
		return mStream->Read(sizeof(uint32_t), pData);
	}

	EResult CStreamReader::ReadInt64(int64_t * pData)
	{
		return mStream->Read(sizeof(int64_t), pData);
	}

	EResult CStreamReader::ReadUInt64(uint64_t * pData)
	{
		return mStream->Read(sizeof(uint64_t), pData);
	}

	EResult CStreamReader::ReadFloat(float * pData)
	{
		return mStream->Read(sizeof(float), pData);
	}

	EResult CStreamReader::ReadDouble(double * pData)
	{
		return mStream->Read(sizeof(double), pData);
	}

	EResult CStreamReader::ReadBuffer(uint32_t puiSize, void * pData)
	{
		return mStream->Read(puiSize, pData);
	}
}