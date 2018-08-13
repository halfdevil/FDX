
#include "Math/Matrix4F.h"

namespace FDX
{
	CMatrix4F CMatrix4F::IdentityMatrix()
	{
		return CMatrix4F(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}

	CMatrix4F CMatrix4F::ZeroMatrix()
	{
		return CMatrix4F(
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);
	}

	CMatrix4F CMatrix4F::InvertMatrix(const CMatrix4F& pM)
	{
		float lDet = pM.GetDeterminant();
		if (lDet == 0)
			return CMatrix4F();

		float lInvDet = 1 / lDet;

		return CMatrix4F(
			lInvDet * (
				pM.M2[1][1] * (pM.M2[2][2] * pM.M2[3][3] - pM.M2[2][3] * pM.M2[3][2]) -
				pM.M2[2][1] * (pM.M2[1][2] * pM.M2[3][3] - pM.M2[1][3] * pM.M2[3][2]) +
				pM.M2[3][1] * (pM.M2[1][2] * pM.M2[2][3] - pM.M2[1][3] * pM.M2[2][2])
				),
			-lInvDet * (
				pM.M2[0][1] * (pM.M2[2][2] * pM.M2[3][3] - pM.M2[2][3] * pM.M2[3][2]) -
				pM.M2[2][1] * (pM.M2[0][2] * pM.M2[3][3] - pM.M2[0][3] * pM.M2[3][2]) +
				pM.M2[3][1] * (pM.M2[0][2] * pM.M2[2][3] - pM.M2[0][3] * pM.M2[2][2])
				),
			lInvDet * (
				pM.M2[0][1] * (pM.M2[1][2] * pM.M2[3][3] - pM.M2[1][3] * pM.M2[3][2]) -
				pM.M2[1][1] * (pM.M2[0][2] * pM.M2[3][3] - pM.M2[0][3] * pM.M2[3][2]) +
				pM.M2[3][1] * (pM.M2[0][2] * pM.M2[1][3] - pM.M2[0][3] * pM.M2[1][2])
				),
			-lInvDet * (
				pM.M2[0][1] * (pM.M2[1][2] * pM.M2[2][3] - pM.M2[1][3] * pM.M2[2][2]) -
				pM.M2[1][1] * (pM.M2[0][2] * pM.M2[2][3] - pM.M2[0][3] * pM.M2[2][2]) +
				pM.M2[2][1] * (pM.M2[0][2] * pM.M2[1][3] - pM.M2[0][3] * pM.M2[1][2])
				),

			-lInvDet * (
				pM.M2[1][0] * (pM.M2[2][2] * pM.M2[3][3] - pM.M2[2][3] * pM.M2[3][2]) -
				pM.M2[2][0] * (pM.M2[1][2] * pM.M2[3][3] - pM.M2[1][3] * pM.M2[3][2]) +
				pM.M2[3][0] * (pM.M2[1][2] * pM.M2[2][3] - pM.M2[1][3] * pM.M2[2][2])
				),
			lInvDet * (
				pM.M2[0][0] * (pM.M2[2][2] * pM.M2[3][3] - pM.M2[2][3] * pM.M2[3][2]) -
				pM.M2[2][0] * (pM.M2[0][2] * pM.M2[3][3] - pM.M2[0][3] * pM.M2[3][2]) +
				pM.M2[3][0] * (pM.M2[0][2] * pM.M2[2][3] - pM.M2[0][3] * pM.M2[2][2])
				),
			-lInvDet * (
				pM.M2[0][0] * (pM.M2[1][2] * pM.M2[3][3] - pM.M2[1][3] * pM.M2[3][2]) -
				pM.M2[1][0] * (pM.M2[0][2] * pM.M2[3][3] - pM.M2[0][3] * pM.M2[3][2]) +
				pM.M2[3][0] * (pM.M2[0][2] * pM.M2[1][3] - pM.M2[0][3] * pM.M2[1][2])
				),
			lInvDet * (
				pM.M2[0][0] * (pM.M2[1][2] * pM.M2[2][3] - pM.M2[1][3] * pM.M2[2][2]) -
				pM.M2[1][0] * (pM.M2[0][2] * pM.M2[2][3] - pM.M2[0][3] * pM.M2[2][2]) +
				pM.M2[2][0] * (pM.M2[0][2] * pM.M2[1][3] - pM.M2[0][3] * pM.M2[1][2])
				),

			lInvDet * (
				pM.M2[1][0] * (pM.M2[2][1] * pM.M2[3][3] - pM.M2[2][3] * pM.M2[3][1]) -
				pM.M2[2][0] * (pM.M2[1][1] * pM.M2[3][3] - pM.M2[1][3] * pM.M2[3][1]) +
				pM.M2[3][0] * (pM.M2[1][1] * pM.M2[2][3] - pM.M2[1][3] * pM.M2[2][1])
				),
			-lInvDet * (
				pM.M2[0][0] * (pM.M2[2][1] * pM.M2[3][3] - pM.M2[2][3] * pM.M2[3][1]) -
				pM.M2[2][0] * (pM.M2[0][1] * pM.M2[3][3] - pM.M2[0][3] * pM.M2[3][1]) +
				pM.M2[3][0] * (pM.M2[0][1] * pM.M2[2][3] - pM.M2[0][3] * pM.M2[2][1])
				),
			lInvDet * (
				pM.M2[0][0] * (pM.M2[1][1] * pM.M2[3][3] - pM.M2[1][3] * pM.M2[3][1]) -
				pM.M2[1][0] * (pM.M2[0][1] * pM.M2[3][3] - pM.M2[0][3] * pM.M2[3][1]) +
				pM.M2[3][0] * (pM.M2[0][1] * pM.M2[1][3] - pM.M2[0][3] * pM.M2[1][1])
				),
			-lInvDet * (
				pM.M2[0][0] * (pM.M2[1][1] * pM.M2[2][3] - pM.M2[1][3] * pM.M2[2][1]) -
				pM.M2[1][0] * (pM.M2[0][1] * pM.M2[2][3] - pM.M2[0][3] * pM.M2[2][1]) +
				pM.M2[2][0] * (pM.M2[0][1] * pM.M2[1][3] - pM.M2[0][3] * pM.M2[1][1])
				),

			-lInvDet * (
				pM.M2[1][0] * (pM.M2[2][1] * pM.M2[3][2] - pM.M2[2][2] * pM.M2[3][1]) -
				pM.M2[2][0] * (pM.M2[1][1] * pM.M2[3][2] - pM.M2[1][2] * pM.M2[3][1]) +
				pM.M2[3][0] * (pM.M2[1][1] * pM.M2[2][2] - pM.M2[1][2] * pM.M2[2][1])
				),
			lInvDet * (
				pM.M2[0][0] * (pM.M2[2][1] * pM.M2[3][2] - pM.M2[2][2] * pM.M2[3][1]) -
				pM.M2[2][0] * (pM.M2[0][1] * pM.M2[3][2] - pM.M2[0][2] * pM.M2[3][1]) +
				pM.M2[3][0] * (pM.M2[0][1] * pM.M2[2][2] - pM.M2[0][2] * pM.M2[2][1])
				),
			-lInvDet * (
				pM.M2[0][0] * (pM.M2[1][1] * pM.M2[3][2] - pM.M2[1][2] * pM.M2[3][1]) -
				pM.M2[1][0] * (pM.M2[0][1] * pM.M2[3][2] - pM.M2[0][2] * pM.M2[3][1]) +
				pM.M2[3][0] * (pM.M2[0][1] * pM.M2[1][2] - pM.M2[0][2] * pM.M2[1][1])
				),
			lInvDet * (
				pM.M2[0][0] * (pM.M2[1][1] * pM.M2[2][2] - pM.M2[1][2] * pM.M2[2][1]) -
				pM.M2[1][0] * (pM.M2[0][1] * pM.M2[2][2] - pM.M2[0][2] * pM.M2[2][1]) +
				pM.M2[2][0] * (pM.M2[0][1] * pM.M2[1][2] - pM.M2[0][2] * pM.M2[1][1])
				)
		);
	}

	CMatrix4F CMatrix4F::TransposeMatrix(const CMatrix4F& pM)
	{
		return CMatrix4F(
			pM.M2[0][0],
			pM.M2[1][0],
			pM.M2[2][0],
			pM.M2[3][0],

			pM.M2[0][1],
			pM.M2[1][1],
			pM.M2[2][1],
			pM.M2[3][1],

			pM.M2[0][2],
			pM.M2[1][2],
			pM.M2[2][2],
			pM.M2[3][2],

			pM.M2[0][3],
			pM.M2[1][3],
			pM.M2[2][3],
			pM.M2[3][3]
		);
	}

	CMatrix4F CMatrix4F::TranslationMatrix(const CVector3F& pV)
	{
		return CMatrix4F(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			pV.X, pV.Y, pV.Z, 1
		);
	}

	CMatrix4F CMatrix4F::ScaleMatrix(const CVector3F& pV)
	{
		return CMatrix4F(
			pV.X, 0, 0, 0,
			0, pV.Y, 0, 0,
			0, 0, pV.Z, 0,
			0, 0, 0, 1
		);
	}

	CMatrix4F CMatrix4F::RotationXMatrix(float pAngle)
	{
		float lCos = CMathUtil::Cos(pAngle);
		float lSin = CMathUtil::Sin(pAngle);

		return CMatrix4F(
			1, 0, 0, 0,
			0, lCos, lSin, 0,
			0, -lSin, lCos, 0,
			0, 0, 0, 1
		);
	}

	CMatrix4F CMatrix4F::RotationYMatrix(float pAngle)
	{
		float lCos = CMathUtil::Cos(pAngle);
		float lSin = CMathUtil::Sin(pAngle);

		return CMatrix4F(
			lCos, 0, lSin, 0,
			0, 1, 0, 0,
			-lSin, 0, lCos, 0,
			0, 0, 0, 1
		);
	}

	CMatrix4F CMatrix4F::RotationZMatrix(float pAngle)
	{
		float lCos = CMathUtil::Cos(pAngle);
		float lSin = CMathUtil::Sin(pAngle);

		return CMatrix4F(
			lCos, lSin, 0, 0,
			-lSin, lCos, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}

	CMatrix4F CMatrix4F::RotationYPRMatrix(float pYaw, float pPitch, float pRoll)
	{
		float lCh = CMathUtil::Cos(pYaw);
		float lSh = CMathUtil::Sin(pYaw);
		float lCp = CMathUtil::Cos(pPitch);
		float lSp = CMathUtil::Sin(pPitch);
		float lCr = CMathUtil::Cos(pRoll);
		float lSr = CMathUtil::Sin(pRoll);

		return CMatrix4F(
			lCh * lCr + lSh * lSp * lSr,
			lSr * lCp,
			-lSh * lCr - lCh * lSp * lSr,
			0,

			-lCh * lSr + lSh * lSp * lCr,
			lCr * lCp,
			lSr * lSh + lCh * lSp * lCr,
			0,

			lSh * lCp,
			-lSp,
			lCh * lCp,
			0,

			0,
			0,
			0,
			0
		);
	}

	CMatrix4F CMatrix4F::RotationAxisAngleMatrix(const CVector3F& pAxis, float pAngle)
	{
		float lSin = CMathUtil::Sin(pAngle);
		float lCos = CMathUtil::Cos(pAngle);
		float lCc = 1 - lCos;

		return CMatrix4F(
			pAxis.X * pAxis.X + (1.0f - pAxis.X * pAxis.X) * lCos,
			pAxis.X * pAxis.Y * lCc + pAxis.Z * lSin,
			pAxis.X * pAxis.Z * lCc - pAxis.Y * lSin,
			0,

			pAxis.X * pAxis.Y * lCc - pAxis.Z * lSin,
			pAxis.Y * pAxis.Y + (1.0f - pAxis.Y * pAxis.Y) * lCos,
			pAxis.Y * pAxis.Z * lCc + pAxis.X * lSin,
			0,

			pAxis.X * pAxis.Z * lCc + pAxis.Y * lSin,
			pAxis.Y * pAxis.Z * lCc - pAxis.X * lSin,
			pAxis.Z * pAxis.Z + (1.0f - pAxis.Z * pAxis.Z) * lCos,
			0,

			0,
			0,
			0,
			1
		);
	}

	CMatrix4F CMatrix4F::PerspectiveMatrix(float pFieldOfView, float pAspectRatio, float pNearPlane, float pFarPlane)
	{
		float lH = CMathUtil::Cos(pFieldOfView * 0.5f) / CMathUtil::Sin(pFieldOfView * 0.5f);
		float lW = lH / pAspectRatio;

		return CMatrix4F(
			lW,	0, 0, 0,
			0, lH, 0, 0,
			0, 0, -(pFarPlane + pNearPlane) / (pFarPlane - pNearPlane), -1,
			0, 0, -(2 * pFarPlane * pNearPlane) / (pFarPlane - pNearPlane),	0
		);
	}

	CMatrix4F CMatrix4F::OrthogonalMatrix(float pLeft, float pTop, float pRight, float pBottom, float pNearPlane, float pFarPlane)
	{
		return CMatrix4F(
			2 / (pRight - pLeft), 0, 0, 0,
			0, 2 / (pTop - pBottom), 0, 0,
			0, 0, -2 / (pFarPlane - pNearPlane), 0,
			-(pRight + pLeft) / (pRight - pLeft), -(pTop + pBottom) / (pTop - pBottom), -(pFarPlane + pNearPlane) / (pFarPlane - pNearPlane), 1
		);
	}

	CMatrix4F CMatrix4F::LookAtMatrix(const CVector3F& pEye, const CVector3F& pTarget, const CVector3F& pUp)
	{
		CVector3F lFront = pTarget - pEye;
		lFront.Normalize();

		CVector3F lSide = lFront.Cross(pUp);
		lSide.Normalize();

		CVector3F lUp = lSide.Cross(lFront);

		return CMatrix4F(
			lSide.X, lUp.X, -lFront.X, 0,
			lSide.Y, lUp.Y, -lFront.Y, 0,
			lSide.Z, lUp.Z, -lFront.Z, 0,
			-lSide.Dot(pEye), -lUp.Dot(pEye), lFront.Dot(pEye), 1
		);
	}
}