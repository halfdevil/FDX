#pragma once

#include "Math/Quaternion.h"

namespace FDX
{
	class FDX_EXPORT CDualQuaternion
	{
	public:

		inline CDualQuaternion();
		inline CDualQuaternion(const CQuaternion &pOrdinary, const CQuaternion &pDual);
		inline CDualQuaternion(const CQuaternion &pQ0, const CVector3F &pTrans);

		void FromQuatTrans(const CQuaternion &pQ0, const CVector3F &pTrans);
		void ToMatrix(CMatrix4F &pMat);

	public:

		CQuaternion Ordinary;
		CQuaternion Dual;
	};

	inline CDualQuaternion::CDualQuaternion()
	{
	}

	inline CDualQuaternion::CDualQuaternion(const CQuaternion & pOrdinary, const CQuaternion & pDual)
		: Ordinary(pOrdinary), Dual(pDual)
	{
	}

	inline CDualQuaternion::CDualQuaternion(const CQuaternion & pQ0, const CVector3F & pTrans)
	{
		FromQuatTrans(pQ0, pTrans);
	}
}