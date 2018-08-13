
#include "Physics/ParticleContactResolver.h"

namespace FDX
{
	CParticleContactResolver::CParticleContactResolver(uint32_t puiNumIterations)
		: muiNumIterations(puiNumIterations)
	{
	}

	CParticleContactResolver::~CParticleContactResolver()
	{
	}

	void CParticleContactResolver::SetIterations(uint32_t puiNumIterations)
	{
		muiNumIterations = puiNumIterations;
	}

	void CParticleContactResolver::ResolveContacts(Array<CParticleContact*>& pContacts, float pfDuration)
	{
		uint32_t luiIterationsUsed = 0;

		while (luiIterationsUsed < muiNumIterations)
		{
			float lfMax = 999999.0f;
			CParticleContact *lMaxContact = nullptr;

			for (const auto &lContact : pContacts)
			{
				float lfSepVelocity = lContact->CalculateSeperatingVelocity();
				if (lfSepVelocity < lfMax && (lfSepVelocity < 0 || lContact->mfPenetration > 0))
				{
					lfMax = lfSepVelocity;
					lMaxContact = lContact;
				}

				if (lMaxContact == nullptr)
					break;

				lMaxContact->Resolve(pfDuration);

				const auto &lParticleMovement = lMaxContact->mParticleMovement;
				for (auto &lContact : pContacts)
				{
					if (lContact->mParticle[0] == lMaxContact->mParticle[0])
					{
						lContact->mfPenetration -= lParticleMovement[0].Dot(lContact->mContactNormal);
					}
					else if (lContact->mParticle[0] == lMaxContact->mParticle[1])
					{
						lContact->mfPenetration -= lParticleMovement[1].Dot(lContact->mContactNormal);
					}

					if (lContact->mParticle[1])
					{
						if (lContact->mParticle[1] == lMaxContact->mParticle[0])
						{
							lContact->mfPenetration += lParticleMovement[0].Dot(lContact->mContactNormal);
						}
						else if (lContact->mParticle[1] == lMaxContact->mParticle[1])
						{
							lContact->mfPenetration += lParticleMovement[1].Dot(lContact->mContactNormal);
						}
					}
				}
			}

			luiIterationsUsed++;
		}
	}
}