#pragma once

#include "Core/Core.h"
#include "Core/Result.h"

namespace FDX
{
	enum class EBufferUsage
	{
		Static,
		Dynamic
	};

	enum class EMapFlags
	{
		Read,
		Write,
		ReadWrite
	};

	class FDX_EXPORT IBuffer
	{
	public:

		virtual ~IBuffer()
		{
		}

		virtual const String& GetName() const = 0;

		virtual EBufferUsage GetUsage() = 0;
		virtual uint32_t GetSize() const = 0;

		virtual EResult UploadData(uint32_t puiOffset, uint32_t puiSize,
			const void *pData) = 0;

		virtual EResult Map(EMapFlags pMapFlags, uint32_t puiOffset,
			uint32_t puiSize, void **pData) = 0;

		virtual EResult Unmap() = 0;
	};
}