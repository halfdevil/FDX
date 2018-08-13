#pragma once

#include "Math/Matrix4F.h"
#include "Math/Color4F.h"
#include "Math/Quaternion.h"
#include "Core/Model.h"

namespace FDX
{
	enum class EFdmChunkType : uint32_t
	{
		Header = 0x0100,
		Materials = 0x0200,
		Meshes = 0x0300,		
		Vertices = 0x0400,
		Indices = 0x0500,
		MeshParts = 0x0600,		
		Joints = 0x0700,
		Animations = 0x0800,
		KeyFrames = 0x0900,
		End = 0x0999
	};

	struct SFdmChunk
	{
		EFdmChunkType Type;
		uint32_t Size;
	};

	struct SFdmHeader
	{
		char Identifier[4];
		char Name[32];
		char Author[32];
		char Email[32];
		char Version;
		uint32_t NumMaterials;
		uint32_t NumMeshes;		
		uint32_t NumVertices;
		uint32_t NumIndices;
		uint32_t NumMeshParts;
		uint32_t NumJoints;
		uint32_t NumAnimations;
		uint32_t NumKeyFrames;		
	};

	struct SFdmMaterial
	{
		char Name[32];
		CColor4F Ambient;
		CColor4F Diffuse;		
		CColor4F Emissive;
		CColor4F Specular;
		float SpecularPower;
		char DiffuseMap[128];
		char NormalMap[128];
		char SpecularMap[128];
	};

	struct SFdmVertex
	{
		CVector3F Position;
		CVector3F Normal;
		CVector2F TexCoord1;
		CVector2F TexCoord2;
		CVector2F TexCoord3;
		CVector4F BlendIndices;
		CVector4F BlendWeights;
	};

	struct SFdmMeshPart
	{
		uint32_t MaterialIndex;
		uint32_t StartIndex;
		uint32_t NumIndices;
	};

	struct SFdmMesh
	{
		char Name[32];
		uint32_t StartVertex;
		uint32_t StartIndex;
		uint32_t StartMeshPart;
		uint32_t NumVertices;
		uint32_t NumIndices;
		uint32_t NumMeshParts;
	};

	struct SFdmJoint
	{
		int32_t Index;
		int32_t Parent;
		CMatrix4F BindPose;
	};

	struct SFdmKeyFrame
	{
		float Time;
		int32_t JointIndex;
		CVector3F Position;
		CQuaternion Orientation;
	};

	struct SFdmAnimation
	{
		char Name[32];
		float Duration;
		uint32_t StartKeyFrame;
		uint32_t NumKeyFrames;
	};
}