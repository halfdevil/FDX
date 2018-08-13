#include "Core/DaeModelLoader.h"
#include "Math/Quaternion.h"
#include "FS/FileSystem.h"
#include "OS/System.h"
#include "OS/GameLogger.h"
#include "Core/StringUtil.h"

namespace FDX
{
	struct SDaeFloatArray
	{
		uint32_t Stride;
		Array<float> Array;
	};

	struct SDaeUInt16Array
	{
		Array<uint16_t> Array;
	};

	struct SDaeStringArray
	{
		Array<std::string> Array;
	};

	struct SDaeSubMesh
	{
		uint32_t MaterialIndex;
		UnorderedMap<std::string, uint32_t> Semantics;
		Array<uint16_t> Array;
	};

	struct SDaeVertexWeights
	{
		uint32_t Index;
		float Weight;
	};

	static CMatrix4F lConversionMatrix = CMatrix4F(
		-1, 0, 0, 0,
		0, 0, 1, 0,
		0, 1, 0, 0,
		0, 0, 0, 1
	);

	void GetWeightsFromArray(Array<SDaeVertexWeights> &pWeights, CVector4F &pOutWeights, CVector4F &pOutIndices)
	{
		float lfTotalWeight = 0.0f;

		std::sort(pWeights.begin(), pWeights.end(),
			[](const SDaeVertexWeights &pA, const SDaeVertexWeights &pB) { return pA.Weight > pB.Weight; });

		if (pWeights.size() < 4)
		{
			uint32_t luiSize = pWeights.size();
			for (uint32_t lIdx = 0; lIdx < 4 - luiSize; lIdx++)
			{
				SDaeVertexWeights lWeights;
				lWeights.Index = 0;
				lWeights.Weight = 0.0f;

				pWeights.push_back(std::move(lWeights));
			}
		}

		for (uint32_t lIdx = 0; lIdx < 4; lIdx++)
		{
			pOutWeights[lIdx] = pWeights[lIdx].Weight;
			pOutIndices[lIdx] = (float)pWeights[lIdx].Index;
			lfTotalWeight += pWeights[lIdx].Weight;
		}

		for (uint32_t lIdx = 0; lIdx < 4; lIdx++)
		{
			pOutWeights[lIdx] /= lfTotalWeight;
		}
	}

	CDaeMaterial::CDaeMaterial()
		: mfSpecularPower(0.0f)
	{
	}

	CDaeMaterial::~CDaeMaterial()
	{
	}

	CDaeModelMeshPart::CDaeModelMeshPart()
		: muiMaterialIndex(0),
		muiStartIndex(0),
		muiNumIndices(0)
	{
	}

	CDaeModelMeshPart::~CDaeModelMeshPart()
	{
	}

	CDaeModelKeyFrame::CDaeModelKeyFrame()
		: mfTime(0.0f),
		muiBoneIndex(0)
	{
	}

	CDaeModelKeyFrame::~CDaeModelKeyFrame()
	{
	}

	CDaeModelJoint::CDaeModelJoint()
	{
	}

	CDaeModelJoint::~CDaeModelJoint()
	{
	}

	CDaeModelAnimation::CDaeModelAnimation()
		: mfDuration(0.0f)
	{
	}

	CDaeModelAnimation::~CDaeModelAnimation()
	{
	}

	CDaeModelSkinningData::CDaeModelSkinningData()
	{
	}

	CDaeModelSkinningData::~CDaeModelSkinningData()
	{
	}

	CDaeModelMesh::CDaeModelMesh()
	{
	}

	CDaeModelMesh::~CDaeModelMesh()
	{
	}

	CDaeModel::CDaeModel()
		: mSkinningData(nullptr)
	{
	}

	CDaeModel::~CDaeModel()
	{
	}

	EResult CDaeModel::Create(const String & pstrFilePath)
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
					String lstrDirectory;
					ISystem::Get().GetDirectory(pstrFilePath, lstrDirectory);

					auto lNode = lDocument.FirstChildElement("COLLADA");

					LoadMaterials(lNode, lstrDirectory);
					LoadMeshes(lNode);
					LoadSkeleton(lNode);
					LoadAnimations(lNode);

					if (mJoints.size() != 0)
					{
						static_cast<CDaeModelSkinningData*>(mSkinningData.get())->
							mSkeleton = std::move(mJoints);
					}
				}
			}
		}

		return lResult;
	}

	void CDaeModel::LoadAnimations(tinyxml2::XMLNode * pNode)
	{
		UnorderedMap<std::string, UnorderedMap<std::string, std::string>> lSemantics;
		UnorderedMap<std::string, UnorderedMap<std::string, SDaeFloatArray>> lFloatArrays;
		UnorderedMap<std::string, std::string> lChannels;

		auto lAnimationsNode = pNode->FirstChildElement("library_animations");
		auto lAnimationNode = lAnimationsNode->FirstChildElement("animation");

		while (lAnimationNode != nullptr)
		{
			auto lSourceNode = lAnimationNode->FirstChildElement("source");
			auto lChannelNode = lAnimationNode->FirstChildElement("channel");

			while (lChannelNode != nullptr)
			{
				std::string lstrSource = lChannelNode->Attribute("source");
				lstrSource = lstrSource.substr(1);

				std::string lstrTarget = lChannelNode->Attribute("target");
				if (lstrTarget.find_first_of('/') != std::string::npos)
					lstrTarget = lstrTarget.substr(0, lstrTarget.find_first_of('/'));

				lChannels[lstrSource] = lstrTarget;
				lChannelNode = lChannelNode->NextSiblingElement("channel");
			}

			auto lSamplerNode = lAnimationNode->FirstChildElement("sampler");
			auto lInputNode = lSamplerNode->FirstChildElement("input");
			auto lSamplerId = lSamplerNode->Attribute("id");

			while (lInputNode != nullptr)
			{
				std::string lstrSemantic = lInputNode->Attribute("semantic");
				std::string lstrSource = lInputNode->Attribute("source");

				lstrSource = lstrSource.substr(1);
				lSemantics[lSamplerId][lstrSemantic] = lstrSource;

				lInputNode = lInputNode->NextSiblingElement("input");
			}

			while (lSourceNode != nullptr)
			{
				std::string lstrId = lSourceNode->Attribute("id");
				auto lFloatArrayNode = lSourceNode->FirstChildElement("float_array");

				if (lFloatArrayNode != nullptr)
				{
					std::string lstrFloatArray = lFloatArrayNode->GetText();
					std::istringstream lFloatArrayStream(lstrFloatArray);
					std::string lstrFloat;
					SDaeFloatArray lFloatArray;

					while (std::getline(lFloatArrayStream, lstrFloat, ' '))
						lFloatArray.Array.push_back((float)atof(lstrFloat.c_str()));

					lFloatArray.Stride = atoi(lSourceNode->FirstChildElement("technique_common")->
						FirstChildElement("accessor")->Attribute("stride"));
					lFloatArrays[lSamplerId][lstrId] = std::move(lFloatArray);
				}

				lSourceNode = lSourceNode->NextSiblingElement("source");
			}

			lAnimationNode = lAnimationNode->NextSiblingElement("animation");
		}

		UnorderedMap<std::string, Array<CVector3F>> lJointPositions;
		UnorderedMap<std::string, Array<CQuaternion>> lJointOrientations;

		Array<float> lTimes;
		float lfDuration = 0.0f;

		for (auto &lAnimation : lSemantics)
		{
			auto &lInput = lFloatArrays[lAnimation.first][lAnimation.second["INPUT"]];
			auto &lOutput = lFloatArrays[lAnimation.first][lAnimation.second["OUTPUT"]];

			if (lInput.Array.size() > 1)
			{
				lfDuration = lInput.Array[1] - lInput.Array[0];
				lfDuration *= lInput.Array.size();
			}

			if (lTimes.size() == 0)
			{
				for (const auto &lTime : lInput.Array)
					lTimes.push_back(lTime);
			}

			for (uint32_t lIdx = 0; lIdx < lTimes.size(); lIdx++)
			{
				CMatrix4F lTransform = CMatrix4F(lOutput.Array.data() +
					lOutput.Stride * lIdx);

				lTransform = CMatrix4F::TransposeMatrix(lTransform);

				if (mJointIndices[lChannels[lAnimation.first]] == 0)
					lTransform = lConversionMatrix * lTransform;

				CVector3F lPosition = CVector3F(lTransform.M2[3][0],
					lTransform.M2[3][1], lTransform.M2[3][2]);
				CQuaternion lOrientation = CQuaternion::FromMatrix(lTransform);

				lJointPositions[lAnimation.first].push_back(lPosition);
				lJointOrientations[lAnimation.first].push_back(lOrientation);
			}
		}

		Array<IModelAnimationPtr> lAnimations;
		Array<IModelKeyFramePtr> lKeyFrames;

		CDaeModelAnimation *lAnimation = new CDaeModelAnimation();
		lAnimation->mfDuration = lfDuration;

		for (uint32_t lIdx = 0; lIdx < lTimes.size(); lIdx++)
		{
			Array<CVector3F> lOrderedJointPositions;
			Array<CQuaternion> lOrderedJointOrientations;

			lOrderedJointPositions.resize(mJointIndices.size());
			lOrderedJointOrientations.resize(mJointIndices.size());

			for (const auto &lIt : lJointPositions)
			{
				const auto &lPositions = lIt.second;
				const auto &lOrientations = lJointOrientations[lIt.first];

				lOrderedJointPositions[mJointIndices[lChannels[lIt.first]]] =
					lPositions[lIdx];

				lOrderedJointOrientations[mJointIndices[lChannels[lIt.first]]] =
					lOrientations[lIdx];
			}

			for (uint32_t lJdx = 0; lJdx < lOrderedJointPositions.size(); lJdx++)
			{
				CDaeModelKeyFrame *lKeyFrame = new CDaeModelKeyFrame();
				lKeyFrame->mfTime = lTimes[lIdx];
				lKeyFrame->muiBoneIndex = lJdx;
				lKeyFrame->mPosition = lOrderedJointPositions[lJdx];
				lKeyFrame->mOrientation = lOrderedJointOrientations[lJdx];

				lKeyFrames.push_back(IModelKeyFramePtr(lKeyFrame));
			}			
		}

		lAnimation->mKeyFrames = std::move(lKeyFrames);
		lAnimations.push_back(IModelAnimationPtr(lAnimation));

		if (!mSkinningData)
			mSkinningData = std::make_unique<CDaeModelSkinningData>();
		
		static_cast<CDaeModelSkinningData*>(mSkinningData.get())->
			mAnimations = std::move(lAnimations);
	}

	void CDaeModel::LoadSkeleton(tinyxml2::XMLNode * pNode)
	{
		if (mJointIndices.size() != 0)
		{
			auto lVisualScenesNode = pNode->FirstChildElement("library_visual_scenes");

			if (lVisualScenesNode != nullptr)
			{
				auto lVisualSceneNode = lVisualScenesNode->FirstChildElement("visual_scene");

				if (lVisualSceneNode != nullptr)
				{
					mJoints.resize(mJointIndices.size());
					LoadJoints(lVisualSceneNode, -1);
				}
			}
		}
	}

	void CDaeModel::LoadJoints(tinyxml2::XMLElement * pNode, int32_t piParentIndex)
	{
		auto lChildNode = pNode->FirstChildElement("node");
		while (lChildNode != nullptr)
		{
			std::string lstrType = lChildNode->Attribute("type");
			if (lstrType.compare("JOINT") == 0)
			{
				std::string lstrId = lChildNode->Attribute("id");
				std::string lstrFloatArray = lChildNode->FirstChildElement("matrix")->GetText();

				std::istringstream lFloatArrayStream(lstrFloatArray);
				std::string lstrFloat;
				Array<float> lFloatArray;

				while (std::getline(lFloatArrayStream, lstrFloat, ' '))
					lFloatArray.push_back((float)atof(lstrFloat.c_str()));

				CDaeModelJoint *lJoint = new CDaeModelJoint();
				CMatrix4F lTransform = CMatrix4F::TransposeMatrix(CMatrix4F(lFloatArray.data()));

				auto lIt = mJointIndices.find(lstrId);
				if (lIt != mJointIndices.end())
				{
					if (mJointIndices[lstrId] == 0)
						lTransform = lConversionMatrix * lTransform;

					lJoint->miIndex = mJointIndices[lstrId];
					lJoint->mBindPose = lTransform;
					lJoint->miParent = piParentIndex;

					LoadJoints(lChildNode, lJoint->miIndex);
					mJoints[lJoint->miIndex] = IModelJointPtr(lJoint);
				}
			}
			else
			{
				LoadJoints(lChildNode, piParentIndex);
			}

			lChildNode = lChildNode->NextSiblingElement("node");
		}
	}

	void CDaeModel::LoadMaterials(tinyxml2::XMLNode * pNode, const String & pstrPath)
	{
		UnorderedMap<std::string, std::string> lImages;
		UnorderedMap<std::string, std::string> lEffects;

		auto lImagesNode = pNode->FirstChildElement("library_images");
		if (lImagesNode != nullptr)
		{
			auto lImageNode = lImagesNode->FirstChildElement("image");
			while (lImageNode != nullptr)
			{
				auto lInitFrom = lImageNode->FirstChildElement("init_from");
				if (lInitFrom)
					lImages[lImageNode->Attribute("id")] = lInitFrom->GetText();

				lImageNode = lImageNode->NextSiblingElement("image");
			}
		}

		auto lEffectsNode = pNode->FirstChildElement("library_effects");
		auto lEffectNode = lEffectsNode->FirstChildElement("effect");

		while (lEffectNode != nullptr)
		{
			auto lCommonNode = lEffectNode->FirstChildElement("profile_COMMON");
			auto lTechniqueNode = lCommonNode->FirstChildElement("technique");

			if (lTechniqueNode != nullptr)
			{
				auto lPhongNode = lTechniqueNode->FirstChildElement("phong");
				auto lDiffuseNode = lPhongNode->FirstChildElement("diffuse");

				if (lDiffuseNode)
				{
					auto lTexture = lDiffuseNode->FirstChildElement("texture");
					if (lTexture != nullptr)
					{
						auto lTextureSampler = lTexture->Attribute("texture");
						auto lNewParamNode = lCommonNode->FirstChildElement("newparam");

						UnorderedMap<std::string, std::string> lSurfaceParams;
						UnorderedMap<std::string, std::string> lSamplerParams;

						while (lNewParamNode != nullptr)
						{
							auto lChildNode = lNewParamNode->FirstChildElement("surface");
							if (lChildNode != nullptr)
							{
								lSurfaceParams[lNewParamNode->Attribute("sid")] =
									lChildNode->FirstChildElement("init_from")->GetText();
							}

							lChildNode = lNewParamNode->FirstChildElement("sampler2D");
							if (lChildNode != nullptr)
							{
								lSamplerParams[lNewParamNode->Attribute("sid")] =
									lChildNode->FirstChildElement("source")->GetText();
							}

							lNewParamNode = lNewParamNode->NextSiblingElement("newparam");
						}

						auto lIt = lSamplerParams.find(lTextureSampler);
						if (lIt != lSamplerParams.end())
						{
							auto lIt2 = lSurfaceParams.find((*lIt).second);
							if (lIt2 != lSurfaceParams.end())
							{
								lEffects[lEffectNode->Attribute("id")] = lImages[(*lIt2).second];
							}
						}
					}
				}
			}

			lEffectNode = lEffectNode->NextSiblingElement("effect");
		}

		auto lMaterialsNode = pNode->FirstChildElement("library_materials");
		auto lMaterialNode = lMaterialsNode->FirstChildElement("material");

		while (lMaterialNode != nullptr)
		{
			auto lInstanceEffectNode = lMaterialNode->FirstChildElement("instance_effect");
			if (lInstanceEffectNode != nullptr)
			{
				std::string lstrEffect = lInstanceEffectNode->Attribute("url");
				lstrEffect = lstrEffect.substr(1);

				auto lIt = lEffects.find(lstrEffect);
				if (lIt != lEffects.end())
				{
					String lstrTexturePath;
					String lstrTextureFile;
					String lstrDiffuseFilePath;

					ISystem::Get().ToString((*lIt).second.c_str(), lstrTexturePath);
					ISystem::Get().GetFileName(lstrTexturePath, lstrTextureFile);
					ISystem::Get().CombinePath(pstrPath, lstrTextureFile, lstrDiffuseFilePath);

					CDaeMaterial *lMaterial = new CDaeMaterial();
					lMaterial->mstrDiffuseMap = lstrDiffuseFilePath;
					mMaterials.push_back(IModelMaterialPtr(lMaterial));
					mMaterialIndices[lMaterialNode->Attribute("id")] = mMaterials.size() - 1;
				}
			}

			lMaterialNode = lMaterialNode->NextSiblingElement("material");
		}
	}

	void CDaeModel::LoadMeshes(tinyxml2::XMLNode * pNode)
	{
		UnorderedMap<std::string, SDaeFloatArray> lFloatSources;
		UnorderedMap<std::string, SDaeStringArray> lStringSources;
		UnorderedMap<std::string, std::string> lSemantics;

		Array<SDaeSubMesh> lSubMeshes;
		Array<uint32_t> lBlendIndicesCounts;
		Array<float> lBlendIndices;
		Array<SModelVertex> lVertices;
		Array<SDaeUInt16Array> lVertexIndices;

		auto lGeometriesNode = pNode->FirstChildElement("library_geometries");
		auto lGeometryNode = lGeometriesNode->FirstChildElement("geometry");
		auto lMeshNode = lGeometryNode->FirstChildElement("mesh");
		auto lSourceNode = lMeshNode->FirstChildElement("source");

		while (lSourceNode != nullptr)
		{
			SDaeFloatArray lFloatArray;

			std::string lstrFloatArray = lSourceNode->FirstChildElement("float_array")->GetText();
			std::istringstream lFloatArrayStream(lstrFloatArray);
			std::string lstrFloat;

			while (std::getline(lFloatArrayStream, lstrFloat, ' '))
				lFloatArray.Array.push_back((float)atof(lstrFloat.c_str()));

			lFloatArray.Stride = atoi(lSourceNode->FirstChildElement("technique_common")->
				FirstChildElement("accessor")->Attribute("stride"));
			lFloatSources[lSourceNode->Attribute("id")] = std::move(lFloatArray);

			lSourceNode = lSourceNode->NextSiblingElement("source");
		}

		auto lVerticesNode = lMeshNode->FirstChildElement("vertices");
		auto lInputNode = lVerticesNode->FirstChildElement("input");

		while (lInputNode != nullptr)
		{
			std::string lstrSemantic = lInputNode->Attribute("semantic");
			std::string lstrSource = lInputNode->Attribute("source");

			lstrSource = lstrSource.substr(1);
			lSemantics[lstrSemantic] = lstrSource;

			lInputNode = lInputNode->NextSiblingElement("input");
		}

		auto lTrianglesNode = lMeshNode->FirstChildElement("triangles");
		while (lTrianglesNode != nullptr)
		{
			auto lInputNode = lTrianglesNode->FirstChildElement("input");
			while (lInputNode != nullptr)
			{
				std::string lstrSemantic = lInputNode->Attribute("semantic");
				std::string lstrSource = lInputNode->Attribute("source");

				lstrSource = lstrSource.substr(1);
				lSemantics[lstrSemantic] = lstrSource;

				lInputNode = lInputNode->NextSiblingElement("input");
			}

			auto lPNode = lTrianglesNode->FirstChildElement("p");
			std::string lstrIntArray = lPNode->GetText();
			std::istringstream lIntArray(lstrIntArray);
			std::string lstrInt;

			SDaeSubMesh lSubMesh;
			lSubMesh.MaterialIndex = mMaterialIndices[lTrianglesNode->Attribute("material")];

			SDaeUInt16Array lTempArray;
			while (std::getline(lIntArray, lstrInt, ' '))
				lTempArray.Array.push_back((uint16_t)atoi(lstrInt.c_str()));

			lVertexIndices.push_back(std::move(lTempArray));
			lSubMeshes.push_back(std::move(lSubMesh));
			lTrianglesNode = lTrianglesNode->NextSiblingElement("triangles");
		}

		auto lPolyListNode = lMeshNode->FirstChildElement("polylist");
		while (lPolyListNode != nullptr)
		{
			uint32_t luiIndicesStride = 0;
			UnorderedMap<std::string, uint32_t> lVertexSemantics;

			auto lInputNode = lPolyListNode->FirstChildElement("input");
			while (lInputNode != nullptr)
			{
				std::string lstrSemantic = lInputNode->Attribute("semantic");
				std::string lstrSource = lInputNode->Attribute("source");
				uint32_t luiOffset = atoi(lInputNode->Attribute("offset"));

				lstrSource = lstrSource.substr(1);
				lSemantics[lstrSemantic] = lstrSource;
				lVertexSemantics[lstrSemantic] = luiOffset;

				luiIndicesStride++;
				lInputNode = lInputNode->NextSiblingElement("input");
			}

			auto lPNode = lPolyListNode->FirstChildElement("p");
			std::string lstrIntArray = lPNode->GetText();
			std::istringstream lIntArray(lstrIntArray);
			std::string lstrInt;

			SDaeSubMesh lSubMesh;
			lSubMesh.Semantics = std::move(lVertexSemantics);
			lSubMesh.MaterialIndex = mMaterialIndices[lPolyListNode->Attribute("material")];

			SDaeUInt16Array lTempArray;
			while (std::getline(lIntArray, lstrInt, ' '))
				lTempArray.Array.push_back((uint16_t)atoi(lstrInt.c_str()));

			lVertexIndices.push_back(std::move(lTempArray));
			lSubMeshes.push_back(std::move(lSubMesh));
			lPolyListNode = lPolyListNode->NextSiblingElement("polylist");
		}

		auto lControllersNode = pNode->FirstChildElement("library_controllers");
		auto lControllerNode = lControllersNode->FirstChildElement("controller");
		auto lSkinNode = lControllerNode->FirstChildElement("skin");
		lSourceNode = lSkinNode->FirstChildElement("source");

		while (lSourceNode != nullptr)
		{
			auto lChildNode = lSourceNode->FirstChildElement("Name_array");
			if (lChildNode != nullptr)
			{
				std::string lstrNameArray = lChildNode->GetText();
				std::istringstream lNameArray(lstrNameArray);
				std::string lstrName;

				SDaeStringArray lStringArray;
				while (std::getline(lNameArray, lstrName, ' '))
					lStringArray.Array.push_back(lstrName);

				lStringSources[lSourceNode->Attribute("id")] = std::move(lStringArray);
			}
			else
			{
				lChildNode = lSourceNode->FirstChildElement("float_array");
				if (lChildNode != nullptr)
				{
					std::string lstrFloatArray = lChildNode->GetText();
					std::istringstream lFloatArrayStream(lstrFloatArray);
					std::string lstrFloat;

					SDaeFloatArray lFloatArray;
					while (std::getline(lFloatArrayStream, lstrFloat, ' '))
						lFloatArray.Array.push_back((float)atof(lstrFloat.c_str()));

					lFloatArray.Stride = atoi(lSourceNode->FirstChildElement("technique_common")->
						FirstChildElement("accessor")->Attribute("stride"));
					lFloatSources[lSourceNode->Attribute("id")] = std::move(lFloatArray);
				}
			}

			lSourceNode = lSourceNode->NextSiblingElement("source");
		}

		auto lJointsNode = lSkinNode->FirstChildElement("joints");
		if (lJointsNode != nullptr)
		{
			auto lInputNode = lJointsNode->FirstChildElement("input");
			while (lInputNode != nullptr)
			{
				std::string lstrSemantic = lInputNode->Attribute("semantic");
				std::string lstrSource = lInputNode->Attribute("source");

				lstrSource = lstrSource.substr(1);
				lSemantics[lstrSemantic] = lstrSource;

				lInputNode = lInputNode->NextSiblingElement("input");
			}
		}

		auto lVertexWeightsNode = lSkinNode->FirstChildElement("vertex_weights");
		if (lVertexWeightsNode != nullptr)
		{
			auto lInputNode = lVertexWeightsNode->FirstChildElement("input");
			while (lInputNode != nullptr)
			{
				std::string lstrSemantic = lInputNode->Attribute("semantic");
				std::string lstrSource = lInputNode->Attribute("source");

				lstrSource = lstrSource.substr(1);
				lSemantics[lstrSemantic] = lstrSource;

				lInputNode = lInputNode->NextSiblingElement("input");
			}

			auto lVCountNode = lVertexWeightsNode->FirstChildElement("vcount");
			if (lVCountNode != nullptr)
			{
				std::string lstrVCount = lVCountNode->GetText();
				std::istringstream lVCountStream(lstrVCount);
				std::string lstrVIndex;

				while (std::getline(lVCountStream, lstrVIndex, ' '))
					lBlendIndicesCounts.push_back(atoi(lstrVIndex.c_str()));
			}

			auto lVNode = lVertexWeightsNode->FirstChildElement("v");
			if (lVNode != nullptr)
			{
				std::string lstrV = lVNode->GetText();
				std::istringstream lVStream(lstrV);
				std::string lstrBlendIndex;

				while (std::getline(lVStream, lstrBlendIndex, ' '))
					lBlendIndices.push_back((float)atof(lstrBlendIndex.c_str()));
			}
		}

		SDaeFloatArray &lPositions = lFloatSources[lSemantics["POSITION"]];
		SDaeFloatArray &lNormals = lFloatSources[lSemantics["NORMAL"]];
		SDaeFloatArray &lTexCoords = lFloatSources[lSemantics["TEXCOORD"]];
		SDaeFloatArray &lBlendWeights = lFloatSources[lSemantics["WEIGHT"]];

		Array<CVector3F> lVertexPositions;
		Array<CVector3F> lVertexNormals;
		Array<CVector2F> lVertexTexCoords;

		uint32_t luiNumVertices = lPositions.Array.size() / lPositions.Stride;
		lVertexPositions.resize(luiNumVertices);

		for (uint32_t lIdx = 0; lIdx < luiNumVertices; lIdx++)
		{
			lVertexPositions[lIdx].X = lPositions.Array[lIdx * lPositions.Stride + 0];
			lVertexPositions[lIdx].Y = lPositions.Array[lIdx * lPositions.Stride + 1];
			lVertexPositions[lIdx].Z = lPositions.Array[lIdx * lPositions.Stride + 2];
			lVertexPositions[lIdx] = lConversionMatrix.Transform(lVertexPositions[lIdx]);
		}

		uint32_t luiNumNormals = lNormals.Array.size() / lNormals.Stride;
		lVertexNormals.resize(luiNumNormals);

		for (uint32_t lIdx = 0; lIdx < luiNumNormals; lIdx++)
		{
			lVertexNormals[lIdx].X = lNormals.Array[lIdx * lNormals.Stride + 0];
			lVertexNormals[lIdx].Y = lNormals.Array[lIdx * lNormals.Stride + 1];
			lVertexNormals[lIdx].Z = lNormals.Array[lIdx * lNormals.Stride + 2];
		}

		uint32_t luiNumTexCoords = lTexCoords.Array.size() / lTexCoords.Stride;
		lVertexTexCoords.resize(luiNumTexCoords);

		for (uint32_t lIdx = 0; lIdx < luiNumTexCoords; lIdx++)
		{
			lVertexTexCoords[lIdx].X = lTexCoords.Array[lIdx * lTexCoords.Stride + 0];
			lVertexTexCoords[lIdx].Y = 1.0f - lTexCoords.Array[lIdx * lTexCoords.Stride + 1];
		}

		uint32_t luiStartIndex = 0;
		lVertices.resize(luiNumVertices);

		for (uint32_t lIdx = 0; lIdx < luiNumVertices; lIdx++)
		{
			Array<SDaeVertexWeights> lWeights;
			for (uint32_t lJdx = 0; lJdx < lBlendIndicesCounts[lIdx]; lJdx++)
			{
				uint32_t luiJointIndex = (uint32_t)lBlendIndices[luiStartIndex + 0];
				uint32_t luiWeightIndex = (uint32_t)lBlendIndices[luiStartIndex + 1];

				SDaeVertexWeights lWeight;
				lWeight.Index = luiJointIndex;
				lWeight.Weight = lBlendWeights.Array[luiWeightIndex];

				lWeights.push_back(std::move(lWeight));
				luiStartIndex += 2;
			}

			GetWeightsFromArray(lWeights, lVertices[lIdx].BlendWeights,
				lVertices[lIdx].BlendIndices);
		}

		UnorderedMap<uint32_t, Array<uint32_t>> lDuplicateVertices;

		for (uint32_t lJdx = 0; lJdx < lSubMeshes.size(); lJdx++)
		{
			SDaeSubMesh &lSubMesh = lSubMeshes[lJdx];
			uint32_t luiSize = lVertexIndices[lJdx].Array.size();
			uint32_t luiNumSemantics = lSubMesh.Semantics.size();

			for (uint32_t lIdx = 0; lIdx < luiSize; lIdx += luiNumSemantics)
			{
				uint16_t luiVertexIndex = lVertexIndices[lJdx].Array[lIdx + lSubMesh.Semantics["VERTEX"]];
				uint16_t luiNormalIndex = lVertexIndices[lJdx].Array[lIdx + lSubMesh.Semantics["NORMAL"]];
				uint16_t luiTexCoordIndex = lVertexIndices[lJdx].Array[lIdx + lSubMesh.Semantics["TEXCOORD"]];

				CVector3F &lPosition = lVertexPositions[luiVertexIndex];
				CVector3F &lNormal = lVertexNormals[luiNormalIndex];
				CVector2F &lTexCoord = lVertexTexCoords[luiTexCoordIndex];

				auto lIt = lDuplicateVertices.find(luiVertexIndex);
				if (lIt != lDuplicateVertices.end())
				{
					auto &lDuplicates = (*lIt).second;
					uint32_t lKdx = 0;

					for (lKdx = 0; lKdx < lDuplicates.size(); lKdx++)
					{
						SModelVertex &lDupVertex = lVertices[lDuplicates[lKdx]];
						if (lDupVertex.TexCoord1 == lTexCoord)
							break;
					}

					if (lKdx < lDuplicates.size())
					{
						lSubMesh.Array.push_back((uint16_t)lDuplicates[lKdx]);
					}
					else
					{
						SModelVertex lVertex;
						lVertex.Position = lPosition;
						lVertex.Normal = lNormal;
						lVertex.TexCoord1 = lTexCoord;
						lVertex.BlendIndices = lVertices[luiVertexIndex].BlendIndices;
						lVertex.BlendWeights = lVertices[luiVertexIndex].BlendWeights;

						lVertices.push_back(lVertex);
						lDuplicates.push_back(lVertices.size() - 1);
						lSubMesh.Array.push_back((uint16_t)lVertices.size() - 1);
					}
				}
				else
				{
					lVertices[luiVertexIndex].Position = lPosition;
					lVertices[luiVertexIndex].Normal = lNormal;
					lVertices[luiVertexIndex].TexCoord1 = lTexCoord;

					Array<uint32_t> lVertexIndices;
					lVertexIndices.push_back(luiVertexIndex);

					lDuplicateVertices[luiVertexIndex] = std::move(lVertexIndices);
					lSubMesh.Array.push_back(luiVertexIndex);
				}
			}
		}

		Array<uint16_t> lIndices;
		Array<IModelMeshPartPtr> lMeshParts;
		luiStartIndex = 0;

		for (const auto &lSubMesh : lSubMeshes)
		{
			lIndices.insert(lIndices.end(), lSubMesh.Array.begin(), 
				lSubMesh.Array.end());

			CDaeModelMeshPart *lMeshPart = new CDaeModelMeshPart();
			lMeshPart->muiMaterialIndex = lSubMesh.MaterialIndex;
			lMeshPart->muiStartIndex = luiStartIndex;
			lMeshPart->muiNumIndices = lSubMesh.Array.size();

			lMeshParts.push_back(IModelMeshPartPtr(lMeshPart));
			luiStartIndex += lSubMesh.Array.size();
		}

		auto lIt = lSemantics.find("JOINT");
		if (lIt != lSemantics.end())
		{
			const auto &lJoints = lStringSources[(*lIt).second];
			for (uint32_t lIdx = 0; lIdx < lJoints.Array.size(); lIdx++)
				mJointIndices[lJoints.Array[lIdx]] = lIdx;
		}

		CDaeModelMesh *lMesh = new CDaeModelMesh();
		lMesh->mVertices = std::move(lVertices);
		lMesh->mIndices = std::move(lIndices);
		lMesh->mMeshParts = std::move(lMeshParts);

		mMeshes.push_back(IModelMeshPtr(lMesh));
	}

	void CDaeModel::Reset()
	{

	}

	CDaeModelLoader::CDaeModelLoader()
	{
	}

	CDaeModelLoader::~CDaeModelLoader()
	{
	}

	EModelLoaderType CDaeModelLoader::GetModelLoaderType() const
	{
		return EModelLoaderType::Dae;
	}

	bool CDaeModelLoader::CanLoadModelType(EModelType pType)
	{
		return pType == EModelType::Dae;
	}

	bool CDaeModelLoader::CanLoadModel(const String & pstrFilePath)
	{
		bool lbCanLoad = false;
		uint32_t luiDot = pstrFilePath.find_last_of(_T("."));

		if (luiDot != (uint32_t)String::npos)
		{
			String lstrExt = pstrFilePath.substr(luiDot);
			std::transform(lstrExt.begin(), lstrExt.end(), lstrExt.begin(), ::tolower);

			if (lstrExt == _T(".dae"))
			{
				lbCanLoad = true;
			}
		}

		return lbCanLoad;
	}

	EResult CDaeModelLoader::Load(const String & pstrFilePath, IModel ** pModel)
	{
		EResult lResult = EResult::Success;

		CDaeModel *lModel = new CDaeModel();
		lResult = lModel->Create(pstrFilePath);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG(_T("CDaeModel Create failed: %s"), pstrFilePath.c_str());
		}
		else
		{
			*pModel = lModel;
		}

		return lResult;
	}
}