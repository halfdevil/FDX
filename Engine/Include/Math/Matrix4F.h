#pragma once

#include "Math/Vector3F.h"
#include "Math/Vector4F.h"
#include "Math/AxisAlignedBox.h"

namespace FDX
{
	class FDX_EXPORT CMatrix4F
	{
	public:

		inline CMatrix4F();
		inline CMatrix4F(const float *pM);
		inline CMatrix4F(float pM00, float pM01, float pM02, float pM03,
			float pM10, float pM11, float pM12, float pM13,
			float pM20, float pM21, float pM22, float pM23,
			float pM30, float pM31, float pM32, float pM33);

		inline float GetDeterminant() const;

		inline void Identity();
		inline void Transpose();
		inline void Invert();

		inline CVector3F Transform(const CVector3F &pV) const;
		inline CVector3F TransformInverse(const CVector3F &pV) const;
		inline CAxisAlignedBox Transform(const CAxisAlignedBox &pAabb) const;
		inline CVector3F RotateOnly(const CVector3F &pV) const;

		inline const float* ToPtr() const;

		inline CMatrix4F operator + (const CMatrix4F &pM) const;
		inline CMatrix4F operator - (const CMatrix4F &pM) const;
		inline CMatrix4F operator * (const CMatrix4F &pM) const;

		inline CMatrix4F& operator += (const CMatrix4F &pM);
		inline CMatrix4F& operator -= (const CMatrix4F &pM);
		inline CMatrix4F& operator *= (const CMatrix4F &pM);

		inline CMatrix4F operator * (float pVal) const;
		inline CMatrix4F& operator *= (float pVal);

		static CMatrix4F IdentityMatrix();
		static CMatrix4F ZeroMatrix();
		static CMatrix4F InvertMatrix(const CMatrix4F &pM);
		static CMatrix4F TransposeMatrix(const CMatrix4F &pM);

		static CMatrix4F TranslationMatrix(const CVector3F &pV);
		static CMatrix4F ScaleMatrix(const CVector3F &pV);
		static CMatrix4F RotationXMatrix(float pAngle);
		static CMatrix4F RotationYMatrix(float pAngle);
		static CMatrix4F RotationZMatrix(float pAngle);

		static CMatrix4F RotationYPRMatrix(float pYaw, float pPitch, float pRoll);
		static CMatrix4F RotationAxisAngleMatrix(const CVector3F &pAxis, float pAngle);

		static CMatrix4F PerspectiveMatrix(float pFieldOfView, float pAspectRation, 
			float pNearPlane, float pFarPlane);

		static CMatrix4F OrthogonalMatrix(float pLeft, float pTop, float pRight,
			float pBottom, float pNearPlane, float pFarPlane);

		static CMatrix4F LookAtMatrix(const CVector3F &pEye,
			const CVector3F &pTarget,
			const CVector3F &pUp);

	public:

		union
		{
			float M1[16];
			float M2[4][4];
			CVector4F M3[4];
		};
	};	

	inline CMatrix4F::CMatrix4F()
	{
	}

	inline CMatrix4F::CMatrix4F(const float * pM)
	{
		memcpy(M1, pM, sizeof(float) * 16);
	}

	inline CMatrix4F::CMatrix4F(float pM00, float pM01, float pM02, float pM03,
		float pM10, float pM11, float pM12, float pM13,
		float pM20, float pM21, float pM22, float pM23,
		float pM30, float pM31, float pM32, float pM33)
	{
		M2[0][0] = pM00; M2[0][1] = pM01; M2[0][2] = pM02; M2[0][3] = pM03;
		M2[1][0] = pM10; M2[1][1] = pM11; M2[1][2] = pM12; M2[1][3] = pM13;
		M2[2][0] = pM20; M2[2][1] = pM21; M2[2][2] = pM22; M2[2][3] = pM23;
		M2[3][0] = pM30; M2[3][1] = pM31; M2[3][2] = pM32; M2[3][3] = pM33;
	}

	inline float CMatrix4F::GetDeterminant() const
	{
		return	M2[0][0] * (
			M2[1][1] * (M2[2][2] * M2[3][3] - M2[2][3] * M2[3][2]) -
			M2[2][1] * (M2[1][2] * M2[3][3] - M2[1][3] * M2[3][2]) +
			M2[3][1] * (M2[1][2] * M2[2][3] - M2[1][3] * M2[2][2])
			) -
			M2[1][0] * (
				M2[0][1] * (M2[2][2] * M2[3][3] - M2[2][3] * M2[3][2]) -
				M2[2][1] * (M2[0][2] * M2[3][3] - M2[0][3] * M2[3][2]) +
				M2[3][1] * (M2[0][2] * M2[2][3] - M2[0][3] * M2[2][2])
				) +
			M2[2][0] * (
				M2[0][1] * (M2[1][2] * M2[3][3] - M2[1][3] * M2[3][2]) -
				M2[1][1] * (M2[0][2] * M2[3][3] - M2[0][3] * M2[3][2]) +
				M2[3][1] * (M2[0][2] * M2[1][3] - M2[0][3] * M2[1][2])
				) -
			M2[3][0] * (
				M2[0][1] * (M2[1][2] * M2[2][3] - M2[1][3] * M2[2][2]) -
				M2[1][1] * (M2[0][2] * M2[2][3] - M2[0][3] * M2[2][2]) +
				M2[2][1] * (M2[0][2] * M2[1][3] - M2[0][3] * M2[1][2])
				);
	}

	inline void CMatrix4F::Identity()
	{
		memset(M1, 0, sizeof(float) * 16);

		M2[0][0] = 1.0f;
		M2[1][1] = 1.0f;
		M2[2][2] = 1.0f;
		M2[3][3] = 1.0f;
	}

	inline void CMatrix4F::Transpose()
	{
		*this = CMatrix4F::TransposeMatrix(*this);
	}

	inline void CMatrix4F::Invert()
	{
		*this = CMatrix4F::InvertMatrix(*this);
	}

	inline CVector3F CMatrix4F::Transform(const CVector3F& pV) const
	{
		CVector3F lOut;
		float lfDenom;

		lOut.X = pV.X * M2[0][0] + pV.Y * M2[1][0] + pV.Z * M2[2][0] + M2[3][0];
		lOut.Y = pV.X * M2[0][1] + pV.Y * M2[1][1] + pV.Z * M2[2][1] + M2[3][1];
		lOut.Z = pV.X * M2[0][2] + pV.Y * M2[1][2] + pV.Z * M2[2][2] + M2[3][2];
		lfDenom = pV.X * M2[0][3] + pV.Y * M2[1][3] + pV.Z * M2[2][3] + M2[3][3];

		if (lfDenom != 1.0)
			lOut /= lfDenom;

		return lOut;
	}

	inline CVector3F CMatrix4F::TransformInverse(const CVector3F & pV) const
	{
		CVector3F lOut;
		float lfDenom;

		lOut.X = pV.X * M2[0][0] + pV.Y * M2[0][1] + pV.Z * M2[0][2] - M2[3][0];
		lOut.Y = pV.X * M2[1][0] + pV.Y * M2[1][1] + pV.Z * M2[1][2] - M2[3][1];
		lOut.Z = pV.X * M2[2][0] + pV.Y * M2[2][1] + pV.Z * M2[2][2] - M2[3][2];
		lfDenom = pV.X * M2[0][3] + pV.Y * M2[1][3] + pV.Z * M2[2][3] + M2[3][3];

		if (lfDenom != 1.0)
			lOut /= lfDenom;

		return lOut;
	}

	inline CAxisAlignedBox CMatrix4F::Transform(const CAxisAlignedBox &pAabb) const
	{
		auto lCorners = pAabb.GetCorners();

		for (auto &lCorner : lCorners)
			lCorner = Transform(lCorner);
		
		return CAxisAlignedBox::CreateFromPoints(lCorners);
	}

	inline CVector3F CMatrix4F::RotateOnly(const CVector3F& pV) const
	{
		CVector3F lOut;
		float lfDenom;

		lOut.X = pV.X * M2[0][0] + pV.Y * M2[1][0] + pV.Z * M2[2][0];
		lOut.Y = pV.X * M2[0][1] + pV.Y * M2[1][1] + pV.Z * M2[2][1];
		lOut.Z = pV.X * M2[0][2] + pV.Y * M2[1][2] + pV.Z * M2[2][2];

		lfDenom = M2[0][3] + M2[1][3] + M2[2][3] + M2[3][3];
		if (lfDenom != 1.0)
			lOut /= lfDenom;

		return lOut;
	}

	inline const float * CMatrix4F::ToPtr() const
	{
		return &M1[0];
	}

	inline CMatrix4F CMatrix4F::operator+(const CMatrix4F& pM) const
	{
		return CMatrix4F(
			M2[0][0] + pM.M2[0][0],
			M2[0][1] + pM.M2[0][1],
			M2[0][2] + pM.M2[0][2],
			M2[0][3] + pM.M2[0][3],

			M2[1][0] + pM.M2[1][0],
			M2[1][1] + pM.M2[1][1],
			M2[1][2] + pM.M2[1][2],
			M2[1][3] + pM.M2[1][3],

			M2[2][0] + pM.M2[2][0],
			M2[2][1] + pM.M2[2][1],
			M2[2][2] + pM.M2[2][2],
			M2[2][3] + pM.M2[2][3],

			M2[3][0] + pM.M2[3][0],
			M2[3][1] + pM.M2[3][1],
			M2[3][2] + pM.M2[3][2],
			M2[3][3] + pM.M2[3][3]
		);
	}

	inline CMatrix4F CMatrix4F::operator-(const CMatrix4F& pM) const
	{
		return CMatrix4F(
			M2[0][0] - pM.M2[0][0],
			M2[0][1] - pM.M2[0][1],
			M2[0][2] - pM.M2[0][2],
			M2[0][3] - pM.M2[0][3],

			M2[1][0] - pM.M2[1][0],
			M2[1][1] - pM.M2[1][1],
			M2[1][2] - pM.M2[1][2],
			M2[1][3] - pM.M2[1][3],

			M2[2][0] - pM.M2[2][0],
			M2[2][1] - pM.M2[2][1],
			M2[2][2] - pM.M2[2][2],
			M2[2][3] - pM.M2[2][3],

			M2[3][0] - pM.M2[3][0],
			M2[3][1] - pM.M2[3][1],
			M2[3][2] - pM.M2[3][2],
			M2[3][3] - pM.M2[3][3]
		);
	}

	inline CMatrix4F CMatrix4F::operator*(const CMatrix4F& pM) const
	{
		return CMatrix4F(
			M2[0][0] * pM.M2[0][0] + M2[1][0] * pM.M2[0][1] + M2[2][0] * pM.M2[0][2] + M2[3][0] * pM.M2[0][3],
			M2[0][1] * pM.M2[0][0] + M2[1][1] * pM.M2[0][1] + M2[2][1] * pM.M2[0][2] + M2[3][1] * pM.M2[0][3],
			M2[0][2] * pM.M2[0][0] + M2[1][2] * pM.M2[0][1] + M2[2][2] * pM.M2[0][2] + M2[3][2] * pM.M2[0][3],
			M2[0][3] * pM.M2[0][0] + M2[1][3] * pM.M2[0][1] + M2[2][3] * pM.M2[0][2] + M2[3][3] * pM.M2[0][3],

			M2[0][0] * pM.M2[1][0] + M2[1][0] * pM.M2[1][1] + M2[2][0] * pM.M2[1][2] + M2[3][0] * pM.M2[1][3],
			M2[0][1] * pM.M2[1][0] + M2[1][1] * pM.M2[1][1] + M2[2][1] * pM.M2[1][2] + M2[3][1] * pM.M2[1][3],
			M2[0][2] * pM.M2[1][0] + M2[1][2] * pM.M2[1][1] + M2[2][2] * pM.M2[1][2] + M2[3][2] * pM.M2[1][3],
			M2[0][3] * pM.M2[1][0] + M2[1][3] * pM.M2[1][1] + M2[2][3] * pM.M2[1][2] + M2[3][3] * pM.M2[1][3],

			M2[0][0] * pM.M2[2][0] + M2[1][0] * pM.M2[2][1] + M2[2][0] * pM.M2[2][2] + M2[3][0] * pM.M2[2][3],
			M2[0][1] * pM.M2[2][0] + M2[1][1] * pM.M2[2][1] + M2[2][1] * pM.M2[2][2] + M2[3][1] * pM.M2[2][3],
			M2[0][2] * pM.M2[2][0] + M2[1][2] * pM.M2[2][1] + M2[2][2] * pM.M2[2][2] + M2[3][2] * pM.M2[2][3],
			M2[0][3] * pM.M2[2][0] + M2[1][3] * pM.M2[2][1] + M2[2][3] * pM.M2[2][2] + M2[3][3] * pM.M2[2][3],

			M2[0][0] * pM.M2[3][0] + M2[1][0] * pM.M2[3][1] + M2[2][0] * pM.M2[3][2] + M2[3][0] * pM.M2[3][3],
			M2[0][1] * pM.M2[3][0] + M2[1][1] * pM.M2[3][1] + M2[2][1] * pM.M2[3][2] + M2[3][1] * pM.M2[3][3],
			M2[0][2] * pM.M2[3][0] + M2[1][2] * pM.M2[3][1] + M2[2][2] * pM.M2[3][2] + M2[3][2] * pM.M2[3][3],
			M2[0][3] * pM.M2[3][0] + M2[1][3] * pM.M2[3][1] + M2[2][3] * pM.M2[3][2] + M2[3][3] * pM.M2[3][3]
		);
	}

	inline CMatrix4F& CMatrix4F::operator+=(const CMatrix4F& pM)
	{
		M2[0][0] = M2[0][0] + pM.M2[0][0];
		M2[0][1] = M2[0][1] + pM.M2[0][1];
		M2[0][2] = M2[0][2] + pM.M2[0][2];
		M2[0][3] = M2[0][3] + pM.M2[0][3];

		M2[1][0] = M2[1][0] + pM.M2[1][0];
		M2[1][1] = M2[1][1] + pM.M2[1][1];
		M2[1][2] = M2[1][2] + pM.M2[1][2];
		M2[1][3] = M2[1][3] + pM.M2[1][3];

		M2[2][0] = M2[2][0] + pM.M2[2][0];
		M2[2][1] = M2[2][1] + pM.M2[2][1];
		M2[2][2] = M2[2][2] + pM.M2[2][2];
		M2[2][3] = M2[2][3] + pM.M2[2][3];

		M2[3][0] = M2[3][0] + pM.M2[3][0];
		M2[3][1] = M2[3][1] + pM.M2[3][1];
		M2[3][2] = M2[3][2] + pM.M2[3][2];
		M2[3][3] = M2[3][3] + pM.M2[3][3];

		return *this;
	}

	inline CMatrix4F& CMatrix4F::operator-=(const CMatrix4F& pM)
	{
		M2[0][0] = M2[0][0] - pM.M2[0][0];
		M2[0][1] = M2[0][1] - pM.M2[0][1];
		M2[0][2] = M2[0][2] - pM.M2[0][2];
		M2[0][3] = M2[0][3] - pM.M2[0][3];

		M2[1][0] = M2[1][0] - pM.M2[1][0];
		M2[1][1] = M2[1][1] - pM.M2[1][1];
		M2[1][2] = M2[1][2] - pM.M2[1][2];
		M2[1][3] = M2[1][3] - pM.M2[1][3];

		M2[2][0] = M2[2][0] - pM.M2[2][0];
		M2[2][1] = M2[2][1] - pM.M2[2][1];
		M2[2][2] = M2[2][2] - pM.M2[2][2];
		M2[2][3] = M2[2][3] - pM.M2[2][3];

		M2[3][0] = M2[3][0] - pM.M2[3][0];
		M2[3][1] = M2[3][1] - pM.M2[3][1];
		M2[3][2] = M2[3][2] - pM.M2[3][2];
		M2[3][3] = M2[3][3] - pM.M2[3][3];

		return *this;
	}

	inline CMatrix4F& CMatrix4F::operator*=(const CMatrix4F& pM)
	{
		*this = *this * pM;
		return *this;
	}

	inline CMatrix4F CMatrix4F::operator*(float pVal) const
	{
		return CMatrix4F(
			M2[0][0] * pVal,
			M2[0][1] * pVal,
			M2[0][2] * pVal,
			M2[0][3] * pVal,

			M2[1][0] * pVal,
			M2[1][1] * pVal,
			M2[1][2] * pVal,
			M2[1][3] * pVal,

			M2[2][0] * pVal,
			M2[2][1] * pVal,
			M2[2][2] * pVal,
			M2[2][3] * pVal,

			M2[3][0] * pVal,
			M2[3][1] * pVal,
			M2[3][2] * pVal,
			M2[3][3] * pVal
		);
	}

	inline CMatrix4F& CMatrix4F::operator*=(float pVal)
	{
		M2[0][0] = M2[0][0] * pVal;
		M2[0][1] = M2[0][1] * pVal;
		M2[0][2] = M2[0][2] * pVal;
		M2[0][3] = M2[0][3] * pVal;

		M2[1][0] = M2[1][0] * pVal;
		M2[1][1] = M2[1][1] * pVal;
		M2[1][2] = M2[1][2] * pVal;
		M2[1][3] = M2[1][3] * pVal;

		M2[2][0] = M2[2][0] * pVal;
		M2[2][1] = M2[2][1] * pVal;
		M2[2][2] = M2[2][2] * pVal;
		M2[2][3] = M2[2][3] * pVal;

		M2[3][0] = M2[3][0] * pVal;
		M2[3][1] = M2[3][1] * pVal;
		M2[3][2] = M2[3][2] * pVal;
		M2[3][3] = M2[3][3] * pVal;

		return *this;
	}
}