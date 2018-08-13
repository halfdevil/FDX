#pragma once

#include "Scene/SceneNode.h"
#include "Scene/Renderable.h"
#include "Scene/TerrainHeightMap.h"
#include "Scene/TerrainMaterial.h"

namespace FDX
{
	class CTerrainEntity;

	class FDX_EXPORT CTerrainPatch : public CRenderable
	{
	public:

		friend class CTerrainEntity;

		CTerrainPatch(uint32_t puiPatchIndex);
		~CTerrainPatch();

		int32_t GetCurrentLOD() const
		{
			return miCurrentLOD;
		}

		const CVector3F& GetCenter() const
		{
			return mCenter;
		}

		CTerrainPatch* GetTop() const
		{
			return mTop;
		}

		CTerrainPatch* GetBottom() const
		{
			return mBottom;
		}

		CTerrainPatch* GetLeft() const
		{
			return mLeft;
		}

		CTerrainPatch* GetRight() const
		{
			return mRight;
		}

		void SetCurrentLOD(int32_t piLOD);
		void SetMaterial(IMaterial *pMaterial);
		void SetDrawOperation(IDrawOperation *pDrawOperation);

		void UpdateGraphics(CCamera *pCamera, IProgram *pProgram);

	private:

		int32_t miCurrentLOD;
		uint32_t muiPatchIndex;
		CVector3F mCenter;
		CVector4F mPatchOuterLevels;

		CTerrainPatch *mTop;
		CTerrainPatch *mBottom;
		CTerrainPatch *mLeft;
		CTerrainPatch *mRight;
	};

	using CTerrainPatchPtr = SmartPtr<CTerrainPatch>;

	class CTerrainNode;
	using CTerrainNodePtr = SmartPtr<CTerrainNode>;

	class FDX_EXPORT CTerrainNode : public CSceneNode
	{
	public:

		friend class CTerrainEntity;

		CTerrainNode(CTerrainEntity *pTerrain, const Array<CVector3F> &pPositions, 
			uint32_t puiStartX, uint32_t puiStartZ, uint32_t puiSize, uint32_t puiTerrainSize, 
			uint32_t puiPatchSize, uint32_t puiNumPatchesPerRow);

		~CTerrainNode();

		CTerrainNode* GetChildUL() const
		{
			return GetChildAs<CTerrainNode>(0);
		}

		CTerrainNode* GetChildUR() const
		{
			return GetChildAs<CTerrainNode>(1);
		}

		CTerrainNode* GetChildLL() const
		{
			return GetChildAs<CTerrainNode>(2);
		}

		CTerrainNode* GetChildLR() const
		{
			return GetChildAs<CTerrainNode>(3);
		}

		CTerrainPatch* GetTerrainPatch() const
		{
			return mTerrainPatch;
		}

		bool IsLeafNode() const
		{
			return mbIsLeafNode;
		}

		void UpdatePatches(CCamera *pCamera);

	private:

		void DisableNode();
		void CreateChildNodes(const Array<CVector3F> &pPositions, 
			uint32_t puiTerrainSize, uint32_t puiPatchSize, 
			uint32_t puiNumPatches);

	private:

		uint32_t muiStartX;
		uint32_t muiStartZ;
		uint32_t muiSize;
		bool mbIsLeafNode;
		CTerrainPatch *mTerrainPatch;	
		CTerrainEntity *mTerrain;
	};

	class FDX_EXPORT CTerrainParameters
	{
	public:

		CTerrainParameters();
		~CTerrainParameters();

		uint32_t GetSize() const
		{
			return muiSize;
		}

		uint32_t GetPatchSize() const
		{
			return muiPatchSize;
		}

		const String& GetHeightMap() const
		{
			return mstrHeightMap;
		}

		const String& GetColorMap() const
		{
			return mstrColorMap;
		}

		const String& GetDetailMap() const
		{
			return mstrDetailMap;
		}

		const String& GetDiffuseMap(uint32_t puiIndex) const
		{
			return mDiffuseMaps[puiIndex];
		}

		float GetHeightScale() const
		{
			return mfHeightScale;
		}

		float GetHeightOffset() const
		{
			return mfHeightOffset;
		}

		float GetTileScale() const
		{
			return mfTileScale;
		}

		ETerrainHeightMapType GetHeightMapType() const
		{
			return mHeightMapType;
		}

		void SetSize(uint32_t puiSize);
		void SetPatchSize(uint32_t puiPatchSize);
		void SetHeightMap(const String &pstrHeightMap);
		void SetColorMap(const String &pstrColorMap);
		void SetHeightScale(float pfHeightScale);
		void SetHeightOffset(float pfHeightOffset);
		void SetTileScale(float pfTileScale);
		void SetHeightMapType(ETerrainHeightMapType pHeightMapType);
		void SetDetailMap(const String &pstrDetailMap);
		void SetDiffuseMap(uint32_t puiIndex, const String &pstrDiffuseMap);

	private:

		ETerrainHeightMapType mHeightMapType;
		uint32_t muiSize;
		uint32_t muiPatchSize;
		String mstrHeightMap;
		String mstrColorMap;		
		String mstrDetailMap;
		Array<String> mDiffuseMaps;
		float mfHeightScale;
		float mfHeightOffset;
		float mfTileScale;		
	};

	class FDX_EXPORT CTerrainEntity : public CSceneEntity
	{
	public:

		CTerrainEntity();
		~CTerrainEntity();

		const CTerrainParameters& GetTerrainParameters() const
		{
			return mTerrainParameters;
		}

		CTerrainPatch* GetTerrainPatch(uint32_t puiIdx) const
		{
			if (puiIdx < mTerrainPatches.size())
				return mTerrainPatches[puiIdx].get();

			return nullptr;
		}

		CTerrainNode* GetTerrainQuadTree() const
		{
			return mTerrainQuadTree.get();
		}

		uint32_t GetMaxLOD() const
		{
			return muiMaxLOD;
		}

		const Array<float>& GetLODDistanceThresholds() const
		{
			return mLODDistanceThresholds;
		}

		EResult Load(const CTerrainParameters &pParameters);

		void StartPass(EPassType pPassType, IProgram *pProgram);
		void EndPass(EPassType pPassType);
		void UpdatePatchLevels();

		float GetHeightAt(const CVector3F &pPosition);
		float GetHeightAt(float pfX, float pfZ);

		bool HasCameraMoved(CCamera *pCamera);

	private:

		void CreatePatches();
		void CalculatePatchData(const Array<CVector3F> &pPositions);
		void CalculateDistanceThresholds();		

		EResult LoadMaterial();
		EResult LoadRenderData();
		
	private:

		uint32_t muiSize;
		uint32_t muiPatchSize;
		uint32_t muiNumPatchesPerRow;
		uint32_t muiMaxLOD;

		CTerrainParameters mTerrainParameters;
		CTerrainHeightMapPtr mTerrainHeightMap;
		CTerrainNodePtr mTerrainQuadTree;
		Array<CTerrainPatchPtr> mTerrainPatches;
		Array<float> mLODDistanceThresholds;
		IMaterialPtr mMaterial;
		IDrawOperationPtr mDrawOperation;

		CVector3F mOldCameraPos;
		CVector3F mOldCameraRot;
		CVector3F mOldCameraUp;
		float mfOldCameraFOV;
		float mfCameraMovementDelta;
		float mfCameraRotationDelta;
		float mfCameraFOVDelta;
		float mfCameraUpDelta;
	};

	using CTerrainEntityPtr = SmartPtr<CTerrainEntity>;
}