
#include "Math/Matrix3F.h"

namespace FDX
{
	CMatrix3F CMatrix3F::FromMatrix4(const CMatrix4F & pMat)
	{
		return CMatrix3F(pMat.M2[0][0], pMat.M2[0][1], pMat.M2[0][2],
			pMat.M2[1][0], pMat.M2[1][1], pMat.M2[1][2], 
			pMat.M2[2][0], pMat.M2[2][1], pMat.M2[2][2]);
	}

	CMatrix3F CMatrix3F::IdentityMatrix()
	{
		return CMatrix3F(
			1, 0, 0, 
			0, 1, 0, 
			0, 0, 1 
		);
	}

	CMatrix3F CMatrix3F::ZeroMatrix()
	{
		return CMatrix3F(
			0, 0, 0, 
			0, 0, 0, 
			0, 0, 0 
		);
	}

	CMatrix3F CMatrix3F::InvertMatrix(const CMatrix3F & pM)
	{
		float lfDet = pM.GetDeterminant();
		if (lfDet == 0)
			return CMatrix3F();

		float lfInvDet = 1.0f / lfDet;

		return CMatrix3F(
			lfInvDet * (pM.M2[1][1] * pM.M2[2][2] - pM.M2[1][2] * pM.M2[2][1]),
			lfInvDet * (pM.M2[0][2] * pM.M2[2][1] - pM.M2[0][1] * pM.M2[2][2]),
			lfInvDet * (pM.M2[0][1] * pM.M2[1][2] - pM.M2[0][2] * pM.M2[1][1]),
			lfInvDet * (pM.M2[1][2] * pM.M2[2][0] - pM.M2[1][0] * pM.M2[2][2]),
			lfInvDet * (pM.M2[0][0] * pM.M2[2][2] - pM.M2[0][2] * pM.M2[2][0]),
			lfInvDet * (pM.M2[0][2] * pM.M2[1][0] - pM.M2[0][0] * pM.M2[1][2]),
			lfInvDet * (pM.M2[1][0] * pM.M2[2][1] - pM.M2[1][1] * pM.M2[2][0]),
			lfInvDet * (pM.M2[0][1] * pM.M2[2][0] - pM.M2[0][0] * pM.M2[2][1]),
			lfInvDet * (pM.M2[0][0] * pM.M2[1][1] - pM.M2[0][1] * pM.M2[1][0])
		);
	}

	CMatrix3F CMatrix3F::TransposeMatrix(const CMatrix3F & pM)
	{
		return CMatrix3F(
			pM.M2[0][0],
			pM.M2[1][0],
			pM.M2[2][0],

			pM.M2[0][1],
			pM.M2[1][1],
			pM.M2[2][1],

			pM.M2[0][2],
			pM.M2[1][2],
			pM.M2[2][2]
		);
	}

	CMatrix3F CMatrix3F::InvertTransposeMatrix(const CMatrix3F & pM)
	{
		float lfDet = pM.M2[0][0] * (pM.M2[1][1] * pM.M2[2][2] - pM.M2[1][2] * pM.M2[2][1]) -
			pM.M2[0][1] * (pM.M2[1][0] * pM.M2[2][2] - pM.M2[2][0] * pM.M2[1][2]) +
			pM.M2[0][2] * (pM.M2[1][0] * pM.M2[2][1] - pM.M2[2][0] * pM.M2[1][1]);

		if (lfDet == 0)
			return CMatrix3F();

		float lfInvDet = 1.0f / lfDet;

		return CMatrix3F(
			lfInvDet * (pM.M2[1][1] * pM.M2[2][2] - pM.M2[1][2] * pM.M2[2][1]),
			lfInvDet * (pM.M2[2][0] * pM.M2[1][2] - pM.M2[1][0] * pM.M2[2][2]),
			lfInvDet * (pM.M2[1][0] * pM.M2[2][1] - pM.M2[2][0] * pM.M2[1][1]),
			lfInvDet * (pM.M2[2][1] * pM.M2[0][2] - pM.M2[0][1] * pM.M2[2][2]),
			lfInvDet * (pM.M2[0][0] * pM.M2[2][2] - pM.M2[2][0] * pM.M2[0][2]),
			lfInvDet * (pM.M2[2][0] * pM.M2[0][1] - pM.M2[0][0] * pM.M2[2][1]),
			lfInvDet * (pM.M2[0][1] * pM.M2[1][2] - pM.M2[1][1] * pM.M2[0][2]),
			lfInvDet * (pM.M2[1][0] * pM.M2[0][2] - pM.M2[0][0] * pM.M2[1][2]),
			lfInvDet * (pM.M2[0][0] * pM.M2[1][1] - pM.M2[1][0] * pM.M2[0][1])
		);
	}
}