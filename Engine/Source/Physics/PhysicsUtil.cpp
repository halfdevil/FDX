
#include "Physics/PhysicsUtil.h"

namespace FDX
{
	float CPhysicsUtil::mfSleepEpsilon = 0.3f;
	CVector3F CPhysicsUtil::mGravity = CVector3F(0.0f, -9.81f, 0.0f);
	CVector3F CPhysicsUtil::mHighGravity = CVector3F(0.0f, -19.62f, 0.0f);

	float CPhysicsUtil::GetSleepEpsilon()
	{
		return mfSleepEpsilon;
	}

	void CPhysicsUtil::SetSleepEpsilon(float pfSleepEpsilon)
	{
		mfSleepEpsilon = pfSleepEpsilon;
	}

	CVector3F CPhysicsUtil::GetGravity()
	{
		return mGravity;
	}

	void CPhysicsUtil::SetGravity(const CVector3F & pGravity)
	{
		mGravity = pGravity;
	}

	CVector3F CPhysicsUtil::GetHighGravity()
	{
		return mHighGravity;
	}

	void CPhysicsUtil::SetHighGravity(const CVector3F & pGravity)
	{
		mHighGravity = pGravity;
	}
}