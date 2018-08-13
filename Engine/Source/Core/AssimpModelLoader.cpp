
#include "Core/AssimpModelLoader.h"
#include "FS/FileSystem.h"
#include "OS/GameLogger.h"
#include "OS/System.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

namespace FDX
{
	CMatrix4F ToMatrix4(const aiMatrix4x4 &pAssimpMatrix)
	{
		return CMatrix4F(
			pAssimpMatrix[0][0], pAssimpMatrix[1][0], pAssimpMatrix[2][0], pAssimpMatrix[3][0],
			pAssimpMatrix[0][1], pAssimpMatrix[1][1], pAssimpMatrix[2][1], pAssimpMatrix[3][1],
			pAssimpMatrix[0][2], pAssimpMatrix[1][2], pAssimpMatrix[2][2], pAssimpMatrix[3][2],
			pAssimpMatrix[0][3], pAssimpMatrix[1][3], pAssimpMatrix[2][3], pAssimpMatrix[3][3]
		);
	}

	CAssimpModelJoint::CAssimpModelJoint()
	{
	}

	CAssimpModelKeyFrame::CAssimpModelKeyFrame()
		: mfTime(0.0f),
		muiBoneIndex(0)
	{
	}

	CAssimpModelAnimation::CAssimpModelAnimation()
		: mfDuration(0.0f)
	{
	}

	CAssimpModelSkinningData::CAssimpModelSkinningData()
	{
	}

	CAssimpModelMaterial::CAssimpModelMaterial()
		: mfSpecularPower(0.0f)
	{
	}

	CAssimpModelMeshPart::CAssimpModelMeshPart()
		: muiMaterialIndex(0),
		muiStartIndex(0),
		muiNumIndices(0)
	{
	}

	CAssimpModelMesh::CAssimpModelMesh()
	{
	}

	CAssimpModel::CAssimpModel()
	{
	}

	EResult CAssimpModel::Create(const String & pstrFilePath)
	{
		EResult lResult = EResult::Success;
		IFileStream *lStream = nullptr;

		lResult = IFileSystem::Get().GetStream(pstrFilePath, &lStream);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("IFileSystem::GetStream() failed for: %s", pstrFilePath.c_str());
		}
		else
		{
			String lstrExt;
			ISystem::Get().GetExtension(pstrFilePath, lstrExt);

			if (lstrExt == ".ezm")
				mModelType = EModelType::Ezm;
			else if (lstrExt == ".dae")
				mModelType = EModelType::Dae;
			else if (lstrExt == ".obj")
				mModelType = EModelType::Obj;
			else if (lstrExt == ".fbx")
				mModelType = EModelType::Fbx;			
			
			String lstrTexturePath;
			ISystem::Get().GetDirectory(pstrFilePath, lstrTexturePath);
			
			Assimp::Importer lImporter;
			lImporter.SetIOHandler(new CAssimpIOSystem(lstrTexturePath));

			const aiScene *lScene = lImporter.ReadFile(pstrFilePath,
				aiProcess_Triangulate | 
				aiProcess_LimitBoneWeights | 
				aiProcess_GenSmoothNormals |
				aiProcess_FlipUVs | 
				aiProcess_JoinIdenticalVertices |
				aiProcess_SortByPType);

			if (lScene == nullptr)
			{
				LOG_DEBUG("Assimp::Importer::ReadFile() failed for: %s", pstrFilePath.c_str());
				lResult = EResult::LoadFailed;
			}
			else
			{
				lResult = LoadMeshes(lScene);
				if (lResult != EResult::Success)
				{
					LOG_DEBUG("LoadMeshes() failed for: %s", pstrFilePath.c_str());
				}
				else
				{
					lResult = LoadMaterials(lScene, lstrTexturePath);
					if (lResult != EResult::Success)
					{
						LOG_DEBUG("LoadMaterials() failed for: %s", pstrFilePath.c_str());
					}

					lResult = LoadSkeleton(lScene);
					if (lResult != EResult::Success)
					{
						LOG_DEBUG("LoadSkeleton() failed for: %s", pstrFilePath.c_str());
					}
					else
					{
						lResult = LoadAnimations(lScene);
						if (lResult != EResult::Success)
						{
							LOG_DEBUG("LoadSkeleton() failed for: %s", pstrFilePath.c_str());
						}
					}
				}
			}
		}

		return lResult;
	}

	EResult CAssimpModel::LoadMaterials(const aiScene * pAssimpScene, const String &pstrTexturePath)
	{
		EResult lResult = EResult::Success;

		if (!pAssimpScene->HasMaterials())
		{
			LOG_DEBUG("aiScene::HasMeshes() returned false");
			lResult = EResult::LoadFailed;
		}
		else
		{
			for (uint32_t lIdx = 0; lIdx < pAssimpScene->mNumMaterials; lIdx++)
			{
				aiString lstrPath;
				aiColor4D lColor;
				ai_real lfSpecPower;

				const aiMaterial *lAssimpMaterial = pAssimpScene->mMaterials[lIdx];
				auto lMaterial = std::make_unique<CAssimpModelMaterial>();
				
				if (aiGetMaterialColor(lAssimpMaterial, AI_MATKEY_COLOR_AMBIENT, &lColor) == AI_SUCCESS)
					lMaterial->mAmbient = CColor4F(lColor.r, lColor.g, lColor.b, lColor.a);

				if (aiGetMaterialColor(lAssimpMaterial, AI_MATKEY_COLOR_DIFFUSE, &lColor) == AI_SUCCESS)
					lMaterial->mDiffuse = CColor4F(lColor.r, lColor.g, lColor.b, lColor.a);

				if (aiGetMaterialColor(lAssimpMaterial, AI_MATKEY_COLOR_EMISSIVE, &lColor) == AI_SUCCESS)
					lMaterial->mEmissive = CColor4F(lColor.r, lColor.g, lColor.b, lColor.a);

				if (aiGetMaterialColor(lAssimpMaterial, AI_MATKEY_COLOR_SPECULAR, &lColor) == AI_SUCCESS)
					lMaterial->mSpecular = CColor4F(lColor.r, lColor.g, lColor.b, lColor.a);

				if (aiGetMaterialFloat(lAssimpMaterial, AI_MATKEY_SHININESS, &lfSpecPower) == AI_SUCCESS)
					lMaterial->mfSpecularPower = lfSpecPower;

				if (lAssimpMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
				{					
					if (lAssimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &lstrPath) == AI_SUCCESS)
					{						
						String lstrTextureFileName;

						ISystem::Get().GetFileName(lstrPath.C_Str(), lstrTextureFileName);
						ISystem::Get().CombinePath(pstrTexturePath, lstrTextureFileName, 
							lMaterial->mstrDiffuseMap);
					}
				}

				if (lAssimpMaterial->GetTextureCount(aiTextureType_NORMALS) > 0)
				{
					if (lAssimpMaterial->GetTexture(aiTextureType_NORMALS, 0, &lstrPath) == AI_SUCCESS)
					{
						String lstrTextureFileName;

						ISystem::Get().GetFileName(lstrPath.C_Str(), lstrTextureFileName);
						ISystem::Get().CombinePath(pstrTexturePath, lstrTextureFileName,
							lMaterial->mstrNormalMap);
					}
				}

				if (lAssimpMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0)
				{
					if (lAssimpMaterial->GetTexture(aiTextureType_SPECULAR, 0, &lstrPath) == AI_SUCCESS)
					{
						String lstrTextureFileName;

						ISystem::Get().GetFileName(lstrPath.C_Str(), lstrTextureFileName);
						ISystem::Get().CombinePath(pstrTexturePath, lstrTextureFileName,
							lMaterial->mstrSpecularMap);
					}
				}

				mMaterials.push_back(std::move(lMaterial));
			}
		}

		return lResult;
	}

	EResult CAssimpModel::LoadMeshes(const aiScene * pAssimpScene)
	{
		EResult lResult = EResult::Success;

		if (!pAssimpScene->HasMeshes())
		{
			LOG_DEBUG("aiScene::HasMeshes() returned false");
			lResult = EResult::LoadFailed;
		}
		else
		{
			for (uint32_t lIdx = 0; lIdx < pAssimpScene->mNumMeshes; lIdx++)
			{
				lResult = LoadMesh(pAssimpScene->mMeshes[lIdx]);
				if (lResult != EResult::Success)
				{
					LOG_DEBUG("CAssimpModel::LoadMesh() failed for %d", lIdx);
					break;
				}
			}
		}

		return lResult;
	}

	EResult CAssimpModel::LoadMesh(const aiMesh * pAssimpMesh)
	{
		EResult lResult = EResult::Success;

		if (!pAssimpMesh->HasPositions())
		{
			LOG_DEBUG("aiScene::HasMeshes() returned false");
			lResult = EResult::LoadFailed;
		}
		else
		{
			const aiVector3D lZero3D(0.0f, 0.0f, 0.0f);		

			Array<SModelVertex> lVertices;
			Array<uint16_t> lIndices;
			Array<IModelMeshPartPtr> lMeshParts;

			for (uint32_t lIdx = 0; lIdx < pAssimpMesh->mNumVertices; lIdx++)
			{
				const aiVector3D *lPosition = &(pAssimpMesh->mVertices[lIdx]);
				const aiVector3D *lNormal = pAssimpMesh->HasNormals() ? &(pAssimpMesh->mNormals[lIdx]) : &lZero3D;
				const aiVector3D *lTexCoord = pAssimpMesh->HasTextureCoords(0) ? &(pAssimpMesh->mTextureCoords[0][lIdx]) : &lZero3D;
				
				lVertices.emplace_back(CVector3F(lPosition->x, lPosition->y, lPosition->z),
					CVector3F(lNormal->x, lNormal->y, lNormal->z),
					CVector2F(lTexCoord->x, lTexCoord->y));
			}

			if (pAssimpMesh->HasFaces())
			{
				for (uint32_t lIdx = 0; lIdx < pAssimpMesh->mNumFaces; lIdx++)
				{
					const aiFace &lFace = pAssimpMesh->mFaces[lIdx];

					if (lFace.mNumIndices == 3)
					{
						lIndices.push_back((uint16_t)lFace.mIndices[0]);
						lIndices.push_back((uint16_t)lFace.mIndices[1]);
						lIndices.push_back((uint16_t)lFace.mIndices[2]);
					}
				}

				auto lMeshPart = std::make_unique<CAssimpModelMeshPart>();
				lMeshPart->muiMaterialIndex = pAssimpMesh->mMaterialIndex;
				lMeshPart->muiStartIndex = 0;
				lMeshPart->muiNumIndices = lIndices.size();

				lMeshParts.push_back(std::move(lMeshPart));
			}

			auto lMesh = std::make_unique<CAssimpModelMesh>();

			if (pAssimpMesh->mName.length != 0)			
				lMesh->mstrName = String(pAssimpMesh->mName.C_Str());			

			lMesh->mVertices = std::move(lVertices);
			lMesh->mIndices = std::move(lIndices);
			lMesh->mMeshParts = std::move(lMeshParts);

			mMeshes.push_back(std::move(lMesh));
		}

		return lResult;
	}

	EResult CAssimpModel::LoadSkeleton(const aiScene *pAssimpScene)
	{
		EResult lResult = EResult::Success;

		if (!pAssimpScene->HasMeshes())
		{
			LOG_DEBUG("aiScene::HasMeshes() returned false");
			lResult = EResult::Success;
		}
		else
		{
			mSkinningData = std::make_unique<CAssimpModelSkinningData>();		

			lResult = LoadJointHierarchy(pAssimpScene->mRootNode);
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("LoadJointHierarchy() failed");
			}
			else
			{
				for (uint32_t lIdx = 0; lIdx < pAssimpScene->mNumMeshes; lIdx++)
				{
					const aiMesh *lAssimpMesh = pAssimpScene->mMeshes[lIdx];
					if (!lAssimpMesh->HasBones())
					{
						LOG_DEBUG("aiScene::HasBones() returned false");
						continue;
					}
					else
					{
						auto lSkinningData = dynamic_cast<CAssimpModelSkinningData*>
							(mSkinningData.get());

						lSkinningData->mRootTransform = ToMatrix4(pAssimpScene->mRootNode->mTransformation);						
						if (mModelType == EModelType::Dae)
						{
							lSkinningData->mRootTransform = lSkinningData->mRootTransform *
								CMatrix4F(
									-1, 0, 0, 0,
									0, 0, 1, 0,
									0, 1, 0, 0,
									0, 0, 0, 1
								);
						}

						for (uint32_t lIdx = 0; lIdx < lAssimpMesh->mNumBones; lIdx++)
						{
							const aiBone *lBone = lAssimpMesh->mBones[lIdx];
							auto lBoneMap = mBoneMapping.find(lBone->mName.C_Str());

							if (lBoneMap != mBoneMapping.end())
							{
								auto lJoint = dynamic_cast<CAssimpModelJoint*>(
									lSkinningData->mSkeleton[lBoneMap->second].get());

								lJoint->mbAnimated = true;
								lJoint->mInvBindPose = ToMatrix4(lBone->mOffsetMatrix);
							}
						}

						lResult = LoadVertexWeights(lAssimpMesh, lIdx);
						if (lResult != EResult::Success)
						{
							LOG_DEBUG("LoadVertexWeights() failed");
						}
					}
				}
			}
		}
		
		return lResult;
	}

	EResult CAssimpModel::LoadAnimations(const aiScene * pAssimpScene)
	{
		EResult lResult = EResult::Success;

		if (!pAssimpScene->HasAnimations())
		{
			LOG_DEBUG("aiScene::HasAnimations() returned false");
			lResult = EResult::Success;
		}
		else
		{
			for (uint32_t lIdx = 0; lIdx < pAssimpScene->mNumAnimations; lIdx++)
			{
				lResult = LoadAnimation(pAssimpScene->mAnimations[lIdx]);
				if (lResult != EResult::Success)
				{
					LOG_DEBUG("LoadAnimation() failed");
				}
			}
		}

		return lResult;
	}

	EResult CAssimpModel::LoadAnimation(const aiAnimation * pAssimpAnimation)
	{
		EResult lResult = EResult::Success;

		if (pAssimpAnimation->mNumChannels == 0)
		{
			LOG_DEBUG("aiAnimation::HasNumChannels() returned failed");
			lResult = EResult::LoadFailed;
		}
		else
		{
			Array<IModelKeyFramePtr> lKeyFrames;
						
			for (uint32_t lIdx = 0; lIdx < pAssimpAnimation->mNumChannels; lIdx++)
			{
				const aiNodeAnim *lChannel = pAssimpAnimation->mChannels[lIdx];	
				auto lBone = mBoneMapping.find(lChannel->mNodeName.C_Str());

				if (lBone != mBoneMapping.end())
				{
					for (uint32_t lJdx = 0; lJdx < lChannel->mNumPositionKeys; lJdx++)
					{
						auto lKeyFrame = std::make_unique<CAssimpModelKeyFrame>();
						const aiVectorKey *lPositionKey = &(lChannel->mPositionKeys[lJdx]);
						const aiVectorKey *lScaleKey = &(lChannel->mScalingKeys[lJdx]);
						const aiQuatKey *lOrientationKey = &(lChannel->mRotationKeys[lJdx]);

						lKeyFrame->mfTime = (float)lPositionKey->mTime;
						lKeyFrame->muiBoneIndex = lBone->second;
						
						lKeyFrame->mPosition = CVector3F(lPositionKey->mValue.x,
							lPositionKey->mValue.y, lPositionKey->mValue.z);

						lKeyFrame->mScale = CVector3F(lScaleKey->mValue.x,
							lScaleKey->mValue.y, lScaleKey->mValue.z);

						lKeyFrame->mOrientation = CQuaternion(lOrientationKey->mValue.x,
							lOrientationKey->mValue.y, lOrientationKey->mValue.z,
							-lOrientationKey->mValue.w);

						lKeyFrames.push_back(std::move(lKeyFrame));
					}
				}
			}

			std::sort(lKeyFrames.begin(), lKeyFrames.end(), 
				[](const IModelKeyFramePtr &pA, const IModelKeyFramePtr &pB) -> bool {
					if (pA->GetTime() < pB->GetTime())
						return true;

					if (pA->GetTime() > pB->GetTime())
						return false;

					if (pA->GetBoneIndex() < pB->GetBoneIndex())
						return true;

					if (pA->GetBoneIndex() > pB->GetBoneIndex())
						return false;

					return false;
				}
			);

			auto lAnimation = std::make_unique<CAssimpModelAnimation>();
			lAnimation->mfDuration = (float)(pAssimpAnimation->mDuration /
				pAssimpAnimation->mTicksPerSecond);

			lAnimation->mstrName = String(pAssimpAnimation->mName.C_Str());
			lAnimation->mKeyFrames = std::move(lKeyFrames);

			dynamic_cast<CAssimpModelSkinningData*>(mSkinningData.get())->
				mAnimations.push_back(std::move(lAnimation));
		}

		return lResult;
	}

	EResult CAssimpModel::LoadJointHierarchy(const aiNode * pAssimpNode)
	{
		EResult lResult = EResult::Success;

		if (pAssimpNode)
		{
			auto lSkinningData = dynamic_cast<CAssimpModelSkinningData*>
				(mSkinningData.get());

			auto lJoint = std::make_unique<CAssimpModelJoint>();
			lJoint->mbAnimated = false;
			lJoint->mBindPose = ToMatrix4(pAssimpNode->mTransformation);
			lJoint->miIndex = lSkinningData->mSkeleton.size();
			lJoint->miParent = -1;

			if (pAssimpNode->mParent != nullptr)
			{
				auto lParentBone = mBoneMapping.find(pAssimpNode->mParent->mName.C_Str());
				if (lParentBone != mBoneMapping.end())
				{
					lJoint->miParent = lParentBone->second;
				}
			}

			mBoneMapping[pAssimpNode->mName.C_Str()] = lJoint->miIndex;
			lSkinningData->mSkeleton.push_back(std::move(lJoint));

			for (uint32_t lIdx = 0; lIdx < pAssimpNode->mNumChildren; lIdx++)
				LoadJointHierarchy(pAssimpNode->mChildren[lIdx]);
		}

		return lResult;
	}

	EResult CAssimpModel::LoadVertexWeights(const aiMesh *pAssimpMesh, uint32_t puiMeshIdx)
	{
		EResult lResult = EResult::Success;

		struct SVertexWeight
		{
			int32_t BoneIndex;
			float BoneWeight;

			SVertexWeight(uint32_t pBoneIndex, float pfBoneWeight)
				: BoneIndex(pBoneIndex), BoneWeight(pfBoneWeight)
			{
			}
		};

		Array<Array<SVertexWeight>> lWeights;
		lWeights.resize(pAssimpMesh->mNumVertices);

		for (uint32_t lIdx = 0; lIdx < pAssimpMesh->mNumBones; lIdx++)
		{
			const aiBone *lBone = pAssimpMesh->mBones[lIdx];
			const int32_t liBoneIndex = mBoneMapping[lBone->mName.C_Str()];

			if (liBoneIndex >= 0)
			{
				for (uint32_t lJdx = 0; lJdx < lBone->mNumWeights; lJdx++)
				{
					const aiVertexWeight *lWeight = &(lBone->mWeights[lJdx]);
					lWeights[lWeight->mVertexId].emplace_back(liBoneIndex, lWeight->mWeight);
				}
			}
		}

		const uint32_t luiMaxNumWeights = 4;
		auto lMesh = dynamic_cast<CAssimpModelMesh*>(mMeshes[puiMeshIdx].get());

		for (uint32_t lIdx = 0; lIdx < pAssimpMesh->mNumVertices; lIdx++)
		{
			auto &lVertexWeights = lWeights[lIdx];
			const uint32_t luiNumWeights = lVertexWeights.size();

			if (luiNumWeights > 0)
			{
				if (luiNumWeights < luiMaxNumWeights)
				{
					for (uint32_t lJdx = 0; lJdx < luiMaxNumWeights - luiNumWeights; lJdx++)
						lVertexWeights.emplace_back(0, 0.0f);
				}

				std::sort(lVertexWeights.begin(), lVertexWeights.end(), 
					[](const SVertexWeight &pA, const SVertexWeight &pB) {
						if (pA.BoneWeight > pB.BoneWeight)
							return true;

						return false;
					}
				);

				float lfTotalWeight = 0;
				for (uint32_t lJdx = 0; lJdx < luiMaxNumWeights; lJdx++)
					lfTotalWeight += lVertexWeights[lJdx].BoneWeight;

				lfTotalWeight = 1.0f / lfTotalWeight;
				for (uint32_t lJdx = 0; lJdx < luiMaxNumWeights; lJdx++)
					lVertexWeights[lJdx].BoneWeight *= lfTotalWeight;

				lMesh->mVertices[lIdx].BlendIndices = CVector4F((float)lVertexWeights[0].BoneIndex,
					(float)lVertexWeights[1].BoneIndex, (float)lVertexWeights[2].BoneIndex,
					(float)lVertexWeights[3].BoneIndex);

				lMesh->mVertices[lIdx].BlendWeights = CVector4F(lVertexWeights[0].BoneWeight,
					lVertexWeights[1].BoneWeight, lVertexWeights[2].BoneWeight,
					lVertexWeights[3].BoneWeight);
			}
		}

		return lResult;
	}

	CAssimpIOStream::CAssimpIOStream(IFileStream * pStream)
		: mStream(pStream)
	{
	}

	size_t CAssimpIOStream::Read(void * pvBuffer, size_t pSize, size_t pCount)
	{
		EResult lResult = mStream->Read(pSize * pCount, pvBuffer);
		if (lResult != EResult::Success)
		{
			return (size_t)0;
		}

		return pCount;
	}

	size_t CAssimpIOStream::Write(const void * pvBuffer, size_t pSize, size_t pCount)
	{
		EResult lResult = mStream->Write(pSize * pCount, pvBuffer);
		if (lResult != EResult::Success)
		{
			return (size_t)0;
		}

		return pCount;
	}

	aiReturn CAssimpIOStream::Seek(size_t pOffset, aiOrigin pOrigin)
	{
		ESeekOrigin lSeekOrigin = ESeekOrigin::Begining;

		switch (pOrigin)
		{
		case aiOrigin_SET:
			lSeekOrigin = ESeekOrigin::Begining;
			break;

		case aiOrigin_CUR:
			lSeekOrigin = ESeekOrigin::Current;
			break;

		case aiOrigin_END:
			lSeekOrigin = ESeekOrigin::End;
			break;
		}

		mStream->Seek(lSeekOrigin, (int32_t)pOffset);

		return aiReturn_SUCCESS;
	}

	size_t CAssimpIOStream::Tell() const
	{
		return (size_t)mStream->GetPosition();
	}

	size_t CAssimpIOStream::FileSize() const
	{
		return (size_t)mStream->GetSize();
	}

	void CAssimpIOStream::Flush()
	{
		mStream->Flush();
	}

	CAssimpIOSystem::CAssimpIOSystem(const String & pstrModelPath)
		: mstrModelPath(pstrModelPath)
	{
	}

	bool CAssimpIOSystem::Exists(const char * pFile) const
	{
		String lstrFilePath;
		String lstrFileName;

		ISystem::Get().GetFileName(pFile, lstrFileName);
		ISystem::Get().CombinePath(mstrModelPath, lstrFileName, lstrFilePath);

		return IFileSystem::Get().IsExist(lstrFilePath);
	}

	char CAssimpIOSystem::getOsSeparator() const
	{
		char lchSeparator = IFileSystem::Get().GetPathSeperator();
		return (lchSeparator == '\\') ? '\\' : '/';
	}

	Assimp::IOStream * CAssimpIOSystem::Open(const char * pFile, const char * pMode)
	{
		CAssimpIOStream *lIOStream = nullptr;

		String lstrFilePath;
		String lstrFileName;

		ISystem::Get().GetFileName(pFile, lstrFileName);
		ISystem::Get().CombinePath(mstrModelPath, lstrFileName, lstrFilePath);

		if (IFileSystem::Get().IsExist(lstrFilePath))
		{
			EResult lResult = EResult::Success;
			IFileStream *lStream = nullptr;

			lResult = IFileSystem::Get().GetStream(lstrFilePath, &lStream);
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("IFileSystem::GetStream() failed for: %s", lstrFilePath.c_str());
			}
			else
			{
				lIOStream = new CAssimpIOStream(lStream);
			}
		}

		return lIOStream;
	}

	void CAssimpIOSystem::Close(Assimp::IOStream * pFile)
	{
		SAFE_DELETE(pFile);
	}

	EModelLoaderType CAssimpModelLoader::GetModelLoaderType() const
	{
		return EModelLoaderType::Assimp;
	}

	bool CAssimpModelLoader::CanLoadModelType(EModelType pType)
	{
		return pType == EModelType::Ezm ||
			pType == EModelType::Obj ||
			pType == EModelType::Fbx ||
			pType == EModelType::Dae;
	}

	bool CAssimpModelLoader::CanLoadModel(const String & pstrFilePath)
	{
		bool lbCanLoad = false;
		uint32_t luiDot = pstrFilePath.find_last_of(".");

		if (luiDot != (uint32_t)String::npos)
		{
			String lstrExt = pstrFilePath.substr(luiDot);
			std::transform(lstrExt.begin(), lstrExt.end(), lstrExt.begin(), ::tolower);

			if (lstrExt == ".ezm" ||
				lstrExt == ".dae" ||
				lstrExt == ".obj" ||
				lstrExt == ".fbx")
			{
				lbCanLoad = true;
			}
		}

		return lbCanLoad;
	}

	EResult CAssimpModelLoader::Load(const String & pstrFilePath, IModel ** pModel)
	{
		EResult lResult = EResult::Success;

		CAssimpModel *lModel = new CAssimpModel();
		lResult = lModel->Create(pstrFilePath);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CAssimpModel::Create() failed for: %s", pstrFilePath.c_str());
			SAFE_DELETE(lModel);
		}
		else
		{
			*pModel = lModel;
		}

		return lResult;
	}
}