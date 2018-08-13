#pragma once

#include "Graphics/RenderData.h"
#include "Graphics/GraphicsDevice.h"
#include "Graphics/DrawOperation.h"
#include "Scene/Material.h"

namespace FDX
{
	class FDX_EXPORT CBatchRenderer
	{
	public:

		CBatchRenderer(const String &pstrName, IGraphicsDevice *pGraphicsDevice);
		~CBatchRenderer();

		EPassType GetPassType() const
		{
			return mPassType;
		}

		bool HasIndexBuffer() const
		{
			return muiMaxNumIndices > 0;
		}

		uint32_t GetNumVertices() const
		{
			return muiNumVertices;
		}

		uint32_t GetNumIndices() const
		{
			return muiNumIndices;
		}

		uint32_t GetMaxNumVertices() const
		{
			return muiMaxNumVertices;
		}

		uint32_t GetMaxNumIndices() const
		{
			return muiMaxNumIndices;
		}

		IDrawOperation* GetDrawOperation() const
		{
			return mDrawOperation.get();
		}

		void SetPassType(EPassType pPassType)
		{
			if (muiNumVertices > 0)
				Flush();

			mPassType = pPassType;
		}

		EResult Create(uint32_t puiMaxNumVertices, uint32_t puiMaxNumIndices,
			CVertexFormat *pVertexFormat, EIndexType pIndexType);

		EResult Render(IMaterial *pMaterial, const void *pVertices,
			const void *pIndices, uint32_t puiNumVertices, uint32_t puiNumIndices);

		EResult Flush();

	private:

		EResult Add(const void *pVertices, const void *pIndices,
			uint32_t puiNumVertices, uint32_t puiNumIndices);

	private:

		String mstrName;
		IGraphicsDevice *mGraphicsDevice;
		IRenderDataPtr mRenderData;
		IDrawOperationPtr mDrawOperation;
		IMaterial *mMaterial;
		CVertexFormat *mVertexFormat;
		CVertexBuffer *mVertexBuffer;
		CIndexBuffer *mIndexBuffer;
		EIndexType mIndexType;
		EPassType mPassType;

		uint32_t muiMaxNumVertices;
		uint32_t muiMaxNumIndices;
		uint32_t muiNumVertices;
		uint32_t muiNumIndices;
	};

	using CBatchRendererPtr = SmartPtr<CBatchRenderer>;
}