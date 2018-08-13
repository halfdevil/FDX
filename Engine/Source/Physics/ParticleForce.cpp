
#include "Physics/ParticleForce.h"

namespace FDX
{
	CParticleForceGravity::CParticleForceGravity(const CVector3F & pGravity)
		: mGravity(pGravity)
	{
	}

	CParticleForceGravity::~CParticleForceGravity()
	{
	}

	void CParticleForceGravity::SetGravity(const CVector3F & pGravity)
	{
		mGravity = pGravity;
	}

	void CParticleForceGravity::UpdateForce(CParticle * pParticle, float pfDuration)
	{
		if (!pParticle->HasFinitieMass())
			return;

		pParticle->AddForce(mGravity * pParticle->GetMass());
	}

	CParticleForceDrag::CParticleForceDrag(float pfK1, float pfK2)
		: mfK1(pfK1), mfK2(pfK2)
	{
	}

	CParticleForceDrag::~CParticleForceDrag()
	{
	}

	void CParticleForceDrag::SetK1(float pfK1)
	{
		mfK1 = pfK1;
	}

	void CParticleForceDrag::SetK2(float pfK2)
	{
		mfK2 = pfK2;
	}

	void CParticleForceDrag::UpdateForce(CParticle * pParticle, float pfDuration)
	{
		CVector3F lForce = pParticle->GetVelocity();
		
		float lfDragCoeff = lForce.GetLength();
		lfDragCoeff = mfK1 * lfDragCoeff + mfK2 * lfDragCoeff * lfDragCoeff;

		lForce.Normalize();
		lForce *= -lfDragCoeff;

		pParticle->AddForce(lForce);
	}
}