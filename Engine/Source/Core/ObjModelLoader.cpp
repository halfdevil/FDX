
#include "Core/ObjModelLoader.h"
#include "FS/FileSystem.h"
#include "OS/System.h"
#include "OS/GameLogger.h"
#include "Core/StringUtil.h"

namespace FDX
{
	CObjMaterial::CObjMaterial()
	{
	}

	CObjMaterial::~CObjMaterial()
	{
	}

	CObjModelMeshPart::CObjModelMeshPart()
	{
	}

	CObjModelMeshPart::~CObjModelMeshPart()
	{
	}

	CObjModelMesh::CObjModelMesh()
	{
	}

	CObjModelMesh::~CObjModelMesh()
	{
	}

	int32_t CObjModelMesh::AddVertex(const SModelVertex & pVertex)
	{
		int32_t liIndex = -1;

		for (uint32_t lIdx = 0; lIdx < mVertices.size(); lIdx++)
		{
			const auto &lVertex = mVertices[lIdx];

			if (lVertex.Position.X == pVertex.Position.X &&
				lVertex.Position.Y == pVertex.Position.Y &&
				lVertex.Position.Z == pVertex.Position.Z)
			{
				liIndex = lIdx;
				break;
			}
		}

		if (liIndex == -1)
		{
			mVertices.push_back(pVertex);
			liIndex = static_cast<int32_t>(mVertices.size()) - 1;
		}

		return liIndex;
	}

	void CObjModelMesh::AddFace(uint16_t puiI1, uint16_t puiI2, uint16_t puiI3)
	{
		mIndices.push_back(puiI1);
		mIndices.push_back(puiI2);
		mIndices.push_back(puiI3);
	}

	CObjModel::CObjModel()
	{
	}

	CObjModel::~CObjModel()
	{
	}

	EResult CObjModel::Create(const String & pstrFilePath)
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
				Array<CVector3F> lPositions;
				Array<CVector3F> lNormals;
				Array<CVector2F> lTexCoords;				
				Map<uint32_t, Array<SObjFace>> lFaces;

				int32_t liMatIndex = -1;
				CObjModelMesh *lObjMesh = nullptr;

				bool lbHasNormals = false;
				bool lbHasTexCoords = false;
				bool lbPopulateMesh = false;

				std::string lstrFileData(reinterpret_cast<const char*>(lModelData.get()));
				std::istringstream lBuffer(lstrFileData);
				std::string lstrLine;
				std::string lstrLast;

				while (std::getline(lBuffer, lstrLine))
				{
					CStringUtil::Trim(lstrLine);

					if (lstrLine.find_first_of("#") != std::string::npos)
						continue;

					int32_t liSpaceIdx = lstrLine.find_first_of(" ");
					std::string lstrPrefix = lstrLine.substr(0, liSpaceIdx);
					CStringUtil::Trim(lstrPrefix);

					if (lstrPrefix.length() == 0)
						continue;

					if (lstrPrefix.compare("g") == 0 || lstrPrefix.compare("o") == 0)
					{
						String lstrName;

						lstrLine = lstrLine.substr(liSpaceIdx + 1);
						ISystem::Get().ToString(lstrLine.c_str(), lstrName);

						lObjMesh = new CObjModelMesh();
						lObjMesh->mstrName = lstrName;

						mMeshes.push_back(IModelMeshPtr(lObjMesh));
					}

					if (lstrPrefix.compare("vt") == 0)
					{
						lstrLine = lstrLine.substr(liSpaceIdx + 1);

						CVector2F lTexCoord;
						std::istringstream lstrTexCoord(lstrLine);

						lstrTexCoord >> lTexCoord.X;
						lstrTexCoord >> lTexCoord.Y;

						lTexCoords.push_back(lTexCoord);
						lbHasTexCoords = true;
					}

					if (lstrPrefix.compare("v") == 0)
					{
						if (lbPopulateMesh)
						{
							if (lObjMesh != nullptr)
							{
								PopulateModelMesh(lObjMesh, lPositions,
									lNormals, lTexCoords, lFaces);
							}

							lbPopulateMesh = false;
							lFaces.clear();
						}

						lstrLine = lstrLine.substr(liSpaceIdx + 1);

						CVector3F lPosition;
						std::istringstream lstrPosition(lstrLine);

						lstrPosition >> lPosition.X;
						lstrPosition >> lPosition.Y;
						lstrPosition >> lPosition.Z;

						lPositions.push_back(lPosition);
					}

					if (lstrPrefix.compare("vn") == 0)
					{
						lstrLine = lstrLine.substr(liSpaceIdx + 1);

						CVector3F lNormal;
						std::istringstream lstrNormal(lstrLine);

						lstrNormal >> lNormal.X;
						lstrNormal >> lNormal.Y;
						lstrNormal >> lNormal.Z;

						lNormals.push_back(lNormal);
						lbHasNormals = true;
					}

					if (lstrPrefix.compare("f") == 0)
					{
						lstrLine = lstrLine.substr(liSpaceIdx + 1);

						if (liMatIndex != -1)
							AddFace(lstrLine, lFaces[liMatIndex], liSpaceIdx, lbHasTexCoords, lbHasNormals);

						lbPopulateMesh = true;
					}

					if (lstrPrefix.compare("mtllib") == 0)
					{
						String lstrDirectory;
						String lstrFileName;
						String lstrFilePath;

						lstrLine = lstrLine.substr(lstrLine.find_first_of("mtllib") + 7);
						ISystem::Get().ToString(lstrLine.c_str(), lstrFileName);

						ISystem::Get().GetDirectory(pstrFilePath, lstrDirectory);
						ISystem::Get().CombinePath(lstrDirectory, lstrFileName, lstrFilePath);

						ReadMaterialLibrary(lstrFilePath);
					}

					if (lstrPrefix.compare("usemtl") == 0)
					{
						std::string lstrTmp = lstrLine.substr(liSpaceIdx + 1);
						int32_t liIndex = -1;

						for (uint32_t lIdx = 0; lIdx < mMaterials.size(); lIdx++)
						{
							String lstrMaterial;
							ISystem::Get().ToString(lstrTmp.c_str(), lstrMaterial);

							if (mMaterials[lIdx]->GetName().compare(lstrMaterial) == 0)
							{
								liIndex = lIdx;
								break;
							}
						}

						liMatIndex = liIndex;
					}

					lstrLast = lstrPrefix;
				}
								
				if (lObjMesh != nullptr)
				{
					PopulateModelMesh(lObjMesh, lPositions,
						lNormals, lTexCoords, lFaces);
				}
			}
		}

		return lResult;
	}

	EResult CObjModel::AddFace(const std::string & pstrLine, Array<SObjFace>& pFaces,
		int32_t piSpaceIndex, bool pbHasTexCoords, bool pbHasNormals)
	{
		EResult lResult = EResult::Success;

		SObjFace lFace;
		int32_t liStart = 0;
		int32_t liCount = 1;

		std::string lstrFaceData;
		std::string lstrNormalData;
		std::string lstrTexCoordData;
		std::string lstrLine2;

		piSpaceIndex = pstrLine.find_first_of(" ", liStart + 1);
		while (piSpaceIndex != -1)
		{
			lstrLine2 = pstrLine.substr(liStart, piSpaceIndex - liStart);

			int32_t liFirstSlash = lstrLine2.find("/");
			int32_t liSecondSlash = lstrLine2.find("/", liFirstSlash + 1);

			lstrFaceData.append(lstrLine2.substr(0, liFirstSlash));
			lstrFaceData.append(" ");

			if (pbHasTexCoords)
			{
				lstrTexCoordData.append(lstrLine2.substr(liFirstSlash + 1,
					(liSecondSlash - liFirstSlash) - 1));
				lstrTexCoordData.append(" ");
			}

			if (pbHasNormals)
			{
				lstrNormalData.append(lstrLine2.substr(liSecondSlash + 1));
				lstrNormalData.append(" ");
			}

			liStart = piSpaceIndex;
			piSpaceIndex = pstrLine.find_first_of(" ", liStart + 1);
			liCount++;
		}

		lstrLine2 = pstrLine.substr(pstrLine.find_last_of(" "));

		int32_t liFirstSlash = lstrLine2.find("/");
		int32_t liSecondSlash = lstrLine2.find("/", liFirstSlash + 1);

		lstrFaceData.append(lstrLine2.substr(0, liFirstSlash));

		if (pbHasTexCoords)
		{
			lstrTexCoordData.append(lstrLine2.substr(liFirstSlash + 1,
				(liSecondSlash - liFirstSlash) - 1));
		}

		if (pbHasNormals)
		{
			lstrNormalData.append(lstrLine2.substr(liSecondSlash + 1));
		}

		std::istringstream lFaceData(lstrFaceData);
		std::istringstream lNormalData(lstrNormalData);
		std::istringstream lTexCoordData(lstrTexCoordData);

		lFaceData >> lFace.Px;
		lFaceData >> lFace.Py;
		lFaceData >> lFace.Pz;
		lFace.Px -= 1;
		lFace.Py -= 1;
		lFace.Pz -= 1;

		if (!lstrNormalData.empty())
		{
			lNormalData >> lFace.Nx;
			lNormalData >> lFace.Ny;
			lNormalData >> lFace.Nz;
			lFace.Nx -= 1;
			lFace.Ny -= 1;
			lFace.Nz -= 1;
			lFace.HasNormals = true;
		}

		if (!lstrTexCoordData.empty())
		{
			lTexCoordData >> lFace.Tx;
			lTexCoordData >> lFace.Ty;
			lTexCoordData >> lFace.Tz;
			lFace.Tx -= 1;
			lFace.Ty -= 1;
			lFace.Tz -= 1;
			lFace.HasTexCoords = true;
		}

		pFaces.push_back(lFace);
		if (liCount == 4)
		{
			uint16_t liTmpP = 0;
			uint16_t liTmpN = 0;
			uint16_t liTmpT = 0;

			lFaceData >> liTmpP;
			lNormalData >> liTmpN;
			lTexCoordData >> liTmpT;

			liTmpP -= 1;
			lFace.Py = lFace.Pz;
			lFace.Pz = liTmpP;

			liTmpN -= 1;
			lFace.Ny = lFace.Nz;
			lFace.Nz = liTmpN;

			liTmpT -= 1;
			lFace.Ty = lFace.Tz;
			lFace.Tz = liTmpT;

			pFaces.push_back(lFace);
		}

		return lResult;
	}

	EResult CObjModel::ReadMaterialLibrary(const String & pstrFilePath)
	{
		EResult lResult = EResult::Success;
		IFileStream *lStream = nullptr;

		lResult = IFileSystem::Get().GetStream(pstrFilePath, &lStream);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG(_T("ObjModel mtllib GetStream failed: %s"), pstrFilePath.c_str());
		}
		else
		{
			SmartPtr<uint8_t[]> lMatData = std::make_unique<uint8_t[]>(lStream->GetSize() + 1);
			memset(lMatData.get(), 0, lStream->GetSize());

			lResult = lStream->Read(lStream->GetSize(), lMatData.get());

			if (lResult != EResult::Success)
			{
				LOG_DEBUG(_T("ObjModel mtllib read from stream failed: %s"), pstrFilePath.c_str());
			}
			else
			{
				std::istringstream lstrBuffer(reinterpret_cast<const char*>(lMatData.get()));
				std::string lstrLine;
				CObjMaterial *lMaterial;

				while (std::getline(lstrBuffer, lstrLine))
				{
					CStringUtil::Trim(lstrLine);

					if (lstrLine.find_first_of("#") != std::string::npos)
						continue;

					if (lstrLine.length() == 0)
						continue;

					int32_t liSpaceIdx = lstrLine.find_first_of(" ");
					std::string lstrPrefix = lstrLine.substr(0, liSpaceIdx);
					CStringUtil::Trim(lstrPrefix);

					if (lstrPrefix.compare("newmtl") == 0)
					{
						String lstrName;
						ISystem::Get().ToString(lstrLine.substr(liSpaceIdx + 1).c_str(), lstrName);

						lMaterial = new CObjMaterial();
						lMaterial->mstrName = lstrName;
						mMaterials.push_back(IModelMaterialPtr(lMaterial));
					}

					if (lstrPrefix.compare("Ka") == 0)
					{
						lstrLine = lstrLine.substr(liSpaceIdx + 1);

						CColor4F lAmbient;
						std::istringstream lstrAmbient(lstrLine);

						lstrAmbient >> lAmbient.R;
						lstrAmbient >> lAmbient.G;
						lstrAmbient >> lAmbient.B;

						lMaterial->mAmbient = lAmbient;
					}

					if (lstrPrefix.compare("Kd") == 0)
					{
						lstrLine = lstrLine.substr(liSpaceIdx + 1);

						CColor4F lDiffuse;
						std::istringstream lstrDiffuse(lstrLine);

						lstrDiffuse >> lDiffuse.R;
						lstrDiffuse >> lDiffuse.G;
						lstrDiffuse >> lDiffuse.B;

						lMaterial->mDiffuse = lDiffuse;
					}

					if (lstrPrefix.compare("Ke") == 0)
					{
						lstrLine = lstrLine.substr(liSpaceIdx + 1);

						CColor4F lEmissive;
						std::istringstream lstrEmissive(lstrLine);

						lstrEmissive >> lEmissive.R;
						lstrEmissive >> lEmissive.G;
						lstrEmissive >> lEmissive.B;

						lMaterial->mEmissive = lEmissive;
					}

					if (lstrPrefix.compare("Ks") == 0)
					{
						lstrLine = lstrLine.substr(liSpaceIdx + 1);

						CColor4F lSpecular;
						std::istringstream lstrSpecular(lstrLine);

						lstrSpecular >> lSpecular.R;
						lstrSpecular >> lSpecular.G;
						lstrSpecular >> lSpecular.B;

						lMaterial->mSpecular = lSpecular;
					}

					if (lstrPrefix.compare("Ns") == 0)
					{
						lstrLine = lstrLine.substr(liSpaceIdx + 1);

						float lfSpecPower;
						std::istringstream lstrSpecPower(lstrLine);

						lstrSpecPower >> lfSpecPower;
						lMaterial->mfSpecularPower = lfSpecPower;
					}

					if (lstrPrefix.compare("map_Kd") == 0)
					{
						String lstrTempPath;
						String lstrFileName;
						String lstrDirectory;
						String lstrFullPath;

						lstrLine = lstrLine.substr(liSpaceIdx + 1);
						
						ISystem::Get().ToString(lstrLine.substr(liSpaceIdx + 1).c_str(), lstrTempPath);
						ISystem::Get().GetFileName(lstrTempPath, lstrFileName);
						ISystem::Get().GetDirectory(pstrFilePath, lstrDirectory);
						ISystem::Get().CombinePath(lstrDirectory, lstrFileName, lstrFullPath);

						lMaterial->mstrDiffuseMap = lstrFullPath;
					}
				}
			}
		}

		return lResult;
	}

	EResult CObjModel::PopulateModelMesh(CObjModelMesh * pMesh, const Array<CVector3F>& lPositions, const Array<CVector3F>& lNormals, 
		const Array<CVector2F>& lTexCoords, const Map<uint32_t, Array<SObjFace>>& pFaces)
	{		
		EResult lResult = EResult::Success;
		uint32_t luiNumIndices = 0;
		uint32_t luiStartIndex = 0;

		for (const auto &lIt : pFaces)
		{			
			luiStartIndex += luiNumIndices;
			luiNumIndices = 0;

			for (const auto &lFace : lIt.second)
			{
				SModelVertex lVertex;
				uint16_t luiI1 = 0, luiI2 = 0, luiI3 = 0;

				lVertex.Position = lPositions[lFace.Px];
				lVertex.Normal = lFace.HasNormals ? lNormals[lFace.Nx] : CVector3F();
				lVertex.TexCoord1 = lFace.HasTexCoords ? lTexCoords[lFace.Tx] : CVector2F();
				luiI1 = pMesh->AddVertex(lVertex);

				lVertex.Position = lPositions[lFace.Py];
				lVertex.Normal = lFace.HasNormals ? lNormals[lFace.Ny] : CVector3F();
				lVertex.TexCoord1 = lFace.HasTexCoords ? lTexCoords[lFace.Ty] : CVector2F();
				luiI2 = pMesh->AddVertex(lVertex);

				lVertex.Position = lPositions[lFace.Pz];
				lVertex.Normal = lFace.HasNormals ? lNormals[lFace.Nz] : CVector3F();
				lVertex.TexCoord1 = lFace.HasTexCoords ? lTexCoords[lFace.Tz] : CVector2F();
				luiI3 = pMesh->AddVertex(lVertex);

				pMesh->AddFace(luiI1, luiI2, luiI3);
				luiNumIndices += 3;
			}

			CObjModelMeshPart *lMeshPart = new CObjModelMeshPart();
			lMeshPart->muiMaterialIndex = lIt.first;
			lMeshPart->muiStartIndex = luiStartIndex;
			lMeshPart->muiNumIndices = luiNumIndices;

			pMesh->mMeshParts.push_back(IModelMeshPartPtr(lMeshPart));			
		}

		return lResult;
	}

	CObjModelLoader::CObjModelLoader()
	{
	}

	CObjModelLoader::~CObjModelLoader()
	{
	}

	EModelLoaderType CObjModelLoader::GetModelLoaderType() const
	{
		return EModelLoaderType::Obj;
	}

	bool CObjModelLoader::CanLoadModelType(EModelType pType)
	{
		return pType == EModelType::Obj;
	}

	bool CObjModelLoader::CanLoadModel(const String & pstrFilePath)
	{
		bool lbCanLoad = false;
		uint32_t luiDot = pstrFilePath.find_last_of(_T("."));

		if (luiDot != (uint32_t)String::npos)
		{
			String lstrExt = pstrFilePath.substr(luiDot);
			std::transform(lstrExt.begin(), lstrExt.end(), lstrExt.begin(), ::tolower);

			if (lstrExt == _T(".obj"))
			{
				lbCanLoad = true;
			}
		}

		return lbCanLoad;
	}

	EResult CObjModelLoader::Load(const String & pstrFilePath, IModel ** pModel)
	{
		EResult lResult = EResult::Success;

		CObjModel *lModel = new CObjModel();
		lResult = lModel->Create(pstrFilePath);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG(_T("CObjModel Create failed: %s"), pstrFilePath.c_str());
		}
		else
		{
			*pModel = lModel;
		}

		return lResult;
	}	
}