#pragma once

#include "Core/Core.h"
#include "Math/MathUtil.h"

namespace FDX
{
	class FDX_EXPORT CVector2F
	{
	public:

		inline CVector2F();
		inline CVector2F(float pfVal);
		inline CVector2F(float pfX, float pfY);

		inline float GetLength() const;
		inline float GetLengthSqr() const;
		inline float Dot(const CVector2F &pV) const;

		inline float Normalize();
		inline float* ToPtr();

		inline CVector2F Lerp(const CVector2F &pV, float pAmount) const;

		inline CVector2F operator + (const CVector2F &pV) const;
		inline CVector2F operator - (const CVector2F &pV) const;
		inline CVector2F operator * (const CVector2F &pV) const;
		inline CVector2F operator / (const CVector2F &pV) const;

		inline CVector2F& operator += (const CVector2F &pV);
		inline CVector2F& operator -= (const CVector2F &pV);
		inline CVector2F& operator *= (const CVector2F &pV);
		inline CVector2F& operator /= (const CVector2F &pV);

		inline CVector2F operator * (float pVal) const;
		inline CVector2F operator / (float pVal) const;

		inline CVector2F& operator *= (float pVal);
		inline CVector2F& operator /= (float pVal);

		inline float operator[] (uint32_t puiIndex);

		inline bool operator == (const CVector2F &pV) const;
		inline bool operator != (const CVector2F &pV) const;

	public:

		float X;
		float Y;
	};	

	inline CVector2F::CVector2F()
		: X(0), Y(0)
	{
	}

	inline CVector2F::CVector2F(float pVal)
		: X(pVal), Y(pVal)
	{
	}

	inline CVector2F::CVector2F(float pX, float pY)
		: X(pX), Y(pY)
	{
	}

	inline float CVector2F::GetLength() const
	{
		return CMathUtil::Sqrt(X * X + Y * Y);
	}

	inline float CVector2F::GetLengthSqr() const
	{
		return X * X + Y * Y;
	}

	inline float CVector2F::Dot(const CVector2F &pV) const
	{
		return X * pV.X + Y * pV.Y;
	}

	inline float CVector2F::Normalize()
	{
		float lLen = CMathUtil::Sqrt(X * X + Y * Y);
		if (lLen != 0)
		{
			float lInvLen = 1 / lLen;

			X *= lInvLen;
			Y *= lInvLen;
		}

		return lLen;
	}

	inline float * CVector2F::ToPtr()
	{
		return &X;
	}

	inline CVector2F CVector2F::Lerp(const CVector2F& pV, float pAmount) const
	{
		return CVector2F(CMathUtil::Lerp(X, pV.X, pAmount),
			CMathUtil::Lerp(Y, pV.Y, pAmount));
	}

	inline CVector2F CVector2F::operator+(const CVector2F& pV) const
	{
		return CVector2F(X + pV.X, Y + pV.Y);
	}

	inline CVector2F CVector2F::operator-(const CVector2F& pV) const
	{
		return CVector2F(X - pV.X, Y - pV.Y);
	}

	inline CVector2F CVector2F::operator*(const CVector2F& pV) const
	{
		return CVector2F(X * pV.X, Y * pV.Y);
	}

	inline CVector2F CVector2F::operator/(const CVector2F& pV) const
	{
		return CVector2F(X / pV.X, Y / pV.Y);
	}

	inline CVector2F& CVector2F::operator+=(const CVector2F& pV)
	{
		X += pV.X;
		Y += pV.Y;

		return *this;
	}

	inline CVector2F& CVector2F::operator-=(const CVector2F& pV)
	{
		X -= pV.X;
		Y -= pV.Y;

		return *this;
	}

	inline CVector2F& CVector2F::operator*=(const CVector2F& pV)
	{
		X *= pV.X;
		Y *= pV.Y;

		return *this;
	}

	inline CVector2F& CVector2F::operator/=(const CVector2F& pV)
	{
		X /= pV.X;
		Y /= pV.Y;

		return *this;
	}

	inline CVector2F CVector2F::operator*(float pVal) const
	{
		return CVector2F(X * pVal, Y * pVal);
	}

	inline CVector2F CVector2F::operator/(float pVal) const
	{
		return CVector2F(X / pVal, Y / pVal);
	}

	inline CVector2F& CVector2F::operator*=(float pVal)
	{
		X *= pVal;
		Y *= pVal;

		return *this;
	}

	inline CVector2F& CVector2F::operator/=(float pVal)
	{
		X /= pVal;
		Y /= pVal;

		return *this;
	}

	inline float CVector2F::operator[](uint32_t puiIndex)
	{
		return *(&X + puiIndex);
	}

	inline bool CVector2F::operator==(const CVector2F& pV) const
	{
		return X == pV.X && Y == pV.Y;
	}

	inline bool CVector2F::operator!=(const CVector2F& pV) const
	{
		return X != pV.X || Y != pV.Y;
	}
}