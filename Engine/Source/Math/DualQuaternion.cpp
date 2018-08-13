
#include "Math/DualQuaternion.h"

namespace FDX
{
	void CDualQuaternion::FromQuatTrans(const CQuaternion & pQ0, const CVector3F & pTrans)
	{
		Ordinary = pQ0;
		Dual.W = -0.5f * (pTrans.X * pQ0.X + pTrans.Y * pQ0.Y + pTrans.Z * pQ0.Z);
		Dual.X = 0.5f * (pTrans.X * pQ0.W + pTrans.Y * pQ0.Z - pTrans.Z * pQ0.Y);
		Dual.Y = 0.5f * (-pTrans.X * pQ0.Z + pTrans.Y * pQ0.W + pTrans.Z * pQ0.X);
		Dual.Z = 0.5f * (pTrans.X * pQ0.Y - pTrans.Y * pQ0.X + pTrans.Z * pQ0.W);
	}

	void CDualQuaternion::ToMatrix(CMatrix4F & pMat)
	{
		float lfLen2 = Ordinary.Dot(Ordinary);
		float lfW = Ordinary.W, lfX = Ordinary.X, lfY = Ordinary.Y, lfZ = Ordinary.Z;
		float lfT0 = Dual.W, lfT1 = Dual.X, lfT2 = Dual.Y, lfT3 = Dual.Z;

		pMat.M2[0][0] = lfW * lfW + lfX * lfX - lfY * lfY - lfZ*lfZ;
		pMat.M2[1][0] = 2 * lfX * lfY - 2 * lfW * lfZ;
		pMat.M2[2][0] = 2 * lfX * lfZ + 2 * lfW * lfY;
		pMat.M2[0][1] = 2 * lfX * lfY + 2 * lfW * lfZ;
		pMat.M2[1][1] = lfW * lfW + lfY * lfY - lfX * lfX - lfZ * lfZ;
		pMat.M2[2][1] = 2 * lfY * lfZ - 2 * lfW * lfX;
		pMat.M2[0][2] = 2 * lfX * lfZ - 2 * lfW * lfY;
		pMat.M2[1][2] = 2 * lfY * lfZ + 2 * lfW * lfX;
		pMat.M2[2][2] = lfW * lfW + lfZ * lfZ - lfX * lfX - lfY * lfY;

		pMat.M2[3][0] = -2 * lfT0 * lfX + 2 * lfW * lfT1 - 2 * lfT2 * lfZ + 2 * lfY * lfT3;
		pMat.M2[3][1] = -2 * lfT0 * lfY + 2 * lfT1 * lfZ - 2 * lfX * lfT3 + 2 * lfW * lfT2;
		pMat.M2[3][2] = -2 * lfT0 * lfZ + 2 * lfX * lfT2 + 2 * lfW * lfT3 - 2 * lfT1 * lfY;

		pMat.M2[0][3] = 0;
		pMat.M2[1][3] = 0;
		pMat.M2[2][3] = 0;
		pMat.M2[3][3] = lfLen2;
		pMat *= (1.0f /lfLen2);
	}
}