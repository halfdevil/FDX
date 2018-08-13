#pragma once

#include "Graphics/VertexBuffer.h"
#include <GL/gl3w.h>

namespace FDX
{
	class CGLVertexBuffer : public CVertexBuffer
	{
	public:

		CGLVertexBuffer(const String &pstrName);
		~CGLVertexBuffer();

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
			return muiNumVertices * mFormat->GetSize();
		}

		uint32_t GetNumVertices() const
		{
			return muiNumVertices;
		}

		uint32_t GetVertexSize() const
		{
			return mFormat->GetSize();
		}

		CVertexFormat* GetFormat() const
		{
			return mFormat;
		}

		EResult Create(EBufferUsage pUsage, CVertexFormat *pFormat, 
			uint32_t puiNumVertices);

		EResult UploadData(uint32_t puiOffset, uint32_t puiSize,
			const void *pData);

		EResult Map(EMapFlags pMapFlags, uint32_t puiOffset,
			uint32_t puiSize, void **pData);

		EResult Unmap();

	private:

		String mstrName;
		EBufferUsage mUsage;
		CVertexFormat *mFormat;
		uint32_t muiNumVertices;
		GLuint muiHandle;
	};
}
