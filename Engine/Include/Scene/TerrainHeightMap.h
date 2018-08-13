#pragma once

#include "Core/Core.h"
#include "Core/Result.h"

namespace FDX
{
	enum class ETerrainHeightMapType
	{
		Raw,
		Image
	};

	class FDX_EXPORT CTerrainHeightMap
	{
	public:

		CTerrainHeightMap(ETerrainHeightMapType pHeightMapType);
		~CTerrainHeightMap();

		const Array<float>& GetHeightMapData() const
		{
			return mHeightMapData;
		}

		float GetHeightScale() const
		{
			return mfHeightScale;
		}

		float GetHeightOffset() const
		{
			return mfHeightOffset;
		}

		uint32_t GetNumRows() const
		{
			return muiNumRows;
		}

		uint32_t GetNumCols() const
		{
			return muiNumCols;
		}

		float GetHeightAt(uint32_t puiIndex) const
		{
			return mHeightMapData[puiIndex];
		}

		virtual EResult Load(const String &pstrFileName, float pfHeightScale, 
			float pfHeightOffset, uint32_t puiNumRows, uint32_t puiNumCols) = 0;

	protected:

		bool InBounds(int32_t puiX, int32_t puiZ);
		float SampleHeight3x3(int32_t puiX, int32_t puiZ); 

		void Filter3x3();

	protected:

		ETerrainHeightMapType mHeightMapType;
		Array<float> mHeightMapData;
		float mfHeightScale;
		float mfHeightOffset;
		uint32_t muiNumRows;
		uint32_t muiNumCols;
	};

	using CTerrainHeightMapPtr = SmartPtr<CTerrainHeightMap>;

	class FDX_EXPORT CRawTerrainHeightMap : public CTerrainHeightMap
	{
	public:

		CRawTerrainHeightMap();
		~CRawTerrainHeightMap();

		EResult Load(const String &pstrFileName, float pfHeightScale,
			float pfHeightOffset, uint32_t puiNumRows, uint32_t puiNumCols);
	};

	class FDX_EXPORT CImageTerrainHeightMap : public CTerrainHeightMap
	{
	public:

		CImageTerrainHeightMap();
		~CImageTerrainHeightMap();

		EResult Load(const String &pstrFileName, float pfHeightScale,
			float pfHeightOffset, uint32_t puiNumRows, uint32_t puiNumCols);
	};
}