#pragma once

#include "Core/Core.h"
#include "Math/MathUtil.h"

namespace FDX
{
	class FDX_EXPORT CVector3F
	{
	public:

		inline CVector3F();
		inline CVector3F(float pVal);
		inline CVector3F(float pX, float pY, float pZ);

		inline void Set(float pfX, float pfY, float pfZ);
		inline void Clear();

		inline float GetLength() const;
		inline float GetLengthSqr() const;
		inline float Dot(const CVector3F &pV) const;

		inline float Normalize();
		inline float* ToPtr();
		inline const float* ToPtr() const;
		inline uint32_t GetHashCode() const;

		inline float Distance(const CVector3F &pV) const;

		inline CVector3F Cross(const CVector3F &pV) const;
		inline CVector3F Lerp(const CVector3F &pV, float pAmount) const;

		inline CVector3F operator + (const CVector3F &pV) const;
		inline CVector3F operator - (const CVector3F &pV) const;
		inline CVector3F operator * (const CVector3F &pV) const;
		inline CVector3F operator / (const CVector3F &pV) const;

		inline CVector3F& operator += (const CVector3F &pV);
		inline CVector3F& operator -= (const CVector3F &pV);
		inline CVector3F& operator *= (const CVector3F &pV);
		inline CVector3F& operator /= (const CVector3F &pV);

		inline CVector3F operator * (float pVal) const;
		inline CVector3F operator / (float pVal) const;

		inline CVector3F& operator *= (float pVal);
		inline CVector3F& operator /= (float pVal);

		inline float operator[] (uint32_t puiIndex) const;

		inline bool operator == (const CVector3F &pV) const;
		inline bool operator != (const CVector3F &pV) const;

	public:

		float X;
		float Y;
		float Z;
	};	

	inline CVector3F::CVector3F()
		: X(0), Y(0), Z(0)
	{
	}

	inline CVector3F::CVector3F(float pVal)
		: X(pVal), Y(pVal), Z(pVal)
	{
	}

	inline CVector3F::CVector3F(float pX, float pY, float pZ)
		: X(pX), Y(pY), Z(pZ)
	{
	}

	inline void CVector3F::Set(float pfX, float pfY, float pfZ)
	{
		X = pfX;
		Y = pfY;
		Z = pfZ;
	}

	inline void CVector3F::Clear()
	{
		X = 0.0f;
		Y = 0.0f;
		Z = 0.0f;
	}

	inline float CVector3F::GetLength() const
	{
		return CMathUtil::Sqrt(X * X + Y * Y + Z * Z);
	}

	inline float CVector3F::GetLengthSqr() const
	{
		return X * X + Y * Y + Z * Z;
	}

	inline float CVector3F::Dot(const CVector3F &pV) const
	{
		return X * pV.X + Y * pV.Y + Z * pV.Z;
	}

	inline float CVector3F::Normalize()
	{
		float lLen = CMathUtil::Sqrt(X * X + Y * Y + Z * Z);
		if (lLen != 0)
		{
			float lInvLen = 1 / lLen;

			X *= lInvLen;
			Y *= lInvLen;
			Z *= lInvLen;
		}

		return lLen;
	}

	inline float * CVector3F::ToPtr()
	{
		return &X;
	}

	inline const float * CVector3F::ToPtr() const
	{
		return &X;
	}

	inline uint32_t CVector3F::GetHashCode() const
	{
		return static_cast<uint32_t>(((std::hash<float>()(X) ^
			(std::hash<float>()(Y) << 1)) >> 1) ^
			(std::hash<float>{}(Z) << 1));
	}

	inline float CVector3F::Distance(const CVector3F & pV) const
	{
		float lfX = X - pV.X;
		float lfY = Y - pV.Y;
		float lfZ = Z - pV.Z;

		return CMathUtil::Sqrt(lfX * lfX + lfY * lfY + lfZ * lfZ);
	}

	inline CVector3F CVector3F::Lerp(const CVector3F& pV, float pAmount) const
	{
		return CVector3F(CMathUtil::Lerp(X, pV.X, pAmount),
			CMathUtil::Lerp(Y, pV.Y, pAmount),
			CMathUtil::Lerp(Z, pV.Z, pAmount));
	}

	inline CVector3F CVector3F::Cross(const CVector3F& pV) const
	{
		return CVector3F(Y * pV.Z - Z * pV.Y,
			Z * pV.X - X * pV.Z,
			X * pV.Y - Y * pV.X);
	}

	inline CVector3F CVector3F::operator+(const CVector3F& pV) const
	{
		return CVector3F(X + pV.X, Y + pV.Y, Z + pV.Z);
	}

	inline CVector3F CVector3F::operator-(const CVector3F& pV) const
	{
		return CVector3F(X - pV.X, Y - pV.Y, Z - pV.Z);
	}

	inline CVector3F CVector3F::operator*(const CVector3F& pV) const
	{
		return CVector3F(X * pV.X, Y * pV.Y, Z * pV.Z);
	}

	inline CVector3F CVector3F::operator/(const CVector3F& pV) const
	{
		return CVector3F(X / pV.X, Y / pV.Y, Z / pV.Z);
	}

	inline CVector3F& CVector3F::operator+=(const CVector3F& pV)
	{
		X += pV.X;
		Y += pV.Y;
		Z += pV.Z;

		return *this;
	}

	inline CVector3F& CVector3F::operator-=(const CVector3F& pV)
	{
		X -= pV.X;
		Y -= pV.Y;
		Z -= pV.Z;

		return *this;
	}

	inline CVector3F& CVector3F::operator*=(const CVector3F& pV)
	{
		X *= pV.X;
		Y *= pV.Y;
		Z *= pV.Z;

		return *this;
	}

	inline CVector3F& CVector3F::operator/=(const CVector3F& pV)
	{
		X /= pV.X;
		Y /= pV.Y;
		Z /= pV.Z;

		return *this;
	}

	inline CVector3F CVector3F::operator*(float pVal) const
	{
		return CVector3F(X * pVal, Y * pVal, Z * pVal);
	}

	inline CVector3F CVector3F::operator/(float pVal) const
	{
		return CVector3F(X / pVal, Y / pVal, Z / pVal);
	}

	inline CVector3F& CVector3F::operator*=(float pVal)
	{
		X *= pVal;
		Y *= pVal;
		Z *= pVal;

		return *this;
	}

	inline CVector3F& CVector3F::operator/=(float pVal)
	{
		X /= pVal;
		Y /= pVal;
		Z /= pVal;

		return *this;
	}

	inline float CVector3F::operator[](uint32_t puiIndex) const
	{
		return *(&X + puiIndex);
	}

	inline bool CVector3F::operator==(const CVector3F& pV) const
	{
		return X == pV.X && Y == pV.Y && Z == pV.Z;
	}

	inline bool CVector3F::operator!=(const CVector3F& pV) const
	{
		return X != pV.X || Y != pV.Y || Z != pV.Z;
	}
}