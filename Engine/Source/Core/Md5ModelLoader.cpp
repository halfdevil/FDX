
#include "Core/Md5ModelLoader.h"
#include "OS/GameLogger.h"
#include "OS/System.h"

namespace FDX
{
	static CMatrix4F ConversionMatrix = CMatrix4F(
		-1, 0, 0, 0,
		0, 0, 1, 0,
		0, 1, 0, 0,
		0, 0, 0, 1
	);

	uint32_t Split(const AsciiString &pstrString, const AsciiString &pstrDelim, Array<AsciiString> &pParts)
	{
		int32_t luiStart = 0;
		int32_t luiEnd = 0;

		for (uint32_t lIdx = 0; lIdx < pstrString.length(); lIdx++)
		{
			if (pstrDelim.find(pstrString[lIdx]) != AsciiString::npos)
			{
				pParts.push_back(AsciiString(pstrString, luiStart, lIdx - luiStart));
				luiStart = lIdx + 1;
			}
		}

		return pParts.size();
	}

	CMd5Parser::CMd5Parser()
		: miSeek(-1),
		miSeekData(0)
	{
	}

	EResult CMd5Parser::Parse(const AsciiString &pstrData)
	{
		EResult lResult = EResult::Success;

		if (Split(pstrData, " \r\n\t", mData) == 0)
		{
			LOG_DEBUG(_T("Invalid data in md5 file"));
			lResult = EResult::LoadFailed;
		}
		else
		{
			while (miSeekData < mData.size())
			{
				if (mData[miSeekData].length() > 0)
				{
					if (mData[miSeekData][0] == '/')
					{
						ParseComment();
					}
					else if (mData[miSeekData][0] == '"')
					{
						ParseString();
					}
					else if (isdigit(mData[miSeekData][0]) || mData[miSeekData][0] == '-')
					{
						ParseNumeric();
					}
					else if (mData[miSeekData][0] == '(')
					{
						ParseVector();
					}
					else if (isalpha(mData[miSeekData][0]))
					{
						ParseKey();
					}
					else
					{
						miSeekData++;
					}
				}
				else
				{
					miSeekData++;
				}
			}
		}

		return lResult;
	}

	void CMd5Parser::SeekToBegining()
	{
		miSeek = 0;
	}

	void CMd5Parser::SeekToKey(const char * pstrKey)
	{
		AsciiString lstrKey(pstrKey);
		std::transform(lstrKey.begin(), lstrKey.end(),
			lstrKey.begin(), ::tolower);

		miSeek++;

		while (miSeek < mKeys.size() && mKeys[miSeek].Name != lstrKey)
			miSeek++;
	}

	CVector2F CMd5Parser::GetVector2()
	{
		CVector2F lVec;

		if (miSeek < mKeys.size())
		{
			SMd5ParseKey &lParseKey = mKeys[miSeek];
			uint32_t luiSeekElement = lParseKey.SeekElement;

			if (luiSeekElement < lParseKey.Elements.size())
			{
				if (lParseKey.Elements[luiSeekElement].ValueType == EMd5ElementType::Vector2)
					lVec = lParseKey.Elements[luiSeekElement].ValueVector2;

				lParseKey.SeekElement++;
			}
		}

		return lVec;
	}

	CVector3F CMd5Parser::GetVector3()
	{
		CVector3F lVec;

		if (miSeek < mKeys.size())
		{
			SMd5ParseKey &lParseKey = mKeys[miSeek];
			uint32_t luiSeekElement = lParseKey.SeekElement;

			if (luiSeekElement < lParseKey.Elements.size())
			{
				if (lParseKey.Elements[luiSeekElement].ValueType == EMd5ElementType::Vector3)
					lVec = lParseKey.Elements[luiSeekElement].ValueVector3;

				lParseKey.SeekElement++;
			}
		}

		return lVec;
	}

	float CMd5Parser::GetNumeric()
	{
		float lfNumeric = 0.0f;

		if (miSeek < mKeys.size())
		{
			SMd5ParseKey &lParseKey = mKeys[miSeek];
			uint32_t luiSeekElement = lParseKey.SeekElement;

			if (luiSeekElement < lParseKey.Elements.size())
			{
				if (lParseKey.Elements[luiSeekElement].ValueType == EMd5ElementType::Numeric)
					lfNumeric = lParseKey.Elements[luiSeekElement].ValueNumeric;

				lParseKey.SeekElement++;
			}
		}

		return lfNumeric;
	}

	AsciiString CMd5Parser::GetString()
	{
		AsciiString lstrString;

		if (miSeek < mKeys.size())
		{
			SMd5ParseKey &lParseKey = mKeys[miSeek];
			uint32_t luiSeekElement = lParseKey.SeekElement;

			if (luiSeekElement < lParseKey.Elements.size())
			{
				if (lParseKey.Elements[luiSeekElement].ValueType == EMd5ElementType::String)
					lstrString = lParseKey.Elements[luiSeekElement].ValueString;

				lParseKey.SeekElement++;
			}
		}

		return lstrString;
	}

	bool CMd5Parser::IsEOFKey()
	{
		if (miSeek < mKeys.size())
		{
			SMd5ParseKey &lParseKey = mKeys[miSeek];
			uint32_t luiSeekElement = lParseKey.SeekElement;

			if (luiSeekElement < lParseKey.Elements.size())
				return false;
		}

		return true;
	}

	void CMd5Parser::ParseComment()
	{
		miSeekData++;

		while (miSeekData < mData.size() && (mData[miSeekData].length() != 0 || mData[miSeekData + 1].length() != 0))
			miSeekData++;

		miSeekData += 2;
	}

	void CMd5Parser::ParseVector()
	{
		if (mData[miSeekData + 3][0] == ')')
		{
			CVector2F lVec;
			miSeekData++;

			lVec.X = (float)atof(mData[miSeekData++].c_str());
			lVec.Y = (float)atof(mData[miSeekData++].c_str());

			miSeekData++;

			int32_t liLastKey = mKeys.size() - 1;
			if (liLastKey >= 0)
			{
				SMd5Element lElement;
				lElement.ValueType = EMd5ElementType::Vector2;
				lElement.ValueVector2 = lVec;

				mKeys[liLastKey].Elements.push_back(std::move(lElement));
			}
		}
		else
		{
			CVector3F lVec;
			miSeekData++;

			lVec.X = (float)atof(mData[miSeekData++].c_str());
			lVec.Y = (float)atof(mData[miSeekData++].c_str());
			lVec.Z = (float)atof(mData[miSeekData++].c_str());

			miSeekData++;

			int32_t liLastKey = mKeys.size() - 1;
			if (liLastKey >= 0)
			{
				SMd5Element lElement;
				lElement.ValueType = EMd5ElementType::Vector3;
				lElement.ValueVector3 = lVec;

				mKeys[liLastKey].Elements.push_back(std::move(lElement));
			}
		}
	}

	void CMd5Parser::ParseNumeric()
	{
		float lfNumeric = (float)atof(mData[miSeekData++].c_str());
		int32_t liLastKey = mKeys.size() - 1;

		if (liLastKey >= 0)
		{
			SMd5Element lElement;
			lElement.ValueType = EMd5ElementType::Numeric;
			lElement.ValueNumeric = lfNumeric;

			mKeys[liLastKey].Elements.push_back(std::move(lElement));
		}
	}

	void CMd5Parser::ParseKey()
	{
		AsciiString lstrName = mData[miSeekData++];
		std::transform(lstrName.begin(), lstrName.end(), lstrName.begin(), ::tolower);

		SMd5ParseKey lParseKey;
		lParseKey.SeekElement = 0;
		lParseKey.Name = lstrName;

		mKeys.push_back(std::move(lParseKey));
	}

	void CMd5Parser::ParseString()
	{
		AsciiString lstrString;

		if (mData[miSeekData].substr(1).find_last_of('"') >= 0)
		{
			lstrString = mData[miSeekData].substr(1,
				mData[miSeekData].length() - 2);
		}
		else
		{
			lstrString = mData[miSeekData++].substr(1);
			while (mData[miSeekData].find('"') == AsciiString::npos)
				lstrString += " " + mData[miSeekData++];

			lstrString += " " + mData[miSeekData].substr(1,
				mData[miSeekData].length() - 2);
		}

		int32_t liLastKey = mKeys.size() - 1;

		if (liLastKey >= 0)
		{
			SMd5Element lElement;
			lElement.ValueType = EMd5ElementType::String;
			lElement.ValueString = lstrString;

			mKeys[liLastKey].Elements.push_back(std::move(lElement));
		}

		miSeekData++;
	}

	EResult CMd5Model::Create(const String & pstrFilePath)
	{
		return EResult();
	}

	EResult CMd5Model::LoadMeshes(const String & pstrMd5MeshFilePath)
	{
		EResult lResult = EResult::Success;
		IFileStream *lStream = nullptr;

		lResult = IFileSystem::Get().GetStream(pstrMd5MeshFilePath, &lStream);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG(_T("IFileSystem::GetStream() failed for: %s"), pstrMd5MeshFilePath.c_str());
		}
		else
		{
			SmartPtr<char[]> lModelData = std::make_unique<char[]>(lStream->GetSize() + 1);
			memset(lModelData.get(), 0, lStream->GetSize());

			lResult = lStream->Read(lStream->GetSize(), lModelData.get());
			if (lResult != EResult::Success)
			{
				LOG_DEBUG(_T("Stream::Read() failed for: %s"), pstrMd5MeshFilePath.c_str());
			}
			else
			{
				auto lParser = std::make_unique<CMd5Parser>();

				lParser->Parse(AsciiString(lModelData.get()));
				lParser->SeekToKey("MD5Version");

				uint32_t luiVersion = (uint32_t)lParser->GetNumeric();
				if (luiVersion == 10)
				{
					lParser->SeekToKey("numjoints");
					muiNumJoints = (uint32_t)lParser->GetNumeric();

					lParser->SeekToKey("nummeshes");
					muiNumMeshes = (uint32_t)lParser->GetNumeric();

					lParser->SeekToKey("joints");
					for (uint32_t lIdx = 0; lIdx < muiNumJoints; lIdx++)
					{
						CMd5Parser::SJointInfo lJoint;

						lJoint.Name = lParser->GetString();
						lJoint.Parent = (int32_t)lParser->GetNumeric();
						lJoint.Position = lParser->GetVector3();

						CVector3F lOrientation = lParser->GetVector3();
						lJoint.Rotation.X = lOrientation.X;
						lJoint.Rotation.Y = lOrientation.Y;
						lJoint.Rotation.Z = lOrientation.Z;

						lJoint.Rotation.GenerateW();
						lJoint.Rotation.Normalize();

						mMd5Joints.push_back(std::move(lJoint));
					}

					for (uint32_t lIdx = 0; lIdx < mMd5Joints.size(); lIdx++)
						mMd5BoneMap[mMd5Joints[lIdx].Name] = lIdx;

					for (uint32_t lIdx = 0; lIdx < muiNumMeshes; lIdx++)
					{
						mMd5TrianglesProcessed.clear();

						lParser->SeekToKey("mesh");
						CMd5Parser::SMeshInfo lMesh;

						lParser->SeekToKey("shader");
						lMesh.Shader = lParser->GetString();

						lParser->SeekToKey("numverts");
						uint32_t luiNumVerts = (uint32_t)lParser->GetNumeric();

						for (uint32_t lJdx = 0; lJdx < luiNumVerts; lJdx++)
						{
							lParser->SeekToKey("vert");

							CMd5Parser::SVertexInfo lVertex;
							lVertex.VertexIndex = (uint32_t)lParser->GetNumeric();
							lVertex.TexCoord = lParser->GetVector2();
							lVertex.StartWeight = (uint32_t)lParser->GetNumeric();
							lVertex.WeightCount = (uint32_t)lParser->GetNumeric();

							lMesh.Vertices.push_back(std::move(lVertex));
						}

						lParser->SeekToKey("numtris");
						uint32_t luiNumTris = (uint32_t)lParser->GetNumeric();

						for (uint32_t lJdx = 0; lJdx < luiNumTris; lJdx++)
						{
							lParser->SeekToKey("tri");

							CMd5Parser::STriangleInfo lTriangle;
							lTriangle.TriangleIndex = (uint32_t)lParser->GetNumeric();
							lTriangle.VertexIndices.push_back((uint16_t)lParser->GetNumeric());
							lTriangle.VertexIndices.push_back((uint16_t)lParser->GetNumeric());
							lTriangle.VertexIndices.push_back((uint16_t)lParser->GetNumeric());

							lMesh.Triangles.push_back(std::move(lTriangle));
						}

						lParser->SeekToKey("numweights");
						uint32_t luiNumWeights = (uint32_t)lParser->GetNumeric();

						for (uint32_t lJdx = 0; lJdx < luiNumWeights; lJdx++)
						{
							lParser->SeekToKey("weight");

							CMd5Parser::SWeightInfo lWeight;
							lWeight.WeightIndex = (uint32_t)lParser->GetNumeric();
							lWeight.JointIndex = (uint32_t)lParser->GetNumeric();
							lWeight.Bias = lParser->GetNumeric();
							lWeight.Position = lParser->GetVector3();

							lMesh.Weights.push_back(std::move(lWeight));
						}

						CreateMesh(lMesh);
					}

					if (muiNumJoints > 0)
					{
						Array<IModelJointPtr> lJoints(mMd5Joints.size());
						for (uint32_t lIdx = 0; lIdx < mMd5Joints.size(); lIdx++)
						{
							auto lJoint = std::make_unique<CMd5ModelJoint>();
							lJoint->miParent = mMd5Joints[lIdx].Parent;
							lJoint->miIndex = lIdx;

							CMatrix4F lTransform = CMatrix4F::TranslationMatrix(mMd5Joints[lIdx].Position) *
								mMd5Joints[lIdx].Rotation.GetMatrix4();

							if (lJoint->miParent == -1)
							{
								lJoint->mBindPose = ConversionMatrix * lTransform;
							}
							else
							{
								lJoint->mBindPose = lTransform;
							}

							lJoints[lIdx] = std::move(lJoint);
						}

						mSkinningData = std::make_unique<CMd5ModelSkinningData>();
						dynamic_cast<CMd5ModelSkinningData*>(mSkinningData.get())->
							mSkeleton = std::move(lJoints);
					}
				}
			}
		}
	}

	EResult CMd5Model::LoadAnimations(const Array<String>& pstrMd5AnimFilePaths)
	{
		EResult lResult = EResult::Success;

		for (const auto &lAnimFile : pstrMd5AnimFilePaths)
		{
			IFileStream *lStream = nullptr;

			lResult = IFileSystem::Get().GetStream(lAnimFile, &lStream);
			if (lResult != EResult::Success)
			{
				LOG_DEBUG(_T("IFileSystem::GetStream() failed for: %s"), lAnimFile.c_str());
			}
			else
			{
				SmartPtr<char[]> lAnimData = std::make_unique<char[]>(lStream->GetSize() + 1);
				memset(lAnimData.get(), 0, lStream->GetSize());

				lResult = lStream->Read(lStream->GetSize(), lAnimData.get());
				if (lResult != EResult::Success)
				{
					LOG_DEBUG(_T("Stream::Read() failed for: %s"), lAnimFile.c_str());
				}
				else
				{
					mMd5Hierarchies.clear();
					mMd5AxisAlignedBoxes.clear();
					mMd5BaseFrames.clear();
					mMd5Frames.clear();

					auto lParser = std::make_unique<CMd5Parser>();

					lParser->Parse(AsciiString(lAnimData.get()));
					lParser->SeekToKey("md5version");

					uint32_t luiVersion = (uint32_t)lParser->GetNumeric();
					if (luiVersion == 10)
					{
						lParser->SeekToKey("numframes");
						muiNumFrames = (uint32_t)lParser->GetNumeric();

						lParser->SeekToKey("numjoints");
						muiNumJoints = (uint32_t)lParser->GetNumeric();

						lParser->SeekToKey("framerate");
						muiFrameRate = (uint32_t)lParser->GetNumeric();

						lParser->SeekToKey("numanimatedcomponents");
						muiNumAnimatedComponents = (uint32_t)lParser->GetNumeric();

						lParser->SeekToKey("hierarchy");
						for (uint32_t lIdx = 0; lIdx < muiNumJoints; lIdx++)
						{
							CMd5Parser::SHierarchyInfo lHierarchy;
							lHierarchy.Name = lParser->GetString();
							lHierarchy.Parent = (int32_t)lParser->GetNumeric();
							lHierarchy.StartIndex = (uint32_t)lParser->GetNumeric();
							lHierarchy.Flags = (uint32_t)lParser->GetNumeric();

							mMd5Hierarchies.push_back(std::move(lHierarchy));
						}

						lParser->SeekToKey("bounds");
						for (uint32_t lIdx = 0; lIdx < muiNumFrames; lIdx++)
						{
							CAxisAlignedBox lAabb;
							lAabb.Min = lParser->GetVector3();
							lAabb.Max = lParser->GetVector3();

							mMd5AxisAlignedBoxes.push_back(std::move(lAabb));
						}

						lParser->SeekToKey("baseframe");
						for (uint32_t lIdx = 0; lIdx < muiNumJoints; lIdx++)
						{
							CMd5Parser::SBaseFrame lBaseFrame;
							lBaseFrame.Position = lParser->GetVector3();
							lBaseFrame.Orientation = lParser->GetVector3();

							mMd5BaseFrames.push_back(std::move(lBaseFrame));
						}

						for (uint32_t lIdx = 0; lIdx < muiNumFrames; lIdx++)
						{
							Array<float> lFrames;

							lParser->SeekToKey("frame");
							uint32_t luiFrameNum = (uint32_t)lParser->GetNumeric();

							while (!lParser->IsEOFKey())
								lFrames.push_back(lParser->GetNumeric());

							mMd5Frames.push_back(std::move(lFrames));
						}
					}

					if (mSkinningData != nullptr)
					{
						auto lstrName = lAnimFile;
						if (lstrName.find_last_of('/') != std::string::npos)
						{
							lstrName = lstrName.substr(
								lstrName.find_last_of('/') + 1);
						}
						else if (lstrName.find_last_of('\\') != std::string::npos)
						{
							lstrName = lstrName.substr(
								lstrName.find_last_of('\\') + 1);
						}

						CreateAnimation(lstrName);
					}
				}
			}
		}

		return lResult;
	}

	void CMd5Model::CreateMesh(CMd5Parser::SMeshInfo & pMd5Mesh)
	{
		EResult lResult = EResult::Success;
		AsciiString lstrDiffuseMap = pMd5Mesh.Shader;

		if (lstrDiffuseMap.find_last_of('/') != std::string::npos)
		{
			lstrDiffuseMap = lstrDiffuseMap.substr(
				lstrDiffuseMap.find_last_of('/') + 1);
		}
		else if (lstrDiffuseMap.find_last_of('\\') != std::string::npos)
		{
			lstrDiffuseMap = lstrDiffuseMap.substr(
				lstrDiffuseMap.find_last_of('\\') + 1);
		}

		if (lstrDiffuseMap.find_last_of('.') == std::string::npos)
			lstrDiffuseMap.append(".tga");

		CMd5Parser::SMaterialInfo lMaterialInfo;
		lMaterialInfo.DiffuseTexture = lstrDiffuseMap;
		CreateMaterial(lMaterialInfo);

		Array<SModelVertex> lVertices(pMd5Mesh.Vertices.size());
		uint32_t luiIndex = 0;

		for (const auto &lVertex : pMd5Mesh.Vertices)
		{
			CVector3F lPosition;

			for (uint32_t lIdx = 0; lIdx < lVertex.WeightCount; lIdx++)
			{
				CMd5Parser::SWeightInfo &lWeight = pMd5Mesh.Weights[lVertex.StartWeight + lIdx];
				CMd5Parser::SJointInfo &lJoint = mMd5Joints[lWeight.JointIndex];
				CVector3F lWv = lJoint.Rotation.Rotate(lWeight.Position);

				CMatrix4F lTransform = CMatrix4F::TranslationMatrix(lJoint.Position) *
					lJoint.Rotation.GetMatrix4();

				lTransform = ConversionMatrix * lTransform;
				lPosition += lTransform.Transform(lWeight.Position) * lWeight.Bias;
			}

			lVertices[luiIndex].Position = lPosition;
			lVertices[luiIndex].TexCoord1 = lVertex.TexCoord;

			luiIndex++;
		}

		CalculateNormals(pMd5Mesh, lVertices);

		Array<uint16_t> lIndices;
		for (uint32_t lIdx = 0; lIdx < pMd5Mesh.Triangles.size(); lIdx++)
		{
			if (CheckCreateTriangle(pMd5Mesh, lIdx))
			{
				mMd5TrianglesProcessed.push_back(lIdx);

				CMd5Parser::STriangleInfo &lTriangle = pMd5Mesh.Triangles[lIdx];
				for (uint32_t lJdx = 0; lJdx < 3; lJdx++)
				{
					CMd5Parser::SVertexInfo &lVertex = pMd5Mesh.Vertices[lTriangle.VertexIndices[lJdx]];
					Array<CMd5Parser::SBoneWeight> lWeights;

					GetBoneWeights(pMd5Mesh, lVertex, lWeights);

					if (lWeights[0].BoneName.length() != 0)
						lVertices[lTriangle.VertexIndices[lJdx]].BlendIndices.X = (float)lWeights[0].JointIndex;

					if (lWeights[1].BoneName.length() != 0)
						lVertices[lTriangle.VertexIndices[lJdx]].BlendIndices.Y = (float)lWeights[1].JointIndex;

					if (lWeights[2].BoneName.length() != 0)
						lVertices[lTriangle.VertexIndices[lJdx]].BlendIndices.Z = (float)lWeights[2].JointIndex;

					if (lWeights[3].BoneName.length() != 0)
						lVertices[lTriangle.VertexIndices[lJdx]].BlendIndices.W = (float)lWeights[3].JointIndex;

					lVertices[lTriangle.VertexIndices[lJdx]].BlendWeights.X = lWeights[0].Weight;
					lVertices[lTriangle.VertexIndices[lJdx]].BlendWeights.Y = lWeights[1].Weight;
					lVertices[lTriangle.VertexIndices[lJdx]].BlendWeights.Z = lWeights[2].Weight;
					lVertices[lTriangle.VertexIndices[lJdx]].BlendWeights.W = lWeights[3].Weight;
				}

				lIndices.push_back(lTriangle.VertexIndices[2]);
				lIndices.push_back(lTriangle.VertexIndices[1]);
				lIndices.push_back(lTriangle.VertexIndices[0]);
			}
		}

		Array<uint16_t> lBoneMap(mMd5BoneMap.size());
		for (uint32_t lIdx = 0; lIdx < mMd5Joints.size(); lIdx++)
		{
			if (mMd5BoneMap.find(mMd5Joints[lIdx].Name) != mMd5BoneMap.end())
			{
				lBoneMap[mMd5BoneMap[mMd5Joints[lIdx].Name]] = lIdx;
			}
		}

		auto lMeshPart = std::make_unique<CMd5ModelMeshPart>();
		lMeshPart->muiMaterialIndex = mMaterials.size() - 1;
		lMeshPart->muiStartIndex = 0;
		lMeshPart->muiNumIndices = lIndices.size();

		auto lMesh = std::make_unique<CMd5ModelMesh>();
		lMesh->mVertices = std::move(lVertices);
		lMesh->mIndices = std::move(lIndices);
		lMesh->mBoneMap = std::move(lBoneMap);
		lMesh->mMeshParts.push_back(std::move(lMeshPart));
		mMeshes.push_back(std::move(lMesh));

		if (pMd5Mesh.Triangles.size() > mMd5TrianglesProcessed.size())
		{
			CreateMesh(pMd5Mesh);
		}
	}

	void CMd5Model::CreateMaterial(CMd5Parser::SMaterialInfo & pMd5Material)
	{
		auto lMaterial = std::make_unique<CMd5Material>();

		ISystem::Get().ToString(pMd5Material.DiffuseTexture.c_str(), lMaterial->mstrDiffuseMap);
		ISystem::Get().ToString(pMd5Material.NormalTexture.c_str(), lMaterial->mstrNormalMap);
		ISystem::Get().ToString(pMd5Material.SpecularTexture.c_str(), lMaterial->mstrSpecularMap);

		mMaterials.push_back(std::move(lMaterial));
	}

	void CMd5Model::CreateAnimation(const String & pstrName)
	{
		auto lAnimation = std::make_unique<CMd5ModelAnimation>();
		lAnimation->mstrName = pstrName;
		lAnimation->mfDuration = (float)(muiFrameRate * muiNumFrames) / 1000.0f;

		Array<IModelKeyFramePtr> lKeyFrames;
		for (uint32_t lIdx = 0; lIdx < muiNumFrames; lIdx++)
			CreateSkeletonByFrame(lIdx, lKeyFrames);

		lAnimation->mKeyFrames = std::move(lKeyFrames);
		dynamic_cast<CMd5ModelSkinningData*>(mSkinningData.get())->
			mAnimations.push_back(std::move(lAnimation));
	}

	void CMd5Model::CreateSkeletonByFrame(uint32_t puiFrameIndex, Array<IModelKeyFramePtr>& pKeyFrames)
	{
		const auto &lInfos = mMd5Frames[puiFrameIndex];

		for (uint32_t lIdx = 0; lIdx < mMd5Hierarchies.size(); lIdx++)
		{
			CMd5Parser::SBaseFrame &lBaseFrame = mMd5BaseFrames[lIdx];
			CVector3F lPosition = lBaseFrame.Position;
			CQuaternion lOrientation;

			lOrientation.X = lBaseFrame.Orientation.X;
			lOrientation.Y = lBaseFrame.Orientation.Y;
			lOrientation.Z = lBaseFrame.Orientation.Z;

			CMd5Parser::SHierarchyInfo &lHierarchy = mMd5Hierarchies[lIdx];
			uint32_t luiIndexFlag = 0;

			if ((lHierarchy.Flags & 1) == 1)
			{
				lPosition.X = lInfos[lHierarchy.StartIndex + luiIndexFlag];
				luiIndexFlag++;
			}

			if ((lHierarchy.Flags & 2) == 2)
			{
				lPosition.Y = lInfos[lHierarchy.StartIndex + luiIndexFlag];
				luiIndexFlag++;
			}

			if ((lHierarchy.Flags & 4) == 4)
			{
				lPosition.Z = lInfos[lHierarchy.StartIndex + luiIndexFlag];
				luiIndexFlag++;
			}

			if ((lHierarchy.Flags & 8) == 8)
			{
				lOrientation.X = lInfos[lHierarchy.StartIndex + luiIndexFlag];
				luiIndexFlag++;
			}

			if ((lHierarchy.Flags & 16) == 16)
			{
				lOrientation.Y = lInfos[lHierarchy.StartIndex + luiIndexFlag];
				luiIndexFlag++;
			}

			if ((lHierarchy.Flags & 32) == 32)
			{
				lOrientation.Z = lInfos[lHierarchy.StartIndex + luiIndexFlag];
				luiIndexFlag++;
			}

			lOrientation.GenerateW();
			lOrientation.Normalize();

			auto lKeyFrame = std::make_unique<CMd5ModelKeyFrame>();
			lKeyFrame->muiBoneIndex = lIdx;
			lKeyFrame->mfTime = (float)(muiFrameRate * puiFrameIndex) / 1000.0f;
			lKeyFrame->mOrientation = lOrientation;
			lKeyFrame->mPosition = lPosition;
			
			pKeyFrames.push_back(std::move(lKeyFrame));
		}
	}

	void CMd5Model::GetBoneNamesByVertex(CMd5Parser::SMeshInfo & pMd5Mesh, 
		const CMd5Parser::SVertexInfo & pVertex, Array<AsciiString>& pBoneNames)
	{
		Array<CMd5Parser::SBoneWeight> lBoneWeights;
		GetBoneWeights(pMd5Mesh, pVertex, lBoneWeights);

		for (uint32_t lIdx = 0; lIdx < 4; lIdx++)
			pBoneNames.push_back(lBoneWeights[lIdx].BoneName);
	}

	void CMd5Model::GetBoneWeights(CMd5Parser::SMeshInfo & pMd5Mesh, const CMd5Parser::SVertexInfo & pVertexInfo, 
		Array<CMd5Parser::SBoneWeight>& pWeights)
	{
		for (uint32_t lIdx = 0; lIdx < pVertexInfo.WeightCount; lIdx++)
		{
			const auto &lWeight = pMd5Mesh.Weights[pVertexInfo.StartWeight + lIdx];
			const auto &lJoint = mMd5Joints[lWeight.JointIndex];

			CMd5Parser::SBoneWeight lBoneWeight;
			lBoneWeight.BoneName = lJoint.Name;
			lBoneWeight.JointIndex = lWeight.JointIndex;
			lBoneWeight.Weight = lWeight.Bias;

			pWeights.push_back(std::move(lBoneWeight));
		}

		std::sort(pWeights.begin(), pWeights.end(),
			[](const CMd5Parser::SBoneWeight &pA, const CMd5Parser::SBoneWeight &pB)
			{
				if (pA.Weight < pB.Weight)
					return true;

				return false;
			}
		);

		NormalizeWeights(pWeights, 4);
	}

	void CMd5Model::NormalizeWeights(Array<CMd5Parser::SBoneWeight>& pWeights, uint32_t puiMaxWeights)
	{
		if (pWeights.size() < puiMaxWeights)
		{
			uint32_t luiSize = pWeights.size();

			for (uint32_t lIdx = 0; lIdx < puiMaxWeights - luiSize; lIdx++)
			{
				CMd5Parser::SBoneWeight lWeight;
				lWeight.JointIndex = -1;
				lWeight.Weight = 0.0f;

				pWeights.push_back(std::move(lWeight));
			}
		}

		pWeights.resize(puiMaxWeights);
		float lfTotalWeight = 0.0f;

		for (const auto &lWeight : pWeights)
			lfTotalWeight += lWeight.Weight;

		for (auto &lWeight : pWeights)
			lWeight.Weight /= lfTotalWeight;
	}

	void CMd5Model::CalculateNormals(CMd5Parser::SMeshInfo & pMd5Mesh, Array<SModelVertex>& pVertices)
	{
		for (const auto &lTriangle : pMd5Mesh.Triangles)
		{
			uint16_t lIdx1 = lTriangle.VertexIndices[0];
			uint16_t lIdx2 = lTriangle.VertexIndices[1];
			uint16_t lIdx3 = lTriangle.VertexIndices[2];

			CVector3F lEdge1 = pVertices[lIdx1].Position - pVertices[lIdx2].Position;
			CVector3F lEdge2 = pVertices[lIdx3].Position - pVertices[lIdx2].Position;
			CVector3F lNormal = lEdge1.Cross(lEdge2);

			pVertices[lIdx1].Normal += lNormal;
			pVertices[lIdx2].Normal += lNormal;
			pVertices[lIdx3].Normal += lNormal;
		}
	}

	bool CMd5Model::CheckCreateTriangle(CMd5Parser::SMeshInfo & pMd5Mesh, uint32_t puiTriangleIndex)
	{
		if (std::find(mMd5TrianglesProcessed.begin(), mMd5TrianglesProcessed.end(),
			puiTriangleIndex) != mMd5TrianglesProcessed.end())
		{
			return false;
		}

		return true;
	}
}