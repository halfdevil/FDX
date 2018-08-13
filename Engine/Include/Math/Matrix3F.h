#pragma once

#include "Math/Vector3F.h"
#include "Math/Matrix4F.h"

namespace FDX
{
	class FDX_EXPORT CMatrix3F
	{
	public:

		inline CMatrix3F();
		inline CMatrix3F(const float *pM);
		inline CMatrix3F(float pM00, float pM01, float pM02,
			float pM10, float pM11, float pM12,
			float pM20, float pM21, float pM22);

		inline float GetDeterminant() const;

		inline void Identity();
		inline void Transpose();
		inline void Invert();

		inline CVector3F Transform(const CVector3F &pV) const;
		inline CVector3F RotateOnly(const CVector3F &pV) const;

		inline const float* ToPtr() const;

		inline CMatrix3F operator + (const CMatrix3F &pM) const;
		inline CMatrix3F operator - (const CMatrix3F &pM) const;
		inline CMatrix3F operator * (const CMatrix3F &pM) const;

		inline CMatrix3F& operator += (const CMatrix3F &pM);
		inline CMatrix3F& operator -= (const CMatrix3F &pM);
		inline CMatrix3F& operator *= (const CMatrix3F &pM);

		inline CMatrix3F operator * (float pVal) const;
		inline CMatrix3F& operator *= (float pVal);

		static CMatrix3F FromMatrix4(const CMatrix4F &pMat);
		static CMatrix3F IdentityMatrix();
		static CMatrix3F ZeroMatrix();
		static CMatrix3F InvertMatrix(const CMatrix3F &pM);
		static CMatrix3F TransposeMatrix(const CMatrix3F &pM);
		static CMatrix3F InvertTransposeMatrix(const CMatrix3F &pM);

	public:

		union
		{
			float M1[9];
			float M2[3][3];
			CVector3F M3[3];
		};
	};

	inline CMatrix3F::CMatrix3F()
	{
	}

	inline CMatrix3F::CMatrix3F(const float * pM)
	{
		memcpy(M1, pM, sizeof(float) * 9);
	}

	inline CMatrix3F::CMatrix3F(float pM00, float pM01, float pM02,
		float pM10, float pM11, float pM12,
		float pM20, float pM21, float pM22)
	{
		M2[0][0] = pM00; M2[0][1] = pM01; M2[0][2] = pM02;
		M2[1][0] = pM10; M2[1][1] = pM11; M2[1][2] = pM12;
		M2[2][0] = pM20; M2[2][1] = pM21; M2[2][2] = pM22;
	}

	inline float CMatrix3F::GetDeterminant() const
	{
		return M2[0][0] * (M2[1][1] * M2[2][2] - M2[1][2] * M2[2][1]) -
			M2[1][0] * (M2[0][1] * M2[2][2] - M2[0][2] * M2[2][1]) +
			M2[2][0] * (M2[0][1] * M2[1][2] - M2[0][2] * M2[1][1]);
	}

	inline void CMatrix3F::Identity()
	{
		memset(M1, 0, sizeof(float) * 9);

		M2[0][0] = 1.0f;
		M2[1][1] = 1.0f;
		M2[2][2] = 1.0f;
	}

	inline void CMatrix3F::Transpose()
	{
		*this = CMatrix3F::TransposeMatrix(*this);
	}

	inline void CMatrix3F::Invert()
	{
		*this = CMatrix3F::InvertMatrix(*this);
	}

	inline CVector3F CMatrix3F::Transform(const CVector3F & pV) const
	{
		CVector3F lOut;

		lOut.X = pV.X * M2[0][0] + pV.Y * M2[1][0] + pV.Z * M2[2][0];
		lOut.Y = pV.X * M2[0][1] + pV.Y * M2[1][1] + pV.Z * M2[2][1];
		lOut.Z = pV.X * M2[0][2] + pV.Y * M2[1][2] + pV.Z * M2[2][2];

		return lOut;
	}

	inline CVector3F CMatrix3F::RotateOnly(const CVector3F & pV) const
	{
		CVector3F lOut;

		lOut.X = pV.X * M2[0][0] + pV.Y * M2[1][0] + pV.Z * M2[2][0];
		lOut.Y = pV.X * M2[0][1] + pV.Y * M2[1][1] + pV.Z * M2[2][1];
		lOut.Z = pV.X * M2[0][2] + pV.Y * M2[1][2] + pV.Z * M2[2][2];

		return lOut;
	}

	inline const float * CMatrix3F::ToPtr() const
	{
		return &M1[0];
	}

	inline CMatrix3F CMatrix3F::operator+(const CMatrix3F & pM) const
	{
		return CMatrix3F(
			M2[0][0] + pM.M2[0][0],
			M2[0][1] + pM.M2[0][1],
			M2[0][2] + pM.M2[0][2],

			M2[1][0] + pM.M2[1][0],
			M2[1][1] + pM.M2[1][1],
			M2[1][2] + pM.M2[1][2],

			M2[2][0] + pM.M2[2][0],
			M2[2][1] + pM.M2[2][1],
			M2[2][2] + pM.M2[2][2]
		);
	}

	inline CMatrix3F CMatrix3F::operator-(const CMatrix3F & pM) const
	{
		return CMatrix3F(
			M2[0][0] - pM.M2[0][0],
			M2[0][1] - pM.M2[0][1],
			M2[0][2] - pM.M2[0][2],

			M2[1][0] - pM.M2[1][0],
			M2[1][1] - pM.M2[1][1],
			M2[1][2] - pM.M2[1][2],

			M2[2][0] - pM.M2[2][0],
			M2[2][1] - pM.M2[2][1],
			M2[2][2] - pM.M2[2][2]
		);
	}

	inline CMatrix3F CMatrix3F::operator*(const CMatrix3F & pM) const
	{
		return CMatrix3F(
			M2[0][0] * pM.M2[0][0] + M2[1][0] * pM.M2[0][1] + M2[2][0] * pM.M2[0][2],
			M2[0][1] * pM.M2[0][0] + M2[1][1] * pM.M2[0][1] + M2[2][1] * pM.M2[0][2],
			M2[0][2] * pM.M2[0][0] + M2[1][2] * pM.M2[0][1] + M2[2][2] * pM.M2[0][2],

			M2[0][0] * pM.M2[1][0] + M2[1][0] * pM.M2[1][1] + M2[2][0] * pM.M2[1][2],
			M2[0][1] * pM.M2[1][0] + M2[1][1] * pM.M2[1][1] + M2[2][1] * pM.M2[1][2],
			M2[0][2] * pM.M2[1][0] + M2[1][2] * pM.M2[1][1] + M2[2][2] * pM.M2[1][2],

			M2[0][0] * pM.M2[2][0] + M2[1][0] * pM.M2[2][1] + M2[2][0] * pM.M2[2][2],
			M2[0][1] * pM.M2[2][0] + M2[1][1] * pM.M2[2][1] + M2[2][1] * pM.M2[2][2],
			M2[0][2] * pM.M2[2][0] + M2[1][2] * pM.M2[2][1] + M2[2][2] * pM.M2[2][2]
		);
	}

	inline CMatrix3F & CMatrix3F::operator+=(const CMatrix3F & pM)
	{
		M2[0][0] = M2[0][0] + pM.M2[0][0];
		M2[0][1] = M2[0][1] + pM.M2[0][1];
		M2[0][2] = M2[0][2] + pM.M2[0][2];

		M2[1][0] = M2[1][0] + pM.M2[1][0];
		M2[1][1] = M2[1][1] + pM.M2[1][1];
		M2[1][2] = M2[1][2] + pM.M2[1][2];

		M2[2][0] = M2[2][0] + pM.M2[2][0];
		M2[2][1] = M2[2][1] + pM.M2[2][1];
		M2[2][2] = M2[2][2] + pM.M2[2][2];

		return *this;
	}

	inline CMatrix3F & CMatrix3F::operator-=(const CMatrix3F & pM)
	{
		M2[0][0] = M2[0][0] - pM.M2[0][0];
		M2[0][1] = M2[0][1] - pM.M2[0][1];
		M2[0][2] = M2[0][2] - pM.M2[0][2];

		M2[1][0] = M2[1][0] - pM.M2[1][0];
		M2[1][1] = M2[1][1] - pM.M2[1][1];
		M2[1][2] = M2[1][2] - pM.M2[1][2];

		M2[2][0] = M2[2][0] - pM.M2[2][0];
		M2[2][1] = M2[2][1] - pM.M2[2][1];
		M2[2][2] = M2[2][2] - pM.M2[2][2];

		return *this;
	}

	inline CMatrix3F & CMatrix3F::operator*=(const CMatrix3F & pM)
	{
		*this = *this * pM;
		return *this;
	}

	inline CMatrix3F CMatrix3F::operator*(float pVal) const
	{
		return CMatrix3F(
			M2[0][0] * pVal,
			M2[0][1] * pVal,
			M2[0][2] * pVal,

			M2[1][0] * pVal,
			M2[1][1] * pVal,
			M2[1][2] * pVal,

			M2[2][0] * pVal,
			M2[2][1] * pVal,
			M2[2][2] * pVal
		);
	}

	inline CMatrix3F & CMatrix3F::operator*=(float pVal)
	{
		M2[0][0] = M2[0][0] * pVal;
		M2[0][1] = M2[0][1] * pVal;
		M2[0][2] = M2[0][2] * pVal;

		M2[1][0] = M2[1][0] * pVal;
		M2[1][1] = M2[1][1] * pVal;
		M2[1][2] = M2[1][2] * pVal;

		M2[2][0] = M2[2][0] * pVal;
		M2[2][1] = M2[2][1] * pVal;
		M2[2][2] = M2[2][2] * pVal;

		return *this;
	}
}