#pragma once

#include "Core/ModelLoader.h"
#include "Math/AxisAlignedBox.h"

namespace FDX
{
	class CMd5Model;
	class CMd5Material;
	class CMd5ModelMesh;
	class CMd5ModelJoint;
	class CMd5ModelKeyFrame;
	class CMd5ModelMeshPart;
	class CMd5ModelAnimation;
	class CMd5ModelSkinningData;

	using CMd5ModelPtr = SmartPtr<CMd5Model>;
	using CMd5MaterialPtr = SmartPtr<CMd5Material>;
	using CMd5ModelMeshPtr = SmartPtr<CMd5ModelMesh>;
	using CMd5ModelJointPtr = SmartPtr<CMd5ModelJoint>;
	using CMd5ModelKeyFramePtr = SmartPtr<CMd5ModelKeyFrame>;
	using CMd5ModelMeshPartPtr = SmartPtr<CMd5ModelMeshPart>;
	using CMd5ModelAnimationPtr = SmartPtr<CMd5ModelAnimation>;
	using CMd5ModelSkinningDataPtr = SmartPtr<CMd5ModelSkinningData>;

	class CMd5ModelJoint : public IModelJoint
	{
	public:

		friend class CMd5Model;

		CMd5ModelJoint();
		~CMd5ModelJoint() override = default;

		int32_t GetIndex() const override
		{
			return miIndex;
		}

		int32_t GetParent() const override
		{
			return miParent;
		}

		const CMatrix4F& GetBindPose() const override
		{
			return mBindPose;
		}

	private:

		int32_t miIndex;
		int32_t miParent;
		CMatrix4F mBindPose;
	};

	class CMd5ModelKeyFrame : public IModelKeyFrame
	{
	public:

		friend class CMd5Model;
		friend class CMd5ModelMesh;

		CMd5ModelKeyFrame();
		~CMd5ModelKeyFrame() override = default;

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

		virtual const CQuaternion& GetOrientation() const override
		{
			return mOrientation;
		}

	private:

		float mfTime;
		uint32_t muiBoneIndex;
		CVector3F mPosition;
		CQuaternion mOrientation;
	};

	class CMd5ModelAnimation : public IModelAnimation
	{
	public:

		friend class CMd5Model;
		friend class CMd5ModelMesh;

		CMd5ModelAnimation();
		~CMd5ModelAnimation() override = default;

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

	class CMd5ModelSkinningData : public IModelSkinningData
	{
	public:

		friend class CMd5Model;
		friend class CMd5ModelMesh;

		CMd5ModelSkinningData();
		~CMd5ModelSkinningData() override = default;

		const Array<IModelJointPtr>& GetSkeleton() const override
		{
			return mSkeleton;
		}

		const Array<IModelAnimationPtr>& GetAnimations() const override
		{
			return mAnimations;
		}

	private:

		Array<IModelJointPtr> mSkeleton;
		Array<IModelAnimationPtr> mAnimations;
	};
	
	class CMd5Material : public IModelMaterial
	{
	public:

		friend class CMd5Model;

		CMd5Material();
		~CMd5Material() override = default;

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

	class CMd5ModelMeshPart : public IModelMeshPart
	{
	public:

		friend class CMd5Model;
		friend class CMd5ModelMesh;

		CMd5ModelMeshPart();
		~CMd5ModelMeshPart() override = default;

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

	class CMd5ModelMesh : public IModelMesh
	{
	public:

		friend class CMd5Model;

		CMd5ModelMesh();
		~CMd5ModelMesh() override = default;

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

		const Array<uint16_t>& GetBoneMap() const
		{
			return mBoneMap;
		}

	private:

		String mstrName;
		Array<SModelVertex> mVertices;
		Array<uint16_t> mIndices;
		Array<uint16_t> mBoneMap;
		Array<IModelMeshPartPtr> mMeshParts;
	};

	class FDX_EXPORT CMd5Parser
	{
	public:

		struct SJointInfo
		{
			std::string Name;
			int32_t Parent;
			CVector3F Position;
			CQuaternion Rotation;
		};

		struct SVertexInfo
		{
			uint32_t VertexIndex;
			uint32_t StartWeight;
			uint32_t WeightCount;
			CVector2F TexCoord;
		};

		struct STriangleInfo
		{
			uint32_t TriangleIndex;
			Array<uint16_t> VertexIndices;
		};

		struct SWeightInfo
		{
			uint32_t WeightIndex;
			uint32_t JointIndex;
			CVector3F Position;
			float Bias;
		};

		struct SBoneWeight
		{
			std::string BoneName;
			uint32_t JointIndex;
			float Weight;
		};

		struct SHierarchyInfo
		{
			std::string Name;
			int32_t Parent;
			uint32_t Flags;
			uint32_t StartIndex;
		};

		struct SBaseFrame
		{
			CVector3F Position;
			CVector3F Orientation;
		};

		struct SKeyFrame
		{
			float Time;
			int32_t JointIndex;
			CVector3F Position;
			CQuaternion Orientation;
		};

		struct SMeshInfo
		{
			std::string Shader;
			Array<SVertexInfo> Vertices;
			Array<STriangleInfo> Triangles;
			Array<SWeightInfo> Weights;
		};

		struct SMaterialInfo
		{
			std::string DiffuseTexture;
			std::string NormalTexture;
			std::string SpecularTexture;
		};

		struct SMeshPartInfo
		{
			uint32_t MaterialIndex;
			uint32_t StartIndex;
			uint32_t NumIndices;
		};

		enum class EMd5ElementType
		{
			String,
			Numeric,
			Vector2,
			Vector3,
			Empty
		};

		struct SMd5Element
		{
			EMd5ElementType ValueType;
			std::string ValueString;
			float ValueNumeric;
			CVector2F ValueVector2;
			CVector3F ValueVector3;
		};

		struct SMd5ParseKey
		{
			std::string Name;
			uint32_t SeekElement;
			Array<SMd5Element> Elements;
		};

	public:

		CMd5Parser();
		~CMd5Parser() = default;

		EResult Parse(const AsciiString &pstrData);

		void SeekToBegining();
		void SeekToKey(const char *pstrKey);

		CVector2F GetVector2();
		CVector3F GetVector3();		
		AsciiString GetString();
		float GetNumeric();

		bool IsEOFKey();

	private:

		void ParseComment();
		void ParseVector();
		void ParseNumeric();
		void ParseKey();
		void ParseString();

	private:

		Array<SMd5ParseKey> mKeys;
		Array<std::string> mData;

		int32_t miSeekData;
		int32_t miSeek;
	};

	class FDX_EXPORT CMd5Model : public IModel
	{
	public:

		CMd5Model();
		~CMd5Model() override;

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

		EResult LoadMeshes(const String &pstrMd5MeshFilePath);
		EResult LoadAnimations(const Array<String> &pstrMd5AnimFilePaths);

		void CreateMesh(CMd5Parser::SMeshInfo &mMd5Mesh);
		void CreateMaterial(CMd5Parser::SMaterialInfo &mMd5Material);
		void CreateAnimation(const String &pstrName);

		void CreateSkeletonByFrame(uint32_t puiFrameIndex, 
			Array<IModelKeyFramePtr> &pKeyFrames);

		void GetBoneNamesByVertex(CMd5Parser::SMeshInfo &pMd5Mesh, 
			const CMd5Parser::SVertexInfo &pVertex, 
			Array<std::string> &pBoneNames);

		void GetBoneWeights(CMd5Parser::SMeshInfo &pMd5Mesh, 
			const CMd5Parser::SVertexInfo &pVertexInfo, 
			Array<CMd5Parser::SBoneWeight> &pWeights);

		void NormalizeWeights(Array<CMd5Parser::SBoneWeight> &pWeights, uint32_t puiMaxWeights);		
		void CalculateNormals(CMd5Parser::SMeshInfo &pMd5Mesh, Array<SModelVertex> &pVertices);
		bool CheckCreateTriangle(CMd5Parser::SMeshInfo &pMd5Mesh, uint32_t puiTriangleIndex);

	private:

		Array<CMd5Parser::SMeshInfo> mMd5Meshes;
		Array<CMd5Parser::SJointInfo> mMd5Joints;
		Array<CMd5Parser::SMaterialInfo> mMd5Materials;

		uint32_t muiNumFrames;
		uint32_t muiNumJoints;
		uint32_t muiNumMeshes;
		uint32_t muiFrameRate;
		uint32_t muiNumAnimatedComponents;

		UnorderedMap<std::string, uint32_t> mMd5BoneMap;
		Array<uint32_t> mMd5TrianglesProcessed;
		Array<Array<float>> mMd5Frames;
		Array<CMd5Parser::SBaseFrame> mMd5BaseFrames;
		Array<CMd5Parser::SHierarchyInfo> mMd5Hierarchies;
		Array<CAxisAlignedBox> mMd5AxisAlignedBoxes;

		Array<IModelMaterialPtr> mMaterials;
		Array<IModelMeshPtr> mMeshes;
		IModelSkinningDataPtr mSkinningData;
	};
}