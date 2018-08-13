
#include "Graphics/Canvas.h"
#include "Graphics/ProgramManager.h"
#include "Graphics/VertexCacheManager.h"
#include "Graphics/CanvasMaterial.h"
#include "OS/GameLogger.h"

#define Coord(a, a1, a2) ((a - a1) / (a2 - a1))

namespace FDX
{
	CCanvas::CCanvas()
	{
	}

	CCanvas::~CCanvas()
	{
	}

	EResult CCanvas::Create(IGraphicsDevice *pGraphicsDevice, uint32_t puiMaxNumVertices, 
		uint32_t puiMaxNumIndices)
	{
		EResult lResult = EResult::Success;
		CVertexFormat *lVertexFormat = nullptr;

		lResult = CSpriteBatch::Create(pGraphicsDevice, puiMaxNumVertices, 
			puiMaxNumIndices);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CSpriteBatch::Create() failed for VertexPCT");
		}
		else
		{
			lResult = CVertexCacheManager::Get().GetVertexFormat("VertexPCT", &lVertexFormat);
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("CVertexCacheManager::GetVertexFormat() failed for VertexPCT");
			}
			else
			{				
				mMaterial = std::make_unique<CCanvasMaterial>();
				lResult = mMaterial->Create();

				if (lResult != EResult::Success)
				{
					LOG_DEBUG("CSpriteBatchMaterial::Create() failed");
				}
				else
				{
					CTexture2D *lTexture = nullptr;
					lResult = pGraphicsDevice->CreateTexture2D("FontTexture",
						ETextureUsage::Normal, &lTexture);

					if (lResult != EResult::Success)
					{
						LOG_DEBUG("CreateTexture2D failed for TextTexture");
					}
					else
					{
						mFontTexture = CTexture2DPtr(lTexture);
						mMaterial->SetStartPassCallback(std::bind(&CCanvas::StartPass,
							this, std::placeholders::_1, std::placeholders::_2));

						mMaterial->SetEndPassCallback(std::bind(&CCanvas::EndPass,
							this, std::placeholders::_1));
					}
				}
			}
		}

		return lResult;
	}

	EResult CCanvas::Begin()
	{
		EResult lResult = EResult::Success;

		lResult = CSpriteBatch::Begin();
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CCanvas::Begin() failed");
		}

		return lResult;
	}

	EResult CCanvas::End()
	{
		EResult lResult = EResult::Success;
		
		lResult = CSpriteBatch::End();
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CCanvas::End() failed");
		}

		return lResult;
	}

	EResult CCanvas::DrawRect(const CVector2F & pPosition, const CVector2F & pSize, const CColor4F & pColor)
	{
		EResult lResult = EResult::Success;

		if (mCurrentTexture != nullptr)
			SwitchTexture(nullptr);

		float lfX2 = pPosition.X + pSize.X;
		float lfY2 = pPosition.Y + pSize.Y;

		float lfU = 0.0f;
		float lfV = 0.0f;
		float lfU2 = 1.0f;
		float lfV2 = 1.0f;
		
		uint16_t luiNumVertices = (uint16_t)mBatchRenderer->GetNumVertices();

		Array<SVertexPCT> lVertices;
		lVertices.emplace_back(pPosition.X, pPosition.Y, 0.0f, pColor.R, pColor.G, pColor.B, pColor.A, lfU, lfV);
		lVertices.emplace_back(pPosition.X, lfY2, 0.0f, pColor.R, pColor.G, pColor.B, pColor.A, lfU, lfV2);
		lVertices.emplace_back(lfX2, lfY2, 0.0f, pColor.R, pColor.G, pColor.B, pColor.A, lfU2, lfV2);
		lVertices.emplace_back(lfX2, pPosition.Y, 0.0f, pColor.R, pColor.G, pColor.B, pColor.A, lfU2, lfV);
			
		Array<uint16_t> lIndices;
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

	EResult CCanvas::DrawBorder(const CVector2F & pPosition, const CVector2F & pSize, uint32_t pRectBorder,
		float pfBorderWidth, UnorderedMap<EBorderType, CColor4F> &pBorderColors)
	{
		EResult lResult = EResult::Success;

		if (mCurrentTexture != nullptr)
			SwitchTexture(nullptr);

		float lfX1 = pPosition.X;
		float lfY1 = pPosition.Y;
		float lfX2 = pPosition.X + pSize.X;
		float lfY2 = pPosition.Y + pSize.Y;

		float lfX11 = pPosition.X + pfBorderWidth;
		float lfY11 = pPosition.Y + pfBorderWidth;
		float lfX12 = pPosition.X + pSize.X - pfBorderWidth;
		float lfY12 = pPosition.Y + pSize.Y - pfBorderWidth;

		float lfU = 0.0f;
		float lfV = 0.0f;
		float lfU2 = 1.0f;
		float lfV2 = 1.0f;

		if (!(pRectBorder & EBorderType::Top))
			lfY11 -= pfBorderWidth;

		if (!(pRectBorder & EBorderType::Left))
			lfX11 -= pfBorderWidth;

		if (!(pRectBorder & EBorderType::Bottom))
			lfY12 += pfBorderWidth;

		if (!(pRectBorder & EBorderType::Right))
			lfX12 += pfBorderWidth;

		Array<SVertexPCT> lVertices;

		if (pRectBorder & EBorderType::Top)
		{
			const auto &lColor = pBorderColors[EBorderType::Top];

			lVertices.emplace_back(lfX1, lfY1, 0.0f,
				lColor.R, lColor.G, lColor.B, lColor.A, lfU, lfV);
			lVertices.emplace_back(lfX11, lfY11, 0.0f,
				lColor.R, lColor.G, lColor.B, lColor.A, lfU, lfV);
			lVertices.emplace_back(lfX12, lfY11, 0.0f,
				lColor.R, lColor.G, lColor.B, lColor.A, lfU, lfV);
			lVertices.emplace_back(lfX2, lfY1, 0.0f,
				lColor.R, lColor.G, lColor.B, lColor.A, lfU, lfV);
		}

		if (pRectBorder & EBorderType::Left)
		{
			const auto &lColor = pBorderColors[EBorderType::Left];

			lVertices.emplace_back(lfX1, lfY1, 0.0f,
				lColor.R, lColor.G, lColor.B, lColor.A, lfU, lfV);
			lVertices.emplace_back(lfX1, lfY2, 0.0f,
				lColor.R, lColor.G, lColor.B, lColor.A, lfU, lfV);
			lVertices.emplace_back(lfX11, lfY12, 0.0f,
				lColor.R, lColor.G, lColor.B, lColor.A, lfU, lfV);
			lVertices.emplace_back(lfX11, lfY11, 0.0f,
				lColor.R, lColor.G, lColor.B, lColor.A, lfU, lfV);
		}

		if (pRectBorder & EBorderType::Bottom)
		{
			const auto &lColor = pBorderColors[EBorderType::Bottom];

			lVertices.emplace_back(lfX11, lfY12, 0.0f,
				lColor.R, lColor.G, lColor.B, lColor.A, lfU, lfV);
			lVertices.emplace_back(lfX1, lfY2, 0.0f,
				lColor.R, lColor.G, lColor.B, lColor.A, lfU, lfV);
			lVertices.emplace_back(lfX2, lfY2, 0.0f,
				lColor.R, lColor.G, lColor.B, lColor.A, lfU, lfV);
			lVertices.emplace_back(lfX12, lfY12, 0.0f,
				lColor.R, lColor.G, lColor.B, lColor.A, lfU, lfV);
		}

		if (pRectBorder & EBorderType::Right)
		{
			const auto &lColor = pBorderColors[EBorderType::Right];

			lVertices.emplace_back(lfX12, lfY11, 0.0f,
				lColor.R, lColor.G, lColor.B, lColor.A, lfU, lfV);
			lVertices.emplace_back(lfX12, lfY12, 0.0f,
				lColor.R, lColor.G, lColor.B, lColor.A, lfU, lfV);
			lVertices.emplace_back(lfX2, lfY2, 0.0f,
				lColor.R, lColor.G, lColor.B, lColor.A, lfU, lfV);
			lVertices.emplace_back(lfX2, lfY1, 0.0f,
				lColor.R, lColor.G, lColor.B, lColor.A, lfU, lfV);
		}

		uint16_t luiNumVertices = (uint16_t)mBatchRenderer->GetNumVertices();

		Array<uint16_t> lIndices;
		for (uint32_t lIdx = 0; lIdx < (lVertices.size() / 4); lIdx++)
		{
			lIndices.push_back(luiNumVertices);
			lIndices.push_back(luiNumVertices + 1);
			lIndices.push_back(luiNumVertices + 2);
			lIndices.push_back(luiNumVertices + 2);
			lIndices.push_back(luiNumVertices + 3);
			lIndices.push_back(luiNumVertices);

			luiNumVertices += 4;
		}

		lResult = mBatchRenderer->Render(mMaterial.get(), lVertices.data(),
			lIndices.data(), lVertices.size(), lIndices.size());

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CBatchRenderer::Render() failed");
		}

		return lResult;
	}

	EResult CCanvas::DrawRectWithBorder(const CVector2F & pPosition, const CVector2F & pSize, 
		const CColor4F & pColor, uint32_t pRectBorder, float pfBorderWidth,
		UnorderedMap<EBorderType, CColor4F> &pBorderColors)
	{
		EResult lResult = EResult::Success;

		if (mCurrentTexture != nullptr)
			SwitchTexture(nullptr);

		float lfX1 = pPosition.X;
		float lfY1 = pPosition.Y;
		float lfX2 = pPosition.X + pSize.X;
		float lfY2 = pPosition.Y + pSize.Y;

		float lfU = 0.0f;
		float lfV = 0.0f;
		float lfU2 = 1.0f;
		float lfV2 = 1.0f;

		Array<SVertexPCT> lVertices;

		lVertices.emplace_back(lfX1, lfY1, 0.0f, pColor.R, pColor.G, pColor.B, pColor.A, lfU, lfV);
		lVertices.emplace_back(lfX1, lfY2, 0.0f, pColor.R, pColor.G, pColor.B, pColor.A, lfU, lfV2);
		lVertices.emplace_back(lfX2, lfY2, 0.0f, pColor.R, pColor.G, pColor.B, pColor.A, lfU2, lfV2);
		lVertices.emplace_back(lfX2, lfY1, 0.0f, pColor.R, pColor.G, pColor.B, pColor.A, lfU2, lfV);

		uint16_t luiNumVertices = (uint16_t)mBatchRenderer->GetNumVertices();

		Array<uint16_t> lIndices;
		for (uint32_t lIdx = 0; lIdx < (lVertices.size() / 4); lIdx++)
		{
			lIndices.push_back(luiNumVertices);
			lIndices.push_back(luiNumVertices + 1);
			lIndices.push_back(luiNumVertices + 2);
			lIndices.push_back(luiNumVertices + 2);
			lIndices.push_back(luiNumVertices + 3);
			lIndices.push_back(luiNumVertices);

			luiNumVertices += 4;
		}

		lResult = mBatchRenderer->Render(mMaterial.get(), lVertices.data(),
			lIndices.data(), lVertices.size(), lIndices.size());

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CBatchRenderer::Render() failed");
		}
		else
		{
			lResult = DrawBorder(pPosition, pSize, pRectBorder, pfBorderWidth, pBorderColors);
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("CCanvas::DrawBorder() failed");
			}
		}

		return lResult;
	}

	EResult CCanvas::DrawText(const CVector2F & pPosition, const CVector2F & pRectSize, 
		const String & pstrString, uint32_t puiSize, 
		const CColor4F & pColor, IFont * pFont)
	{
		EResult lResult = EResult::Success;
		CBitmap *lBitmap = nullptr;

		lResult = pFont->DrawText(pstrString, puiSize, pColor, true, &lBitmap);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("DrawText() failed");
		}
		else
		{
			UpdateFontTexture(lBitmap);

			lResult = Draw(mFontTexture.get(), pPosition, pRectSize);
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("TextureRect() failed");
			}

			SAFE_DELETE(lBitmap);
		}

		return lResult;
	}

	EResult CCanvas::DrawText(const CVector2F & pPosition, const CVector2F & pSrcPosition, 
		const CVector2F & pSrcSize, const String & pstrString, uint32_t puiSize, 
		const CColor4F & pColor, IFont * pFont)
	{
		EResult lResult = EResult::Success;
		CBitmap *lBitmap = nullptr;

		lResult = pFont->DrawText(pstrString, puiSize, pColor, true, &lBitmap);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("DrawText() failed");
		}
		else
		{
			UpdateFontTexture(lBitmap);

			lResult = Draw(mFontTexture.get(), pPosition, pSrcSize, pSrcPosition, pSrcSize, false, false);

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("TextureRect() failed");
			}

			SAFE_DELETE(lBitmap);
		}

		return lResult;
	}

	EResult CCanvas::UpdateFontTexture(CBitmap * pBitmap)
	{
		EResult lResult = EResult::Success;
		
		lResult = mFontTexture->Load(pBitmap);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("Texture2D load failed");
		}

		return lResult;
	}

	void CCanvas::StartPass(EPassType pPassType, IProgram * pProgram)
	{
		CSpriteBatch::StartPass(pPassType, pProgram);
	}

	void CCanvas::EndPass(EPassType pPassType)
	{
		CSpriteBatch::EndPass(pPassType);
	}

	EResult CCanvas::SetupStates(IGraphicsDevice * pGraphicsDevice)
	{
		EResult lResult = EResult::Success;

		lResult = CSpriteBatch::SetupStates(pGraphicsDevice);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CSpriteBatch::SetupStates() failed");
		}
		else
		{
			IBlendState *lBlendState;
			SBlendStateProperties lBlendStateProperties;

			lBlendStateProperties.BlendEnabled = true;
			lBlendStateProperties.SourceColorBlend = EBlendOption::SrcAlpha;
			lBlendStateProperties.DestinationColorBlend = EBlendOption::OneMinusSrcAlpha;

			lResult = pGraphicsDevice->CreateBlendState(lBlendStateProperties, &lBlendState);
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("CreateBlendState() failed");
			}
			else
			{
				mEnableBlendState = IBlendStatePtr(lBlendState);
				SetBlendState(mEnableBlendState.get());
			}
		}

		return lResult;
	}
}