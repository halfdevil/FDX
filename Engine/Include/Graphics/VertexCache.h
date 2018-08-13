#pragma once

#include "Graphics/RenderData.h"

namespace FDX
{
	class IGraphicsDevice;

	class FDX_EXPORT CVertexCache
	{
	public:

		CVertexCache(IGraphicsDevice *pGraphicsDevice, const String &pstrName);
		~CVertexCache();

		IRenderData* GetRenderData() const
		{
			return mRenderData.get();
		}

		CVertexFormat* GetVertexFormat() const
		{
			return mVertexFormat;
		}

		EIndexType GetIndexType() const
		{
			return mIndexType;
		}

		uint32_t GetMaxNumVertices() const
		{
			return muiMaxNumVertices;
		}

		uint32_t GetMaxNumIndices() const
		{
			return muiMaxNumIndices;
		}

		uint32_t GetNumVertices() const
		{
			return muiNumVertices;
		}

		uint32_t GetNumIndices() const
		{
			return muiNumIndices;
		}

		bool IsFull() const
		{
			return muiNumVertices >= muiMaxNumVertices;
		}

		bool IsEmpty() const
		{
			return muiNumVertices == 0;
		}

		bool CanAddVertices(uint32_t puiNumVertices)
		{
			return muiNumVertices + puiNumVertices <= muiMaxNumVertices;
		}

		bool HasIndexBuffer() const
		{
			return mbHasIndexBuffer;
		}

		EResult Create(CVertexFormat *pVertexFormat, EIndexType pIndexType,
			uint32_t puiMaxNumVertices, uint32_t puiMaxNumIndices);

		EResult AddVertices(uint32_t muiNumVertices, const void *pData);
		EResult AddIndices(uint32_t muiNumIndices, const void *pData);

	private:

		String mstrName;
		IGraphicsDevice *mGraphicsDevice;
		IRenderDataPtr mRenderData;
		CVertexBuffer *mVertexBuffer;
		CIndexBuffer *mIndexBuffer;
		CVertexFormat *mVertexFormat;
		EIndexType mIndexType;
		uint32_t muiMaxNumVertices;
		uint32_t muiMaxNumIndices;
		uint32_t muiNumVertices;
		uint32_t muiNumIndices;
		bool mbHasIndexBuffer;
	};

	using CVertexCachePtr = SmartPtr<CVertexCache>;
}