
#include "Scene/TerrainEntity.h"
#include "OS/GameLogger.h"
#include "Core/ResourceManager.h"
#include "Graphics/TextureManager.h"
#include "Graphics/VertexCacheManager.h"
#include "Graphics/VertexTypes.h"
#include "Scene/DebugRenderer.h"

namespace FDX
{
	CTerrainPatch::CTerrainPatch(uint32_t puiPatchIndex)
		: miCurrentLOD(-1),
		muiPatchIndex(puiPatchIndex),
		mTop(nullptr),
		mBottom(nullptr),
		mLeft(nullptr),
		mRight(nullptr)
	{
	}

	CTerrainPatch::~CTerrainPatch()
	{
	}

	void CTerrainPatch::SetCurrentLOD(int32_t piLOD)
	{
		miCurrentLOD = piLOD;
	}

	void CTerrainPatch::SetMaterial(IMaterial * pMaterial)
	{
		mMaterial = pMaterial;
	}

	void CTerrainPatch::SetDrawOperation(IDrawOperation * pDrawOperation)
	{
		mDrawOperation = pDrawOperation;
	}

	void CTerrainPatch::UpdateGraphics(CCamera * pCamera, IProgram * pProgram)
	{
		if (pProgram)
		{
			pProgram->GetConstants()->SetInt("uPatchIndex", (int32_t)muiPatchIndex);
			pProgram->GetConstants()->SetVector4("uPatchOuterLevels", mPatchOuterLevels);
		}
	}

	CTerrainNode::CTerrainNode(CTerrainEntity * pTerrain, const Array<CVector3F>& pPositions, 
		uint32_t puiStartX, uint32_t puiStartZ, uint32_t puiSize, uint32_t puiTerrainSize, 
		uint32_t puiPatchSize, uint32_t puiNumPatchesPerRow)
		: mTerrain(pTerrain),
		muiStartX(puiStartX),
		muiStartZ(puiStartZ),
		muiSize(puiSize),
		mTerrainPatch(nullptr)
	{		
		mbIsLeafNode = muiSize <= puiPatchSize;

		Array<CVector3F> lPositions;
		for (uint32_t lIdx = muiStartZ; lIdx <= muiStartZ + muiSize; lIdx++)
		{
			for (uint32_t lJdx = muiStartX; lJdx <= muiStartX + muiSize; lJdx++)
			{
				if (lIdx < puiTerrainSize && lJdx < puiTerrainSize)
					lPositions.push_back(pPositions[lIdx * puiTerrainSize + lJdx]);
			}
		}

		mAxisAlignedBox = CAxisAlignedBox::CreateFromPoints(lPositions);

		if (mbIsLeafNode)
		{
			mTerrainPatch = mTerrain->GetTerrainPatch(muiStartX / (puiPatchSize - 1) + 
				(muiStartZ / (puiPatchSize - 1)) * puiNumPatchesPerRow);

			CDebugRenderer::Get().AddAxisAlignedBox(mAxisAlignedBox);
		}
		else
			CreateChildNodes(pPositions, puiTerrainSize, puiPatchSize, puiNumPatchesPerRow);
	}

	CTerrainNode::~CTerrainNode()
	{
	}

	void CTerrainNode::UpdatePatches(CCamera * pCamera)
	{		
		const CFrustum &lCamFrustum = pCamera->GetFrustum();		

		if (lCamFrustum.Contains(mAxisAlignedBox) != EContainmentType::Culled)
		{
			mbVisible = true;

			if (mbIsLeafNode)
			{				
				CVector3F lCamPosition = pCamera->GetPosition();
				lCamPosition.Y = 0.0f;

				const auto &lLODDistanceThresholds = mTerrain->GetLODDistanceThresholds();				
				float lfDist = lCamPosition.Distance(mTerrainPatch->GetCenter());				

				mTerrainPatch->SetCurrentLOD(1 << mTerrain->GetMaxLOD());

				for (uint32_t lIdx = 0; lIdx < mTerrain->GetMaxLOD(); lIdx++)
				{
					if (lfDist > lLODDistanceThresholds[lIdx])
					{
						mTerrainPatch->SetCurrentLOD(1 << (lIdx + 1));
						break;
					}
				}
			}
			else
			{
				GetChildUL()->UpdatePatches(pCamera);
				GetChildUR()->UpdatePatches(pCamera);
				GetChildLL()->UpdatePatches(pCamera);
				GetChildLR()->UpdatePatches(pCamera);
			}
		}
		else
		{
			DisableNode();
		}
	}

	void CTerrainNode::DisableNode()
	{
		mbVisible = false;

		if (!mbIsLeafNode)
		{
			GetChildUL()->DisableNode();
			GetChildUR()->DisableNode();
			GetChildLL()->DisableNode();
			GetChildLR()->DisableNode();
		}
		else
		{			
			mTerrainPatch->SetCurrentLOD(-1);
		}
	}

	void CTerrainNode::CreateChildNodes(const Array<CVector3F>& pPositions, 
		uint32_t puiTerrainSize, uint32_t puiPatchSize, 
		uint32_t puiNumPatches)
	{
		CTerrainNodePtr lNodeUL = std::make_unique<CTerrainNode>(mTerrain, pPositions, muiStartX, 
			muiStartZ, muiSize / 2, puiTerrainSize, puiPatchSize, puiNumPatches);

		CTerrainNodePtr lNodeUR = std::make_unique<CTerrainNode>(mTerrain, pPositions, muiStartX + muiSize / 2,
			muiStartZ, muiSize / 2, puiTerrainSize, puiPatchSize, puiNumPatches);

		CTerrainNodePtr lNodeLL = std::make_unique<CTerrainNode>(mTerrain, pPositions, muiStartX,
			muiStartZ + muiSize / 2, muiSize / 2, puiTerrainSize, puiPatchSize, puiNumPatches);

		CTerrainNodePtr lNodeLR = std::make_unique<CTerrainNode>(mTerrain, pPositions, muiStartX + muiSize / 2,
			muiStartZ + muiSize / 2, muiSize / 2, puiTerrainSize, puiPatchSize, puiNumPatches);

		AddChild(std::move(lNodeUL));
		AddChild(std::move(lNodeUR));
		AddChild(std::move(lNodeLL));
		AddChild(std::move(lNodeLR));
	}

	CTerrainParameters::CTerrainParameters()
		: muiSize(0),
		muiPatchSize(0),
		mfHeightScale(0.0f),
		mfHeightOffset(0.0f)
	{
		mDiffuseMaps.resize(4);
	}

	CTerrainParameters::~CTerrainParameters()
	{
	}

	void CTerrainParameters::SetSize(uint32_t puiSize)
	{
		muiSize = puiSize;
	}

	void CTerrainParameters::SetPatchSize(uint32_t puiPatchSize)
	{
		muiPatchSize = puiPatchSize;
	}

	void CTerrainParameters::SetHeightMap(const String & pstrHeightMap)
	{
		mstrHeightMap = pstrHeightMap;
	}

	void CTerrainParameters::SetColorMap(const String & pstrColorMap)
	{
		mstrColorMap = pstrColorMap;
	}
	
	void CTerrainParameters::SetHeightScale(float pfHeightScale)
	{
		mfHeightScale = pfHeightScale;
	}

	void CTerrainParameters::SetHeightOffset(float pfHeightOffset)
	{
		mfHeightOffset = pfHeightOffset;
	}

	void CTerrainParameters::SetTileScale(float pfTileScale)
	{
		mfTileScale = pfTileScale;
	}

	void CTerrainParameters::SetHeightMapType(ETerrainHeightMapType pHeightMapType)
	{
		mHeightMapType = pHeightMapType;
	}

	void CTerrainParameters::SetDetailMap(const String & pstrDetailMap)
	{
		mstrDetailMap = pstrDetailMap;
	}

	void CTerrainParameters::SetDiffuseMap(uint32_t puiIndex, const String & pstrDiffuseMap)
	{
		if (puiIndex < mDiffuseMaps.size())
			mDiffuseMaps[puiIndex] = pstrDiffuseMap;
	}

	CTerrainEntity::CTerrainEntity()
		: mTerrainQuadTree(nullptr),
		mTerrainHeightMap(nullptr),
		mMaterial(nullptr),
		mDrawOperation(nullptr),
		mfOldCameraFOV(0.0f),
		mfCameraFOVDelta(0.0f),
		mfCameraMovementDelta(0.0f),
		mfCameraRotationDelta(0.0f)
	{
	}

	CTerrainEntity::~CTerrainEntity()
	{
	}

	EResult CTerrainEntity::Load(const CTerrainParameters & pParameters)
	{
		EResult lResult = EResult::Success;
		
		if (pParameters.GetHeightMapType() == ETerrainHeightMapType::Image)		
			mTerrainHeightMap = std::make_unique<CImageTerrainHeightMap>();

		lResult = mTerrainHeightMap->Load(pParameters.GetHeightMap(),
			pParameters.GetHeightScale(), pParameters.GetHeightOffset(), 0, 0);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CImageTerrainHeightMap::Load() failed for %s", 
				pParameters.GetHeightMap().c_str());
		}
		else
		{
			mTerrainParameters = pParameters;

			lResult = LoadMaterial();
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("Unable to load Terrain material");
			}
			else
			{
				lResult = LoadRenderData();
				if (lResult != EResult::Success)
				{
					LOG_DEBUG("Unable to load Terrain render data");
				}
			}

			if (lResult == EResult::Success)
			{
				switch (pParameters.GetPatchSize())
				{
				case 2:
					muiMaxLOD = 1;
					break;

				case 4:
					muiMaxLOD = 2;
					break;

				case 8:
					muiMaxLOD = 3;
					break;

				case 16:
					muiMaxLOD = 4;
					break;

				case 32:
					muiMaxLOD = 5;
					break;

				case 64:
					muiMaxLOD = 6;
					break;

				default:
					muiMaxLOD = 3;
					break;
				}

				Array<CVector3F> lPositions(pParameters.GetSize() * pParameters.GetSize());
				float lfHalfSize = (float)pParameters.GetSize() * 0.5f *
					pParameters.GetTileScale();

				uint32_t lKdx = 0;
				for (float lIdx = -lfHalfSize; lIdx < lfHalfSize; lIdx += pParameters.GetTileScale())
				{
					for (float lJdx = -lfHalfSize; lJdx < lfHalfSize; lJdx += pParameters.GetTileScale())
					{
						lPositions[lKdx] = CVector3F(lJdx, mTerrainHeightMap->GetHeightAt(lKdx), lIdx);
						lKdx++;
					}
				}

				CreatePatches();
				CalculatePatchData(lPositions);
				CalculateDistanceThresholds();

				mTerrainQuadTree = std::make_unique<CTerrainNode>(this, lPositions, 0, 0, 
					mTerrainParameters.GetSize(), mTerrainParameters.GetSize(), 
					mTerrainParameters.GetPatchSize(), muiNumPatchesPerRow);

				mOldCameraPos = CVector3F(-999999.0f);
				mOldCameraRot = CVector3F(-999999.0f);
				mOldCameraUp = CVector3F(-999999.0f);

				mfCameraMovementDelta = 10.0f * mTerrainParameters.GetTileScale();
				mfCameraRotationDelta = ToRadians(1.0f);
				mfCameraFOVDelta = 0.1f;
				mfCameraUpDelta = 1.0f - CMathUtil::Cos(mfCameraRotationDelta);
			}
		}

		return lResult;
	}

	void CTerrainEntity::StartPass(EPassType pPassType, IProgram * pProgram)
	{
		if (pProgram)
		{
			pProgram->GetConstants()->SetFloat("uTileScale", mTerrainParameters.GetTileScale());
			pProgram->GetConstants()->SetFloat("uHeightScale", mTerrainParameters.GetHeightScale());
			pProgram->GetConstants()->SetFloat("uHeightOffset", mTerrainParameters.GetHeightOffset());
			pProgram->GetConstants()->SetFloat("uPatchSize", (float)mTerrainParameters.GetPatchSize());
			pProgram->GetConstants()->SetFloat("uSize", (float)mTerrainParameters.GetSize());
			pProgram->GetConstants()->SetInt("uNumPatches", muiNumPatchesPerRow);
		}
	}

	void CTerrainEntity::EndPass(EPassType pPassType)
	{
	}

	void CTerrainEntity::CreatePatches()
	{
		muiNumPatchesPerRow = mTerrainParameters.GetSize() / 
			mTerrainParameters.GetPatchSize();

		for (uint32_t lIdx = 0; lIdx < muiNumPatchesPerRow * muiNumPatchesPerRow; lIdx++)
			mTerrainPatches.push_back(std::make_unique<CTerrainPatch>(lIdx));
	}

	void CTerrainEntity::CalculatePatchData(const Array<CVector3F> &pPositions)
	{
		for (uint32_t lIdx = 0; lIdx < muiNumPatchesPerRow; lIdx++)
		{
			for (uint32_t lJdx = 0; lJdx < muiNumPatchesPerRow; lJdx++)
			{
				uint32_t luiIndex = lIdx * muiNumPatchesPerRow + lJdx;

				mTerrainPatches[luiIndex]->miCurrentLOD = 1 << muiMaxLOD;
				mTerrainPatches[luiIndex]->mMaterial = mMaterial.get();
				mTerrainPatches[luiIndex]->mDrawOperation = mDrawOperation.get();

				Array<CVector3F> lPatchPositions;
				for (uint32_t lKdx = lIdx * mTerrainParameters.GetPatchSize(); 
					lKdx < (lIdx + 1) * mTerrainParameters.GetPatchSize(); lKdx++)
				{
					for (uint32_t lLdx = lJdx * mTerrainParameters.GetPatchSize(); 
						lLdx < (lJdx + 1) * mTerrainParameters.GetPatchSize(); lLdx++)
					{
						lPatchPositions.emplace_back(pPositions[lKdx * mTerrainParameters.GetSize() + lLdx]);
					}
				}

				mTerrainPatches[luiIndex]->mAxisAlignedBox = CAxisAlignedBox::CreateFromPoints(lPatchPositions);
				mTerrainPatches[luiIndex]->mCenter = mTerrainPatches[luiIndex]->mAxisAlignedBox.GetCenter();
				
				if (lJdx > 0)
					mTerrainPatches[luiIndex]->mLeft = mTerrainPatches[lIdx * muiNumPatchesPerRow + lJdx - 1].get();
				else
					mTerrainPatches[luiIndex]->mLeft = nullptr;

				if (lJdx < muiNumPatchesPerRow - 1)
					mTerrainPatches[luiIndex]->mRight = mTerrainPatches[lIdx * muiNumPatchesPerRow + lJdx + 1].get();
				else
					mTerrainPatches[luiIndex]->mRight = nullptr;

				if (lIdx > 0)
					mTerrainPatches[luiIndex]->mTop = mTerrainPatches[(lIdx - 1) * muiNumPatchesPerRow + lJdx].get();
				else
					mTerrainPatches[luiIndex]->mTop = nullptr;

				if (lIdx < muiNumPatchesPerRow - 1)
					mTerrainPatches[luiIndex]->mBottom = mTerrainPatches[(lIdx + 1) * muiNumPatchesPerRow + lJdx].get();
				else
					mTerrainPatches[luiIndex]->mBottom = nullptr;
			}
		}
	}

	void CTerrainEntity::CalculateDistanceThresholds()
	{
		mLODDistanceThresholds.resize(muiMaxLOD);

		float lfTerrainSize = mTerrainParameters.GetPatchSize() * 
			mTerrainParameters.GetPatchSize() * 
			mTerrainParameters.GetTileScale();

		for (uint32_t lIdx = 0; lIdx < muiMaxLOD; lIdx++)
		{
			mLODDistanceThresholds[muiMaxLOD - lIdx - 1] = (float)(
				lfTerrainSize * ((lIdx + 1 + lIdx / 2) * (lIdx + 1 + lIdx / 2)));
		}
	}

	void CTerrainEntity::UpdatePatchLevels()
	{
		for (uint32_t lIdx = 0; lIdx < muiNumPatchesPerRow; lIdx++)
		{
			for (uint32_t lJdx = 0; lJdx < muiNumPatchesPerRow; lJdx++)
			{
				uint32_t luiIndex = lIdx * muiNumPatchesPerRow + lJdx;		
				int32_t liCurrentLOD = mTerrainPatches[luiIndex]->GetCurrentLOD();

				if (liCurrentLOD > -1)
				{
					mTerrainPatches[luiIndex]->mPatchOuterLevels = CVector4F((float)liCurrentLOD);
					
					if (lJdx > 0 && mTerrainPatches[luiIndex]->mLeft->GetCurrentLOD() > 
						mTerrainPatches[luiIndex]->GetCurrentLOD())
					{
						mTerrainPatches[luiIndex]->mPatchOuterLevels.X = 
							(float)mTerrainPatches[luiIndex]->mLeft->GetCurrentLOD();
					}

					if (lJdx < muiNumPatchesPerRow - 1 && mTerrainPatches[luiIndex]->mRight->GetCurrentLOD() >
						mTerrainPatches[luiIndex]->GetCurrentLOD())
					{
						mTerrainPatches[luiIndex]->mPatchOuterLevels.Y =
							(float)mTerrainPatches[luiIndex]->mRight->GetCurrentLOD();
					}

					if (lIdx > 0 && mTerrainPatches[luiIndex]->mTop->GetCurrentLOD() >
						mTerrainPatches[luiIndex]->GetCurrentLOD())
					{
						mTerrainPatches[luiIndex]->mPatchOuterLevels.Z =
							(float)mTerrainPatches[luiIndex]->mTop->GetCurrentLOD();
					}

					if (lIdx < muiNumPatchesPerRow - 1 && mTerrainPatches[luiIndex]->mBottom->GetCurrentLOD() >
						mTerrainPatches[luiIndex]->GetCurrentLOD())
					{
						mTerrainPatches[luiIndex]->mPatchOuterLevels.W =
							(float)mTerrainPatches[luiIndex]->mBottom->GetCurrentLOD();
					}
				}
			}
		}
	}

	float CTerrainEntity::GetHeightAt(const CVector3F & pPosition)
	{
		return GetHeightAt(pPosition.X, pPosition.Z);
	}

	float CTerrainEntity::GetHeightAt(float pfX, float pfZ)
	{
		float lfHeight = -9999999.0f;
		float lfHalfSize = 0.5f * mTerrainParameters.GetSize() * 
			mTerrainParameters.GetTileScale();

		CVector2F lPosInGrid(pfX + lfHalfSize,
			pfZ + lfHalfSize);
		
		CVector2F lPatchPos(lPosInGrid / 
			mTerrainParameters.GetTileScale());

		if (lPatchPos.X >= 0 && lPatchPos.X < (float)mTerrainParameters.GetSize() &&
			lPatchPos.Y >= 0 && lPatchPos.Y < (float)mTerrainParameters.GetSize())
		{
			CVector2F lPatchOffset(lPatchPos.X - (int32_t)lPatchPos.X,
				lPatchPos.Y - (int32_t)lPatchPos.Y);

			int32_t liVertexIndex = (int32_t)lPatchPos.X + (int32_t)lPatchPos.Y * 
				mTerrainParameters.GetSize();

			float lfHeight1 = mTerrainHeightMap->GetHeightAt(liVertexIndex + 1);
			float lfHeight2 = mTerrainHeightMap->GetHeightAt(liVertexIndex);
			float lfHeight3 = mTerrainHeightMap->GetHeightAt(liVertexIndex + mTerrainParameters.GetSize() + 1);
			float lfHeight4 = mTerrainHeightMap->GetHeightAt(liVertexIndex + mTerrainParameters.GetSize());

			float lfHeightIncX, lfHeightIncY;

			if (lPatchOffset.X > lPatchOffset.Y)
			{
				lfHeightIncX = lfHeight1 - lfHeight2;
				lfHeightIncY = lfHeight3 - lfHeight1;
			}
			else
			{
				lfHeightIncX = lfHeight3 - lfHeight4;
				lfHeightIncY = lfHeight4 - lfHeight2;
			}

			lfHeight = lfHeight2 + lfHeightIncX * lPatchOffset.X +
				lfHeightIncY * lPatchOffset.Y;
		}

		return lfHeight;
	}

	bool CTerrainEntity::HasCameraMoved(CCamera * pCamera)
	{
		CVector3F lCamPos = pCamera->GetPosition();
		CVector3F lCamRot = pCamera->GetRotation();
		CVector3F lCamUp = pCamera->GetUp();

		if (CMathUtil::Abs(lCamRot.X - mOldCameraRot.X) < mfCameraRotationDelta &&
			CMathUtil::Abs(lCamRot.Y - mOldCameraRot.Y) < mfCameraRotationDelta)
		{
			if (CMathUtil::Abs(lCamPos.X - mOldCameraPos.X) < mfCameraMovementDelta &&
				CMathUtil::Abs(lCamPos.Y - mOldCameraPos.Y) < mfCameraMovementDelta &&
				CMathUtil::Abs(lCamPos.Z - mOldCameraPos.Z) < mfCameraMovementDelta)
			{
				if (CMathUtil::Abs(pCamera->GetFOV() - mfOldCameraFOV) < mfCameraFOVDelta &&
					lCamUp.Dot(mOldCameraUp) < mfCameraUpDelta)
				{
					return false;
				}
			}
		}

		return true;
	}

	EResult CTerrainEntity::LoadMaterial()
	{
		EResult lResult = EResult::Success;

		auto lMaterial = std::make_unique<CTerrainMaterial>();
		lResult = lMaterial->Create();

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CTerrainMaterial::Create() failed");
		}
		else
		{
			lMaterial->SetStartPassCallback(std::bind(&CTerrainEntity::StartPass,
				this, std::placeholders::_1, std::placeholders::_2));

			lMaterial->SetEndPassCallback(std::bind(&CTerrainEntity::EndPass,
				this, std::placeholders::_1));

			CTextureProperties lTextureProps;
			lTextureProps.SetFilter(ETextureFilter::Bilinear);
			lTextureProps.SetWrapS(ETextureWrap::Repeat);
			lTextureProps.SetWrapT(ETextureWrap::Repeat);

			CTexture2D *lHeightMap;
			CTexture2D *lColorMap;
			CTexture2D *lDetailMap;

			lResult = CTextureManager::Get().AddTexture2D(mTerrainParameters.GetHeightMap(),
				mTerrainParameters.GetHeightMap(), lTextureProps, &lHeightMap);

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("AddTexture2D() failed for %s", mTerrainParameters.GetHeightMap().c_str());
			}
			else
			{
				lMaterial->AddTexture(EPassType::Diffuse, lHeightMap, "uHeightMap");
				lMaterial->AddTexture(EPassType::Light, lHeightMap, "uHeightMap");

				if (!mTerrainParameters.GetColorMap().empty())
				{
					lResult = CTextureManager::Get().AddTexture2D(mTerrainParameters.GetColorMap(),
						mTerrainParameters.GetColorMap(), lTextureProps, &lColorMap);

					if (lResult != EResult::Success)
					{
						LOG_DEBUG("AddTexture2D() failed for %s", mTerrainParameters.GetColorMap().c_str());
					}
					else
					{
						lMaterial->AddTexture(EPassType::Diffuse, lColorMap, "uColorMap");
						lMaterial->AddTexture(EPassType::Light, lColorMap, "uColorMap");

						if (!mTerrainParameters.GetDetailMap().empty())
						{
							lResult = CTextureManager::Get().AddTexture2D(mTerrainParameters.GetDetailMap(),
								mTerrainParameters.GetDetailMap(), lTextureProps, &lDetailMap);

							if (lResult != EResult::Success)
							{
								LOG_DEBUG("AddTexture2D() failed for %s", mTerrainParameters.GetDetailMap().c_str());
							}
							else
							{
								lMaterial->AddTexture(EPassType::Diffuse, lDetailMap, "uDetailMap");
								lMaterial->AddTexture(EPassType::Light, lDetailMap, "uDetailMap");
							}
						}

						Array<String> lDiffuseMapNames =
						{
							"uDiffuseMap1",
							"uDiffuseMap2",
							"uDiffuseMap3",
							"uDiffuseMap4"
						};

						for (uint32_t lIdx = 0; lIdx < 4; lIdx++)
						{
							CTexture2D *lDiffuseMap = nullptr;
							String lstrDiffuseMap = mTerrainParameters.GetDiffuseMap(lIdx);
							
							if (!lstrDiffuseMap.empty())
							{
								lResult = CTextureManager::Get().AddTexture2D(lstrDiffuseMap,
									lstrDiffuseMap, lTextureProps, &lDiffuseMap);

								if (lResult != EResult::Success)
								{
									LOG_DEBUG("AddTexture2D() failed for %s", lstrDiffuseMap.c_str());
									break;
								}
								else
								{
									lMaterial->AddTexture(EPassType::Diffuse, lDiffuseMap, lDiffuseMapNames[lIdx]);
									lMaterial->AddTexture(EPassType::Light, lDiffuseMap, lDiffuseMapNames[lIdx]);
								}
							}
						}
					}
				}

				mMaterial = std::move(lMaterial);
			}
		}

		return lResult;
	}

	EResult CTerrainEntity::LoadRenderData()
	{
		EResult lResult = EResult::Success;
		IDrawOperation *lDrawOperation;

		SVertexP lVertices[] =
		{
			SVertexP(-0.5f, 0.0f, -0.5f),
			SVertexP(0.5f, 0.0f, -0.5f),			
			SVertexP(-0.5f, 0.0f, 0.5f),
			SVertexP(0.5f, 0.0f, 0.5f)
		};

		uint16_t lIndices[] =
		{
			0, 1, 2, 3
		};

		lResult = CVertexCacheManager::Get().AddVertices("VertexP",
			EIndexType::SixteenBit, 4, 4, lVertices, lIndices, &lDrawOperation);

		if (lResult == EResult::Success)
		{
			mDrawOperation = IDrawOperationPtr(lDrawOperation);
			mDrawOperation->SetNumPatchVertices(4);
			mDrawOperation->SetPrimitiveType(EPrimitiveType::Patches);
		}

		return lResult;
	}
}