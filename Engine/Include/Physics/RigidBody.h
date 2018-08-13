#pragma once

#include "Math/Vector3F.h"
#include "Math/Matrix3F.h"
#include "Math/Matrix4F.h"
#include "Math/Quaternion.h"

namespace FDX
{
	class FDX_EXPORT CRigidBody
	{
	public:

		CRigidBody();
		virtual ~CRigidBody();

		float GetInverseMass() const
		{
			return mfInverseMass;
		}

		float GetLinearDamping() const
		{
			return mfLinearDamping;
		}

		float GetAngularDamping() const
		{
			return mfAngularDamping;
		}

		const CVector3F& GetPosition() const
		{
			return mPosition;
		}

		const CQuaternion& GetOrientation() const
		{
			return mOrientation;
		}

		const CVector3F& GetVelocity() const
		{
			return mVelocity;
		}

		const CVector3F& GetRotation() const
		{
			return mRotation;
		}

		const CMatrix3F& GetInverseInertiaTensor() const
		{
			return mInverseInertiaTensor;
		}

		const CMatrix3F& GetInverseInertiaTensorWorld() const
		{
			return mInverseInertiaTensorWorld;
		}

		const CMatrix4F& GetTransformationMatrix() const
		{
			return mTransformMatrix;
		}

		float GetMotion() const
		{
			return mfMotion;
		}

		bool IsAwake() const
		{
			return mbIsAwake;
		}

		bool CanSleep() const
		{
			return mbCanSleep;
		}

		const CVector3F& GetAccumForce() const
		{
			return mAccumForce;
		}

		const CVector3F& GetAccumTorque() const
		{
			return mAccumTorque;
		}

		const CVector3F& GetAcceleration() const
		{
			return mAcceleration;
		}

		const CVector3F& GetLastFrameAcceleration() const
		{
			return mLastFrameAcceleration;
		}

		CMatrix3F GetInertiaTensor() const;
		
		inline bool HasFiniteMass();
		inline float GetMass() const;
		inline void SetMass(float pfMass);

		inline void SetInverseMass(float pfInverseMass);
		inline void SetLinearDamping(float pfLinearDamping);
		inline void SetAngularDamping(float pfAngularDamping);

		inline void SetPosition(const CVector3F &pPosition);
		inline void SetPosition(float pfX, float pfY, float pfZ);

		inline void SetOrientation(const CQuaternion &pOrientation);
		inline void SetOrientation(float pfX, float pfY, float pfZ, float pfW = 1.0f);
		
		inline void SetInverseInertiaTensor(const CMatrix3F &pInverseInertiaTensor);
		inline void SetInertiaTensor(const CMatrix3F &pInverseInertiaTensor);

		inline void SetVelocity(const CVector3F &pVelocity);
		inline void SetVelocity(float pfX, float pfY, float pfZ);
		inline void AddVelocity(const CVector3F &pDeltaVelocity);

		inline void SetRotation(const CVector3F &pPosition);
		inline void SetRotation(float pfX, float pfY, float pfZ);
		inline void AddRotation(const CVector3F &pDeltaRotation);

		inline void SetMotion(float pfMotion);
		inline void SetIsAwake(bool pbIsAwake);
		inline void SetCanSleep(bool pbCanSleep);

		inline void SetAcceleration(const CVector3F &pAcceleration);
		inline void SetAcceleration(float pfX, float pfY, float pfZ);

		inline void SetLastFrameAcceleration(const CVector3F &pAcceleration);
		inline void SetLastFrameAcceleration(float pfX, float pfY, float pfZ);

		inline CVector3F GetPointInLocalSpace(const CVector3F &pPoint);
		inline CVector3F GetPointInWorldSpace(const CVector3F &pPoint);
				
		virtual void AddForce(const CVector3F &pForce);
		virtual void AddForceAtPoint(const CVector3F &pForce, const CVector3F &pPoint);
		virtual void AddForceAtBodyPoint(const CVector3F &pForce, const CVector3F &pPoint);
		virtual void AddTorque(const CVector3F &pTorque);

		virtual void CalculateDerivedData();
		virtual void ClearAccumulator();
		virtual void Update(float pfDuration);

	protected:

		void TransformInertiaTensor(const CMatrix3F &pIITBody,
			const CMatrix4F &pRotMat,
			CMatrix3F &pIITWorld);

		void CalculateTransformMatrix(const CVector3F &pPosition,
			const CQuaternion &pOrientation,
			CMatrix4F &pTransform);

	protected:

		float mfInverseMass;
		float mfLinearDamping;
		float mfAngularDamping;

		CVector3F mPosition;
		CQuaternion mOrientation;
		CVector3F mVelocity;
		CVector3F mRotation;
		CMatrix4F mTransformMatrix;
		CMatrix3F mInverseInertiaTensor;
		CMatrix3F mInverseInertiaTensorWorld;
		
		float mfMotion;
		bool mbIsAwake;
		bool mbCanSleep;

		CVector3F mAccumForce;
		CVector3F mAccumTorque;
		CVector3F mAcceleration;
		CVector3F mLastFrameAcceleration;
	};
}