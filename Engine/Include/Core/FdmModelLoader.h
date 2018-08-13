#pragma once

#include "Core/ModelLoader.h"
#include "FS/FileStream.h"
#include "FS/StreamReader.h"
#include "Core/FdmModelStructs.h"

namespace FDX
{
	class CFdmModelJoint : public IModelJoint
	{
	public:

		CFdmModelJoint();
		~CFdmModelJoint();

		int32_t GetIndex() const
		{
			return miIndex;
		}

		int32_t GetParent() const
		{
			return miParent;
		}

		bool IsAnimated() const
		{
			return mbAnimated;
		}

		const CMatrix4F& GetBindPose() const
		{
			return mBindPose;
		}

		const CMatrix4F& GetInvBindPose() const override
		{
			return mInvBindPose;
		}

		inline void SetIndex(const int32_t piIndex);
		inline void SetParent(const int32_t piParent);
		inline void SetAnimated(bool pbAnimated);
		inline void SetBindPose(const CMatrix4F &pBindPose);

	private:

		int32_t miIndex;
		int32_t miParent;
		bool mbAnimated;
		CMatrix4F mBindPose;
		CMatrix4F mInvBindPose;
	};

	class CFdmModelKeyFrame : public IModelKeyFrame
	{
	public:

		CFdmModelKeyFrame();
		~CFdmModelKeyFrame();

		float GetTime() const
		{
			return mfTime;
		}

		uint32_t GetBoneIndex() const
		{
			return muiBoneIndex;
		}

		const CVector3F& GetPosition() const
		{
			return mPosition;
		}

		const CVector3F& GetScale() const
		{
			return mScale;
		}

		const CQuaternion& GetOrientation() const
		{
			return mOrientation;
		}

		inline void SetTime(float pfTime);
		inline void SetBoneIndex(uint32_t puiBoneIndex);
		inline void SetPosition(const CVector3F &pPosition);
		inline void SetScale(const CVector3F &pScale);
		inline void SetOrientation(const CQuaternion &pOrientation);

	private:

		float mfTime;
		uint32_t muiBoneIndex;
		CVector3F mPosition;
		CVector3F mScale;
		CQuaternion mOrientation;
	};

	class CFdmModelAnimation : public IModelAnimation
	{
	public:

		CFdmModelAnimation();
		~CFdmModelAnimation();

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

		inline void SetName(const String &pstrName);
		inline void SetDuration(float pfDuration);
		inline void SetKeyFrames(Array<IModelKeyFramePtr> &&pKeyFrames);

	private:

		String mstrName;
		float mfDuration;
		Array<IModelKeyFramePtr> mKeyFrames;
	};

	class CFdmModelSkinningData : public IModelSkinningData
	{
	public:

		CFdmModelSkinningData();
		~CFdmModelSkinningData();

		const Array<IModelJointPtr>& GetSkeleton() const
		{
			return mSkeleton;
		}

		const Array<IModelAnimationPtr>& GetAnimations() const
		{
			return mAnimations;
		}

		const CMatrix4F& GetRootTransform() const
		{
			return mRootTransform;
		}

		inline void SetRootTransform(const CMatrix4F &pTransform);
		inline void SetSkeleton(Array<IModelJointPtr> &&pSkeleton);
		inline void SetAnimations(Array<IModelAnimationPtr> &&pAnimations);

	private:

		CMatrix4F mRootTransform;
		Array<IModelJointPtr> mSkeleton;
		Array<IModelAnimationPtr> mAnimations;
	};

	class CFdmModelMaterial : public IModelMaterial
	{
	public:

		CFdmModelMaterial();
		~CFdmModelMaterial();

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

		const String& GetNormalMap() const
		{
			return mstrNormalMap;
		}

		const String& GetSpecularMap() const
		{
			return mstrSpecularMap;
		}

		inline void SetName(const String &pstrName);
		inline void SetAmbient(const CColor4F &pAmbient);
		inline void SetDiffuse(const CColor4F &pDiffuse);
		inline void SetEmissive(const CColor4F &pEmissive);
		inline void SetSpecular(const CColor4F &pSpecular);
		inline void SetSpecularPower(float pfSpecularPower);
		inline void SetDiffuseMap(const String &pstrDiffuseMap);
		inline void SetNormalMap(const String &pstrNormalMap);
		inline void SetSpecularMap(const String &pstrSpecularMap);

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

	class CFdmModelMeshPart : public IModelMeshPart
	{
	public:

		CFdmModelMeshPart();
		~CFdmModelMeshPart();

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

		inline void SetMaterialIndex(uint32_t puiMaterialIndex);
		inline void SetStartIndex(uint32_t puiStartIndex);
		inline void SetNumIndices(uint32_t puiNumIndices);

	private:

		uint32_t muiMaterialIndex;
		uint32_t muiStartIndex;
		uint32_t muiNumIndices;
	};

	class CFdmModelMesh : public IModelMesh
	{
	public:

		CFdmModelMesh();
		~CFdmModelMesh();

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

		inline void SetName(const String &pstrName);
		inline void SetVertices(Array<SModelVertex> &&pVertices);
		inline void SetIndices(Array<uint16_t> &&pIndices);
		inline void SetMeshParts(Array<IModelMeshPartPtr> &&pMeshParts);

	private:

		String mstrName;
		Array<SModelVertex> mVertices;
		Array<uint16_t> mIndices;
		Array<IModelMeshPartPtr> mMeshParts;
	};

	class CFdmModel : public IModel
	{
	public:

		CFdmModel();
		~CFdmModel();

		EModelType GetModelType() const
		{
			return EModelType::Fdm;
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

		EResult Load(const String &pstrFilePath);

	private:
		
		EFdmChunkType GetNextChunk(CStreamReader *pReader, SFdmChunk &pChunk);

		EResult ReadChunks(CStreamReader *pReader);
		EResult ReadHeader(CStreamReader *pReader);
		EResult ReadMaterials(CStreamReader *pReader);
		EResult ReadMeshes(CStreamReader *pReader);
		EResult ReadVertices(CStreamReader *pReader);
		EResult ReadIndices(CStreamReader *pReader);
		EResult ReadMeshParts(CStreamReader *pReader);
		EResult ReadJoints(CStreamReader *pReader);
		EResult ReadAnimations(CStreamReader *pReader);
		EResult ReadKeyFrames(CStreamReader *pReader);

		EResult Process();
		EResult ProcessMaterials();
		EResult ProcessMeshes();
		EResult ProcessAnimations();
		EResult ProcessSkeleton();

		void Reset();

	private:

		Array<IModelMaterialPtr> mMaterials;
		Array<IModelMeshPtr> mMeshes;
		IModelSkinningDataPtr mSkinningData;

		SFdmHeader mFdmHeader;
		Array<SFdmMaterial> mFdmMaterials;
		Array<SFdmMesh> mFdmMeshes;
		Array<SFdmVertex> mFdmVertices;
		Array<uint16_t> mFdmIndices;
		Array<SFdmMeshPart> mFdmMeshParts;
		Array<SFdmJoint> mFdmJoints;
		Array<SFdmAnimation> mFdmAnimations;
		Array<SFdmKeyFrame> mFdmKeyFrames;
	};
}