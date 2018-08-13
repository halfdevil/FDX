
#include "Scene/TerrainHeightMap.h"
#include "Core/ResourceManager.h"
#include "OS/GameLogger.h"
#include "Core/BinaryResource.h"
#include "Core/ImageResource.h"
#include "Core/Bitmap.h"

namespace FDX
{
	CTerrainHeightMap::CTerrainHeightMap(ETerrainHeightMapType pHeightMapType)
		: mHeightMapType(pHeightMapType),
		mfHeightScale(0.0f),
		mfHeightOffset(0.0f),
		muiNumRows(0),
		muiNumCols(0)
	{
	}

	CTerrainHeightMap::~CTerrainHeightMap()
	{
	}

	bool CTerrainHeightMap::InBounds(int32_t puiX, int32_t puiZ)
	{
		return puiX >= 0 && puiX < (int32_t)muiNumCols && 
			puiZ >= 0 && puiZ < (int32_t)muiNumRows;
	}

	float CTerrainHeightMap::SampleHeight3x3(int32_t puiX, int32_t puiZ)
	{
		float lfAvg = 0.0f;
		float lfNum = 0.0f;

		for (int32_t lIdx = puiZ - 1; lIdx < puiZ + 1; lIdx++)
		{
			for (int32_t lJdx = puiX - 1; lJdx < puiX + 1; lJdx++)
			{
				if (InBounds(lJdx, lIdx))
				{
					lfAvg += mHeightMapData[lIdx * muiNumCols + lJdx];
					lfNum += 1.0f;
				}
			}
		}

		return lfAvg / lfNum;
	}

	void CTerrainHeightMap::Filter3x3()
	{
		Array<float> lTempData(muiNumRows * muiNumCols);

		for (uint32_t lIdx = 0; lIdx < muiNumRows; lIdx++)
		{
			for (uint32_t lJdx = 0; lJdx < muiNumCols; lJdx++)
			{
				lTempData[lIdx * muiNumCols + lJdx] = SampleHeight3x3(lJdx, lIdx);
			}
		}

		mHeightMapData = std::move(lTempData);
	}

	CRawTerrainHeightMap::CRawTerrainHeightMap()
		: CTerrainHeightMap(ETerrainHeightMapType::Raw)
	{
	}

	CRawTerrainHeightMap::~CRawTerrainHeightMap()
	{
	}

	EResult CRawTerrainHeightMap::Load(const String & pstrFileName, float pfHeightScale, 
		float pfHeightOffset, uint32_t puiNumRows, uint32_t puiNumCols)
	{
		EResult lResult = EResult::Success;
		CBinaryResource *lResource;

		lResult = CResourceManager::Get().GetResource<CBinaryResource>(pstrFileName, &lResource);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("GetResource<CBinaryResource>() failed for HeightMap: %s", pstrFileName.c_str());
		}
		else
		{
			uint32_t luiIndex = 0;
			uint8_t *lData = lResource->GetData();

			mHeightMapData.resize(puiNumRows * puiNumCols);

			for (uint32_t lIdx = 0; lIdx < puiNumRows; lIdx++)
			{
				for (uint32_t lJdx = 0; lJdx < puiNumCols; lJdx++)
				{
					mHeightMapData[lIdx * puiNumCols + lJdx] = 
						(float)lData[lIdx * puiNumCols + lJdx] * pfHeightScale + pfHeightOffset;
				}
			}

			Filter3x3();

			muiNumRows = puiNumRows;
			muiNumCols = puiNumCols;
			mfHeightScale = pfHeightScale;
			mfHeightOffset = pfHeightOffset;
		}

		return lResult;
	}

	CImageTerrainHeightMap::CImageTerrainHeightMap()
		: CTerrainHeightMap(ETerrainHeightMapType::Image)
	{
	}

	CImageTerrainHeightMap::~CImageTerrainHeightMap()
	{
	}

	EResult CImageTerrainHeightMap::Load(const String &pstrFileName, float pfHeightScale,
		float pfHeightOffset, uint32_t puiNumRows, uint32_t puiNumCols)
	{
		EResult lResult = EResult::Success;
		CImageResource *lImageResource;

		lResult = CResourceManager::Get().GetResource<CImageResource>(pstrFileName, &lImageResource);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("GetResource<CImageResource>() failed for HeightMap: %s", pstrFileName.c_str());
		}
		else
		{
			CBitmap lBitmap;
			lResult = lBitmap.Create(lImageResource->GetImage());

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("CBitmap::Create() failed for HeightMap: %s", pstrFileName.c_str());
			}
			else
			{
				muiNumRows = lBitmap.GetHeight();
				muiNumCols = lBitmap.GetWidth();

				mHeightMapData.resize(muiNumRows * muiNumCols);

				for (uint32_t lIdx = 0; lIdx < muiNumRows; lIdx++)
				{
					for (uint32_t lJdx = 0; lJdx < muiNumCols; lJdx++)
					{
						mHeightMapData[lIdx * muiNumCols + lJdx] =
							(float)lBitmap.GetPixel(lJdx, lIdx).R * pfHeightScale + pfHeightOffset;
					}
				}

				Filter3x3();

				mfHeightScale = pfHeightScale;
				mfHeightOffset = pfHeightOffset;
			}
		}

		return lResult;
	}
}