
#include "Graphics/BatchRenderer.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CBatchRenderer::CBatchRenderer(const String &pstrName, IGraphicsDevice *pGraphicsDevice)
		: mstrName(pstrName),
		mGraphicsDevice(pGraphicsDevice),
		mRenderData(nullptr),
		mDrawOperation(nullptr),
		mMaterial(nullptr),
		mVertexFormat(nullptr),
		mVertexBuffer(nullptr),
		mIndexBuffer(nullptr),
		muiMaxNumVertices(0),
		muiMaxNumIndices(0),
		muiNumVertices(0),
		muiNumIndices(0)
	{
	}

	CBatchRenderer::~CBatchRenderer()
	{
	}

	EResult CBatchRenderer::Create(uint32_t puiMaxNumVertices, uint32_t puiMaxNumIndices, 
		CVertexFormat * pVertexFormat, EIndexType pIndexType)
	{
		EResult lResult = EResult::Success;
		IRenderData *lRenderData = nullptr;
		IDrawOperation *lDrawOperation = nullptr;

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
						mIndexBuffer = mRenderData->GetIndexBuffer(luiIndexBufferId);
					}
					else
					{
						mIndexBuffer = nullptr;
					}

					lResult = mGraphicsDevice->CreateDrawOperation(mRenderData.get(),
						&lDrawOperation);

					if (lResult != EResult::Success)
					{
						LOG_DEBUG("CreateDrawOperation() failed");
					}
					else
					{
						mDrawOperation = IDrawOperationPtr(lDrawOperation);
						mDrawOperation->SetActiveIndexBuffer(muiMaxNumIndices > 0 ? 0 : -1);
						mDrawOperation->SetActiveVertexBuffer(0);
						mDrawOperation->SetNumIndices(0);
						mDrawOperation->SetStartVertex(0);
						mDrawOperation->SetNumVertices(0);
					}
				}
			}
		}

		return lResult;
	}

	EResult CBatchRenderer::Render(IMaterial * pMaterial, const void * pVertices,
		const void * pIndices, uint32_t puiNumVertices, uint32_t puiNumIndices)
	{
		EResult lResult = EResult::Success;

		if (mMaterial != pMaterial)
		{
			if (mMaterial != nullptr)
				Flush();

			mMaterial = pMaterial;
		}

		if ((muiNumVertices + puiNumVertices >= muiMaxNumVertices) ||
			(muiMaxNumIndices > 0 && (muiNumIndices + puiNumIndices >= muiMaxNumIndices)))
		{
			Flush();
		}

		lResult = Add(pVertices, pIndices, puiNumVertices, puiNumIndices);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CBatchRenderer::Add() failed");
		}

		return lResult;
	}

	EResult CBatchRenderer::Flush()
	{
		EResult lResult = EResult::Success;

		if (muiNumVertices == 0)
			return lResult;

		IProgram *lProgram = mMaterial->GetProgram(mPassType);

		if (lProgram != nullptr)
		{
			IBlendState *lBlendState = mMaterial->GetBlendState(mPassType);
			IRasterizerState *lRasterizerState = mMaterial->GetRasterizerState(mPassType);
			IDepthState *lDepthState = mMaterial->GetDepthState(mPassType);
			IStencilState *lStencilState = mMaterial->GetStencilState(mPassType);

			mDrawOperation->SetNumVertices(muiNumVertices);
			mDrawOperation->SetNumIndices(muiNumIndices);

			lProgram->Bind();
			
			if (lBlendState)
				lBlendState->Apply();

			if (lRasterizerState)
				lRasterizerState->Apply();

			if (lDepthState)
				lDepthState->Apply();

			if (lStencilState)
				lStencilState->Apply();

			for (uint32_t lIdx = 0; lIdx < mMaterial->GetNumTextures(mPassType); lIdx++)
			{
				uint32_t luiLoc = 0;
				ITexture *lTexture = mMaterial->GetTexture(mPassType, lIdx, &luiLoc);

				lProgram->GetConstants()->SetTexture(luiLoc, lIdx, nullptr);
			}

			mMaterial->StartPass(mPassType);
			mDrawOperation->Draw(mDrawOperation->GetActiveVertexBuffer(),
				mDrawOperation->GetActiveIndexBuffer());
			mMaterial->EndPass(mPassType);
		}

		muiNumVertices = 0;
		muiNumIndices = 0;

		return lResult;
	}

	EResult CBatchRenderer::Add(const void * pVertices, const void * pIndices, 
		uint32_t puiNumVertices, uint32_t puiNumIndices)
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

			lResult = mVertexBuffer->UploadData(luiOffset, luiSize, pVertices);
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("VertexBuffer UploadData() failed for %s", mstrName.c_str());
			}
			else
			{
				muiNumVertices += puiNumVertices;

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

					lResult = mIndexBuffer->UploadData(luiOffset, luiSize, pIndices);
					if (lResult != EResult::Success)
					{
						LOG_DEBUG("IndexBuffer UploadData() failed for %s", mstrName.c_str());
					}
					else
					{
						muiNumIndices += puiNumIndices;
					}
				}
			}
		}

		return lResult;
	}
}