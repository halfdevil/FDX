
#include "Physics/RigidBody.h"
#include "Physics/PhysicsUtil.h"

namespace FDX
{
	CRigidBody::CRigidBody()
		: mfInverseMass(0.0f),
		mfLinearDamping(0.0f),
		mfAngularDamping(0.0f),
		mfMotion(0.0f),
		mbIsAwake(false),
		mbCanSleep(false)
	{
	}

	CRigidBody::~CRigidBody()
	{
	}

	CMatrix3F CRigidBody::GetInertiaTensor() const
	{
		return CMatrix3F::InvertMatrix(mInverseInertiaTensor);
	}

	inline bool CRigidBody::HasFiniteMass()
	{
		return mfInverseMass >= 0.0f;
	}

	inline float CRigidBody::GetMass() const
	{
		if (mfInverseMass == 0.0f)
			return 999999.0f;

		return 1.0f / mfInverseMass;
	}

	inline void CRigidBody::SetMass(float pfMass)
	{
		mfInverseMass = 1.0f / pfMass;
	}

	inline void CRigidBody::SetInverseMass(float pfInverseMass)
	{
		mfInverseMass = pfInverseMass;
	}

	inline void CRigidBody::SetLinearDamping(float pfLinearDamping)
	{
		mfLinearDamping = pfLinearDamping;
	}

	inline void CRigidBody::SetAngularDamping(float pfAngularDamping)
	{
		mfAngularDamping = pfAngularDamping;
	}

	inline void CRigidBody::SetPosition(const CVector3F & pPosition)
	{
		mPosition = pPosition;
	}

	inline void CRigidBody::SetPosition(float pfX, float pfY, float pfZ)
	{
		mPosition.Set(pfX, pfY, pfZ);
	}

	inline void CRigidBody::SetOrientation(const CQuaternion & pOrientation)
	{
		mOrientation = pOrientation;
	}

	inline void CRigidBody::SetOrientation(float pfX, float pfY, float pfZ, float pfW)
	{
		mOrientation.Set(pfX, pfY, pfZ, pfW);
	}

	inline void CRigidBody::SetInverseInertiaTensor(const CMatrix3F & pInverseInertiaTensor)
	{
		mInverseInertiaTensor = pInverseInertiaTensor;
	}

	inline void CRigidBody::SetInertiaTensor(const CMatrix3F & pInertiaTensor)
	{
		mInverseInertiaTensor = CMatrix3F::InvertMatrix(pInertiaTensor);
	}

	inline void CRigidBody::SetVelocity(const CVector3F & pVelocity)
	{
		mVelocity = pVelocity;
	}

	inline void CRigidBody::SetVelocity(float pfX, float pfY, float pfZ)
	{
		mVelocity.Set(pfX, pfY, pfZ);
	}

	inline void CRigidBody::AddVelocity(const CVector3F & pDeltaVelocity)
	{
		mVelocity += pDeltaVelocity;
	}

	inline void CRigidBody::SetRotation(const CVector3F & pRotation)
	{
		mRotation = pRotation;
	}

	inline void CRigidBody::SetRotation(float pfX, float pfY, float pfZ)
	{
		mRotation.Set(pfX, pfY, pfZ);
	}

	inline void CRigidBody::AddRotation(const CVector3F & pDeltaRotation)
	{
		mRotation += pDeltaRotation;
	}

	inline void CRigidBody::SetMotion(float pfMotion)
	{
		mfMotion = pfMotion;
	}

	inline void CRigidBody::SetIsAwake(bool pbIsAwake)
	{
		if (pbIsAwake)
		{
			mbIsAwake = true;
			mfMotion = CPhysicsUtil::GetSleepEpsilon() * 2.0f;
		}
		else
		{
			mbIsAwake = false;
			mVelocity.Clear();
			mRotation.Clear();
		}
	}

	inline void CRigidBody::SetCanSleep(bool pbCanSleep)
	{
		mbCanSleep = pbCanSleep;

		if (!mbCanSleep && !mbIsAwake)
			SetIsAwake(true);
	}

	inline void CRigidBody::SetAcceleration(const CVector3F & pAcceleration)
	{
		mAcceleration = pAcceleration;
	}

	inline void CRigidBody::SetAcceleration(float pfX, float pfY, float pfZ)
	{
		mAcceleration.Set(pfX, pfY, pfZ);
	}

	inline void CRigidBody::SetLastFrameAcceleration(const CVector3F & pAcceleration)
	{
		mLastFrameAcceleration = pAcceleration;
	}

	inline void CRigidBody::SetLastFrameAcceleration(float pfX, float pfY, float pfZ)
	{
		mLastFrameAcceleration.Set(pfX, pfY, pfZ);
	}

	inline CVector3F CRigidBody::GetPointInLocalSpace(const CVector3F & pPoint)
	{
		return mTransformMatrix.TransformInverse(pPoint);
	}

	inline CVector3F CRigidBody::GetPointInWorldSpace(const CVector3F & pPoint)
	{
		return mTransformMatrix.Transform(pPoint);
	}

	void CRigidBody::AddForce(const CVector3F & pForce)
	{
		mAccumForce += pForce;
		mbIsAwake = true;
	}

	void CRigidBody::AddForceAtPoint(const CVector3F & pForce, const CVector3F & pPoint)
	{
		CVector3F lPoint = pPoint;
		lPoint -= mPosition;

		mAccumForce += pForce;
		mAccumTorque += lPoint.Cross(pForce);
	}

	void CRigidBody::AddForceAtBodyPoint(const CVector3F & pForce, const CVector3F & pPoint)
	{
		AddForceAtPoint(pForce, GetPointInWorldSpace(pPoint));
	}

	void CRigidBody::AddTorque(const CVector3F & pTorque)
	{
		mAccumTorque += pTorque;
		mbIsAwake = true;
	}

	void CRigidBody::CalculateDerivedData()
	{
		mOrientation.Normalize();

		CalculateTransformMatrix(mPosition, mOrientation, mTransformMatrix);
		TransformInertiaTensor(mInverseInertiaTensor, 
			mTransformMatrix, mInverseInertiaTensorWorld);
	}

	void CRigidBody::ClearAccumulator()
	{
		mAccumForce.Clear();
		mAccumTorque.Clear();
	}

	void CRigidBody::Update(float pfDuration)
	{
		if (!mbIsAwake)
			return;

		mLastFrameAcceleration = mAcceleration;
		mLastFrameAcceleration += mAccumForce * mfInverseMass;

		CVector3F lAngularAcceleration =
			mInverseInertiaTensorWorld.Transform(mAccumTorque);

		mVelocity += mLastFrameAcceleration * pfDuration;
		mRotation += lAngularAcceleration * pfDuration;

		mVelocity *= CMathUtil::Pow(mfLinearDamping, pfDuration);
		mRotation *= CMathUtil::Pow(mfAngularDamping, pfDuration);

		mPosition += mVelocity * pfDuration;
		mOrientation += mRotation * pfDuration;

		CalculateDerivedData();
		ClearAccumulator();

		if (mbCanSleep)
		{
			float lfCurrMotion = mVelocity.GetLengthSqr() +
				mRotation.GetLengthSqr();

			float lfBias = CMathUtil::Pow(0.5f, pfDuration);
			mfMotion = lfBias * mfMotion + (1.0f - lfBias) * lfCurrMotion;

			if (mfMotion < CPhysicsUtil::GetSleepEpsilon())
				SetIsAwake(false);
			else if (mfMotion > 10 * CPhysicsUtil::GetSleepEpsilon())
				mfMotion = 10 * CPhysicsUtil::GetSleepEpsilon();
		}
	}

	void CRigidBody::TransformInertiaTensor(const CMatrix3F & pIITBody, 
		const CMatrix4F & pRotMat, CMatrix3F & pIITWorld)
	{
		float lfT4 = pRotMat.M2[0][0] * pIITBody.M2[0][0] +
			pRotMat.M2[0][1] * pIITBody.M2[1][0] +
			pRotMat.M2[0][2] * pIITBody.M2[2][0];
		float lfT9 = pRotMat.M2[0][0] * pIITBody.M2[0][1] +
			pRotMat.M2[0][1] * pIITBody.M2[1][1] +
			pRotMat.M2[0][2] * pIITBody.M2[2][1];
		float lfT14 = pRotMat.M2[0][0] * pIITBody.M2[0][2] +
			pRotMat.M2[0][1] * pIITBody.M2[1][2] +
			pRotMat.M2[0][2] * pIITBody.M2[2][2];

		float lfT28 = pRotMat.M2[1][0] * pIITBody.M2[0][0] +
			pRotMat.M2[1][1] * pIITBody.M2[1][0] +
			pRotMat.M2[1][2] * pIITBody.M2[2][0];
		float lfT33 = pRotMat.M2[1][0] * pIITBody.M2[0][1] +
			pRotMat.M2[1][1] * pIITBody.M2[1][1] +
			pRotMat.M2[1][2] * pIITBody.M2[2][1];
		float lfT38 = pRotMat.M2[1][0] * pIITBody.M2[0][2] +
			pRotMat.M2[1][1] * pIITBody.M2[1][2] +
			pRotMat.M2[1][2] * pIITBody.M2[2][2];

		float lfT52 = pRotMat.M2[2][0] * pIITBody.M2[0][0] +
			pRotMat.M2[2][1] * pIITBody.M2[1][0] +
			pRotMat.M2[2][2] * pIITBody.M2[2][0];
		float lfT57 = pRotMat.M2[2][0] * pIITBody.M2[0][1] +
			pRotMat.M2[2][1] * pIITBody.M2[1][1] +
			pRotMat.M2[2][2] * pIITBody.M2[2][1];
		float lfT62 = pRotMat.M2[2][0] * pIITBody.M2[0][2] +
			pRotMat.M2[2][1] * pIITBody.M2[1][2] +
			pRotMat.M2[2][2] * pIITBody.M2[2][2];

		pIITWorld.M2[0][0] = lfT4 * pRotMat.M2[0][0] +
			lfT9 * pRotMat.M2[0][1] +
			lfT14 * pRotMat.M2[0][2];
		pIITWorld.M2[0][1] = lfT4 * pRotMat.M2[1][0] +
			lfT9 * pRotMat.M2[1][1] +
			lfT14 * pRotMat.M2[1][2];
		pIITWorld.M2[0][2] = lfT4 * pRotMat.M2[2][0] +
			lfT9 * pRotMat.M2[2][1] +
			lfT14 * pRotMat.M2[2][2];

		pIITWorld.M2[1][0] = lfT28 * pRotMat.M2[0][0] +
			lfT33 * pRotMat.M2[0][1] +
			lfT38 * pRotMat.M2[0][2];
		pIITWorld.M2[1][1] = lfT28 * pRotMat.M2[1][0] +
			lfT33 * pRotMat.M2[1][1] +
			lfT38 * pRotMat.M2[1][2];
		pIITWorld.M2[1][2] = lfT28 * pRotMat.M2[2][0] +
			lfT33 * pRotMat.M2[2][1] +
			lfT38 * pRotMat.M2[2][2];

		pIITWorld.M2[2][0] = lfT52 * pRotMat.M2[0][0] +
			lfT57 * pRotMat.M2[0][1] +
			lfT62 * pRotMat.M2[0][2];
		pIITWorld.M2[2][1] = lfT52 * pRotMat.M2[1][0] +
			lfT57 * pRotMat.M2[1][1] +
			lfT62 * pRotMat.M2[1][2];
		pIITWorld.M2[2][2] = lfT52 * pRotMat.M2[2][0] +
			lfT57 * pRotMat.M2[2][1] +
			lfT62 * pRotMat.M2[2][2];
	}

	void CRigidBody::CalculateTransformMatrix(const CVector3F & pPosition, 
		const CQuaternion & pOrientation, CMatrix4F & pTransform)
	{
		pTransform = pOrientation.GetMatrix4() *
			CMatrix4F::TranslationMatrix(pPosition);
	}
}