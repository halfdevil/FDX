#pragma once

#include "Graphics/Buffer.h"
#include "Graphics/VertexFormat.h"

namespace FDX
{
	class FDX_EXPORT CVertexBuffer : public IBuffer
	{
	public:

		virtual ~CVertexBuffer()
		{
		}

		virtual uint32_t GetNumVertices() const = 0;
		virtual uint32_t GetVertexSize() const = 0;

		virtual CVertexFormat* GetFormat() const = 0;
	};

	using CVertexBufferPtr = SmartPtr<CVertexBuffer>;
}