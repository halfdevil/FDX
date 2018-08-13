#pragma once

#include "Input/InputDevice.h"
#include "Math/Vector2F.h"

namespace FDX
{
	class CMouse;

	enum class EMouseButton : int32_t
	{
	    Unknown = -1,
		Left = 0,
		Right = 1,
		Middle = 2,
		Four = 3,
		Five = 4,
		Max = 5
	};

	class FDX_EXPORT CMouseCallback
	{
	public:

		virtual ~CMouseCallback() = default;

		virtual void OnMouseDown(CMouse *pMouse, EMouseButton pButton) {}
		virtual void OnMouseUp(CMouse *pMouse, EMouseButton pButton) {}
		virtual void OnMouseMove(CMouse *pMouse, uint32_t puiX, uint32_t puiY) {}
	};

	class FDX_EXPORT CMouse : public IInputDevice
	{
	public:

		virtual ~CMouse() override = default;

		virtual bool ButtonDown(EMouseButton pButton) = 0;
		virtual bool BottonHeld(EMouseButton pButton) = 0;
		virtual bool DoubleClicked(EMouseButton pButton) = 0;

		virtual CVector2F GetRelativePosition() = 0;
		virtual CVector2F GetAbsolutePosition() = 0;

		virtual const Array<CMouseCallback*>& GetCallbacks() const = 0;
		
		virtual bool IsCustomCursor() const = 0;
		virtual bool WheelMoved() = 0;
		virtual int GetWheelMovement() = 0;

		virtual void SetMouseSensivity(float pfAmount) = 0;
		virtual void SetAbsolutePosition(int32_t piX, int32_t piY) = 0;
		virtual void SetAbsolutePositionBounds(int32_t piX, int32_t piY) = 0;

		virtual void SetDoubleClickLimit(float pfMilliSec) = 0;
		virtual void UpdateDoubleClick(float pfMilliSec) = 0;

		virtual void EnableCustomCursor(bool pbEnable) = 0;

		virtual void AddCallback(CMouseCallback *pCallback) = 0;
		virtual void RemoveCallback(CMouseCallback *pCallback) = 0;
	};

	using CMousePtr = SmartPtr<CMouse>;
}