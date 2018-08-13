
#include "FS/StreamWriter.h"

namespace FDX
{
	CStreamWriter::CStreamWriter(IFileStream * pStream)
		: mStream(pStream)
	{
	}

	CStreamWriter::~CStreamWriter()
	{
		mStream = nullptr;
	}

	EAccessRights CStreamWriter::GetAccessRights() const
	{
		return mStream->GetAccessRights();
	}

	uint32_t CStreamWriter::GetPosition() const
	{
		return mStream->GetPosition();
	}

	uint32_t CStreamWriter::GetSize() const
	{
		return mStream->GetSize();
	}

	int32_t CStreamWriter::Seek(ESeekOrigin pSeekOrigin, int32_t offset)
	{
		return mStream->Seek(pSeekOrigin, offset);
	}

	EResult CStreamWriter::WriteInt8(int8_t pData)
	{
		return mStream->Write(sizeof(int8_t), (const void*)&pData);
	}

	EResult CStreamWriter::WriteUInt8(uint8_t pData)
	{
		return mStream->Write(sizeof(uint8_t), (const void*)&pData);
	}

	EResult CStreamWriter::WriteInt16(int16_t pData)
	{
		return mStream->Write(sizeof(uint16_t), (const void*)&pData);
	}

	EResult CStreamWriter::WriteUInt16(uint16_t pData)
	{
		return mStream->Write(sizeof(uint16_t), (const void*)&pData);
	}

	EResult CStreamWriter::WriteInt32(int32_t pData)
	{
		return mStream->Write(sizeof(int32_t), (const void*)&pData);
	}

	EResult CStreamWriter::WriteUInt32(uint32_t pData)
	{
		return mStream->Write(sizeof(uint32_t), (const void*)&pData);
	}

	EResult CStreamWriter::WriteInt64(int64_t pData)
	{
		return mStream->Write(sizeof(int64_t), (const void*)&pData);
	}

	EResult CStreamWriter::WriteUInt64(uint64_t pData)
	{
		return mStream->Write(sizeof(uint64_t), (const void*)&pData);
	}

	EResult CStreamWriter::WriteFloat(float pData)
	{
		return mStream->Write(sizeof(float), (const void*)&pData);
	}

	EResult CStreamWriter::WriteDouble(double pData)
	{
		return mStream->Write(sizeof(double), (const void*)&pData);
	}

	EResult CStreamWriter::WriteBuffer(uint32_t puiSize, const void * pData)
	{
		return mStream->Write(puiSize, pData);
	}
}
