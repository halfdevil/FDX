#pragma once

#include "Physics/Particle.h"

namespace FDX
{
	class CParticleContactResolver;

	class FDX_EXPORT CParticleContact
	{
	public:

		friend class CParticleContactResolver;

		CParticleContact();
		~CParticleContact();

		void Resolve(float pfDuration);
		float CalculateSeperatingVelocity();

	private:

		void ResolveVelocity(float pfDuration);
		void ResolveInterpenetration(float pfDuration);

	private:

		std::array<CParticle*, 2> mParticle;
		std::array<CVector3F, 2> mParticleMovement;

		CVector3F mContactNormal;
		float mfRestitution;		
		float mfPenetration;
	};
}