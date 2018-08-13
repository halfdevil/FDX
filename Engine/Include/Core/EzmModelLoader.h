#pragma once

#include "Core/ModelLoader.h"
#include "tinyxml2.h"

namespace FDX
{
	class CEzmMaterial : public IModelMaterial
	{
	public:

		friend class CEzmModel;

		CEzmMaterial();
		~CEzmMaterial();

		const String& GetName() const
		{
			return mstrName;
		}

		const CColor4F& GetAmbient() const
		{
			return mAmbient;
		}

		const CColor4F& GetDiffuse() const
		{
			return mDiffuse;
		}

		const CColor4F& GetEmissive() const
		{
			return mEmissive;
		}

		const CColor4F& GetSpecular() const
		{
			return mSpecular;
		}

		float GetSpecularPower() const
		{
			return mfSpecularPower;
		}

		const String& GetDiffuseMap() const
		{
			return mstrDiffuseMap;
		}

		const String& GetSpecularMap() const
		{
			return mstrSpecularMap;
		}

		const String& GetNormalMap() const
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

	class CEzmModelMeshPart : public IModelMeshPart
	{
	public:

		friend class CEzmModel;
		friend class CEzmModelMesh;

		CEzmModelMeshPart();
		~CEzmModelMeshPart();

		uint32_t GetMaterialIndex() const
		{
			return muiMaterialIndex;
		}

		uint32_t GetStartIndex() const
		{
			return muiStartIndex;
		}

		uint32_t GetNumIndices() const
		{
			return muiNumIndices;
		}

	private:

		uint32_t muiMaterialIndex;
		uint32_t muiStartIndex;
		uint32_t muiNumIndices;
	};

	class CEzmModelJoint : public IModelJoint
	{
	public:

		friend class CEzmModel;

		CEzmModelJoint();
		~CEzmModelJoint();

		int32_t GetIndex() const
		{
			return miIndex;
		}

		int32_t GetParent() const
		{
			return miParent;
		}

		const CMatrix4F& GetBindPose() const
		{
			return mBindPose;
		}

	private:

		int32_t miIndex;
		int32_t miParent;
		CMatrix4F mBindPose;
	};

	class CEzmModelKeyFrame : public IModelKeyFrame
	{
	public:

		friend class CEzmModel;
		friend class CEzmModelMesh;

		CEzmModelKeyFrame();
		~CEzmModelKeyFrame();

		float GetTime() const
		{
			return mfTime;
		}

		uint32_t GetBoneIndex() const
		{
			return muiBoneIndex;
		}

		virtual const CVector3F& GetPosition() const
		{
			return mPosition;
		}

		virtual const CQuaternion& GetOrientation() const
		{
			return mOrientation;
		}

	private:

		float mfTime;		
		uint32_t muiBoneIndex;
		CVector3F mPosition;
		CQuaternion mOrientation;
	};

	class CEzmModelAnimation : public IModelAnimation
	{
	public:

		friend class CEzmModel;
		friend class CEzmModelMesh;

		CEzmModelAnimation();
		~CEzmModelAnimation();

		const String& GetName() const
		{
			return mstrName;
		}

		float GetDuration() const
		{
			return mfDuration;
		}

		const Array<IModelKeyFramePtr>& GetKeyFrames() const
		{
			return mKeyFrames;
		}

	private:

		String mstrName;
		float mfDuration;
		Array<IModelKeyFramePtr> mKeyFrames;
	};
	
	class CEzmModelSkinningData : public IModelSkinningData
	{
	public:

		friend class CEzmModel;
		friend class CEzmModelMesh;

		CEzmModelSkinningData();
		~CEzmModelSkinningData();

		const Array<IModelJointPtr>& GetSkeleton() const
		{
			return mSkeleton;
		}

		const Array<IModelAnimationPtr>& GetAnimations() const
		{
			return mAnimations;
		}

	private:

		Array<IModelJointPtr> mSkeleton;
		Array<IModelAnimationPtr> mAnimations;		
	};

	using CEzmModelSkinningDataPtr = SmartPtr<CEzmModelSkinningData>;

	class CEzmModelMesh : public IModelMesh
	{
	public:

		friend class CEzmModel;

		CEzmModelMesh();
		~CEzmModelMesh();

		const String& GetName() const
		{
			return mstrName;
		}

		const Array<SModelVertex>& GetVertices() const
		{
			return mVertices;
		}

		const Array<uint16_t>& GetIndices() const
		{
			return mIndices;
		}

		const Array<IModelMeshPartPtr>& GetMeshParts() const
		{
			return mMeshParts;
		}

		void AddVertex(const SModelVertex &pVertex);
		void AddFace(uint16_t puiI1, uint16_t puiI2, uint16_t puiI3);

	private:

		String mstrName;
		Array<SModelVertex> mVertices;
		Array<uint16_t> mIndices;
		Array<IModelMeshPartPtr> mMeshParts;
	};

	class CEzmModel : public IModel
	{
	public:

		CEzmModel();
		~CEzmModel();

		EModelType GetModelType() const
		{
			return EModelType::Ezm;
		}

		const Array<IModelMaterialPtr>& GetMaterials() const
		{
			return mMaterials;
		}

		const Array<IModelMeshPtr>& GetMeshes() const
		{
			return mMeshes;
		}

		const IModelSkinningDataPtr& GetSkinningData() const
		{
			return mSkinningData;
		}

		EResult Create(const String &pstrFilePath);

	private:
				
		EResult LoadAnimations(tinyxml2::XMLNode *pAnimationsNode, Map<std::string, int32_t> &pBoneIndexMap);		
		EResult LoadSkeleton(tinyxml2::XMLNode *pSkeletonNode, Map<std::string, int32_t> &pBoneIndexMap);

		EResult LoadMaterials(tinyxml2::XMLNode *pMaterialsNode, const String &pstrPath);
		EResult LoadMeshes(tinyxml2::XMLNode *pMeshesNode);
		EResult LoadMesh(tinyxml2::XMLNode *pMeshNode, CEzmModelMesh *pMesh);
		EResult LoadMeshPart(tinyxml2::XMLNode *pMeshPartNode, CEzmModelMesh *pMesh, 
			CEzmModelMeshPart *pMeshPart, uint32_t puiStartIndex);

		Array<IModelMaterialPtr> mMaterials;
		Array<IModelMeshPtr> mMeshes;
		IModelSkinningDataPtr mSkinningData;
	};

	class FDX_EXPORT CEzmModelLoader : public IModelLoader
	{
	public:

		CEzmModelLoader();
		~CEzmModelLoader();

		EModelLoaderType GetModelLoaderType() const;

		bool CanLoadModelType(EModelType pType);
		bool CanLoadModel(const String &pstrFilePath);

		EResult Load(const String &pstrFilePath,
			IModel **pModel);
	};
}