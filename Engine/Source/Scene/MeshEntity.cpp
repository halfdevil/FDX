
#include "Scene/MeshEntity.h"
#include "Scene/MeshMaterial.h"
#include "Graphics/TextureManager.h"
#include "Graphics/VertexCacheManager.h"
#include "OS/GameLogger.h"
#include "OS/GameTimer.h"

namespace FDX
{
	CMeshEntity::CMeshEntity()
		: mAnimationPlayer(nullptr),
		mbAnimate(false),
		mBlendState(nullptr)
	{
		mRenderableType = ERenderableType::Mesh;
	}

	CMeshEntity::~CMeshEntity()
	{
	}

	EResult CMeshEntity::Load(IModel * pModel)
	{
		EResult lResult = EResult::Success;

		IBlendState *lBlendState;
		SBlendStateProperties lBlendStateProperties;

		lBlendStateProperties.BlendEnabled = true;
		lBlendStateProperties.SourceColorBlend = EBlendOption::SrcAlpha;
		lBlendStateProperties.DestinationColorBlend = EBlendOption::OneMinusSrcAlpha;		

		lResult = CTextureManager::Get().GetGraphicsDevice()->
			CreateBlendState(lBlendStateProperties, &lBlendState);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CreateBlendState() failed");
		}
		else
		{
			mBlendState = IBlendStatePtr(lBlendState);
		}

		for (const auto &lModelMat : pModel->GetMaterials())
		{
			auto lMeshMaterial = std::make_unique<CMeshMaterial>();
			lMeshMaterial->Create();

			lMeshMaterial->SetStartPassCallback(std::bind(&CMeshEntity::StartPass, 
				this, std::placeholders::_1, std::placeholders::_2));

			lMeshMaterial->SetEndPassCallback(std::bind(&CMeshEntity::EndPass, 
				this, std::placeholders::_1));

			lMeshMaterial->SetAmbient(lModelMat->GetAmbient());
			lMeshMaterial->SetDiffuse(lModelMat->GetDiffuse());
			lMeshMaterial->SetEmissive(lModelMat->GetEmissive());
			lMeshMaterial->SetSpecular(lModelMat->GetSpecular());
			lMeshMaterial->SetSpecPower(lModelMat->GetSpecularPower());
			
			CTextureProperties lTextureProps;
			lTextureProps.SetFilter(ETextureFilter::Bilinear);	
			lTextureProps.SetWrapS(ETextureWrap::Repeat);
			lTextureProps.SetWrapT(ETextureWrap::Repeat);

			if (!lModelMat->GetDiffuseMap().empty())
			{
				CTexture2D *lDiffuseMap = nullptr;

				if (CTextureManager::Get().AddTexture2D(lModelMat->GetDiffuseMap(),
					lModelMat->GetDiffuseMap(), lTextureProps, &lDiffuseMap) == EResult::Success)
				{
					lMeshMaterial->AddTexture(EPassType::Diffuse, lDiffuseMap, "uDiffuseMap");
					lMeshMaterial->AddTexture(EPassType::Light, lDiffuseMap, "uDiffuseMap");
				}
			}

			mMaterials.push_back(std::move(lMeshMaterial));
		}

		mAxisAlignedBox = CAxisAlignedBox(999999.0f, 999999.0f, 999999.0f,
			-999999.0f, -999999.0f, -999999.0f);

		for (const auto &lMesh : pModel->GetMeshes())
		{
			for (const auto &lVertex : lMesh->GetVertices())
			{
				if (lVertex.Position.X < mAxisAlignedBox.Min.X) mAxisAlignedBox.Min.X = lVertex.Position.X;
				if (lVertex.Position.Y < mAxisAlignedBox.Min.Y) mAxisAlignedBox.Min.Y = lVertex.Position.Y;
				if (lVertex.Position.Z < mAxisAlignedBox.Min.Z) mAxisAlignedBox.Min.Z = lVertex.Position.Z;

				if (lVertex.Position.X > mAxisAlignedBox.Max.X) mAxisAlignedBox.Max.X = lVertex.Position.X;
				if (lVertex.Position.Y > mAxisAlignedBox.Max.Y) mAxisAlignedBox.Max.Y = lVertex.Position.Y;
				if (lVertex.Position.Z > mAxisAlignedBox.Max.Z) mAxisAlignedBox.Max.Z = lVertex.Position.Z;
			}
		}

		lResult = CVertexCacheManager::Get().AddModel("ModelVertex", pModel, mDrawOperations);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CVertexCacheManager AddModel() failed");
		}
		else
		{
			const auto &lModelMaterials = pModel->GetMaterials();
			for (uint32_t lIdx = 0; lIdx < lModelMaterials.size(); lIdx++)
			{
				auto lSubMesh = std::make_unique<CSubMeshEntity>();
				mDrawOperations[lIdx]->SetPrimitiveType(EPrimitiveType::TriangleList);

				lSubMesh->SetMaterial(mMaterials[lIdx].get());
				lSubMesh->SetDrawOperation(mDrawOperations[lIdx].get());

				mSubMeshes.push_back(std::move(lSubMesh));
			}
		}

		if (pModel->GetSkinningData())
		{
			lResult = LoadAnimationPlayer(pModel->GetSkinningData().get());
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("LoadAnimationPlayer() failed");
			}
		}

		return lResult;
	}

	EResult CMeshEntity::LoadAnimationPlayer(IModelSkinningData *pSkinningData)
	{
		EResult lResult = EResult::Success;

		if (!pSkinningData)
			return lResult;

		Array<CMatrix4F> lWorldTransforms;
		Array<CAnimationClipPtr> lAnimationClips;
		Array<CJointPtr> lSkeleton;

		const auto &lModelSkeleton = pSkinningData->GetSkeleton();
		lWorldTransforms.resize(lModelSkeleton.size());

		for (const auto &lModelJoint : lModelSkeleton)
		{
			auto lJoint = std::make_unique<CJoint>();
			lJoint->SetIndex(lModelJoint->GetIndex());
			lJoint->SetParent(lModelJoint->GetParent());
			lJoint->SetAnimated(lModelJoint->IsAnimated());
			lJoint->SetLocalTransform(lModelJoint->GetBindPose());
			lJoint->SetInvLocalTransform(lModelJoint->GetInvBindPose());

			lSkeleton.push_back(std::move(lJoint));
		}

		for (const auto &lModelAnimationClip : pSkinningData->GetAnimations())
		{
			auto lAnimationClip = std::make_unique<CAnimationClip>();
			lAnimationClip->SetDuration(lModelAnimationClip->GetDuration());
			
			Array<CKeyFramePtr> lKeyFrames;
			Array<CJointTransformPtr> lJointTransforms;

			CKeyFramePtr lKeyFrame(nullptr);
			float lfCurrTime = -1.0f;

			for (const auto &lModelKeyFrame : lModelAnimationClip->GetKeyFrames())
			{
				if (lfCurrTime != lModelKeyFrame->GetTime())
				{
					if (lKeyFrame != nullptr)
					{
						lKeyFrame->SetJointTransforms(std::move(lJointTransforms));
						lKeyFrames.push_back(std::move(lKeyFrame));
					}

					lKeyFrame = std::make_unique<CKeyFrame>();			
					lKeyFrame->SetTime(lModelKeyFrame->GetTime());
					lJointTransforms.resize(lSkeleton.size());

					lfCurrTime = lModelKeyFrame->GetTime();
				}

				CJointTransformPtr lJointTransform = std::make_unique<CJointTransform>();
				lJointTransform->SetPosition(lModelKeyFrame->GetPosition());
				lJointTransform->SetScale(lModelKeyFrame->GetScale());
				lJointTransform->SetOrientation(lModelKeyFrame->GetOrientation());

				lJointTransforms[lModelKeyFrame->GetBoneIndex()] = std::move(lJointTransform);
			}

			if (lKeyFrame != nullptr)
			{
				lKeyFrame->SetJointTransforms(std::move(lJointTransforms));
				lKeyFrames.push_back(std::move(lKeyFrame));
			}

			lAnimationClip->SetKeyFrames(std::move(lKeyFrames));
			lAnimationClips.push_back(std::move(lAnimationClip));
		}

		auto lSkinningData = std::make_unique<CSkinningData>();
		lSkinningData->SetAnimationClips(std::move(lAnimationClips));
		lSkinningData->SetSkeleton(std::move(lSkeleton));
		lSkinningData->SetRootTransform(pSkinningData->GetRootTransform());
		lSkinningData->SetInvRootTransform(CMatrix4F::InvertMatrix(pSkinningData->GetRootTransform()));
		mAnimationPlayer = std::make_unique<CAnimationPlayer>(std::move(lSkinningData));

		return lResult;
	}

	void CMeshEntity::SetAnimate(bool pbAnimate)
	{
		if (mAnimationPlayer != nullptr)
			mbAnimate = pbAnimate;
	}

	void CMeshEntity::UpdateTransform()
	{
		CRenderable::UpdateTransform();

		for (const auto &lSubMesh : mSubMeshes)
		{
			lSubMesh->SetPosition(mPosition);
			lSubMesh->SetScale(mScale);
			lSubMesh->SetRotation(mRotation);
		}
	}

	void CMeshEntity::Update()
	{
		if (mbAnimate)
		{
			mAnimationPlayer->Update(static_cast<float>(
				IGameTimer::Get().GetDeltaTime()));
		}
	}

	void CMeshEntity::StartPass(EPassType pPassType, IProgram * pProgram)
	{
		if (mbAnimate)
		{
			pProgram->GetConstants()->SetInt("uAnimate", 1);
			pProgram->GetConstants()->SetMatrix4Array("uBones[0]", 
				mAnimationPlayer->GetAnimatedTransforms());
		}
		else
		{
			pProgram->GetConstants()->SetInt("uAnimate", 0);
		}
	}

	void CMeshEntity::EndPass(EPassType pPassType)
	{
	}
}