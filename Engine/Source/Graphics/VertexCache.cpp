
#include "Graphics/VertexCache.h"
#include "Graphics/GraphicsDevice.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CVertexCache::CVertexCache(IGraphicsDevice * pGraphicsDevice, const String &pstrName)
		: mGraphicsDevice(pGraphicsDevice),
		mstrName(pstrName),
		mVertexFormat(nullptr),
		muiMaxNumVertices(0),
		muiMaxNumIndices(0),
		muiNumVertices(0),
		muiNumIndices(0),
		mbHasIndexBuffer(false)
	{
	}

	CVertexCache::~CVertexCache()
	{
	}

	EResult CVertexCache::Create(CVertexFormat * pVertexFormat, EIndexType pIndexType, 
		uint32_t puiMaxNumVertices, uint32_t puiMaxNumIndices)
	{
		EResult lResult = EResult::Success;
		IRenderData *lRenderData = nullptr;

		lResult = mGraphicsDevice->CreateRenderData(mstrName, &lRenderData);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CreateRenderData() failed for %s", mstrName.c_str());
		}
		else
		{
			int32_t luiVertexBufferId = -1;
			int32_t luiIndexBufferId = -1;

			lResult = lRenderData->AddVertexBuffer(mstrName, EBufferUsage::Static,
				pVertexFormat, puiMaxNumVertices, &luiVertexBufferId);

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("AddVertexBuffer() failed for %s", mstrName.c_str());
				SAFE_DELETE(lRenderData);
			}
			else
			{
				if (puiMaxNumIndices > 0)
				{
					lResult = lRenderData->AddIndexBuffer(mstrName, EBufferUsage::Static,
						pIndexType, puiMaxNumIndices, &luiIndexBufferId);

					if (lResult != EResult::Success)
					{
						LOG_DEBUG("AddIndexBuffer() failed for %s", mstrName.c_str());
						SAFE_DELETE(lRenderData);
					}
				}
				
				if (lResult == EResult::Success)
				{					
					mRenderData = IRenderDataPtr(lRenderData);
					mVertexBuffer = mRenderData->GetVertexBuffer(luiVertexBufferId);					
					muiMaxNumVertices = puiMaxNumVertices;
					muiMaxNumIndices = puiMaxNumIndices;
					mVertexFormat = pVertexFormat;
					mIndexType = pIndexType;

					if (muiMaxNumIndices > 0)
					{
						mbHasIndexBuffer = true;
						mIndexBuffer = mRenderData->GetIndexBuffer(luiIndexBufferId);
					}
					else
					{
						mbHasIndexBuffer = true;
						mIndexBuffer = nullptr;
					}
				}
			}
		}

		return lResult;
	}

	EResult CVertexCache::AddVertices(uint32_t puiNumVertices, const void * pData)
	{
		EResult lResult = EResult::Success;

		if (muiNumVertices + puiNumVertices >= muiMaxNumVertices)
		{
			LOG_DEBUG("Max vertex count reached for %s", mstrName.c_str());
			lResult = EResult::AddFailed;
		}
		else
		{
			uint32_t luiOffset = muiNumVertices * mVertexFormat->GetSize();
			uint32_t luiSize = puiNumVertices * mVertexFormat->GetSize();

			lResult = mVertexBuffer->UploadData(luiOffset, luiSize, pData);
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("VertexBuffer UploadData() failed for %s", mstrName.c_str());
			}
			else
			{
				muiNumVertices += puiNumVertices;
			}
		}

		return lResult;
	}

	EResult CVertexCache::AddIndices(uint32_t puiNumIndices, const void * pData)
	{
		EResult lResult = EResult::Success;

		if (muiNumIndices + puiNumIndices > muiMaxNumIndices)
		{
			LOG_DEBUG("Max index count reached for %s", mstrName.c_str());
			lResult = EResult::AddFailed;
		}
		else
		{
			uint32_t luiIndexSize = (mIndexType == EIndexType::SixteenBit) ? 2 : 4;
			uint32_t luiOffset = muiNumIndices * luiIndexSize;
			uint32_t luiSize = puiNumIndices * luiIndexSize;

			lResult = mIndexBuffer->UploadData(luiOffset, luiSize, pData);
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("IndexBuffer UploadData() failed for %s", mstrName.c_str());
			}
			else
			{
				muiNumIndices += puiNumIndices;
			}
		}

		return lResult;
	}
}