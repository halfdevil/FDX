#pragma once

#include "Input/Mouse.h"
#include "OS/GameWindow.h"
#include <Windows.h>

namespace FDX
{
	class FDX_EXPORT CWin32Mouse : public CMouse, public CGameWindowCallback
	{
	public:

		CWin32Mouse();
		~CWin32Mouse();

		EInputType GetInputType() const
		{
			return EInputType::Mouse;
		}

		const Array<CMouseCallback*>& GetCallbacks() const
		{
			return mCallbacks;
		}

		EResult Create(IGameWindow *pWindow);

		void Sleep();
		void Awake();
		void Update();

		bool ButtonDown(EMouseButton pButton);
		bool BottonHeld(EMouseButton pButton);
		bool DoubleClicked(EMouseButton pButton);

		CVector2F GetRelativePosition();
		CVector2F GetAbsolutePosition();

		bool IsMouseLocked() const;
		bool WheelMoved();
		int GetWheelMovement();

		void SetMouseSensivity(float pfAmount);
		void SetAbsolutePosition(int32_t piX, int32_t piY);
		void SetAbsolutePositionBounds(int32_t piX, int32_t piY);

		void SetDoubleClickLimit(float pfMilliSec);
		void UpdateDoubleClick(float pfMilliSec);

		void ShowOSPointer(bool pbShow);
		void SetIsMouseLocked(bool pbLocked);

		void AddCallback(CMouseCallback *pCallback);
		void RemoveCallback(CMouseCallback *pCallback);

		void OnActivate(IGameWindow *pWindow, bool pbActive) override;
		void OnDestroy(IGameWindow *pWindow) override;

		void OnInput(IGameWindow *pWindow, uint8_t *pInputData) override;
		void OnMouseMove(IGameWindow *pWindow, uint32_t puiX, uint32_t puiY) override;
		void OnMouseLeave(IGameWindow *pWindow) override;
		void OnResize(IGameWindow *pWindow, uint32_t puiWidth, uint32_t puiHeight) override;

	private:

		void LockMouseToWindow(IGameWindow *pWindow);

	private:

		bool mbAwake;
		bool mbMouseLocked;
		bool mbShowCursor;

		RAWINPUTDEVICE mRawDevice;
		Array<CMouseCallback*> mCallbacks;

		CVector2F mAbsolutePosition;
		CVector2F mAbsolutePositionBounds;
		CVector2F mRelativePosition;

		SmartPtr<bool[]> mButtons;
		SmartPtr<bool[]> mHoldButtons;
		SmartPtr<bool[]> mDoubleClicks;
		SmartPtr<float[]> mLastClickTime;

		int32_t miLastWheel;
		int32_t miFrameWheel;
		float mfClickLimit;
		float mfSensitivity;
	};
}