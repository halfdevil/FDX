#pragma once

#include "Math/Vector2F.h"
#include "Math/Vector3F.h"
#include "Math/Matrix4F.h"
#include "Math/Color4F.h"
#include "Math/Quaternion.h"
#include "Graphics/VertexFormat.h"
#include "Graphics/VertexTypes.h"
#include "Core/Image.h"

namespace FDX
{
	class IModelJoint;
	class IModelKeyFrame;
	class IModelAnimation;
	class IModelSkinningData;
	class IModelMaterial;
	class IModelMeshPart;
	class IModelMesh;
	class IModel;

	using IModelJointPtr = SmartPtr<IModelJoint>;
	using IModelKeyFramePtr = SmartPtr<IModelKeyFrame>;
	using IModelAnimationPtr = SmartPtr<IModelAnimation>;
	using IModelSkinningDataPtr = SmartPtr<IModelSkinningData>;
	using IModelMaterialPtr = SmartPtr<IModelMaterial>;
	using IModelMeshPartPtr = SmartPtr<IModelMeshPart>;
	using IModelMeshPtr = SmartPtr<IModelMesh>;
	using IModelPtr = SmartPtr<IModel>;

	enum class EModelType
	{
		Obj,
		Ezm,
		Fbx,
		Fdm,
		Dae
	};

	class FDX_EXPORT IModelJoint
	{
	public:

		virtual ~IModelJoint() = default;

		virtual int32_t GetIndex() const = 0;
		virtual int32_t GetParent() const = 0;
		virtual bool IsAnimated() const = 0;

		virtual const CMatrix4F& GetBindPose() const = 0;
		virtual const CMatrix4F& GetInvBindPose() const = 0;
	};	

	class FDX_EXPORT IModelKeyFrame
	{
	public:

		virtual ~IModelKeyFrame() = default;

		virtual float GetTime() const = 0;
		virtual uint32_t GetBoneIndex() const = 0;
		virtual const CVector3F& GetPosition() const = 0;
		virtual const CVector3F& GetScale() const = 0;
		virtual const CQuaternion& GetOrientation() const = 0;		
	};	

	class FDX_EXPORT IModelAnimation
	{
	public:

	    virtual ~IModelAnimation() = default;

		virtual const String& GetName() const = 0;
		virtual float GetDuration() const = 0;
		virtual const Array<IModelKeyFramePtr>& GetKeyFrames() const = 0;
	};	

	class FDX_EXPORT IModelSkinningData
	{
	public:

	    virtual ~IModelSkinningData() = default;
				
		virtual const CMatrix4F& GetRootTransform() const = 0;
		virtual const Array<IModelJointPtr>& GetSkeleton() const = 0;
		virtual const Array<IModelAnimationPtr>& GetAnimations() const = 0;
	};	

	class FDX_EXPORT IModelMaterial
	{
	public:

		virtual ~IModelMaterial() = default;

		virtual const String& GetName() const = 0;
		virtual float GetSpecularPower() const = 0;

		virtual const CColor4F& GetAmbient() const = 0;
		virtual const CColor4F& GetDiffuse() const = 0;
		virtual const CColor4F& GetEmissive() const = 0;
		virtual const CColor4F& GetSpecular() const = 0;
		
		virtual const String& GetDiffuseMap() const = 0;
		virtual const String& GetNormalMap() const = 0;
		virtual const String& GetSpecularMap() const = 0;
	};	

	class FDX_EXPORT IModelMeshPart
	{
	public:

		virtual ~IModelMeshPart() = default;

		virtual uint32_t GetMaterialIndex() const = 0;
		virtual uint32_t GetStartIndex() const = 0;
		virtual uint32_t GetNumIndices() const = 0;
	};	

	class FDX_EXPORT IModelMesh
	{
	public:

		virtual ~IModelMesh() = default;

		virtual const String& GetName() const = 0;
		virtual const Array<SModelVertex>& GetVertices() const = 0;
		virtual const Array<uint16_t>& GetIndices() const = 0;
		virtual const Array<IModelMeshPartPtr>& GetMeshParts() const = 0;
	};	

	class FDX_EXPORT IModel
	{
	public:

		virtual ~IModel() = default;

		virtual EModelType GetModelType() const = 0;
		virtual const Array<IModelMaterialPtr>& GetMaterials() const = 0;
		virtual const Array<IModelMeshPtr>& GetMeshes() const = 0;
		virtual const IModelSkinningDataPtr& GetSkinningData() const = 0;
	};	
}