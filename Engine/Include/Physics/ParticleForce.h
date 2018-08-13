#pragma once

#include "Physics/Particle.h"

namespace FDX
{
	class FDX_EXPORT IParticleForce
	{
	public:

		virtual ~IParticleForce()
		{
		}

		virtual void UpdateForce(CParticle *pParticle, float pfDuration) = 0;
	};

	class FDX_EXPORT CParticleForceGravity : public IParticleForce
	{
	public:

		CParticleForceGravity(const CVector3F &pGravity);
		~CParticleForceGravity();

		const CVector3F& GetGravity() const
		{
			return mGravity;
		}

		void SetGravity(const CVector3F &pGravity);
		void UpdateForce(CParticle *pParticle, float pfDuration);

	private:

		CVector3F mGravity;
	};

	class FDX_EXPORT CParticleForceDrag : public IParticleForce
	{
	public:

		CParticleForceDrag(float pfK1, float pfK2);
		~CParticleForceDrag();

		float GetK1() const
		{
			return mfK1;
		}

		float GetK2() const
		{
			return mfK2;
		}

		void SetK1(float pfK1);
		void SetK2(float pfK2);

		void UpdateForce(CParticle *pParticle, float pfDuration);

	private:

		float mfK1;
		float mfK2;
	};
}