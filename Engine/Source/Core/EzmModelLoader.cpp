
#include "Core/EzmModelLoader.h"
#include "Math/Quaternion.h"
#include "FS/FileSystem.h"
#include "OS/System.h"
#include "OS/GameLogger.h"
#include "Core/StringUtil.h"

namespace FDX
{	
	void StringToFloatArray(const std::string &pstrString, const std::string &pstrDelim, Array<float> &pArray)
	{
		int32_t luiStart = 0;

		for (uint32_t lIdx = 0; lIdx < pstrString.length(); lIdx++)
		{
			if (pstrDelim.find(pstrString[lIdx]) != std::string::npos)
			{
				std::string lstrPart = std::string(pstrString, luiStart, lIdx - luiStart);
				if (lstrPart.length() != 0)
				{
					pArray.push_back((float)atof(lstrPart.c_str()));
				}
				luiStart = lIdx + 1;
			}
		}

		if (luiStart != pstrString.length())
		{
			std::string lstrPart = std::string(pstrString, luiStart, pstrString.length() - luiStart);
			if (lstrPart.length() != 0)
			{
				pArray.push_back((float)atof(lstrPart.c_str()));
			}
		}
	}

	CEzmMaterial::CEzmMaterial()
		: mfSpecularPower(0.0f)
	{
	}

	CEzmMaterial::~CEzmMaterial()
	{
	}

	CEzmModelMeshPart::CEzmModelMeshPart()
		: muiMaterialIndex(0),
		muiStartIndex(0),
		muiNumIndices(0)
	{
	}

	CEzmModelMeshPart::~CEzmModelMeshPart()
	{
	}

	CEzmModelKeyFrame::CEzmModelKeyFrame()
		: mfTime(0.0f),
		muiBoneIndex(0)		
	{
	}

	CEzmModelKeyFrame::~CEzmModelKeyFrame()
	{
	}

	CEzmModelJoint::CEzmModelJoint()
	{
	}

	CEzmModelJoint::~CEzmModelJoint()
	{
	}

	CEzmModelAnimation::CEzmModelAnimation()
		: mfDuration(0.0f)
	{
	}

	CEzmModelAnimation::~CEzmModelAnimation()
	{
	}

	CEzmModelSkinningData::CEzmModelSkinningData()
	{
	}

	CEzmModelSkinningData::~CEzmModelSkinningData()
	{
	}

	CEzmModelMesh::CEzmModelMesh()
	{
	}

	CEzmModelMesh::~CEzmModelMesh()
	{
	}

	void CEzmModelMesh::AddVertex(const SModelVertex & pVertex)
	{
		mVertices.push_back(pVertex);
	}

	void CEzmModelMesh::AddFace(uint16_t puiI1, uint16_t puiI2, uint16_t puiI3)
	{
		mIndices.push_back(puiI1);
		mIndices.push_back(puiI2);
		mIndices.push_back(puiI3);
	}

	CEzmModel::CEzmModel()
		: mSkinningData(nullptr)
	{
	}

	CEzmModel::~CEzmModel()
	{
	}

	EResult CEzmModel::Create(const String & pstrFilePath)
	{
		EResult lResult = EResult::Success;
		IFileStream *lStream = nullptr;

		lResult = IFileSystem::Get().GetStream(pstrFilePath, &lStream);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG(_T("Model GetStream failed: %s"), pstrFilePath.c_str());
		}
		else
		{
			SmartPtr<uint8_t[]> lModelData = std::make_unique<uint8_t[]>(lStream->GetSize() + 1);
			memset(lModelData.get(), 0, lStream->GetSize());

			lResult = lStream->Read(lStream->GetSize(), lModelData.get());

			if (lResult != EResult::Success)
			{
				LOG_DEBUG(_T("Model data read from stream failed: %s"), pstrFilePath.c_str());
			}
			else
			{
				tinyxml2::XMLDocument lDocument;
				tinyxml2::XMLError lReturn = lDocument.Parse((const char*)lModelData.get(), lStream->GetSize());

				if (lReturn != tinyxml2::XMLError::XML_SUCCESS)
				{
					LOG_DEBUG(_T("Ezm file parsing error: %d"), lReturn);
					lResult = EResult::LoadFailed;
				}
				else
				{
					tinyxml2::XMLElement *lMeshSystemNode = lDocument.FirstChildElement("MeshSystem");

					if (lMeshSystemNode != nullptr)
					{
						Map<std::string, int32_t> lBoneIndexMap;
						tinyxml2::XMLElement *lChildNode = lMeshSystemNode->FirstChildElement();

						while (lChildNode != nullptr)
						{
							if (std::string(lChildNode->Name()).compare("Materials") == 0)
							{
								String lstrDirectory;
								ISystem::Get().GetDirectory(pstrFilePath, lstrDirectory);

								LoadMaterials(lChildNode, lstrDirectory);
							}
							else if (std::string(lChildNode->Name()).compare("Meshes") == 0)
							{
								LoadMeshes(lChildNode);
							}
							else if (std::string(lChildNode->Name()).compare("Skeletons") == 0)
							{
								if (!mSkinningData)
									mSkinningData = std::make_unique<CEzmModelSkinningData>();

								LoadSkeleton(lChildNode->FirstChild(), lBoneIndexMap);
							}
							else if (std::string(lChildNode->Name()).compare("Animations") == 0)
							{
								if (!mSkinningData)
									mSkinningData = std::make_unique<CEzmModelSkinningData>();

								LoadAnimations(lChildNode, lBoneIndexMap);
							}

							lChildNode = lChildNode->NextSiblingElement();
						}
					}
				}
			}
		}

		return lResult;
	}

	EResult CEzmModel::LoadAnimations(tinyxml2::XMLNode * pAnimationsNode, Map<std::string, int32_t> &pBoneIndexMap)
	{
		EResult lResult = EResult::Success;		
		tinyxml2::XMLElement *lChildNode = pAnimationsNode->FirstChildElement("Animation");

		while (lChildNode != nullptr)
		{
			CEzmModelAnimation *lAnimation = new CEzmModelAnimation();
			UnorderedMap<std::string, Array<CVector3F>> lAnimPositions;
			UnorderedMap<std::string, Array<CQuaternion>> lAnimOrientations;

			uint32_t luiFrameCount = atol(lChildNode->Attribute("framecount"));
			float lfDuration = (float)atof(lChildNode->Attribute("duration"));
			lAnimation->mfDuration = lfDuration;

			tinyxml2::XMLElement *lAnimTrackNode = lChildNode->FirstChildElement("AnimTrack");

			while (lAnimTrackNode != nullptr)
			{
				std::string lstrBone = lAnimTrackNode->Attribute("name");
				
				std::string lstrLine;
				std::istringstream lVerticesData(lAnimTrackNode->GetText());

				while (std::getline(lVerticesData, lstrLine, ','))
				{
					std::string lstrToken(lstrLine);
					CStringUtil::Trim(lstrToken);

					if (lstrToken.empty())
						continue;

					Array<float> lFloatArray;
					StringToFloatArray(lstrToken, " ", lFloatArray);
					
					if (lFloatArray.size() > 0)
					{
						uint32_t luiIndex = 0;

						CVector3F lPosition;
						CQuaternion lOrientation;
						CVector3F lScale(1.0f);
												
						lPosition.X = lFloatArray[luiIndex++];
						lPosition.Y = lFloatArray[luiIndex++];						
						lPosition.Z = lFloatArray[luiIndex++];
												
						lOrientation.X = lFloatArray[luiIndex++];						
						lOrientation.Y = lFloatArray[luiIndex++];
						lOrientation.Z = lFloatArray[luiIndex++];
						lOrientation.W = lFloatArray[luiIndex++];

						lAnimPositions[lstrBone].push_back(lPosition);
						lAnimOrientations[lstrBone].push_back(lOrientation);
					}
				}

				lAnimTrackNode = lAnimTrackNode->NextSiblingElement("AnimTrack");
			}

			float lfIncDuraction = lfDuration / luiFrameCount;	
			for (const auto &lAnimPosition : lAnimPositions)
			{				
				const Array<CVector3F> &lPositions = lAnimPosition.second;
				const Array<CQuaternion> &lOrientations = lAnimOrientations[lAnimPosition.first];

				float lfTime = 0.0f;

				for (uint32_t lIdx = 0; lIdx < lAnimPosition.second.size(); lIdx++)
				{
					CEzmModelKeyFrame *lKeyFrame = new CEzmModelKeyFrame();
					lKeyFrame->mfTime = lfTime;
					lKeyFrame->muiBoneIndex = pBoneIndexMap[lAnimPosition.first];
					lKeyFrame->mPosition = lPositions[lIdx];
					lKeyFrame->mOrientation = lOrientations[lIdx];

					lAnimation->mKeyFrames.push_back(IModelKeyFramePtr(lKeyFrame));
					lfTime += lfIncDuraction;
				}
			}

			std::sort(lAnimation->mKeyFrames.begin(), lAnimation->mKeyFrames.end(),
				[](const IModelKeyFramePtr &pA, const IModelKeyFramePtr &pB) {
					if (pA->GetTime() > pB->GetTime())
						return false;
					else if (pA->GetTime() < pB->GetTime())
						return true;
					else if (pA->GetBoneIndex() > pB->GetBoneIndex())
						return false;
					else
						return true;
				}
			);

			static_cast<CEzmModelSkinningData*>(mSkinningData.get())->
				mAnimations.push_back(IModelAnimationPtr(lAnimation));

			lChildNode = lChildNode->NextSiblingElement("Animation");
		}

		return lResult;
	}

	EResult CEzmModel::LoadMaterials(tinyxml2::XMLNode * pMaterialsNode, const String &pstrPath)
	{
		EResult lResult = EResult::Success;
		uint32_t luiNumMaterials = atol(pMaterialsNode->ToElement()->Attribute("count"));

		if (luiNumMaterials > 0)
		{
			tinyxml2::XMLElement *lMaterialNode = pMaterialsNode->FirstChildElement();
			while (lMaterialNode != nullptr)
			{	
				String lstrName;
				ISystem::Get().ToString(lMaterialNode->Attribute("name"), lstrName);
				
				String lstrDiffuseMapName;
				String lstrDiffuseFullPath; 

				std::string lstrMetaData = lMaterialNode->ToElement()->Attribute("meta_data");				

				if (lstrMetaData.find_first_of("diffuse=") != std::string::npos)
				{					
					std::string lstrGarbage("%20");
					std::string lstrDiffuse = lstrMetaData.substr(lstrMetaData.find_first_of("diffuse=") + 
						std::string("diffuse=").length());

					if (lstrDiffuse.find(lstrGarbage) != std::string::npos)
					{
						lstrDiffuse = lstrDiffuse.substr(lstrGarbage.length());
						lstrDiffuse = lstrDiffuse.substr(0, lstrDiffuse.length() - lstrGarbage.length());
					}

					ISystem::Get().ToString(lstrDiffuse.c_str(), lstrDiffuseMapName);
					ISystem::Get().CombinePath(pstrPath, lstrDiffuseMapName, lstrDiffuseFullPath);
				}

				CEzmMaterial *lMaterial = new CEzmMaterial();
				lMaterial->mstrName = lstrName;
				lMaterial->mstrDiffuseMap = lstrDiffuseFullPath;
				mMaterials.push_back(IModelMaterialPtr(lMaterial));

				lMaterialNode = lMaterialNode->NextSiblingElement();
			}
		}

		return lResult;
	}

	EResult CEzmModel::LoadSkeleton(tinyxml2::XMLNode * pSkeletonNode, Map<std::string, int32_t> &pBoneIndexMap)
	{
		EResult lResult = EResult::Success;
		Array<IModelJointPtr> lJoints;

		tinyxml2::XMLElement *lChildNode = pSkeletonNode->FirstChildElement("Bone");
		uint32_t luiBoneCount = 0;

		while (lChildNode != nullptr)
		{
			CEzmModelJoint *lJoint = new CEzmModelJoint();

			std::string lstrName = lChildNode->Attribute("name");
			pBoneIndexMap[lstrName] = luiBoneCount;
			lJoint->miIndex = luiBoneCount;
			
			if (lChildNode->Attribute("parent") != nullptr)
			{
				std::string lstrParent = lChildNode->Attribute("parent");
				lJoint->miParent = pBoneIndexMap[lstrParent];
			}
			else
			{
				lJoint->miParent = -1;
			}

			Array<float> lFloatArray;
			CVector3F lPosition;
			CQuaternion lRotation;
			CVector3F lScale;

			std::string lstrData;
			std::string lstrPosition = lChildNode->Attribute("position");
			std::string lstrOrientation = lChildNode->Attribute("orientation");
			std::string lstrScale = lChildNode->Attribute("scale");

			lFloatArray.clear();
			StringToFloatArray(lstrPosition, " ", lFloatArray);

			lPosition.X = lFloatArray[0];			
			lPosition.Y = lFloatArray[1];			
			lPosition.Z = lFloatArray[2];

			lFloatArray.clear();
			StringToFloatArray(lstrOrientation, " ", lFloatArray);

			lRotation.X = lFloatArray[0];
			lRotation.Y = lFloatArray[1];			
			lRotation.Z = lFloatArray[2];			
			lRotation.W = lFloatArray[3];

			lFloatArray.clear();
			StringToFloatArray(lstrScale, " ", lFloatArray);
						
			lScale.X = lFloatArray[0];
			lScale.Y = lFloatArray[1];
			lScale.Z = lFloatArray[2];
	
			CMatrix4F lTransform = CMatrix4F::TranslationMatrix(lPosition) *
				lRotation.GetMatrix4() *
				CMatrix4F::ScaleMatrix(lScale);

			lJoint->mBindPose = lTransform;
			lJoints.push_back(IModelJointPtr(lJoint));

			lChildNode = lChildNode->NextSiblingElement("Bone");
			luiBoneCount++;
		}

		static_cast<CEzmModelSkinningData*>(mSkinningData.get())->
			mSkeleton = std::move(lJoints);
		
		return lResult;
	}

	EResult CEzmModel::LoadMeshes(tinyxml2::XMLNode * pMeshesNode)
	{
		EResult lResult = EResult::Success;
		uint32_t luiNumMeshes = atol(pMeshesNode->ToElement()->Attribute("count"));

		if (luiNumMeshes > 0)
		{
			CEzmModelMesh *lMesh = new CEzmModelMesh();
			tinyxml2::XMLElement *lMeshNode = pMeshesNode->FirstChildElement("Mesh");

			while (lMeshNode != nullptr)
			{
				LoadMesh(lMeshNode, lMesh);
				mMeshes.push_back(IModelMeshPtr(lMesh));

				lMeshNode = lMeshNode->FirstChildElement("Mesh");
			}
		}

		return lResult;
	}

	EResult CEzmModel::LoadMesh(tinyxml2::XMLNode * pMeshNode, CEzmModelMesh * pMesh)
	{
		EResult lResult = EResult::Success;

		uint32_t luiStartIndex = 0;
		tinyxml2::XMLElement *lChildNode = pMeshNode->FirstChildElement();

		while (lChildNode != nullptr)
		{
			if (std::string(lChildNode->Name()).compare("vertexbuffer") == 0)
			{
				bool lbHasNormal = false;
				bool lbHasTexCoord1 = false;
				bool lbHasTexCoord2 = false;
				bool lbHasTexCoord3 = false;
				bool lbHasBlendWeights = false;
				bool lbHasBlendIndices = false;
				
				std::string lstrSemantic = lChildNode->Attribute("semantic");
				std::istringstream lBuffer(lstrSemantic);
				std::string lstrLine;

				while (std::getline(lBuffer, lstrLine, ' '))
				{
					std::string lstrToken(lstrLine);
					CStringUtil::Trim(lstrToken);

					if (lstrToken.compare("normal") == 0)
						lbHasNormal = true;
					else if (lstrToken.compare("texcoord1") == 0)
						lbHasTexCoord1 = true;
					else if (lstrToken.compare("texcoord2") == 0)
						lbHasTexCoord2 = true;
					else if (lstrToken.compare("texcoord3") == 0)
						lbHasTexCoord3 = true;
					else if (lstrToken.compare("blendweights") == 0)
						lbHasBlendWeights = true;
					else if (lstrToken.compare("blendindices") == 0)
						lbHasBlendIndices = true;
				}

				std::istringstream lVerticesData(lChildNode->GetText());
				while (std::getline(lVerticesData, lstrLine, ','))
				{
					std::string lstrToken(lstrLine);
					CStringUtil::Trim(lstrToken);

					if (lstrToken.empty())
						continue;

					std::istringstream lVeretxData(lstrToken);
					std::string lstrData;

					uint32_t luiIndex = 0;
					Array<float> lFloatArray;

					CVector3F lPosition;
					CVector3F lNormal;
					CVector2F lTexCoord1;
					CVector2F lTexCoord2;
					CVector2F lTexCoord3;
					CVector4F lBlendWeights;
					CVector4F lBlendIndices;

					StringToFloatArray(lstrToken, " ", lFloatArray);

					lPosition.X = lFloatArray[luiIndex++];
					lPosition.Y = lFloatArray[luiIndex++];
					lPosition.Z = lFloatArray[luiIndex++];

					if (lbHasNormal)
					{						
						lNormal.X = lFloatArray[luiIndex++];
						lNormal.Y = lFloatArray[luiIndex++];						
						lNormal.Z = lFloatArray[luiIndex++];
					}

					if (lbHasTexCoord1)
					{						
						lTexCoord1.X = lFloatArray[luiIndex++];
						lTexCoord1.Y = 1.0f - lFloatArray[luiIndex++];
					}

					if (lbHasTexCoord2)
					{
						lTexCoord2.X = lFloatArray[luiIndex++];
						lTexCoord2.Y = 1.0f - lFloatArray[luiIndex++];
					}

					if (lbHasTexCoord3)
					{
						lTexCoord3.X = lFloatArray[luiIndex++];
						lTexCoord3.Y = 1.0f - lFloatArray[luiIndex++];
					}

					if (lbHasBlendWeights)
					{						
						lBlendWeights.X = lFloatArray[luiIndex++];
						lBlendWeights.Y = lFloatArray[luiIndex++];
						lBlendWeights.Z = lFloatArray[luiIndex++];
						lBlendWeights.W = lFloatArray[luiIndex++];
					}

					if (lbHasBlendIndices)
					{
						lBlendIndices.X = lFloatArray[luiIndex++];
						lBlendIndices.Y = lFloatArray[luiIndex++];
						lBlendIndices.Z = lFloatArray[luiIndex++];
						lBlendIndices.W = lFloatArray[luiIndex++];
					}
					
					pMesh->AddVertex(SModelVertex(lPosition,
						lNormal,
						lTexCoord1,
						lTexCoord2,
						lTexCoord3,
						lBlendIndices,
						lBlendWeights));					
				}
			}
			else if (std::string(lChildNode->Name()).compare("MeshSection") == 0)
			{
				CEzmModelMeshPart *lMeshPart = new CEzmModelMeshPart();

				LoadMeshPart(lChildNode, pMesh, lMeshPart, luiStartIndex);
				pMesh->mMeshParts.push_back(IModelMeshPartPtr(lMeshPart));

				luiStartIndex += lMeshPart->GetNumIndices();
			}

			lChildNode = lChildNode->NextSiblingElement();
		}

		return lResult;
	}

	EResult CEzmModel::LoadMeshPart(tinyxml2::XMLNode * pMeshPartNode, CEzmModelMesh * pMesh, 
		CEzmModelMeshPart * pMeshPart, uint32_t puiStartIndex)
	{
		EResult lResult = EResult::Success;

		String lstrMaterialName;
		std::string lstrMaterial = pMeshPartNode->ToElement()->Attribute("material");
		ISystem::Get().ToString(lstrMaterial.c_str(), lstrMaterialName);
		
		tinyxml2::XMLElement *lChildNode = pMeshPartNode->FirstChildElement("indexbuffer");
		if (lChildNode != nullptr)
		{			
			uint32_t luiNumIndices = 0;
			std::istringstream lIndicesData(lChildNode->GetText());
			std::string lstrLine;				

			while (std::getline(lIndicesData, lstrLine, ','))
			{
				std::string lstrToken(lstrLine);
				CStringUtil::Trim(lstrToken);

				if (lstrToken.empty())
					continue;

				Array<float> lFloatArray;
				StringToFloatArray(lstrToken, " ", lFloatArray);

				pMesh->AddFace((uint16_t)lFloatArray[0], 
					(uint16_t)lFloatArray[1], 
					(uint16_t)lFloatArray[2]);

				luiNumIndices += 3;
			}

			pMeshPart->muiStartIndex = puiStartIndex;
			pMeshPart->muiNumIndices = luiNumIndices;

			for (uint32_t lIdx = 0; lIdx < mMaterials.size(); lIdx++)
			{
				if (mMaterials[lIdx]->GetName().compare(lstrMaterialName) == 0)
				{
					pMeshPart->muiMaterialIndex = lIdx;
					break;
				}
			}			
		}

		return lResult;
	}

	CEzmModelLoader::CEzmModelLoader()
	{
	}

	CEzmModelLoader::~CEzmModelLoader()
	{
	}

	EModelLoaderType CEzmModelLoader::GetModelLoaderType() const
	{
		return EModelLoaderType::Ezm;
	}

	bool CEzmModelLoader::CanLoadModelType(EModelType pType)
	{
		return pType == EModelType::Ezm;
	}

	bool CEzmModelLoader::CanLoadModel(const String & pstrFilePath)
	{
		bool lbCanLoad = false;
		uint32_t luiDot = pstrFilePath.find_last_of(_T("."));

		if (luiDot != (uint32_t)String::npos)
		{
			String lstrExt = pstrFilePath.substr(luiDot);
			std::transform(lstrExt.begin(), lstrExt.end(), lstrExt.begin(), ::tolower);

			if (lstrExt == _T(".ezm"))
			{
				lbCanLoad = true;
			}
		}

		return lbCanLoad;
	}

	EResult CEzmModelLoader::Load(const String & pstrFilePath, IModel ** pModel)
	{
		EResult lResult = EResult::Success;

		CEzmModel *lModel = new CEzmModel();
		lResult = lModel->Create(pstrFilePath);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG(_T("CEzmModel Create failed: %s"), pstrFilePath.c_str());
		}
		else
		{
			*pModel = lModel;
		}

		return lResult;
	}
}