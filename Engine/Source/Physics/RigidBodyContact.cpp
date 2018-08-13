
#include "Physics/RigidBodyContact.h"

namespace FDX
{
	CRigidBodyContact::CRigidBodyContact()
		: mfFriction(0.0f),
		mfRestitution(0.0f),
		mfPenetration(0.0f),
		mfDesiredDeltaVelocity(0.0f)
	{
	}

	CRigidBodyContact::~CRigidBodyContact()
	{
	}

	void CRigidBodyContact::SetBodyData(CRigidBody * pBodyOne, CRigidBody * pBodyTwo, 
		float pfFriction, float pfRestitution)
	{
		mBody[0] = pBodyOne;
		mBody[1] = pBodyTwo;
		mfFriction = pfFriction;
		mfRestitution = pfRestitution;
	}

	void CRigidBodyContact::CalculateInternals(float pfDuration)
	{
	}

	void CRigidBodyContact::SwapBodies()
	{
		mContactNormal *= -1.0f;

		auto lTemp = mBody[0];
		mBody[0] = mBody[1];
		mBody[1] = lTemp;
	}

	void CRigidBodyContact::MatchAwakeState()
	{
		if (mBody[1] != nullptr)
		{
			bool lbBody0Wake = mBody[0]->IsAwake();
			bool lbBody1Wake = mBody[1]->IsAwake();

			if (lbBody0Wake ^ lbBody1Wake)
			{
				if (lbBody0Wake)
					mBody[1]->SetIsAwake(true);
				else
					mBody[0]->SetIsAwake(true);
			}
		}
	}

	void CRigidBodyContact::CalculateDesiredDeltaVelocity(float pfDuration)
	{
	}

	void CRigidBodyContact::CalculateLocalVeclocity(uint32_t puiBodyIndex, float pfDuration)
	{
	}

	void CRigidBodyContact::CalculateContactBasis()
	{

	}

	void CRigidBodyContact::ApplyVelocityChange(std::array<CVector3F, 2> pVelocityChange, std::array<CVector3F, 2> pRotationChange)
	{
	}

	void CRigidBodyContact::ApplyPositionChange(std::array<CVector3F, 2> pLinearChange, std::array<CVector3F, 2> pAngularChange, float pfPenetration)
	{
	}

	CVector3F CRigidBodyContact::CalculateFrictionlessImpulse(const CMatrix3F & pInvInertiaTensor)
	{
		return CVector3F();
	}

	CVector3F CRigidBodyContact::CalculateFrictionImpulse(const CMatrix3F & pInvInertiaTensor)
	{
		return CVector3F();
	}
}