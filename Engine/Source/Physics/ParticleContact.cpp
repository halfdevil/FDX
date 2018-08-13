
#include "Physics/ParticleContact.h"
#include "Physics/ParticleContactResolver.h"

namespace FDX
{	
	CParticleContact::CParticleContact()		
	{
		mParticle[0] = mParticle[1] = nullptr;
	}

	CParticleContact::~CParticleContact()
	{
	}

	void CParticleContact::Resolve(float pfDuration)
	{
		ResolveVelocity(pfDuration);
		ResolveInterpenetration(pfDuration);
	}

	float CParticleContact::CalculateSeperatingVelocity()
	{
		CVector3F lRelativeVelocity = mParticle[0]->GetVelocity();
		if (mParticle[1])
			lRelativeVelocity -= mParticle[1]->GetVelocity();

		return lRelativeVelocity.Dot(mContactNormal);
	}

	void CParticleContact::ResolveVelocity(float pfDuration)
	{
		float lfSepVelocity = CalculateSeperatingVelocity();

		if (lfSepVelocity > 0)
			return;

		float lfNewSepVelocity = -lfSepVelocity * mfRestitution;

		CVector3F lAccCausedVelocity = mParticle[0]->GetAcceleration();
		if (mParticle[1])
			lAccCausedVelocity -= mParticle[1]->GetAcceleration();

		float lfAccCausedSepVelocity = lAccCausedVelocity.Dot(mContactNormal) * pfDuration;
		if (lfAccCausedSepVelocity < 0)
		{
			lfNewSepVelocity += mfRestitution * lfAccCausedSepVelocity;

			if (lfNewSepVelocity < 0)
				lfNewSepVelocity = 0;
		}

		float lfDeltaVelocity = lfNewSepVelocity - lfSepVelocity;

		float lfTotalInvMass = mParticle[0]->GetInverseMass();
		if (mParticle[1])
			lfTotalInvMass -= mParticle[1]->GetInverseMass();

		if (lfTotalInvMass <= 0)
			return;

		float lfImpulse = lfDeltaVelocity / lfTotalInvMass;
		CVector3F lImpulsePerInvMass = mContactNormal * lfImpulse;

		mParticle[0]->SetVelocity(mParticle[0]->GetVelocity() +
			lImpulsePerInvMass * mParticle[0]->GetInverseMass());

		if (mParticle[1])
		{
			mParticle[1]->SetVelocity(mParticle[1]->GetVelocity() +
				lImpulsePerInvMass * -mParticle[1]->GetInverseMass());
		}
	}

	void CParticleContact::ResolveInterpenetration(float pfDuration)
	{
		if (mfPenetration <= 0)
			return;

		float lfTotalInvMass = mParticle[0]->GetInverseMass();
		if (mParticle[1])
			lfTotalInvMass -= mParticle[1]->GetInverseMass();

		if (lfTotalInvMass <= 0)
			return;

		CVector3F lMovePerInvMass = mContactNormal * 
			(mfPenetration / lfTotalInvMass);

		mParticleMovement[0] = lMovePerInvMass * mParticle[0]->GetInverseMass();
		if (mParticle[1])
			mParticleMovement[1] = lMovePerInvMass * -mParticle[1]->GetInverseMass();
		else
			mParticleMovement[1].Clear();

		mParticle[0]->SetPosition(mParticle[0]->GetPosition() + mParticleMovement[0]);
		if (mParticle[1])
			mParticle[0]->SetPosition(mParticle[1]->GetPosition() + mParticleMovement[1]);
	}
}