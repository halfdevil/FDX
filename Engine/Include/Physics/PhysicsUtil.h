#pragma once

#include "Math/Vector3F.h"

namespace FDX
{
	class FDX_EXPORT CPhysicsUtil
	{
	public:

		static float GetSleepEpsilon();
		static void SetSleepEpsilon(float pfSleepEpsilon);

		static CVector3F GetGravity();
		static void SetGravity(const CVector3F &pGravity);

		static CVector3F GetHighGravity();
		static void SetHighGravity(const CVector3F &pGravity);

	private:

		static float mfSleepEpsilon;
		static CVector3F mGravity;
		static CVector3F mHighGravity;
	};
}