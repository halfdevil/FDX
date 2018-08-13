#pragma once

#include "Math/Vector3F.h"
#include "Math/Matrix4F.h"

namespace FDX
{
	enum class EPlaneSide
	{
		On, 
		Front,
		Back
	};

	class FDX_EXPORT CPlane
	{
	public:

		inline CPlane();
		inline CPlane(const CVector3F &pNormal, float pfD);
		inline CPlane(float pfNx, float pfNy, float pfNz, float pfD);
		inline CPlane(const CVector3F &pNormal, const CVector3F &pPoint);
		inline CPlane(const CVector3F &pA, const CVector3F &pB, const CVector3F &pC);
	
		inline float Dot(const CVector3F &pVec);
		inline float DotNormal(const CVector3F &pVec);
		inline float DistanceToPlane(const CVector3F &pPoint);
	
		inline CPlane& Normalize();
		inline CPlane Transform(const CMatrix4F &pMat);
	
		inline EPlaneSide SideOfPlane(const CVector3F &pPoint);
		
	public:

		CVector3F Normal;
		float D;
	};

	inline CPlane::CPlane()
	{
	}

	inline CPlane::CPlane(const CVector3F & pNormal, float pfD)
		: Normal(pNormal), D(pfD)
	{
	}

	inline CPlane::CPlane(float pfNx, float pfNy, float pfNz, float pfD)
		: Normal(pfNx, pfNy, pfNz), D(pfD)
	{
	}

	inline CPlane::CPlane(const CVector3F & pNormal, const CVector3F & pPoint)
		: Normal(pNormal)
	{
		D = -Normal.Dot(pPoint);
	}

	inline CPlane::CPlane(const CVector3F & pA, const CVector3F & pB, const CVector3F & pC)
	{
		Normal = (pA - pB).Cross(pC - pA);
		Normal.Normalize();

		D = -Normal.Dot(pA);
	}

	inline float CPlane::Dot(const CVector3F & pVec)
	{
		return Normal.X * pVec.X + Normal.Y * pVec.Y +
			Normal.Z * pVec.Z + D;
	}

	inline float CPlane::DotNormal(const CVector3F & pVec)
	{
		return Normal.X * pVec.X + Normal.Y * pVec.Y +
			Normal.Z * pVec.Z;
	}

	inline float CPlane::DistanceToPlane(const CVector3F & pPoint)
	{
		return Normal.X * pPoint.X + Normal.Y * pPoint.Y +
			Normal.Z * pPoint.Z + D;
	}

	inline CPlane& CPlane::Normalize()
	{
		float lfLen = CMathUtil::Sqrt(Normal.X * Normal.X +
			Normal.Y * Normal.Y + Normal.Z * Normal.Z);

		if (lfLen != 0.0f)
		{
			float lfInvLen = 1.0f / lfLen;

			Normal *= lfInvLen;
			D *= lfInvLen;
		}

		return *this;
	}

	inline CPlane CPlane::Transform(const CMatrix4F & pMat)
	{
		CPlane lOut;

		float lfX = Normal.X;
		float lfY = Normal.Y;
		float lfZ = Normal.Z;

		lOut.Normal.X = lfX * pMat.M2[0][0] + lfY * pMat.M2[1][0] + lfZ * pMat.M2[2][0] + pMat.M2[3][0];
		lOut.Normal.Y = lfX * pMat.M2[0][1] + lfY * pMat.M2[1][1] + lfZ * pMat.M2[2][1] + pMat.M2[3][1];
		lOut.Normal.Z = lfX * pMat.M2[0][2] + lfY * pMat.M2[1][2] + lfZ * pMat.M2[2][2] + pMat.M2[3][2];
		lOut.D = lfX * pMat.M2[0][3] + lfY * pMat.M2[1][3] + lfZ * pMat.M2[2][3] + D * pMat.M2[3][3];

		return lOut;
	}

	inline EPlaneSide CPlane::SideOfPlane(const CVector3F & pPoint)
	{
		float lfDist = DistanceToPlane(pPoint);

		if (lfDist >= 0.005f)
			return EPlaneSide::Front;
		else if (lfDist <= -0.005f)
			return EPlaneSide::Back;

		return EPlaneSide::On;
	}
}