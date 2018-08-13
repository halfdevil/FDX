
#include "Scene/Camera.h"

namespace FDX
{
	CCamera::CCamera()
		: mfYaw(0.0f),
		mfPitch(0.0f),
		mfRoll(0.0f)
	{
	}

	CCamera::~CCamera()
	{
	}

	void CCamera::SetPosition(const CVector3F & pPosition)
	{
		mPosition = pPosition;
	}

	void CCamera::SetTarget(const CVector3F & pTarget)
	{
		mTarget = pTarget;
	}

	void CCamera::SetProjection(float pfFOV, float pfAspect, float pfNearPlane, float pfFarPlane)
	{
		mfFOV = pfFOV;
		mfAspect = pfAspect;
		mfNearPlane = pfNearPlane;
		mfFarPlane = pfFarPlane;

		mProjection = CMatrix4F::PerspectiveMatrix(pfFOV, pfAspect,
			pfNearPlane, pfFarPlane);
	}

	void CCamera::SetView(const CVector3F & pPosition, const CVector3F & pTarget, const CVector3F & pUp)
	{
		mPosition = pPosition;
		mTarget = pTarget;
		mUp = pUp;

		mView = CMatrix4F::LookAtMatrix(pPosition, pTarget, pUp);
	}

	void CCamera::Update()
	{
	}
}