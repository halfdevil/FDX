#pragma once

#include "Physics/RigidBody.h"

namespace FDX
{
	class CRigidBodyContactResolver;

	class FDX_EXPORT CRigidBodyContact
	{
	public:

		friend class CRigidBodyContactResolver;

		CRigidBodyContact();
		~CRigidBodyContact();

		void SetBodyData(CRigidBody *pBodyOne, CRigidBody *pBodyTwo,
			float pfFriction, float pfRestitution);

	protected:

		void CalculateInternals(float pfDuration);
		void SwapBodies();
		void MatchAwakeState();
		void CalculateDesiredDeltaVelocity(float pfDuration);
		void CalculateLocalVeclocity(uint32_t puiBodyIndex, float pfDuration);
		void CalculateContactBasis();

		void ApplyVelocityChange(std::array<CVector3F, 2> pVelocityChange,
			std::array<CVector3F, 2> pRotationChange);

		void ApplyPositionChange(std::array<CVector3F, 2> pLinearChange,
			std::array<CVector3F, 2> pAngularChange,
			float pfPenetration);

		CVector3F CalculateFrictionlessImpulse(const CMatrix3F &pInvInertiaTensor);
		CVector3F CalculateFrictionImpulse(const CMatrix3F &pInvInertiaTensor);

	protected:

		std::array<CRigidBody*, 2> mBody;
		std::array<CVector3F, 2> mRelativeContactPosition;

		float mfFriction;
		float mfRestitution;
		float mfPenetration;

		CVector3F mContactPoint;
		CVector3F mContactNormal;
		CMatrix3F mContactToWorld;
		CVector3F mContactVeclocity;
		float mfDesiredDeltaVelocity;
	};
}