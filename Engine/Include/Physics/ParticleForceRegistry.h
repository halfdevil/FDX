#pragma once

#include "Physics/ParticleForce.h"

namespace FDX
{
	class FDX_EXPORT CParticleForceRegistry
	{
	public:

		CParticleForceRegistry();
		~CParticleForceRegistry();

		void Add(CParticle *pParticle, IParticleForce *pForce);
		void Remove(CParticle *pParticle, IParticleForce *pForce);

		void Clear();
		void UpdateForces(float pfDuration);

	protected:

		struct SParticleForceRegistration
		{
			CParticle *Particle;
			IParticleForce *Force;
		};

		Array<SParticleForceRegistration> mRegistrations;
	};
}