
#include "Graphics/SpriteBatch.h"
#include "OS/GameLogger.h"
#include "Graphics/VertexCacheManager.h"

namespace FDX
{
	CSpriteBatch::CSpriteBatch()
		: mBatchRenderer(nullptr),
		mMaterial(nullptr),
		mBlendState(nullptr),
		mEnableDepthState(nullptr),
		mDisableDepthState(nullptr),
		mDisableBlendState(nullptr),
		mProjMatrix(CMatrix4F::IdentityMatrix()),
		mWorldMatrix(CMatrix4F::IdentityMatrix()),
		mColor(0.0f, 0.0f, 0.0f, 0.0f),
		mCurrentTexture(nullptr)
	{
	}

	CSpriteBatch::~CSpriteBatch()
	{
	}

	void CSpriteBatch::SetBlendState(IBlendState * pBlendState)
	{
		mBlendState = pBlendState;
	}

	void CSpriteBatch::SetProjMatrix(const CMatrix4F & pProj)
	{
		mProjMatrix = pProj;
	}

	void CSpriteBatch::SetWorldMatrix(const CMatrix4F & pWorld)
	{
		mWorldMatrix = pWorld;
	}

	void CSpriteBatch::SetColor(const CColor4F & pColor)
	{
		mColor = pColor;
	}

	EResult CSpriteBatch::Create(IGraphicsDevice *pGraphicsDevice, uint32_t puiMaxNumVertices, 
		uint32_t puiMaxNumIndices)
	{
		EResult lResult = EResult::Success;
		CVertexFormat *lVertexFormat = nullptr;

		lResult = CVertexCacheManager::Get().GetVertexFormat("VertexPCT", &lVertexFormat);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CVertexCacheManager::GetVertexFormat() failed for VertexPT");
		}
		else
		{
			mBatchRenderer = std::make_unique<CBatchRenderer>("SpriteBatch", pGraphicsDevice);
			lResult = mBatchRenderer->Create(puiMaxNumVertices, puiMaxNumIndices, 
				lVertexFormat, EIndexType::SixteenBit);

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("CBatchRenderer::Create() failed");
			}
			else
			{
				mBatchRenderer->GetDrawOperation()->SetPrimitiveType(
					EPrimitiveType::TriangleList);

				mMaterial = std::make_unique<CSpriteBatchMaterial>();
				lResult = mMaterial->Create();

				if (lResult != EResult::Success)
				{
					LOG_DEBUG("CSpriteBatchMaterial::Create() failed");
				}
				else
				{
					mMaterial->SetStartPassCallback(std::bind(&CSpriteBatch::StartPass,
						this, std::placeholders::_1, std::placeholders::_2));

					mMaterial->SetEndPassCallback(std::bind(&CSpriteBatch::EndPass,
						this, std::placeholders::_1));	

					SetupStates(pGraphicsDevice);
				}
			}
		}

		return lResult;
	}

	EResult CSpriteBatch::Begin()
	{
		EResult lResult = EResult::Success;

		mDisableDepthState->Apply();

		if (mBlendState != nullptr)
			mBlendState->Apply();

		return lResult;
	}

	EResult CSpriteBatch::End()
	{
		EResult lResult = EResult::Success;

		mBatchRenderer->Flush();
		mEnableDepthState->Apply();

		if (mBlendState != nullptr)
			mDisableBlendState->Apply();

		mCurrentTexture = nullptr;

		return lResult;
	}

	EResult CSpriteBatch::Draw(CTexture2D * pTexture, const CVector2F &pOrigin, 
		const CVector2F & pPosition, const CVector2F & pSize, 
		const CVector2F & pSrcPosition, const CVector2F & pSrcSize, 
		const CVector2F & pScale, float pfRotation, bool pbFlipX, bool pbFlipY)
	{
		EResult lResult = EResult::Success;

		if (pTexture != mCurrentTexture)
			SwitchTexture(pTexture);

		CVector2F lWorldOrigin = pPosition + pOrigin;
		float lfX = -pOrigin.X;
		float lfY = -pOrigin.Y;
		float lfX2 = pSize.X - pOrigin.X;
		float lfY2 = pSize.Y - pOrigin.Y;

		if (pScale.X != 1.0f && pScale.Y != 1.0f)
		{
			lfX *= pScale.X;
			lfY *= pScale.Y;
			lfX2 *= pScale.X;
			lfY2 *= pScale.Y;
		}

		CVector2F lfT1(lfX, lfY), lfT2(lfX, lfY2); 
		CVector2F lfT3(lfX2, lfY2), lfT4(lfX2, lfY);
		CVector2F lfP1, lfP2, lfP3, lfP4;

		if (pfRotation != 0.0f)
		{
			float lfCos = CMathUtil::Cos(pfRotation);
			float lfSin = CMathUtil::Sin(pfRotation);

			lfP1.X = lfCos * lfT1.X - lfSin * lfT1.Y;
			lfP1.Y = lfSin * lfT1.X + lfCos * lfT1.Y;

			lfP2.X = lfCos * lfT2.X - lfSin * lfT2.Y;
			lfP2.Y = lfSin * lfT2.X + lfCos * lfT2.Y;

			lfP3.X = lfCos * lfT3.X - lfSin * lfT3.Y;
			lfP3.Y = lfSin * lfT3.X + lfCos * lfT3.Y;

			lfP4.X = lfCos * lfT4.X - lfSin * lfT4.Y;
			lfP4.Y = lfSin * lfT4.X + lfCos * lfT4.Y;
		}
		else
		{
			lfP1 = lfT1; lfP2 = lfT2;
			lfP3 = lfT3; lfP4 = lfT4;
		}

		lfP1 += lWorldOrigin;
		lfP2 += lWorldOrigin;
		lfP3 += lWorldOrigin;
		lfP4 += lWorldOrigin;

		float lfU = pSrcPosition.X * mInvTextureSize.X;
		float lfV = (pSrcPosition.Y + pSrcSize.Y) * mInvTextureSize.Y;
		float lfU2 = (pSrcPosition.X + pSrcSize.X) * mInvTextureSize.X;
		float lfV2 = pSrcPosition.Y * mInvTextureSize.Y;

		if (pbFlipX)
		{
			float lfTmp = lfU;
			lfU = lfU2;
			lfU2 = lfTmp;
		}

		if (pbFlipY)
		{
			float lfTmp = lfV;
			lfV = lfV2;
			lfV2 = lfTmp;
		}

		uint16_t luiNumVertices = (uint16_t)mBatchRenderer->GetNumVertices();

		Array<SVertexPCT> lVertices;
		Array<uint16_t> lIndices;

		lVertices.emplace_back(lfP1.X, lfP1.Y, 0.0f, mColor.R, mColor.G, mColor.B, mColor.A, lfU, lfV);
		lVertices.emplace_back(lfP2.X, lfP2.Y, 0.0f, mColor.R, mColor.G, mColor.B, mColor.A, lfU, lfV2);
		lVertices.emplace_back(lfP3.X, lfP3.Y, 0.0f, mColor.R, mColor.G, mColor.B, mColor.A, lfU2, lfV2);
		lVertices.emplace_back(lfP4.X, lfP4.Y, 0.0f, mColor.R, mColor.G, mColor.B, mColor.A, lfU2, lfV);

		lIndices.push_back(luiNumVertices);
		lIndices.push_back(luiNumVertices + 1);
		lIndices.push_back(luiNumVertices + 2);
		lIndices.push_back(luiNumVertices + 2);
		lIndices.push_back(luiNumVertices + 3);
		lIndices.push_back(luiNumVertices);

		lResult = mBatchRenderer->Render(mMaterial.get(), lVertices.data(), 
			lIndices.data(), lVertices.size(), lIndices.size());

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CBatchRenderer::Render() failed");
		}

		return lResult;
	}

	EResult CSpriteBatch::Draw(CTexture2D * pTexture, const CVector2F & pPosition, const CVector2F & pSize, 
		const CVector2F & pSrcPosition, const CVector2F & pSrcSize, bool pbFlipX, bool pbFlipY)
	{
		EResult lResult = EResult::Success;

		if (pTexture != mCurrentTexture)
			SwitchTexture(pTexture);

		float lfX2 = pPosition.X + pSize.X;
		float lfY2 = pPosition.Y + pSize.Y;

		float lfU = pSrcPosition.X * mInvTextureSize.X;
		float lfV = pSrcPosition.Y * mInvTextureSize.Y;
		float lfU2 = (pSrcPosition.X + pSrcSize.X) * mInvTextureSize.X;
		float lfV2 = (pSrcPosition.Y + pSrcSize.Y) * mInvTextureSize.Y;

		if (pbFlipX)
		{
			float lfTmp = lfU;
			lfU = lfU2;
			lfU2 = lfTmp;
		}

		if (pbFlipY)
		{
			float lfTmp = lfV;
			lfV = lfV2;
			lfV2 = lfTmp;
		}

		uint16_t luiNumVertices = (uint16_t)mBatchRenderer->GetNumVertices();

		Array<SVertexPCT> lVertices;
		Array<uint16_t> lIndices;

		lVertices.emplace_back(pPosition.X, pPosition.Y, 0.0f, mColor.R, mColor.G, mColor.B, mColor.A, lfU, lfV);
		lVertices.emplace_back(pPosition.X, lfY2 , 0.0f, mColor.R, mColor.G, mColor.B, mColor.A, lfU, lfV2);
		lVertices.emplace_back(lfX2, lfY2, 0.0f, mColor.R, mColor.G, mColor.B, mColor.A, lfU2, lfV2);
		lVertices.emplace_back(lfX2, pPosition.Y, 0.0f, mColor.R, mColor.G, mColor.B, mColor.A, lfU2, lfV);

		lIndices.push_back(luiNumVertices);
		lIndices.push_back(luiNumVertices + 1);
		lIndices.push_back(luiNumVertices + 2);
		lIndices.push_back(luiNumVertices + 2);
		lIndices.push_back(luiNumVertices + 3);
		lIndices.push_back(luiNumVertices);

		lResult = mBatchRenderer->Render(mMaterial.get(), lVertices.data(),
			lIndices.data(), lVertices.size(), lIndices.size());

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CBatchRenderer::Render() failed");
		}

		return lResult;
	}

	EResult CSpriteBatch::Draw(CTexture2D * pTexture, const CVector2F & pPosition, 
		const CVector2F & pSrcPosition, const CVector2F & pSrcSize)
	{
		EResult lResult = EResult::Success;

		if (pTexture != mCurrentTexture)
			SwitchTexture(pTexture);

		float lfX2 = pPosition.X + pSrcSize.X;
		float lfY2 = pPosition.Y + pSrcSize.Y;

		float lfU = pSrcPosition.X * mInvTextureSize.X;
		float lfV = pSrcPosition.Y * mInvTextureSize.Y;
		float lfU2 = (pSrcPosition.X + pSrcSize.X) * mInvTextureSize.X;
		float lfV2 = (pSrcPosition.Y + pSrcSize.Y) * mInvTextureSize.Y;
		
		uint16_t luiNumVertices = (uint16_t)mBatchRenderer->GetNumVertices();

		Array<SVertexPCT> lVertices;
		Array<uint16_t> lIndices;

		lVertices.emplace_back(pPosition.X, pPosition.Y, 0.0f, mColor.R, mColor.G, mColor.B, mColor.A, lfU, lfV);
		lVertices.emplace_back(pPosition.X, lfY2, 0.0f, mColor.R, mColor.G, mColor.B, mColor.A, lfU, lfV2);
		lVertices.emplace_back(lfX2, lfY2, 0.0f, mColor.R, mColor.G, mColor.B, mColor.A, lfU2, lfV2);
		lVertices.emplace_back(lfX2, pPosition.Y, 0.0f, mColor.R, mColor.G, mColor.B, mColor.A, lfU2, lfV);

		lIndices.push_back(luiNumVertices);
		lIndices.push_back(luiNumVertices + 1);
		lIndices.push_back(luiNumVertices + 2);
		lIndices.push_back(luiNumVertices + 2);
		lIndices.push_back(luiNumVertices + 3);
		lIndices.push_back(luiNumVertices);

		lResult = mBatchRenderer->Render(mMaterial.get(), lVertices.data(),
			lIndices.data(), lVertices.size(), lIndices.size());

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CBatchRenderer::Render() failed");
		}

		return lResult;
	}

	EResult CSpriteBatch::Draw(CTexture2D * pTexture, const CVector2F & pPosition, const CVector2F & pSize)
	{
		EResult lResult = EResult::Success;

		if (pTexture != mCurrentTexture)
			SwitchTexture(pTexture);

		float lfX2 = pPosition.X + pSize.X;
		float lfY2 = pPosition.Y + pSize.Y;

		float lfU = 0.0f;
		float lfV = 0.0f;
		float lfU2 = 1.0f;
		float lfV2 = 1.0f;

		uint16_t luiNumVertices = (uint16_t)mBatchRenderer->GetNumVertices();

		Array<SVertexPCT> lVertices;
		Array<uint16_t> lIndices;

		lVertices.emplace_back(pPosition.X, pPosition.Y, 0.0f, mColor.R, mColor.G, mColor.B, mColor.A, lfU, lfV);
		lVertices.emplace_back(pPosition.X, lfY2, 0.0f, mColor.R, mColor.G, mColor.B, mColor.A, lfU, lfV2);
		lVertices.emplace_back(lfX2, lfY2, 0.0f, mColor.R, mColor.G, mColor.B, mColor.A, lfU2, lfV2);
		lVertices.emplace_back(lfX2, pPosition.Y, 0.0f, mColor.R, mColor.G, mColor.B, mColor.A, lfU2, lfV);

		lIndices.push_back(luiNumVertices);
		lIndices.push_back(luiNumVertices + 1);
		lIndices.push_back(luiNumVertices + 2);
		lIndices.push_back(luiNumVertices + 2);
		lIndices.push_back(luiNumVertices + 3);
		lIndices.push_back(luiNumVertices);

		lResult = mBatchRenderer->Render(mMaterial.get(), lVertices.data(),
			lIndices.data(), lVertices.size(), lIndices.size());

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CBatchRenderer::Render() failed");
		}

		return lResult;
	}

	EResult CSpriteBatch::Draw(CTexture2D * pTexture, const CVector2F & pPosition)
	{
		return Draw(pTexture, pPosition, CVector2F((float)pTexture->GetWidth(), 
			(float)pTexture->GetHeight()));
	}

	void CSpriteBatch::StartPass(EPassType pPassType, IProgram * pProgram)
	{
		if (pProgram != nullptr)
		{
			pProgram->GetConstants()->SetMatrix4("uMVP", mProjMatrix * mWorldMatrix);
			pProgram->GetConstants()->SetColor4("uColor", mColor);
			
			uint32_t luiLoc = 0;
			ITexture *lTexture = mMaterial->GetTexture(pPassType, 0, &luiLoc);

			if (lTexture != nullptr)
				pProgram->GetConstants()->SetTexture(luiLoc, 0, lTexture);
		}
	}

	void CSpriteBatch::EndPass(EPassType pPassType)
	{
	}

	EResult CSpriteBatch::SwitchTexture(CTexture2D * pTexture)
	{
		EResult lResult = EResult::Success;
				
		mBatchRenderer->Flush();
		
		mCurrentTexture = pTexture;
		mMaterial->SetTexture(mCurrentTexture);

		if (pTexture != nullptr)
		{
			mInvTextureSize = CVector2F(1.0f / mCurrentTexture->GetWidth(),
				1.0f / mCurrentTexture->GetHeight());
		}
		else
		{
			mInvTextureSize = CVector2F(0.0f);
		}

		return lResult;
	}

	EResult CSpriteBatch::SetupStates(IGraphicsDevice *pGraphicsDevice)
	{
		EResult lResult = EResult::Success;

		IDepthState *lDepthState = nullptr;
		IBlendState *lBlendState = nullptr;

		SDepthStateProperties lDepthStateProps;
		lDepthStateProps.Enabled = true;
		lDepthStateProps.Writeable = false;
		
		pGraphicsDevice->CreateDepthState(lDepthStateProps, &lDepthState);
		mDisableDepthState = IDepthStatePtr(lDepthState);

		lDepthStateProps.Writeable = false;

		pGraphicsDevice->CreateDepthState(lDepthStateProps, &lDepthState);
		mEnableDepthState = IDepthStatePtr(lDepthState);

		SBlendStateProperties lBlendStateProps;
		lBlendStateProps.BlendEnabled = false;

		pGraphicsDevice->CreateBlendState(lBlendStateProps, &lBlendState);
		mDisableBlendState = IBlendStatePtr(lBlendState);

		return lResult;
	}
}