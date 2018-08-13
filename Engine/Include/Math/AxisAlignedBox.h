#pragma once

#include "Math/Vector3F.h"

namespace FDX
{
	enum class EContainmentType
	{
		Visible,
		Clipped,
		Culled
	};

	class FDX_EXPORT CAxisAlignedBox
	{
	public:

		inline CAxisAlignedBox();

		inline CAxisAlignedBox(const CVector3F &pMin, 
			const CVector3F &pMax);
		
		inline CAxisAlignedBox(float pfXMin, float pfYMin, float pfZMin,
			float pfXMax, float pfYMax, float pfZMax);

		inline CAxisAlignedBox(float pfCubeSize);
		
		inline float GetLengthX() const;
		inline float GetLengthY() const;
		inline float GetLengthZ() const;

		inline CVector3F GetExtents() const;
		inline CVector3F GetCenter() const;

		inline void Merge(const CAxisAlignedBox &pAabb);
		
		inline bool IsContained(const CVector3F &pPoint);
		inline bool IsContained(const CAxisAlignedBox &pAabb);
		inline bool IsOverlapped(const CAxisAlignedBox &pAabb);
		
		inline EContainmentType Contains(const CAxisAlignedBox &pAabb);
		
		inline float GetDistanceToPoint(const CVector3F &pPoint);
		inline float GetDistanceToPointSqr(const CVector3F &pPoint);
		
		inline Array<CVector3F> GetCorners() const;
		
		inline CAxisAlignedBox operator + (const CAxisAlignedBox &pAabb) const;
		inline CAxisAlignedBox operator - (const CAxisAlignedBox &pAabb) const;
		inline CAxisAlignedBox operator * (float pfScale) const;
		inline CAxisAlignedBox operator / (float pfScale) const;
		
		inline CAxisAlignedBox& operator += (const CAxisAlignedBox &pAabb);
		inline CAxisAlignedBox& operator -= (const CAxisAlignedBox &pAabb);
		inline CAxisAlignedBox& operator *= (float pfScale);
		inline CAxisAlignedBox& operator /= (float pfScale);

	public:  

		static CAxisAlignedBox CreateFromPoints(const Array<CVector3F> &pPoints);

	public:

		CVector3F Min;
		CVector3F Max;
	};

	inline CAxisAlignedBox::CAxisAlignedBox()
	{
	}

	inline CAxisAlignedBox::CAxisAlignedBox(const CVector3F & pMin, const CVector3F & pMax)
		: Min(pMin), Max(pMax)
	{
	}

	inline CAxisAlignedBox::CAxisAlignedBox(float pfXMin, float pfYMin, float pfZMin,
		float pfXMax, float pfYMax, float pfZMax)
		: Min(pfXMin, pfYMin, pfZMin),
		Max(pfXMax, pfYMax, pfZMax)
	{
	}

	inline CAxisAlignedBox::CAxisAlignedBox(float pfCubeSize)
		: Min(-0.5f * pfCubeSize),
		Max(0.5f * pfCubeSize)
	{
	}

	inline float CAxisAlignedBox::GetLengthX() const
	{
		return Max.X - Min.X;
	}

	inline float CAxisAlignedBox::GetLengthY() const
	{
		return Max.Y - Min.Y;
	}

	inline float CAxisAlignedBox::GetLengthZ() const
	{
		return Max.Z - Min.Z;
	}

	inline CVector3F CAxisAlignedBox::GetExtents() const
	{
		return Max - Min;
	}

	inline CVector3F CAxisAlignedBox::GetCenter() const
	{
		return (Max + Min) * 0.5f;
	}
	
	inline void CAxisAlignedBox::Merge(const CAxisAlignedBox & pAabb)
	{
		if (pAabb.Min.X < Min.X) Min.X = pAabb.Min.X;
		if (pAabb.Min.Y < Min.Y) Min.Y = pAabb.Min.Y;
		if (pAabb.Min.Z < Min.Z) Min.Z = pAabb.Min.Z;

		if (pAabb.Max.X > Max.X) Max.X = pAabb.Max.X;
		if (pAabb.Max.Y > Max.Y) Max.Y = pAabb.Max.Y;
		if (pAabb.Max.Z > Max.Z) Max.Z = pAabb.Max.Z;
	}

	inline bool CAxisAlignedBox::IsContained(const CVector3F & pPoint)
	{
		return (pPoint.X >= Min.X && pPoint.X <= Max.X) &&
			(pPoint.Y >= Min.Y && pPoint.Y <= Max.Y) &&
			(pPoint.Z >= Min.Z && pPoint.Z <= Max.Z);
	}

	inline bool CAxisAlignedBox::IsContained(const CAxisAlignedBox & pAabb)
	{
		return (pAabb.Min.X >= Min.X && pAabb.Max.X <= Max.X) &&
			(pAabb.Min.Y >= Min.Y && pAabb.Max.Y <= Max.Y) &&
			(pAabb.Min.Z >= Min.Z && pAabb.Max.Z <= Max.Z);
	}

	inline bool CAxisAlignedBox::IsOverlapped(const CAxisAlignedBox & pAabb)
	{
		if (pAabb.Min.X > Max.X || pAabb.Min.Y > Max.Y || pAabb.Min.Z > Max.Z)
			return false;

		if (pAabb.Max.X < Min.X || pAabb.Max.Y < Min.Y || pAabb.Max.Z < Min.Z)
			return false;

		return true;
	}

	inline EContainmentType CAxisAlignedBox::Contains(const CAxisAlignedBox & pAabb)
	{
		if ((pAabb.Min.X >= Min.X && pAabb.Max.X <= Max.X) &&
			(pAabb.Min.Y >= Min.Y && pAabb.Max.Y <= Max.Y) &&
			(pAabb.Min.Z >= Min.Z && pAabb.Max.Z <= Max.Z))
		{
			return EContainmentType::Visible;
		}

		if ((pAabb.Min.X > Max.X || pAabb.Min.Y > Max.Y || pAabb.Min.Z > Max.Z) ||
			(pAabb.Max.X < Min.X || pAabb.Max.Y < Min.Y || pAabb.Max.Z < Min.Z))
		{
			return EContainmentType::Clipped;
		}

		return EContainmentType::Culled;
	}

	inline float CAxisAlignedBox::GetDistanceToPoint(const CVector3F & pPoint)
	{
		return CMathUtil::Sqrt(GetDistanceToPointSqr(pPoint));
	}

	inline float CAxisAlignedBox::GetDistanceToPointSqr(const CVector3F & pPoint)
	{
		float lfSqrDist = 0.0f;

		for (uint32_t lIdx = 0; lIdx < 3; lIdx++)
		{
			float lVec = pPoint[lIdx];

			if (lVec < Min[lIdx])
				lfSqrDist += (Min[lIdx] - lVec) * (Min[lIdx] * lVec);

			if (lVec > Max[lIdx])
				lfSqrDist += (lVec - Max[lIdx]) * (lVec - Max[lIdx]);
		}

		return lfSqrDist;
	}

	inline Array<CVector3F> CAxisAlignedBox::GetCorners() const
	{
		Array<CVector3F> lCorners;

		lCorners.push_back(CVector3F(Min.X, Max.Y, Max.Z));
		lCorners.push_back(CVector3F(Max.X, Max.Y, Max.Z));
		lCorners.push_back(CVector3F(Max.X, Min.Y, Max.Z));
		lCorners.push_back(CVector3F(Min.X, Min.Y, Max.Z));
		lCorners.push_back(CVector3F(Min.X, Max.Y, Min.Z));
		lCorners.push_back(CVector3F(Max.X, Max.Y, Min.Z));
		lCorners.push_back(CVector3F(Max.X, Min.Y, Min.Z));
		lCorners.push_back(CVector3F(Min.X, Min.Y, Min.Z));

		return lCorners;
	}

	inline CAxisAlignedBox CAxisAlignedBox::operator+(const CAxisAlignedBox & pAabb) const
	{
		return CAxisAlignedBox(Min + pAabb.Min, Max + pAabb.Max);
	}

	inline CAxisAlignedBox CAxisAlignedBox::operator-(const CAxisAlignedBox & pAabb) const
	{
		return CAxisAlignedBox(Min + pAabb.Min, Max + pAabb.Max);
	}

	inline CAxisAlignedBox CAxisAlignedBox::operator*(float pfScale) const
	{
		return CAxisAlignedBox(Min * pfScale, Max * pfScale);
	}

	inline CAxisAlignedBox CAxisAlignedBox::operator/(float pfScale) const
	{
		return CAxisAlignedBox(Min / pfScale, Max / pfScale);
	}

	inline CAxisAlignedBox & CAxisAlignedBox::operator+=(const CAxisAlignedBox & pAabb)
	{
		Min += pAabb.Min;
		Max += pAabb.Max;

		return *this;
	}

	inline CAxisAlignedBox & CAxisAlignedBox::operator-=(const CAxisAlignedBox & pAabb)
	{
		Min -= pAabb.Min;
		Max -= pAabb.Max;

		return *this;
	}

	inline CAxisAlignedBox & CAxisAlignedBox::operator*=(float pfScale)
	{
		Min *= pfScale;
		Max *= pfScale;

		return *this;
	}

	inline CAxisAlignedBox & CAxisAlignedBox::operator/=(float pfScale)
	{
		Min /= pfScale;
		Max /= pfScale;

		return *this;
	}
}
