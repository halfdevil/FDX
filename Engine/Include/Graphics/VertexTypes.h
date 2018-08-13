#pragma once

#include "Math/Vector2F.h"
#include "Math/Vector3F.h"
#include "Math/Vector4F.h"
#include "Math/Color4F.h"
#include "Graphics/VertexFormat.h"

namespace FDX
{
	struct FDX_EXPORT SVertexP
	{
		CVector3F Position;

		SVertexP()
		{
		}

		SVertexP(const CVector3F &pPosition)
			: Position(pPosition)
		{
		}

		SVertexP(float pfX, float pfY, float pfZ)
			: Position(pfX, pfY, pfZ)
		{
		}

		static const CVertexElement Elements[1];
		static const uint32_t NumElements;
	};

	struct FDX_EXPORT SVertexPC
	{
		CVector3F Position;
		CColor4F Color;

		SVertexPC()
		{
		}

		SVertexPC(const CVector3F &pPosition, const CColor4F &pColor)
			: Position(pPosition),
			Color(pColor)
		{
		}

		SVertexPC(float pfX, float pfY, float pfZ, 
			float pfR, float pfG, float pfB, float pfA = 1.0f)
			: Position(pfX, pfY, pfZ),
			Color(pfR, pfG, pfB, pfA)
		{
		}

		static const CVertexElement Elements[2];
		static const uint32_t NumElements;
	};

	struct FDX_EXPORT SVertexPCT
	{
		CVector3F Position;
		CColor4F Color;
		CVector2F TexCoord;

		SVertexPCT()
		{
		}

		SVertexPCT(const CVector3F &pPosition, const CColor4F &pColor, const CVector2F &pTexCoord)
			: Position(pPosition),
			Color(pColor),
			TexCoord(pTexCoord)
		{
		}

		SVertexPCT(float pfX, float pfY, float pfZ,
			float pfR, float pfG, float pfB, float pfA,
			float pfU, float pfV)
			: Position(pfX, pfY, pfZ),
			Color(pfR, pfG, pfB, pfA),
			TexCoord(pfU, pfV)
		{
		}

		static const CVertexElement Elements[3];
		static const uint32_t NumElements;
	};

	struct FDX_EXPORT SVertexPT
	{
		CVector3F Position;
		CVector2F TexCoord;

		SVertexPT()
		{
		}

		SVertexPT(const CVector3F &pPosition, const CVector2F &pTexCoord)
			: Position(pPosition),
			TexCoord(pTexCoord)
		{
		}

		SVertexPT(float pfX, float pfY, float pfZ, float pfS, float pfT)
			: Position(pfX, pfY, pfZ),
			TexCoord(pfS, pfT)
		{
		}

		static const CVertexElement Elements[2];
		static const uint32_t NumElements;
	};

	struct FDX_EXPORT SVertexPNT
	{
		CVector3F Position;
		CVector3F Normal;
		CVector2F TexCoord;

		SVertexPNT()
		{
		}

		SVertexPNT(const CVector3F &pPosition, const CVector3F &pNormal, const CVector2F &pTexCoord)
			: Position(pPosition),
			Normal(pNormal),
			TexCoord(pTexCoord)
		{
		}

		SVertexPNT(float pfX, float pfY, float pfZ, float pfNx,
			float pfNy, float pfNz, float pfS, float pfT)
			: Position(pfX, pfY, pfZ),
			Normal(pfNx, pfNy, pfNz),
			TexCoord(pfS, pfT)
		{
		}

		static const CVertexElement Elements[3];
		static const uint32_t NumElements;
	};

	class FDX_EXPORT SVertexCanvas
	{
	public:

		CVector4F Position;
		CVector2F TexCoord;

		SVertexCanvas()
		{
		}

		SVertexCanvas(const CVector4F &pPosition, const CVector2F &pTexCoord)
			: Position(pPosition), TexCoord(pTexCoord)
		{
		}

		SVertexCanvas(float pfX, float pfY, float pfZ, float pfW,
			float pfTx, float pfTy)
			: Position(pfX, pfY, pfZ, pfW), 
			TexCoord(pfTx, pfTy)
		{
		}

	public:

		static const unsigned int NumElements;
		static const CVertexElement Elements[2];
	};

	struct FDX_EXPORT SModelVertex
	{
		CVector3F Position;
		CVector3F Normal;
		CVector2F TexCoord1;
		CVector2F TexCoord2;
		CVector2F TexCoord3;
		CVector4F BlendIndices;
		CVector4F BlendWeights;

		SModelVertex()
		{
		}

		SModelVertex(const CVector3F &pPosition,
			const CVector3F &pNormal,
			const CVector2F &pTexCoord)
			: Position(pPosition),
			Normal(pNormal),
			TexCoord1(pTexCoord)
		{
		}

		SModelVertex(const CVector3F &pPosition,
			const CVector3F &pNormal,
			const CVector2F &pTexCoord1,
			const CVector2F &pTexCoord2,
			const CVector2F &pTexCoord3,
			const CVector4F &pBlendIndices,
			const CVector4F &pBlendWeights)
			: Position(pPosition),
			Normal(pNormal),
			TexCoord1(pTexCoord1),
			TexCoord2(pTexCoord2),
			TexCoord3(pTexCoord3),
			BlendIndices(pBlendIndices),
			BlendWeights(pBlendWeights)
		{
		}

		static const uint32_t NumElements;
		static const CVertexElement Elements[7];
	};
}