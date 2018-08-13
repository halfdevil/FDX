#pragma once

#include "Core/Core.h"
#include "Math/MathUtil.h"

namespace FDX
{
	class FDX_EXPORT CVector4F
	{
	public:

		inline CVector4F();
		inline CVector4F(float pVal);
		inline CVector4F(float pX, float pY, float pZ, float pW);

		inline float GetLength() const;
		inline float GetLengthSqr() const;
		inline float Dot(const CVector4F &pV) const;

		inline float Normalize();
		inline float* ToPtr();

		inline CVector4F Lerp(const CVector4F &pV, float pAmount) const;

		inline CVector4F operator + (const CVector4F &pV) const;
		inline CVector4F operator - (const CVector4F &pV) const;
		inline CVector4F operator * (const CVector4F &pV) const;
		inline CVector4F operator / (const CVector4F &pV) const;

		inline CVector4F& operator += (const CVector4F &pV);
		inline CVector4F& operator -= (const CVector4F &pV);
		inline CVector4F& operator *= (const CVector4F &pV);
		inline CVector4F& operator /= (const CVector4F &pV);

		inline CVector4F operator * (float pVal) const;
		inline CVector4F operator / (float pVal) const;

		inline CVector4F& operator *= (float pVal);
		inline CVector4F& operator /= (float pVal);

		inline float& operator[] (uint32_t puiIndex);

		inline bool operator == (const CVector4F &pV) const;
		inline bool operator != (const CVector4F &pV) const;

	public:

		float X;
		float Y;
		float Z;
		float W;
	};

	inline CVector4F::CVector4F()
		: X(0), Y(0), Z(0), W(0)
	{
	}

	inline CVector4F::CVector4F(float pVal)
		: X(pVal), Y(pVal), Z(pVal), W(pVal)
	{
	}

	inline CVector4F::CVector4F(float pX, float pY, float pZ, float pW)
		: X(pX), Y(pY), Z(pZ), W(pW)
	{
	}

	inline float CVector4F::GetLength() const
	{
		return CMathUtil::Sqrt(X * X + Y * Y + Z * Z + W * W);
	}

	inline float CVector4F::GetLengthSqr() const
	{
		return X * X + Y * Y + Z * Z + W * W;
	}

	inline float CVector4F::Dot(const CVector4F &pV) const
	{
		return X * pV.X + Y * pV.Y + Z * pV.Z + W * pV.W;
	}

	inline float CVector4F::Normalize()
	{
		float lLen = CMathUtil::Sqrt(X * X + Y * Y + Z * Z + W * W);
		if (lLen != 0)
		{
			float lInvLen = 1 / lLen;

			X *= lInvLen;
			Y *= lInvLen;
			Z *= lInvLen;
			W *= lInvLen;
		}

		return lLen;
	}

	inline float * CVector4F::ToPtr()
	{
		return &X;
	}

	inline CVector4F CVector4F::Lerp(const CVector4F& pV, float pAmount) const
	{
		return CVector4F(CMathUtil::Lerp(X, pV.X, pAmount),
			CMathUtil::Lerp(Y, pV.Y, pAmount),
			CMathUtil::Lerp(Z, pV.Z, pAmount),
			CMathUtil::Lerp(W, pV.W, pAmount));
	}

	inline CVector4F CVector4F::operator+(const CVector4F& pV) const
	{
		return CVector4F(X + pV.X, Y + pV.Y, Z + pV.Z, W + pV.W);
	}

	inline CVector4F CVector4F::operator-(const CVector4F& pV) const
	{
		return CVector4F(X - pV.X, Y - pV.Y, Z - pV.Z, W - pV.W);
	}

	inline CVector4F CVector4F::operator*(const CVector4F& pV) const
	{
		return CVector4F(X * pV.X, Y * pV.Y, Z * pV.Z, W * pV.W);
	}

	inline CVector4F CVector4F::operator/(const CVector4F& pV) const
	{
		return CVector4F(X / pV.X, Y / pV.Y, Z / pV.Z, W / pV.W);
	}

	inline CVector4F& CVector4F::operator+=(const CVector4F& pV)
	{
		X += pV.X;
		Y += pV.Y;
		Z += pV.Z;
		W += pV.W;

		return *this;
	}

	inline CVector4F& CVector4F::operator-=(const CVector4F& pV)
	{
		X -= pV.X;
		Y -= pV.Y;
		Z -= pV.Z;
		W -= pV.W;

		return *this;
	}

	inline CVector4F& CVector4F::operator*=(const CVector4F& pV)
	{
		X *= pV.X;
		Y *= pV.Y;
		Z *= pV.Z;
		W *= pV.W;

		return *this;
	}

	inline CVector4F& CVector4F::operator/=(const CVector4F& pV)
	{
		X /= pV.X;
		Y /= pV.Y;
		Z /= pV.Z;
		W /= pV.W;

		return *this;
	}

	inline CVector4F CVector4F::operator*(float pVal) const
	{
		return CVector4F(X * pVal, Y * pVal, Z * pVal, W * pVal);
	}

	inline CVector4F CVector4F::operator/(float pVal) const
	{
		return CVector4F(X / pVal, Y / pVal, Z / pVal, W / pVal);
	}

	inline CVector4F& CVector4F::operator*=(float pVal)
	{
		X *= pVal;
		Y *= pVal;
		Z *= pVal;
		W *= pVal;

		return *this;
	}

	inline CVector4F& CVector4F::operator/=(float pVal)
	{
		X /= pVal;
		Y /= pVal;
		Z /= pVal;
		W /= pVal;

		return *this;
	}

	inline float& CVector4F::operator[](uint32_t puiIndex)
	{
		return *(&X + puiIndex);
	}

	inline bool CVector4F::operator==(const CVector4F& pV) const
	{
		return X == pV.X && Y == pV.Y && Z == pV.Z && W == pV.W;
	}

	inline bool CVector4F::operator!=(const CVector4F& pV) const
	{
		return X != pV.X || Y != pV.Y || Z != pV.Z || W != pV.W;
	}
}