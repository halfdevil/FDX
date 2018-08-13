#pragma once

#include "Core/ModelLoader.h"
#include "FS/FileStream.h"
#include "assimp/scene.h"
#include "assimp/IOStream.hpp"
#include "assimp/IOSystem.hpp"

namespace FDX
{
	class CAssimpModel;
	class CAssimpModelJoint;
	class CAssimpModelKeyFrame;
	class CAssimpModelAnimation;
	class CAssimpModelSkinningData;
	class CAssimpModelMaterial;
	class CAssimpModelMeshPart;
	class CAssimpModelMesh;

	class CAssimpModelJoint : public IModelJoint
	{
	public:

		friend class CAssimpModel;

		CAssimpModelJoint();

		int32_t GetIndex() const override
		{
			return miIndex;
		}

		int32_t GetParent() const override
		{
			return miParent;
		}

		bool IsAnimated() const override
		{
			return mbAnimated;
		}

		const CMatrix4F& GetBindPose() const override
		{
			return mBindPose;
		}

		const CMatrix4F& GetInvBindPose() const override
		{
			return mInvBindPose;
		}

	private:

		int32_t miIndex;
		int32_t miParent;
		bool mbAnimated;
		CMatrix4F mBindPose;
		CMatrix4F mInvBindPose;
	};

	class CAssimpModelKeyFrame : public IModelKeyFrame
	{
	public:

		friend class CAssimpModel;

		CAssimpModelKeyFrame();

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

		virtual const CVector3F& GetScale() const override
		{
			return mScale;
		}

		virtual const CQuaternion& GetOrientation() const override
		{
			return mOrientation;
		}

	private:

		float mfTime;
		uint32_t muiBoneIndex;
		CVector3F mPosition;
		CVector3F mScale;
		CQuaternion mOrientation;
	};

	class CAssimpModelAnimation : public IModelAnimation
	{
	public:

		friend class CAssimpModel;

		CAssimpModelAnimation();

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

	class CAssimpModelSkinningData : public IModelSkinningData
	{
	public:

		friend class CAssimpModel;

		CAssimpModelSkinningData();

		const CMatrix4F& GetRootTransform() const override
		{
			return mRootTransform;
		}

		const Array<IModelJointPtr>& GetSkeleton() const override
		{
			return mSkeleton;
		}

		const Array<IModelAnimationPtr>& GetAnimations() const override
		{
			return mAnimations;
		}
	
	private:

		CMatrix4F mRootTransform;
		Array<IModelJointPtr> mSkeleton;
		Array<IModelAnimationPtr> mAnimations;
	};

	class CAssimpModelMaterial : public IModelMaterial
	{
	public:

		friend class CAssimpModel;

		CAssimpModelMaterial();

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

	class CAssimpModelMeshPart : public IModelMeshPart
	{
	public:

		friend class CAssimpModel;

		CAssimpModelMeshPart();

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

	class CAssimpModelMesh : public IModelMesh
	{
	public:

		friend class CAssimpModel;

		CAssimpModelMesh();

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

	class CAssimpModel : public IModel
	{
	public:

		CAssimpModel();

		EModelType GetModelType() const
		{
			return mModelType;
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

		EResult LoadMaterials(const aiScene *pAssimpScene, const String &pstrTexturePath);
		EResult LoadMeshes(const aiScene *pAssimpScene);		
		EResult LoadMesh(const aiMesh *pAssimpMesh);
		EResult LoadSkeleton(const aiScene *pAssimpScene);
		EResult LoadAnimations(const aiScene *pAssimpScene);
		EResult LoadAnimation(const aiAnimation *pAssimpAnimation);		
		EResult LoadVertexWeights(const aiMesh *pMesh, uint32_t puiMeshIdx);
		EResult LoadJointHierarchy(const aiNode *pAssimpNode);

	private:

		EModelType mModelType;
		Array<IModelMaterialPtr> mMaterials;
		Array<IModelMeshPtr> mMeshes;
		IModelSkinningDataPtr mSkinningData;
		UnorderedMap<String, int32_t> mBoneMapping;
	};

	class CAssimpIOStream : public Assimp::IOStream
	{
	public:

		CAssimpIOStream(IFileStream *pStream);

		size_t Read(void* pvBuffer,
			size_t pSize,
			size_t pCount) override;

		size_t Write(const void* pvBuffer,
			size_t pSize,
			size_t pCount) override;

		aiReturn Seek(size_t pOffset,
			aiOrigin pOrigin) override;

		size_t Tell() const override;
		size_t FileSize() const override;

		void Flush();

	private:

		IFileStream * mStream;
	};

	class CAssimpIOSystem : public Assimp::IOSystem
	{
	public:

		CAssimpIOSystem(const String &pstrModelPath);

		bool Exists(const char* pFile) const override;
		char getOsSeparator() const override;

		Assimp::IOStream* Open(const char* pFile,
			const char* pMode) override;

		void Close(Assimp::IOStream* pFile);

	private:

		String mstrModelPath;
	};

	class FDX_EXPORT CAssimpModelLoader : public IModelLoader
	{
	public:
		
		EModelLoaderType GetModelLoaderType() const;

		bool CanLoadModelType(EModelType pType);
		bool CanLoadModel(const String &pstrFilePath);

		EResult Load(const String &pstrFilePath,
			IModel **pModel);
	};
}