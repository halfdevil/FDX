#pragma once

#include "Math/Vector3F.h"

namespace FDX
{
	class FDX_EXPORT CParticle
	{
	public:

		CParticle();
		virtual ~CParticle();

		float GetInverseMass() const
		{
			return mfInverseMass;
		}

		float GetDamping() const
		{
			return mfDamping;
		}

		const CVector3F& GetPosition() const
		{
			return mPosition;
		}

		const CVector3F& GetVelocity() const
		{
			return mVeclocity;
		}

		const CVector3F& GetAccumForce() const
		{
			return mAccumForce;
		}

		const CVector3F& GetAcceleration() const
		{
			return mAcceleration;
		}

		inline bool HasFinitieMass();
		inline float GetMass() const;
		inline void SetMass(float pfMass);

		inline void SetInverseMass(float pfInvMass);
		inline void SetDamping(float pfDamping);

		inline void SetPosition(const CVector3F &pPosition);
		inline void SetPosition(float pfX, float pfY, float pfZ);

		inline void SetVelocity(const CVector3F &pVelocity);
		inline void SetVelocity(float pfX, float pfY, float pfZ);

		inline void SetAcceleration(const CVector3F &pAcceleration);
		inline void SetAcceleration(float pfX, float pfY, float pfZ);

		virtual void ClearAccumulator();
		virtual void AddForce(const CVector3F &pForce);
		virtual void Update(float pfDuration);

	protected:

		float mfInverseMass;
		float mfDamping;

		CVector3F mPosition;
		CVector3F mVeclocity;
		CVector3F mAccumForce;
		CVector3F mAcceleration;
	};
}