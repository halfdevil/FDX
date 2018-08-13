#pragma once

#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"

namespace FDX
{
	class FDX_EXPORT IRenderData
	{
	public:

		virtual ~IRenderData()
		{
		}

		virtual const String& GetName() const = 0;

		virtual CVertexBuffer* GetVertexBuffer(uint32_t puiId) const = 0;
		virtual CIndexBuffer* GetIndexBuffer(uint32_t puiId) const = 0;

		virtual EResult AddVertexBuffer(const String &pstrName, EBufferUsage pUsage, 
			CVertexFormat *pFormat,	uint32_t puiNumVertices, int32_t *puiId) = 0;

		virtual EResult AddIndexBuffer(const String &pstrName, EBufferUsage pUsage, 
			EIndexType pType, uint32_t puiNumIndices, int32_t *puiId) = 0;

		virtual EResult Activate(int32_t puiVertexBufferId, 
			int32_t puiIndexBufferId) = 0;
	};

	using IRenderDataPtr = SmartPtr<IRenderData>;
}
