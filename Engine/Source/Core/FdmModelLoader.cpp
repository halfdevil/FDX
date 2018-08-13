
#include "Math/Quaternion.h"
#include "FS/FileSystem.h"
#include "OS/System.h"
#include "OS/GameLogger.h"
#include "Core/StringUtil.h"
#include "Core/FdmModelLoader.h"
#include "Core/FdmModelStructs.h"

namespace FDX
{
	CFdmModelJoint::CFdmModelJoint()
		: miIndex(-1), miParent(-1),
		mBindPose(CMatrix4F::IdentityMatrix())
	{
	}

	CFdmModelJoint::~CFdmModelJoint()
	{
	}

	inline void CFdmModelJoint::SetIndex(const int32_t piIndex)
	{
		miIndex = piIndex;
	}

	inline void CFdmModelJoint::SetParent(const int32_t piParent)
	{
		miParent = piParent;
	}

	inline void CFdmModelJoint::SetAnimated(bool pbAnimated)
	{
		mbAnimated = pbAnimated;
	}

	inline void CFdmModelJoint::SetBindPose(const CMatrix4F & pBindPose)
	{
		mBindPose = pBindPose;
	}

	CFdmModelKeyFrame::CFdmModelKeyFrame()
		: muiBoneIndex(0)
	{
	}

	CFdmModelKeyFrame::~CFdmModelKeyFrame()
	{
	}

	inline void CFdmModelKeyFrame::SetTime(float pfTime)
	{
		mfTime = pfTime;
	}

	inline void CFdmModelKeyFrame::SetBoneIndex(uint32_t puiBoneIndex)
	{
		muiBoneIndex = puiBoneIndex;
	}

	inline void CFdmModelKeyFrame::SetPosition(const CVector3F & pPosition)
	{
		mPosition = pPosition;
	}

	inline void CFdmModelKeyFrame::SetScale(const CVector3F & pScale)
	{
		mScale = pScale;
	}

	inline void CFdmModelKeyFrame::SetOrientation(const CQuaternion & pOrientation)
	{
		mOrientation = pOrientation;
	}

	CFdmModelAnimation::CFdmModelAnimation()
	{
	}

	CFdmModelAnimation::~CFdmModelAnimation()
	{
	}

	inline void CFdmModelAnimation::SetName(const String & pstrName)
	{
		mstrName = pstrName;
	}

	inline void CFdmModelAnimation::SetDuration(float pfDuration)
	{
		mfDuration = pfDuration;
	}

	inline void CFdmModelAnimation::SetKeyFrames(Array<IModelKeyFramePtr>&& pKeyFrames)
	{
		mKeyFrames = std::move(pKeyFrames);
	}

	CFdmModelSkinningData::CFdmModelSkinningData()
	{
	}

	CFdmModelSkinningData::~CFdmModelSkinningData()
	{
	}

	inline void CFdmModelSkinningData::SetRootTransform(const CMatrix4F & pTransform)
	{
		mRootTransform = pTransform;
	}

	inline void CFdmModelSkinningData::SetSkeleton(Array<IModelJointPtr>&& pSkeleton)
	{
		mSkeleton = std::move(pSkeleton);
	}

	inline void CFdmModelSkinningData::SetAnimations(Array<IModelAnimationPtr>&& pAnimations)
	{
		mAnimations = std::move(pAnimations);
	}

	CFdmModelMaterial::CFdmModelMaterial()
		: mfSpecularPower(0.0f)
	{
	}

	CFdmModelMaterial::~CFdmModelMaterial()
	{
	}

	inline void CFdmModelMaterial::SetName(const String & pstrName)
	{
		mstrName = pstrName;
	}

	inline void CFdmModelMaterial::SetAmbient(const CColor4F & pAmbient)
	{
		mAmbient = pAmbient;
	}

	inline void CFdmModelMaterial::SetDiffuse(const CColor4F & pDiffuse)
	{
		mDiffuse = pDiffuse;
	}

	inline void CFdmModelMaterial::SetEmissive(const CColor4F & pEmissive)
	{
		mEmissive = pEmissive;
	}

	inline void CFdmModelMaterial::SetSpecular(const CColor4F & pSpecular)
	{
		mSpecular = pSpecular;
	}

	inline void CFdmModelMaterial::SetSpecularPower(float pfSpecularPower)
	{
		mfSpecularPower = pfSpecularPower;
	}

	inline void CFdmModelMaterial::SetDiffuseMap(const String & pstrDiffuseMap)
	{
		mstrDiffuseMap = pstrDiffuseMap;
	}

	inline void CFdmModelMaterial::SetNormalMap(const String & pstrNormalMap)
	{
		mstrNormalMap = pstrNormalMap;
	}

	inline void CFdmModelMaterial::SetSpecularMap(const String & pstrSpecularMap)
	{
		mstrSpecularMap = pstrSpecularMap;
	}

	CFdmModelMeshPart::CFdmModelMeshPart()
		: muiMaterialIndex(0),
		muiStartIndex(0),
		muiNumIndices(0)
	{
	}

	CFdmModelMeshPart::~CFdmModelMeshPart()
	{
	}

	inline void CFdmModelMeshPart::SetMaterialIndex(uint32_t puiMaterialIndex)
	{
		muiMaterialIndex = puiMaterialIndex;
	}

	inline void CFdmModelMeshPart::SetStartIndex(uint32_t puiStartIndex)
	{
		muiStartIndex = puiStartIndex;
	}

	inline void CFdmModelMeshPart::SetNumIndices(uint32_t puiNumIndices)
	{
		muiNumIndices = puiNumIndices;
	}

	CFdmModelMesh::CFdmModelMesh()
	{
	}

	CFdmModelMesh::~CFdmModelMesh()
	{
	}

	inline void CFdmModelMesh::SetName(const String & pstrName)
	{
		mstrName = pstrName;
	}

	inline void CFdmModelMesh::SetVertices(Array<SModelVertex>&& pVertices)
	{
		mVertices = std::move(pVertices);
	}

	inline void CFdmModelMesh::SetIndices(Array<uint16_t>&& pIndices)
	{
		mIndices = std::move(pIndices);
	}

	inline void CFdmModelMesh::SetMeshParts(Array<IModelMeshPartPtr>&& pMeshParts)
	{
		mMeshParts = std::move(pMeshParts);
	}

	CFdmModel::CFdmModel()
	{
	}

	CFdmModel::~CFdmModel()
	{
	}

	EResult CFdmModel::Load(const String & pstrFilePath)
	{
		EResult lResult = EResult::Success;
		IFileStream *lStream = nullptr;

		Reset();

		lResult = IFileSystem::Get().GetStream(pstrFilePath, &lStream);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("Model GetStream failed: %s", pstrFilePath.c_str());
		}
		else
		{
			CStreamReaderPtr lStreamReader = std::make_unique<CStreamReader>(lStream);
			
			lResult = ReadChunks(lStreamReader.get());
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("Unable to read chunks from model %s", pstrFilePath.c_str());
			}
			else
			{
				lResult = Process();
				if (lResult != EResult::Success)
				{
					LOG_DEBUG("Unable to process chunks for model %s", pstrFilePath.c_str());
				}
			}
		}

		Reset();

		return lResult;
	}

	EFdmChunkType CFdmModel::GetNextChunk(CStreamReader *pReader, SFdmChunk & pChunk)
	{
		pReader->ReadBuffer(sizeof(SFdmChunk), &pChunk);
		return pChunk.Type;
	}

	EResult CFdmModel::ReadChunks(CStreamReader * pReader)
	{
		EResult lResult = EResult::Success;

		SFdmChunk lChunk;
		bool lbLoop = true;

		do
		{
			switch (GetNextChunk(pReader, lChunk))
			{
			case EFdmChunkType::Header:
				lResult = ReadHeader(pReader);
				break;

			case EFdmChunkType::Materials:
				lResult = ReadMaterials(pReader);
				break;

			case EFdmChunkType::Meshes:
				lResult = ReadMeshes(pReader);
				break;

			case EFdmChunkType::Vertices:
				lResult = ReadVertices(pReader);
				break;

			case EFdmChunkType::Indices:
				lResult = ReadIndices(pReader);
				break;

			case EFdmChunkType::MeshParts:
				lResult = ReadMeshParts(pReader);
				break;

			case EFdmChunkType::Joints:
				lResult = ReadJoints(pReader);
				break;

			case EFdmChunkType::Animations:
				lResult = ReadAnimations(pReader);
				break;

			case EFdmChunkType::KeyFrames:
				lResult = ReadKeyFrames(pReader);

			case EFdmChunkType::End:
				lbLoop = false;
				break;

			default:
				break;
			}

			if (lResult != EResult::Success)
			{
				LOG_ERROR("Error reading chunk: %d while loading model", lChunk.Type);
				lbLoop = false;
			}
		} while (lbLoop);

		return lResult;
	}

	EResult CFdmModel::ReadHeader(CStreamReader * pReader)
	{
		EResult lResult = EResult::Success;

		lResult = pReader->ReadBuffer(sizeof(SFdmHeader), &mFdmHeader);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("Error reading header");
		}

		return lResult;
	}

	EResult CFdmModel::ReadMaterials(CStreamReader * pReader)
	{
		EResult lResult = EResult::Success;
		SFdmChunk lChunk;

		mFdmMaterials.resize(mFdmHeader.NumMaterials);
		lResult = pReader->ReadBuffer(sizeof(SFdmMaterial) * mFdmHeader.NumMaterials, 
			mFdmMaterials.data());

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("Error reading materials");
		}

		if (GetNextChunk(pReader, lChunk) != EFdmChunkType::End)
		{
			LOG_DEBUG("Chunk end missing");
		}

		return lResult;
	}

	EResult CFdmModel::ReadMeshes(CStreamReader * pReader)
	{
		EResult lResult = EResult::Success;
		SFdmChunk lChunk;

		mFdmMeshes.resize(mFdmHeader.NumMeshes);
		lResult = pReader->ReadBuffer(sizeof(SFdmMesh) * mFdmHeader.NumMeshes,
			mFdmMeshes.data());

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("Error reading meshes");
		}

		if (GetNextChunk(pReader, lChunk) != EFdmChunkType::End)
		{
			LOG_DEBUG("Chunk end missing");
		}

		return lResult;
	}

	EResult CFdmModel::ReadVertices(CStreamReader * pReader)
	{
		EResult lResult = EResult::Success;
		SFdmChunk lChunk;

		mFdmVertices.resize(mFdmHeader.NumVertices);
		lResult = pReader->ReadBuffer(sizeof(SFdmVertex) * mFdmHeader.NumVertices,
			mFdmVertices.data());

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("Error reading vertices");
		}

		if (GetNextChunk(pReader, lChunk) != EFdmChunkType::End)
		{
			LOG_DEBUG("Chunk end missing");
		}

		return lResult;
	}

	EResult CFdmModel::ReadIndices(CStreamReader * pReader)
	{
		EResult lResult = EResult::Success;
		SFdmChunk lChunk;

		mFdmIndices.resize(mFdmHeader.NumIndices);
		lResult = pReader->ReadBuffer(sizeof(uint16_t) * mFdmHeader.NumIndices,
			mFdmIndices.data());

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("Error reading indices");
		}

		if (GetNextChunk(pReader, lChunk) != EFdmChunkType::End)
		{
			LOG_DEBUG("Chunk end missing");
		}

		return lResult;
	}

	EResult CFdmModel::ReadMeshParts(CStreamReader * pReader)
	{
		EResult lResult = EResult::Success;
		SFdmChunk lChunk;

		mFdmMeshParts.resize(mFdmHeader.NumMeshParts);
		lResult = pReader->ReadBuffer(sizeof(SFdmMeshPart) * mFdmHeader.NumMeshParts,
			mFdmMeshParts.data());

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("Error reading mesh parts");
		}

		if (GetNextChunk(pReader, lChunk) != EFdmChunkType::End)
		{
			LOG_DEBUG("Chunk end missing");
		}

		return lResult;
	}

	EResult CFdmModel::ReadJoints(CStreamReader * pReader)
	{
		EResult lResult = EResult::Success;
		SFdmChunk lChunk;

		mFdmJoints.resize(mFdmHeader.NumJoints);
		lResult = pReader->ReadBuffer(sizeof(SFdmJoint) * mFdmHeader.NumJoints,
			mFdmJoints.data());

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("Error reading joints");
		}

		if (GetNextChunk(pReader, lChunk) != EFdmChunkType::End)
		{
			LOG_DEBUG("Chunk end missing");
		}

		return lResult;
	}

	EResult CFdmModel::ReadAnimations(CStreamReader * pReader)
	{
		EResult lResult = EResult::Success;
		SFdmChunk lChunk;

		mFdmAnimations.resize(mFdmHeader.NumAnimations);
		lResult = pReader->ReadBuffer(sizeof(SFdmAnimation) * mFdmHeader.NumAnimations,
			mFdmAnimations.data());

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("Error reading animations");
		}

		if (GetNextChunk(pReader, lChunk) != EFdmChunkType::End)
		{
			LOG_DEBUG("Chunk end missing");
		}

		return lResult;
	}

	EResult CFdmModel::ReadKeyFrames(CStreamReader * pReader)
	{
		EResult lResult = EResult::Success;
		SFdmChunk lChunk;

		mFdmKeyFrames.resize(mFdmHeader.NumKeyFrames);
		lResult = pReader->ReadBuffer(sizeof(SFdmKeyFrame) * mFdmHeader.NumKeyFrames,
			mFdmKeyFrames.data());

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("Error reading keyframes");
		}

		if (GetNextChunk(pReader, lChunk) != EFdmChunkType::End)
		{
			LOG_DEBUG("Chunk end missing");
		}

		return lResult;
	}

	EResult CFdmModel::Process()
	{
		EResult lResult = EResult::Success;

		lResult = ProcessMaterials();
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("Error processing materials");
		}
		else
		{
			lResult = ProcessMeshes();
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("Error processing meshes");
			}
			else
			{
				lResult = ProcessAnimations();
				if (lResult != EResult::Success)
				{
					LOG_DEBUG("Error processing animations");
				}
				else
				{
					lResult = ProcessSkeleton();
					if (lResult != EResult::Success)
					{
						LOG_DEBUG("Error processing skeleton");
					}
				}
			}
		}

		return lResult;
	}

	EResult CFdmModel::ProcessMaterials()
	{
		EResult lResult = EResult::Success;		

		for (const auto &lFdmMaterial : mFdmMaterials)
		{
			auto lMaterial = std::make_unique<CFdmModelMaterial>();

			String lstrName;
			String lstrFileName;

			lMaterial->SetName(lFdmMaterial.Name);
			lMaterial->SetAmbient(lFdmMaterial.Ambient);
			lMaterial->SetDiffuse(lFdmMaterial.Diffuse);
			lMaterial->SetEmissive(lFdmMaterial.Emissive);

			lMaterial->SetSpecular(lFdmMaterial.Specular);
			lMaterial->SetSpecularPower(lFdmMaterial.SpecularPower);
			
			lMaterial->SetDiffuseMap(lFdmMaterial.DiffuseMap);
			lMaterial->SetNormalMap(lFdmMaterial.NormalMap);
			lMaterial->SetSpecularMap(lFdmMaterial.SpecularMap);

			mMaterials.push_back(std::move(lMaterial));
		}

		return lResult;
	}

	EResult CFdmModel::ProcessMeshes()
	{
		EResult lResult = EResult::Success;

		for (const auto &lFdmMesh : mFdmMeshes)
		{			
			Array<SModelVertex> lVertices;
			Array<uint16_t> lIndices;
			Array<IModelMeshPartPtr> lMeshParts;
			CFdmModelMesh *lMesh;
						
			lMesh = new CFdmModelMesh();
			lMesh->SetName(lFdmMesh.Name);

			for (uint32_t lIdx = 0; lIdx < lFdmMesh.NumVertices; lIdx++)
			{
				SModelVertex lVertex;
				SFdmVertex &lFdmVertex = mFdmVertices[lIdx + lFdmMesh.StartVertex];
				
				lVertex.Position = lFdmVertex.Position;
				lVertex.Normal = lFdmVertex.Normal;
				lVertex.TexCoord1 = lFdmVertex.TexCoord1;
				lVertex.TexCoord2 = lFdmVertex.TexCoord2;
				lVertex.TexCoord3 = lFdmVertex.TexCoord3;
				lVertex.BlendIndices = lFdmVertex.BlendIndices;
				lVertex.BlendWeights = lFdmVertex.BlendWeights;

				lVertices.push_back(lVertex);
			}

			for (uint32_t lIdx = 0; lIdx < lFdmMesh.NumIndices; lIdx++)
				lIndices.push_back(mFdmIndices[lIdx + lFdmMesh.StartIndex]);

			for (uint32_t lIdx = 0; lIdx < lFdmMesh.NumMeshParts; lIdx++)
			{
				CFdmModelMeshPart *lMeshPart = new CFdmModelMeshPart();
				SFdmMeshPart &lFdmMeshPart = mFdmMeshParts[lIdx + lFdmMesh.StartMeshPart];

				lMeshPart->SetMaterialIndex(lFdmMeshPart.MaterialIndex);
				lMeshPart->SetStartIndex(lFdmMeshPart.StartIndex);
				lMeshPart->SetNumIndices(lFdmMeshPart.NumIndices);

				lMeshParts.push_back(IModelMeshPartPtr(lMeshPart));
			}

			lMesh->SetVertices(std::move(lVertices));
			lMesh->SetIndices(std::move(lIndices));
			lMesh->SetMeshParts(std::move(lMeshParts));

			mMeshes.push_back(IModelMeshPtr(lMesh));
		}

		return lResult;
	}

	EResult CFdmModel::ProcessAnimations()
	{
		EResult lResult = EResult::Success;
		Array<IModelAnimationPtr> lAnimations;

		for (const auto &lFdmAnimation : mFdmAnimations)
		{
			Array<IModelKeyFramePtr> lKeyFrames;
			CFdmModelAnimation *lAnimation;

			lAnimation = new CFdmModelAnimation();
			lAnimation->SetName(lFdmAnimation.Name);
			lAnimation->SetDuration(lFdmAnimation.Duration);

			for (uint32_t lIdx = 0; lIdx < lFdmAnimation.NumKeyFrames; lIdx++)
			{
				SFdmKeyFrame &lFdmKeyFrame = mFdmKeyFrames[lFdmAnimation.StartKeyFrame + lIdx];
				CFdmModelKeyFrame *lKeyFrame = new CFdmModelKeyFrame();

				lKeyFrame->SetTime(lFdmKeyFrame.Time);
				lKeyFrame->SetBoneIndex(lFdmKeyFrame.JointIndex);
				lKeyFrame->SetPosition(lFdmKeyFrame.Position);
				lKeyFrame->SetOrientation(lFdmKeyFrame.Orientation);

				lKeyFrames.push_back(IModelKeyFramePtr(lKeyFrame));
			}

			lAnimation->SetKeyFrames(std::move(lKeyFrames));
			lAnimations.push_back(IModelAnimationPtr(lAnimation));
		}

		if (!mSkinningData)
			mSkinningData = std::make_unique<CFdmModelSkinningData>();

		static_cast<CFdmModelSkinningData*>(mSkinningData.get())->
			SetAnimations(std::move(lAnimations));

		return lResult;
	}

	EResult CFdmModel::ProcessSkeleton()
	{
		EResult lResult = EResult::Success;
		Array<IModelJointPtr> lJoints;

		for (const auto &lFdmJoint : mFdmJoints)
		{
			CFdmModelJoint *lJoint = new CFdmModelJoint();
			lJoint->SetIndex(lFdmJoint.Index);
			lJoint->SetParent(lFdmJoint.Parent);
			lJoint->SetBindPose(lFdmJoint.BindPose);

			lJoints.push_back(IModelJointPtr(lJoint));
		}

		if (!mSkinningData)
			mSkinningData = std::make_unique<CFdmModelSkinningData>();

		static_cast<CFdmModelSkinningData*>(mSkinningData.get())->
			SetSkeleton(std::move(lJoints));

		return lResult;
	}

	void CFdmModel::Reset()
	{
		mFdmMaterials.clear();
		mFdmMeshes.clear();
		mFdmVertices.clear();
		mFdmIndices.clear();
		mFdmMeshParts.clear();
		mFdmJoints.clear();
		mFdmAnimations.clear();
		mFdmKeyFrames.clear();
	}
}