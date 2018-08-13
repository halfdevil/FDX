#pragma once

#include "Core/ModelLoader.h"

namespace FDX
{
	class CObjModel;
	class CObjModelMesh;
	class CObjModelMeshPart;

	struct SObjFace
	{
		uint16_t Px, Py, Pz;
		uint16_t Nx, Ny, Nz;
		uint16_t Tx, Ty, Tz;

		bool HasNormals;
		bool HasTexCoords;

		SObjFace()
			: Px(0), Py(0), Pz(0),
			Nx(0), Ny(0), Nz(0),
			Tx(0), Ty(0), Tz(0),
			HasNormals(false),
			HasTexCoords(false)
		{
		}
	};

	class CObjMaterial : public IModelMaterial
	{
	public:

		friend class CObjModel;

		CObjMaterial();
		~CObjMaterial();

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
		String mstrSpecularMap;
		String mstrNormalMap;
	};

	class CObjModelMeshPart : public IModelMeshPart
	{
	public:

		friend class CObjModel;
		friend class CObjModelMesh;

		CObjModelMeshPart();
		~CObjModelMeshPart();

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

	class CObjModelMesh : public IModelMesh
	{
	public:

		friend class CObjModel;

		CObjModelMesh();
		~CObjModelMesh();

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

		int32_t AddVertex(const SModelVertex &pVertex);
		void AddFace(uint16_t puiI1, uint16_t puiI2, uint16_t puiI3);

	private:

		String mstrName;
		Array<SModelVertex> mVertices;
		Array<uint16_t> mIndices;
		Array<IModelMeshPartPtr> mMeshParts;
	};

	class CObjModel : public IModel
	{
	public:

		CObjModel();
		~CObjModel();

		EModelType GetModelType() const
		{
			return EModelType::Obj;
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

		EResult AddFace(const std::string &pstrLine, Array<SObjFace> &pFaces, 
			int32_t piSpaceIndex, bool pbHasTexCoords, bool pbHasNormals);

		EResult ReadMaterialLibrary(const String &pstrFilePath);
		EResult PopulateModelMesh(CObjModelMesh *pMesh,
			const Array<CVector3F> &lPositions,
			const Array<CVector3F> &lNormals, 
			const Array<CVector2F> &lTexCoords,
			const Map<uint32_t, Array<SObjFace>> &pFaces);
		
	private:

		Array<IModelMaterialPtr> mMaterials;
		Array<IModelMeshPtr> mMeshes;
		IModelSkinningDataPtr mSkinningData;
	};

	class FDX_EXPORT CObjModelLoader : public IModelLoader
	{
	public:

		CObjModelLoader();
		~CObjModelLoader();

		EModelLoaderType GetModelLoaderType() const;

		bool CanLoadModelType(EModelType pType);
		bool CanLoadModel(const String &pstrFilePath);

		EResult Load(const String &pstrFilePath,
			IModel **pModel);
	};
}