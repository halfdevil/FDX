#pragma once

#include "Physics/ParticleContact.h"

namespace FDX
{
	class FDX_EXPORT CParticleContactResolver
	{
	public:

		CParticleContactResolver(uint32_t puiNumIterations);
		~CParticleContactResolver();

		uint32_t GetNumIterations() const
		{
			return muiNumIterations;
		}

		void SetIterations(uint32_t puiNumIterations);
		void ResolveContacts(Array<CParticleContact*> &pContacts, float pfDuration);

	private:

		uint32_t muiNumIterations;
	};
}