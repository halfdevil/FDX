#pragma once

#include "Graphics/IndexBuffer.h"
#include <GL/gl3w.h>

namespace FDX
{
	class CGLIndexBuffer : public CIndexBuffer
	{
	public:

		CGLIndexBuffer(const String &pstrName);
		~CGLIndexBuffer();

		GLuint GetHandle() const
		{
			return muiHandle;
		}

		const String& GetName() const
		{
			return mstrName;
		}

		EBufferUsage GetUsage()
		{
			return mUsage;
		}

		uint32_t GetSize() const
		{
			return muiNumIndices * (mIndexType == EIndexType::SixteenBit) ? 2 : 4;
		}

		uint32_t GetNumIndices() const
		{
			return muiNumIndices;
		}
		
		EIndexType GetIndexType() const
		{
			return mIndexType;
		}

		EResult Create(EBufferUsage pUsage, EIndexType pIndexType,
			uint32_t puiNumIndices);

		EResult UploadData(uint32_t puiOffset, uint32_t puiSize,
			const void *pData);

		EResult Map(EMapFlags pMapFlags, uint32_t puiOffset,
			uint32_t puiSize, void **pData);

		EResult Unmap();

	private:

		String mstrName;
		EBufferUsage mUsage;
		EIndexType mIndexType;
		uint32_t muiNumIndices;
		GLuint muiHandle;
	};
}
