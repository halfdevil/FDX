
#include "Scene/FreeCamera.h"
#include "Math/Quaternion.h"

namespace FDX
{
	CFreeCamera::CFreeCamera()		
	{
	}

	CFreeCamera::~CFreeCamera()
	{
	}

	void CFreeCamera::Walk(float pfAmount)
	{
		mTranslation += (mLook * pfAmount);
	}

	void CFreeCamera::Strafe(float pfAmount)
	{
		mTranslation += (mRight * pfAmount);
	}

	void CFreeCamera::Lift(float pfAmount)
	{
		mTranslation += (mUp * pfAmount);
	}

	void CFreeCamera::Rotate(float pfYaw, float pfPitch, float pfRoll)
	{
		mfYaw = pfYaw;
		mfPitch = pfPitch;
		mfRoll = pfRoll;
	}

	void CFreeCamera::Update()
	{
		CQuaternion lCurrQuat = CQuaternion::FromEulers(mfYaw, mfPitch, mfRoll);
		mCameraQuat = lCurrQuat * mCameraQuat;

		CMatrix4F lRotMat = mCameraQuat.GetMatrix4();

		mPosition += mTranslation;
		mTranslation = CVector3F();

		mLook = lRotMat.Transform(CVector3F(0, 0, 1));
		mUp = lRotMat.Transform(CVector3F(0, 1, 0));
		mRight = mLook.Cross(mUp);

		mTarget = mPosition + mLook;
		mView = CMatrix4F::LookAtMatrix(mPosition, mTarget, mUp);

		mFrustum.Set(mView * mProjection);
	}
}