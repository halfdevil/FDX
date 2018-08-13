#pragma once

#include "Scene/Camera.h"
#include "Math/Quaternion.h"

namespace FDX
{
	class FDX_EXPORT CFreeCamera : public CCamera
	{
	public:

		CFreeCamera();
		~CFreeCamera();

		void Walk(float pfAmount);
		void Strafe(float pfAmount);
		void Lift(float pfAmount);
		void Rotate(float pfYaw, float pfPitch, float pfRoll);
		void Update() override;

	protected:

		CQuaternion mCameraQuat;
		CVector3F mDirection;
		CVector3F mTranslation;
	};

	using CFreeCameraPtr = SmartPtr<CFreeCamera>;
}