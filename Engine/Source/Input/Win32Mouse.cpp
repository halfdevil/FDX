
#include "Input/Win32Mouse.h"
#include "OS/Win32GameWindow.h"
#include "Input/InputEvent.h"
#include "Core/EventManager.h"
#include "OS/GameLogger.h"

namespace FDX
{
	#ifndef HID_USAGE_PAGE_GENERIC
		#define HID_USAGE_PAGE_GENERIC			((USHORT) 0x01)
	#endif

	#ifndef HID_USAGE_GENERIC_KEYBOARD
		#define HID_USAGE_GENERIC_MOUSE			((USHORT) 0x02)
	#endif

	CWin32Mouse::CWin32Mouse()
		: mbAwake(false),
		mbMouseLocked(false),
		miLastWheel(0),
		miFrameWheel(0),
		mfClickLimit(0.0f),
		mfSensitivity(0.5f),
		mbShowCursor(true)
	{
		mButtons = std::make_unique<bool[]>((uint32_t)EMouseButton::Max);
		mHoldButtons = std::make_unique<bool[]>((uint32_t)EMouseButton::Max);
		mDoubleClicks = std::make_unique<bool[]>((uint32_t)EMouseButton::Max);
		mLastClickTime = std::make_unique<float[]>((uint32_t)EMouseButton::Max);
	}

	CWin32Mouse::~CWin32Mouse()
	{
	}

	EResult CWin32Mouse::Create(IGameWindow * pWindow)
	{
		EResult lResult = EResult::Success;

		mRawDevice.usUsagePage = HID_USAGE_PAGE_GENERIC;
		mRawDevice.usUsage = HID_USAGE_GENERIC_MOUSE;
		mRawDevice.dwFlags = RIDEV_INPUTSINK;
		mRawDevice.hwndTarget = static_cast<CWin32GameWindow*>(pWindow)->GetHWND();

		if (!RegisterRawInputDevices(&mRawDevice, 1, sizeof(RAWINPUTDEVICE)))
		{
			LOG_DEBUG(_T("RegisterRawInputDevices() for mouse failed"));
			lResult = EResult::CreateFailed;
		}
		else
		{			
			memset(mButtons.get(), 0, sizeof(bool) * ((uint32_t)EMouseButton::Max));
			memset(mHoldButtons.get(), 0, sizeof(bool) * ((uint32_t)EMouseButton::Max));
			memset(mDoubleClicks.get(), 0, sizeof(bool) * ((uint32_t)EMouseButton::Max));
			memset(mLastClickTime.get(), 0, sizeof(float) * ((uint32_t)EMouseButton::Max));

			pWindow->AddCallback(this);
			mbAwake = true;
		}

		return lResult;
	}

	void CWin32Mouse::Sleep()
	{
		mbAwake = false;
		memset(mButtons.get(), 0, sizeof(bool) * ((uint32_t)EMouseButton::Max));
		memset(mHoldButtons.get(), 0, sizeof(bool) * ((uint32_t)EMouseButton::Max));
		memset(mDoubleClicks.get(), 0, sizeof(bool) * ((uint32_t)EMouseButton::Max));
		memset(mLastClickTime.get(), 0, sizeof(float) * ((uint32_t)EMouseButton::Max));
	}

	void CWin32Mouse::Awake()
	{
		mbAwake = true;
	}

	void CWin32Mouse::Update()
	{
		memcpy(mHoldButtons.get(), mButtons.get(), 
			sizeof(bool) * ((uint32_t)EMouseButton::Max));

		mRelativePosition.X = 0;
		mRelativePosition.Y = 0;
		miFrameWheel = 0;
	}

	bool CWin32Mouse::ButtonDown(EMouseButton pButton)
	{
		return mButtons[(uint32_t)pButton];
	}

	bool CWin32Mouse::BottonHeld(EMouseButton pButton)
	{
		return mButtons[(uint32_t)pButton] && mHoldButtons[(uint32_t)pButton];
	}

	bool CWin32Mouse::DoubleClicked(EMouseButton pButton)
	{
		return mButtons[(uint32_t)pButton] && mDoubleClicks[(uint32_t)pButton];
	}

	CVector2F CWin32Mouse::GetRelativePosition()
	{
		return mRelativePosition;
	}

	CVector2F CWin32Mouse::GetAbsolutePosition()
	{
		return mAbsolutePosition;
	}

	bool CWin32Mouse::IsMouseLocked() const
	{
		return mbMouseLocked;
	}

	bool CWin32Mouse::WheelMoved()
	{
		return miFrameWheel != 0;
	}

	int CWin32Mouse::GetWheelMovement()
	{
		return miFrameWheel;
	}

	void CWin32Mouse::SetMouseSensivity(float pfAmount)
	{
		if (pfAmount == 0.0f)
			pfAmount = 1.0f;

		mfSensitivity = pfAmount;
	}

	void CWin32Mouse::SetAbsolutePosition(int32_t piX, int32_t piY)
	{
		mAbsolutePosition.X = (float)piX;
		mAbsolutePosition.Y = (float)piY;
	}

	void CWin32Mouse::SetAbsolutePositionBounds(int32_t piX, int32_t piY)
	{
		mAbsolutePositionBounds.X = (float)piX;
		mAbsolutePositionBounds.Y = (float)piY;
	}

	void CWin32Mouse::SetDoubleClickLimit(float pfMilliSec)
	{
		mfClickLimit = pfMilliSec;
	}

	void CWin32Mouse::UpdateDoubleClick(float pfMilliSec)
	{
		for (uint32_t lIdx = 0; lIdx < (uint32_t)EMouseButton::Max; lIdx++)
		{
			if (mLastClickTime[lIdx] > 0)
			{
				mLastClickTime[lIdx] -= pfMilliSec;
				if (mLastClickTime[lIdx] <= 0.0f)
				{
					mDoubleClicks[lIdx] = true;
					mLastClickTime[lIdx] = 0.0f;
				}
			}
		}
	}

	void CWin32Mouse::ShowOSPointer(bool pbShow)
	{
		if (mbShowCursor == pbShow)
			return;

		if (pbShow)
			ShowCursor(TRUE);
		else
			ShowCursor(FALSE);

		mbShowCursor = pbShow;
	}

	void CWin32Mouse::SetIsMouseLocked(bool pbLocked)
	{
		mbMouseLocked = pbLocked;
	}

	void CWin32Mouse::LockMouseToWindow(IGameWindow *pWindow)
	{
		if (mbMouseLocked)
		{
			HWND lhWnd = static_cast<CWin32GameWindow*>(pWindow)->GetHWND();
			
			RECT lRect;
			GetWindowRect(lhWnd, &lRect);

			SetCapture(lhWnd);
			ClipCursor(&lRect);

			mAbsolutePosition = pWindow->GetCursorPosition();
		}
		else
		{
			ReleaseCapture();
			ClipCursor(nullptr);
		}
	}

	void CWin32Mouse::AddCallback(CMouseCallback * pCallback)
	{
		auto lIt = std::find(mCallbacks.begin(),
			mCallbacks.end(), pCallback);

		if (lIt == mCallbacks.end())
			mCallbacks.push_back(pCallback);
	}

	void CWin32Mouse::RemoveCallback(CMouseCallback * pCallback)
	{
		auto lIt = std::find(mCallbacks.begin(),
			mCallbacks.end(), pCallback);

		if (lIt != mCallbacks.end())
			mCallbacks.erase(lIt);
	}

	void CWin32Mouse::OnActivate(IGameWindow * pWindow, bool pbActive)
	{
		if (pbActive)
		{
			Awake();
			
			if (mbMouseLocked)
				LockMouseToWindow(pWindow);

			mAbsolutePosition = pWindow->GetCursorPosition();
		}
		else
		{
			ReleaseCapture();
			ClipCursor(nullptr);

			Sleep();
		}
	}

	void CWin32Mouse::OnDestroy(IGameWindow * pWindow)
	{
		ShowOSPointer(true);
		LockMouseToWindow(nullptr);
	}

	void CWin32Mouse::OnInput(IGameWindow * pWindow, uint8_t *pInputData)
	{
		if (mbAwake)
		{
			RAWINPUT *lRawInput = reinterpret_cast<RAWINPUT*>(pInputData);

			if (lRawInput->header.dwType != RIM_TYPEMOUSE)
				return;
			
			mRelativePosition.X += (float)lRawInput->data.mouse.lLastX * mfSensitivity;
			mRelativePosition.Y += (float)lRawInput->data.mouse.lLastY * mfSensitivity;
			mAbsolutePosition.X += (float)lRawInput->data.mouse.lLastX;
			mAbsolutePosition.Y += (float)lRawInput->data.mouse.lLastY;

			mAbsolutePosition.X = CMathUtil::Max(mAbsolutePosition.X, 0.0f);
			mAbsolutePosition.X = CMathUtil::Min(mAbsolutePosition.X, mAbsolutePositionBounds.X);
			mAbsolutePosition.Y = CMathUtil::Max(mAbsolutePosition.Y, 0.0f);
			mAbsolutePosition.Y = CMathUtil::Min(mAbsolutePosition.Y, mAbsolutePositionBounds.Y);

			if ((lRawInput->data.mouse.usButtonFlags & RI_MOUSE_WHEEL) != 0)
			{
				if (lRawInput->data.mouse.usButtonData == 120)
					miFrameWheel = 1;
				else
					miFrameWheel = -1;
			}

			if (CMathUtil::Abs(mRelativePosition.X) > 0 ||
				CMathUtil::Abs(mRelativePosition.Y) > 0)
			{
				for (const auto &lCallback : mCallbacks)
				{
					lCallback->OnMouseMove(this, (uint32_t)mAbsolutePosition.X, 
						(uint32_t)mAbsolutePosition.Y);
				}
			}

			static const uint32_t lButtonDowns[] =
			{
				RI_MOUSE_BUTTON_1_DOWN,
				RI_MOUSE_BUTTON_2_DOWN,
				RI_MOUSE_BUTTON_3_DOWN,
				RI_MOUSE_BUTTON_4_DOWN,
				RI_MOUSE_BUTTON_5_DOWN
			};

			static const uint32_t lButtonUps[] =
			{
				RI_MOUSE_BUTTON_1_UP,
				RI_MOUSE_BUTTON_2_UP,
				RI_MOUSE_BUTTON_3_UP,
				RI_MOUSE_BUTTON_4_UP,
				RI_MOUSE_BUTTON_5_UP
			};

			for (uint32_t lIdx = 0; lIdx < (uint32_t)EMouseButton::Max; lIdx++)
			{
				bool lbOldState = mButtons[lIdx];

				if ((lRawInput->data.mouse.usButtonFlags & lButtonDowns[lIdx]) != 0)
				{					
					mButtons[lIdx] = true;

					if (mLastClickTime[lIdx] > 0)
						mDoubleClicks[lIdx] = true;

					mLastClickTime[lIdx] = mfClickLimit;

					for (const auto &lCallback : mCallbacks)
						lCallback->OnMouseDown(this, (EMouseButton)lIdx);
				}
				else if ((lRawInput->data.mouse.usButtonFlags & lButtonUps[lIdx]) != 0)
				{
					mButtons[lIdx] = false;
					mHoldButtons[lIdx] = false;

					if (lbOldState)
					{
						for (const auto &lCallback : mCallbacks)
							lCallback->OnMouseUp(this, (EMouseButton)lIdx);
					}
				}
			}
		}
	}

	void CWin32Mouse::OnMouseMove(IGameWindow * pWindow, uint32_t puiX, uint32_t puiY)
	{	
		auto lWindow = static_cast<CWin32GameWindow*>(pWindow);

		TRACKMOUSEEVENT lTrackEvent;
		lTrackEvent.cbSize = sizeof(TRACKMOUSEEVENT);
		lTrackEvent.dwFlags = TME_LEAVE;
		lTrackEvent.hwndTrack = lWindow->GetHWND();
		TrackMouseEvent(&lTrackEvent);

		if (!mbAwake)
		{ 
			Awake();		
			mAbsolutePosition = lWindow->GetCursorPosition();
		}
	}

	void CWin32Mouse::OnMouseLeave(IGameWindow * pWindow)
	{
		Sleep();
	}

	void CWin32Mouse::OnResize(IGameWindow * pWindow, uint32_t puiWidth, uint32_t puiHeight)
	{
		mAbsolutePosition = pWindow->GetCursorPosition();
		SetAbsolutePositionBounds((int32_t)puiWidth, (int32_t)puiHeight);		
	}
}