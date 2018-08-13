#pragma once

#include "Scene/SubMeshEntity.h"
#include "Scene/MeshMaterial.h"
#include "Scene/AnimationPlayer.h"

namespace FDX
{
	class FDX_EXPORT CMeshEntity : public CRenderable
	{
	public:

		CMeshEntity();
		~CMeshEntity();

		CMeshEntity(const CMeshEntity&) = delete;
		CMeshEntity& operator=(const CMeshEntity&) = delete;

		uint32_t GetNumMaterials() const
		{
			return mMaterials.size();
		}

		uint32_t GetNumSubMeshes() const
		{
			return mSubMeshes.size();
		}

		IMaterial* GetMaterial(uint32_t puiIndex) const
		{
			if (puiIndex < mMaterials.size())
				return mMaterials[puiIndex].get();

			return nullptr;
		}

		CSubMeshEntity* GetSubMesh(uint32_t puiIndex) const
		{
			if (puiIndex < mSubMeshes.size())
				return mSubMeshes[puiIndex].get();

			return nullptr;
		}	

		CAnimationPlayer* GetAnimationPlayer() const
		{
			return mAnimationPlayer.get();
		}

		bool GetAnimate() const
		{
			return mbAnimate;
		}

		EResult Load(IModel *pModel);		
		EResult LoadAnimationPlayer(IModelSkinningData *pSkinningData);

		void SetAnimate(bool pbAnimate);
		void UpdateTransform() override;
		void Update() override;

		void StartPass(EPassType pPassType, IProgram *pProgram); 
		void EndPass(EPassType pPassType);

	private:

		Array<IMaterialPtr> mMaterials;
		Array<IDrawOperationPtr> mDrawOperations;
		Array<CSubMeshEntityPtr> mSubMeshes;
		CAnimationPlayerPtr mAnimationPlayer;
		IBlendStatePtr mBlendState;
		bool mbAnimate;
	};

	using CMeshEntityPtr = SmartPtr<CMeshEntity>;
}