
#include "Math/Quaternion.h"

namespace FDX
{
	CQuaternion CQuaternion::FromEulers(float pfYaw, float pfPitch, float pfRoll)
	{
		return CQuaternion(pfYaw, pfPitch, pfRoll);
	}

	CQuaternion CQuaternion::FromAxisAngle(const CVector3F & pAxis, float pfAngle)
	{
		CVector3F lAxis = pAxis;
		lAxis.Normalize();

		float lfHalfAngle = 0.5f * pfAngle;
		float lfSin = CMathUtil::Sin(lfHalfAngle);
		float lfCos = CMathUtil::Cos(lfHalfAngle);

		return CQuaternion(lAxis.X * lfSin, lAxis.Y * lfSin, lAxis.Z * lfSin, lfCos);
	}

	CQuaternion CQuaternion::FromMatrix(const CMatrix3F & pMat)
	{
		CQuaternion lQuat;

		float lfDiagonal = pMat.M2[0][0] + pMat.M2[1][1] + pMat.M2[2][2];
		if (lfDiagonal > 0)
		{
			float w4 = (float)(CMathUtil::Sqrt(lfDiagonal + 1.0f) * 2.0f);
			lQuat.W = w4 / 4.0f;
			lQuat.X = (pMat.M2[2][1] - pMat.M2[1][2]) / w4;
			lQuat.Y = (pMat.M2[0][2] - pMat.M2[2][0]) / w4;
			lQuat.Z = (pMat.M2[1][0] - pMat.M2[0][1]) / w4;
		}
		else if ((pMat.M2[0][0] > pMat.M2[1][1]) && (pMat.M2[0][0] > pMat.M2[2][2]))
		{
			float x4 = (float)(CMathUtil::Sqrt(1.0f + pMat.M2[0][0] - pMat.M2[1][1] - pMat.M2[2][2]) * 2.0f);
			lQuat.W = (pMat.M2[2][1] - pMat.M2[1][2]) / x4;
			lQuat.X = x4 / 4.0f;
			lQuat.Y = (pMat.M2[0][1] + pMat.M2[1][0]) / x4;
			lQuat.Z = (pMat.M2[0][2] + pMat.M2[2][0]) / x4;
		}
		else if (pMat.M2[1][1] > pMat.M2[2][2])
		{
			float y4 = (float)(CMathUtil::Sqrt(1.0f + pMat.M2[1][1] - pMat.M2[0][0] - pMat.M2[2][2]) * 2.0f);
			lQuat.W = (pMat.M2[0][2] - pMat.M2[2][0]) / y4;
			lQuat.X = (pMat.M2[0][1] + pMat.M2[1][0]) / y4;
			lQuat.Y = y4 / 4.0f;
			lQuat.Z = (pMat.M2[1][2] + pMat.M2[2][1]) / y4;
		}
		else
		{
			float z4 = (float)(CMathUtil::Sqrt(1.0f + pMat.M2[2][2] - pMat.M2[0][0] - pMat.M2[1][1]) * 2.0f);
			lQuat.W = (pMat.M2[1][0] - pMat.M2[0][1]) / z4;
			lQuat.X = (pMat.M2[0][2] + pMat.M2[2][0]) / z4;
			lQuat.Y = (pMat.M2[1][2] + pMat.M2[2][1]) / z4;
			lQuat.Z = z4 / 4.0f;
		}

		return lQuat;
	}

	CQuaternion CQuaternion::FromMatrix(const CMatrix4F & pMat)
	{
		CQuaternion lQuat;

		float lfDiagonal = pMat.M2[0][0] + pMat.M2[1][1] + pMat.M2[2][2];
		if (lfDiagonal > 0) 
		{
			float w4 = (float)(CMathUtil::Sqrt(lfDiagonal + 1.0f) * 2.0f);
			lQuat.W = w4 / 4.0f;
			lQuat.X = (pMat.M2[2][1] - pMat.M2[1][2]) / w4;
			lQuat.Y = (pMat.M2[0][2] - pMat.M2[2][0]) / w4;
			lQuat.Z = (pMat.M2[1][0] - pMat.M2[0][1]) / w4;
		}
		else if ((pMat.M2[0][0] > pMat.M2[1][1]) && (pMat.M2[0][0] > pMat.M2[2][2])) 
		{
			float x4 = (float)(CMathUtil::Sqrt(1.0f + pMat.M2[0][0] - pMat.M2[1][1] - pMat.M2[2][2]) * 2.0f);
			lQuat.W = (pMat.M2[2][1] - pMat.M2[1][2]) / x4;
			lQuat.X = x4 / 4.0f;
			lQuat.Y = (pMat.M2[0][1] + pMat.M2[1][0]) / x4;
			lQuat.Z = (pMat.M2[0][2] + pMat.M2[2][0]) / x4;
		}
		else if (pMat.M2[1][1] > pMat.M2[2][2]) 
		{
			float y4 = (float)(CMathUtil::Sqrt(1.0f + pMat.M2[1][1] - pMat.M2[0][0] - pMat.M2[2][2]) * 2.0f);
			lQuat.W = (pMat.M2[0][2] - pMat.M2[2][0]) / y4;
			lQuat.X = (pMat.M2[0][1] + pMat.M2[1][0]) / y4;
			lQuat.Y = y4 / 4.0f;
			lQuat.Z = (pMat.M2[1][2] + pMat.M2[2][1]) / y4;
		}
		else 
		{
			float z4 = (float)(CMathUtil::Sqrt(1.0f + pMat.M2[2][2] - pMat.M2[0][0] - pMat.M2[1][1]) * 2.0f);
			lQuat.W = (pMat.M2[1][0] - pMat.M2[0][1]) / z4;
			lQuat.X = (pMat.M2[0][2] + pMat.M2[2][0]) / z4;
			lQuat.Y = (pMat.M2[1][2] + pMat.M2[2][1]) / z4;
			lQuat.Z = z4 / 4.0f;
		}

		return lQuat;
	}
}