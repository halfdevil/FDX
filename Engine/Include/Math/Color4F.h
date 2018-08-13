#pragma once

#include "Core/Core.h"
#include "Math/MathUtil.h"

namespace FDX
{
	class FDX_EXPORT CColor4F
	{
	public:

		inline CColor4F();
		inline explicit CColor4F(float pfR, float pfG, float pfB, float pfA = 1.0f);
		inline explicit CColor4F(uint8_t puiR, uint8_t puiG, uint8_t puiB, uint8_t puiA = 255);

		inline uint32_t ToUInt() const;
		inline uint32_t ToArgb() const;
		inline uint32_t ToBgra() const;

		inline void FromUInt(uint32_t puiC);
		inline CColor4F Lerp(const CColor4F &pC, float pfVal);

		inline CColor4F operator + (const CColor4F &pC) const;
		inline CColor4F operator - (const CColor4F &pC) const;
		inline CColor4F operator * (float pfVal) const;
		inline CColor4F operator / (float pfVal) const;

		inline CColor4F& operator += (const CColor4F &pC);
		inline CColor4F& operator -= (const CColor4F &pC);
		inline CColor4F& operator *= (float pfVal);

		inline bool operator == (const CColor4F &pC) const;
		inline bool operator != (const CColor4F &pC) const;

	public:

		float R;
		float G;
		float B;
		float A;
	};

	inline CColor4F::CColor4F()
		: R(0.0f), G(0.0f), B(0.0f), A(1.0f)
	{
	}

	inline CColor4F::CColor4F(uint8_t puiR, uint8_t puiG, uint8_t puiB, uint8_t puiA)
		: R(puiR / 255.0f), G(puiG / 255.0f), B(puiB / 255.0f), A(puiA / 255.0f)
	{
	}

	inline CColor4F::CColor4F(float pfR, float pfG, float pfB, float pfA)
		: R(pfR), G(pfG), B(pfB), A(pfA)
	{
	}

	inline uint32_t CColor4F::ToUInt() const
	{
		return ((uint8_t)(R * 255.0f)) << 24 |
			((uint8_t)(G * 255.0f)) << 16 |
			((uint8_t)(B * 255.0f)) << 8 |
			((uint8_t)(A * 255.0f));
	}

	inline uint32_t CColor4F::ToArgb() const
	{
		return ((uint8_t)(A * 255.0f)) << 24 |
			((uint8_t)(R * 255.0f)) << 16 |
			((uint8_t)(G * 255.0f)) << 8 |
			((uint8_t)(B * 255.0f));
	}

	inline uint32_t CColor4F::ToBgra() const
	{
		return ((uint8_t)(B * 255.0f)) << 24 |
			((uint8_t)(G * 255.0f)) << 16 |
			((uint8_t)(R * 255.0f)) << 8 |
			((uint8_t)(A * 255.0f));
	}

	inline void CColor4F::FromUInt(uint32_t puiC)
	{
		R = (float)((puiC & 0xFF000000) >> 24) / 255.0f;
		G = (float)((puiC & 0x00FF0000) >> 16) / 255.0f;
		B = (float)((puiC & 0x0000FF00) >> 8) / 255.0f;
		A = (float)((puiC & 0x000000FF)) / 255.0f;
	}

	inline CColor4F CColor4F::Lerp(const CColor4F & pC, float pfVal)
	{
		return CColor4F(CMathUtil::Lerp(R, pC.R, pfVal),
			CMathUtil::Lerp(G, pC.G, pfVal),
			CMathUtil::Lerp(B, pC.B, pfVal),
			CMathUtil::Lerp(A, pC.A, pfVal));
	}

	inline CColor4F CColor4F::operator+(const CColor4F & pC) const
	{
		return CColor4F(R + pC.R, G + pC.G, B + pC.B, A + pC.A);
	}

	inline CColor4F CColor4F::operator-(const CColor4F & pC) const
	{
		return CColor4F(R - pC.R, G - pC.G, B - pC.B, A - pC.A);
	}

	inline CColor4F CColor4F::operator*(float pfVal) const
	{
		return CColor4F(R * pfVal, G * pfVal, B * pfVal, A * pfVal);
	}

	inline CColor4F CColor4F::operator/(float pfVal) const
	{
		return CColor4F(R / pfVal, G / pfVal, B / pfVal, A / pfVal);
	}

	inline CColor4F & CColor4F::operator+=(const CColor4F & pC)
	{
		R += pC.R;
		G += pC.G;
		B += pC.B;
		A += pC.A;

		return *this;
	}

	inline CColor4F & CColor4F::operator-=(const CColor4F & pC)
	{
		R -= pC.R;
		G -= pC.G;
		B -= pC.B;
		A -= pC.A;

		return *this;
	}

	inline CColor4F & CColor4F::operator*=(float pfVal)
	{
		R *= pfVal;
		G *= pfVal;
		B *= pfVal;
		A *= pfVal;

		return *this;
	}

	inline bool CColor4F::operator==(const CColor4F & pC) const
	{
		return R == pC.R && G == pC.G && B == pC.B && A == pC.A;
	}

	inline bool CColor4F::operator!=(const CColor4F & pC) const
	{
		return R != pC.R || G != pC.G || B != pC.B || A != pC.A;
	}
}