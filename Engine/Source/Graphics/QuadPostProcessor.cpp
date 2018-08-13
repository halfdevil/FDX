#pragma once

#include "Graphics/QuadPostProcessor.h"
#include "Graphics/VertexCacheManager.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CQuadPostProcessor::CQuadPostProcessor()
		: mTexture(nullptr)
	{
	}

	CQuadPostProcessor::~CQuadPostProcessor()
	{
	}

	EResult CQuadPostProcessor::Create()
	{
		EResult lResult = EResult::Success;
		IDrawOperation *lDrawOperation = nullptr;

		Array<SVertexPT> lVertices =
		{
			{ -0.5f, -0.5f, 0.0f, 0.0f, 0.0f },
			{ 0.5f, -0.5f, 0.0f, 1.0f, 0.0f },
			{ 0.5f, 0.5f, 0.0f, 1.0f, 1.0f },
			{ -0.5f, 0.5f, 0.0f, 0.0f, 1.0f }
		};

		Array<uint16_t> lIndices =
		{
			0, 1, 2,
			0, 2, 3
		};

		lResult = CVertexCacheManager::Get().AddVertices("VertexPT", EIndexType::SixteenBit,
			lVertices.size(), lIndices.size(), lVertices.data(), 
			lIndices.data(), &lDrawOperation);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CVertexCacheManager::AddVertices() failed");
		}
		else
		{
			mDrawOperation = IDrawOperationPtr(lDrawOperation);
			mDrawOperation->SetPrimitiveType(EPrimitiveType::TriangleList);
		}

		return lResult;
	}

	void CQuadPostProcessor::SetTexture(CTexture2D * pTexture)
	{
		mTexture = pTexture;
	}

	EResult CQuadPostProcessor::Process(IProgram * pProgram)
	{
		EResult lResult = EResult::Success;

		if (pProgram != nullptr)
		{
			pProgram->Bind();
			pProgram->GetConstants()->SetTexture("uTexture", 0, mTexture);

			mDrawOperation->Draw(mDrawOperation->GetActiveVertexBuffer(),
				mDrawOperation->GetActiveIndexBuffer());
		}

		return lResult;
	}
}