#pragma once

#include "Math/Frustum.h"

namespace FDX
{
	class FDX_EXPORT CCamera
	{
	public:

		CCamera();
		virtual ~CCamera();

		const CVector3F& GetPosition() const
		{
			return mPosition;
		}

		const CVector3F& GetTarget() const
		{
			return mTarget;
		}

		const CVector3F& GetLook() const
		{
			return mLook;
		}

		const CVector3F& GetRight() const
		{
			return mRight;
		}

		const CVector3F& GetUp() const
		{
			return mUp;
		}

		const CMatrix4F& GetProjection() const
		{
			return mProjection;
		}

		const CMatrix4F& GetView() const
		{
			return mView;
		}

		const CFrustum& GetFrustum() const
		{
			return mFrustum;
		}

		CVector3F GetRotation() const
		{
			return CVector3F(mfPitch, mfYaw, mfRoll);
		}
		
		float GetFOV() const
		{
			return mfFOV;
		}

		virtual void SetPosition(const CVector3F &pPosition);
		virtual void SetTarget(const CVector3F &pTarget);
		
		virtual void SetProjection(float pfFOV, float pfAspect, 
			float pfNearPlane, float pfFarPlane);

		virtual void SetView(const CVector3F &pPosition, 
			const CVector3F &pTarget, const CVector3F &pUp);

		virtual void Update();

	protected:

		CVector3F mPosition;
		CVector3F mTarget;
		CVector3F mLook;
		CVector3F mUp;
		CVector3F mRight;

		CMatrix4F mProjection;
		CMatrix4F mView;
		CFrustum mFrustum;

		float mfYaw;
		float mfPitch;
		float mfRoll;
		float mfFOV;
		float mfAspect;
		float mfNearPlane;
		float mfFarPlane;
	};

	using CCameraPtr = SmartPtr<CCamera>;
}