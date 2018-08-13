
#include "Physics/ParticleForceRegistry.h"

namespace FDX
{
	CParticleForceRegistry::CParticleForceRegistry()
	{
	}

	CParticleForceRegistry::~CParticleForceRegistry()
	{
	}

	void CParticleForceRegistry::Add(CParticle * pParticle, IParticleForce * pForce)
	{
		SParticleForceRegistration lRegistration;
		lRegistration.Particle = pParticle;
		lRegistration.Force = pForce;

		mRegistrations.push_back(lRegistration);
	}

	void CParticleForceRegistry::Remove(CParticle * pParticle, IParticleForce * pForce)
	{
		auto lIt = mRegistrations.begin();

		while (lIt != mRegistrations.end())
		{
			const auto &lRegistration = (*lIt);
			if (lRegistration.Particle == pParticle && lRegistration.Force == pForce)
			{
				mRegistrations.erase(lIt);
				break;
			}

			lIt++;
		}
	}

	void CParticleForceRegistry::Clear()
	{
		mRegistrations.clear();
	}

	void CParticleForceRegistry::UpdateForces(float pfDuration)
	{
		for (const auto &lRegistration : mRegistrations)
			lRegistration.Force->UpdateForce(lRegistration.Particle, pfDuration);
	}
}