
#include "Physics/Particle.h"

namespace FDX
{
	CParticle::CParticle()
		: mfInverseMass(0.0f),
		mfDamping(0.0f)
	{
	}

	CParticle::~CParticle()
	{
	}

	inline bool CParticle::HasFinitieMass()
	{
		return mfInverseMass >= 0.0f;
	}

	inline float CParticle::GetMass() const
	{
		if (mfInverseMass == 0.0f)
			return 999999.0f;

		return 1.0f / mfInverseMass;
	}

	inline void CParticle::SetMass(float pfMass)
	{
		mfInverseMass = 1.0f / pfMass;
	}

	inline void CParticle::SetInverseMass(float pfInvMass)
	{
		mfInverseMass = pfInvMass;
	}

	inline void CParticle::SetDamping(float pfDamping)
	{
		mfDamping = pfDamping;
	}

	inline void CParticle::SetPosition(const CVector3F & pPosition)
	{
		mPosition = pPosition;
	}

	inline void CParticle::SetPosition(float pfX, float pfY, float pfZ)
	{
		mPosition.Set(pfX, pfY, pfZ);
	}

	inline void CParticle::SetVelocity(const CVector3F & pVelocity)
	{
		mVeclocity = pVelocity;
	}

	inline void CParticle::SetVelocity(float pfX, float pfY, float pfZ)
	{
		mVeclocity.Set(pfX, pfY, pfZ);
	}

	inline void CParticle::SetAcceleration(const CVector3F & pAcceleration)
	{
		mAcceleration = pAcceleration;
	}

	inline void CParticle::SetAcceleration(float pfX, float pfY, float pfZ)
	{
		mAcceleration.Set(pfX, pfY, pfZ);
	}

	void CParticle::ClearAccumulator()
	{
		mAccumForce.Clear();
	}

	void CParticle::AddForce(const CVector3F & pForce)
	{
		mAccumForce += pForce;
	}

	void CParticle::Update(float pfDuration)
	{
		if (mfInverseMass <= 0.0f)
			return;

		mPosition += mVeclocity * pfDuration;

		CVector3F lResultingAcc = mAcceleration;
		lResultingAcc += mAccumForce * mfInverseMass;

		mVeclocity += lResultingAcc * pfDuration;
		mVeclocity *= CMathUtil::Pow(mfDamping, pfDuration);

		ClearAccumulator();
	}
}