#pragma once

#include "Core/ModelLoader.h"
#include "tinyxml2.h"

namespace FDX
{
	class CDaeMaterial : public IModelMaterial
	{
	public:

		friend class CDaeModel;

		CDaeMaterial();
		~CDaeMaterial() override;

		const String& GetName() const override
		{
			return mstrName;
		}

		const CColor4F& GetAmbient() const override
		{
			return mAmbient;
		}

		const CColor4F& GetDiffuse() const override
		{
			return mDiffuse;
		}

		const CColor4F& GetEmissive() const override
		{
			return mEmissive;
		}

		const CColor4F& GetSpecular() const override
		{
			return mSpecular;
		}

		float GetSpecularPower() const override
		{
			return mfSpecularPower;
		}

		const String& GetDiffuseMap() const override
		{
			return mstrDiffuseMap;
		}

		const String& GetSpecularMap() const override
		{
			return mstrSpecularMap;
		}

		const String& GetNormalMap() const override
		{
			return mstrNormalMap;
		}

	private:

		String mstrName;
		CColor4F mAmbient;
		CColor4F mDiffuse;
		CColor4F mEmissive;
		CColor4F mSpecular;
		float mfSpecularPower;
		String mstrDiffuseMap;
		String mstrNormalMap;
		String mstrSpecularMap;
	};

	class CDaeModelMeshPart : public IModelMeshPart
	{
	public:

		friend class CDaeModel;
		friend class CDaeModelMesh;

		CDaeModelMeshPart();
		~CDaeModelMeshPart() override;

		uint32_t GetMaterialIndex() const override
		{
			return muiMaterialIndex;
		}

		uint32_t GetStartIndex() const override
		{
			return muiStartIndex;
		}

		uint32_t GetNumIndices() const override
		{
			return muiNumIndices;
		}

	private:

		uint32_t muiMaterialIndex;
		uint32_t muiStartIndex;
		uint32_t muiNumIndices;
	};

	class CDaeModelJoint : public IModelJoint
	{
	public:

		friend class CDaeModel;

		CDaeModelJoint();
		~CDaeModelJoint() override;

		int32_t GetIndex() const override
		{
			return miIndex;
		}

		int32_t GetParent() const override
		{
			return miParent;
		}

		const CMatrix4F& GetBindPose() const override
		{
			return mBindPose;
		}

	private:

		int32_t miIndex;
		int32_t miParent;
		CMatrix4F mBindPose;
	};

	class CDaeModelKeyFrame : public IModelKeyFrame
	{
	public:

		friend class CDaeModel;
		friend class CDaeModelMesh;

		CDaeModelKeyFrame();
		~CDaeModelKeyFrame() override;

		float GetTime() const override
		{
			return mfTime;
		}

		uint32_t GetBoneIndex() const override
		{
			return muiBoneIndex;
		}

		virtual const CVector3F& GetPosition() const override
		{
			return mPosition;
		}

		virtual const CQuaternion& GetOrientation() const override
		{
			return mOrientation;
		}

	private:

		float mfTime;
		uint32_t muiBoneIndex;
		CVector3F mPosition;
		CQuaternion mOrientation;
	};

	class CDaeModelAnimation : public IModelAnimation
	{
	public:

		friend class CDaeModel;
		friend class CDaeModelMesh;

		CDaeModelAnimation();
		~CDaeModelAnimation() override;

		const String& GetName() const override
		{
			return mstrName;
		}

		float GetDuration() const override
		{
			return mfDuration;
		}

		const Array<IModelKeyFramePtr>& GetKeyFrames() const override
		{
			return mKeyFrames;
		}

	private:

		String mstrName;
		float mfDuration;
		Array<IModelKeyFramePtr> mKeyFrames;
	};

	class CDaeModelSkinningData : public IModelSkinningData
	{
	public:

		friend class CDaeModel;
		friend class CDaeModelMesh;

		CDaeModelSkinningData();
		~CDaeModelSkinningData() override;

		const Array<IModelJointPtr>& GetSkeleton() const override
		{
			return mSkeleton;
		}

		const Array<IModelAnimationPtr>& GetAnimations() const override
		{
			return mAnimations;
		}

	private:

		Array<IModelJointPtr> mSkeleton;
		Array<IModelAnimationPtr> mAnimations;
	};

	using CDaeModelSkinningDataPtr = SmartPtr<CDaeModelSkinningData>;

	class CDaeModelMesh : public IModelMesh
	{
	public:

		friend class CDaeModel;

		CDaeModelMesh();
		~CDaeModelMesh() override;

		const String& GetName() const override
		{
			return mstrName;
		}

		const Array<SModelVertex>& GetVertices() const override
		{
			return mVertices;
		}

		const Array<uint16_t>& GetIndices() const override
		{
			return mIndices;
		}

		const Array<IModelMeshPartPtr>& GetMeshParts() const override
		{
			return mMeshParts;
		}

	private:

		String mstrName;
		Array<SModelVertex> mVertices;
		Array<uint16_t> mIndices;
		Array<IModelMeshPartPtr> mMeshParts;
	};

	class CDaeModel : public IModel
	{
	public:

		CDaeModel();
		~CDaeModel() override;

		EModelType GetModelType() const override
		{
			return EModelType::Dae;
		}

		const Array<IModelMaterialPtr>& GetMaterials() const override
		{
			return mMaterials;
		}

		const Array<IModelMeshPtr>& GetMeshes() const override
		{
			return mMeshes;
		}

		const IModelSkinningDataPtr& GetSkinningData() const override
		{
			return mSkinningData;
		}

		EResult Create(const String &pstrFilePath);

	private:

		void LoadAnimations(tinyxml2::XMLNode *pNode);
		void LoadSkeleton(tinyxml2::XMLNode *pNode);
		void LoadJoints(tinyxml2::XMLElement *pNode, int32_t piParentIndex);

		void LoadMaterials(tinyxml2::XMLNode *pNode, const String &pstrPath);
		void LoadMeshes(tinyxml2::XMLNode *pNode);
		void Reset();

		Array<IModelMaterialPtr> mMaterials;
		Array<IModelMeshPtr> mMeshes;
		IModelSkinningDataPtr mSkinningData;

		UnorderedMap<std::string, uint32_t> mMaterialIndices;
		UnorderedMap<std::string, uint32_t> mJointIndices;
		Array<IModelJointPtr> mJoints;
	};

	class FDX_EXPORT CDaeModelLoader : public IModelLoader
	{
	public:

		CDaeModelLoader();
		~CDaeModelLoader() override;

		EModelLoaderType GetModelLoaderType() const override;

		bool CanLoadModelType(EModelType pType) override;
		bool CanLoadModel(const String &pstrFilePath) override;

		EResult Load(const String &pstrFilePath,
			IModel **pModel) override;
	};
}