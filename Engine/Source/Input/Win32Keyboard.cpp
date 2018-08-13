
#include "Input/Win32Keyboard.h"
#include "OS/Win32GameWindow.h"
#include "Core/EventManager.h"
#include "Input/InputEvent.h"
#include "OS/GameLogger.h"

namespace FDX
{
	#ifndef HID_USAGE_PAGE_GENERIC
		#define HID_USAGE_PAGE_GENERIC			((USHORT) 0x01)
	#endif

	#ifndef HID_USAGE_GENERIC_KEYBOARD
		#define HID_USAGE_GENERIC_KEYBOARD		((USHORT) 0x06)
	#endif

	CWin32Keyboard::CWin32Keyboard()
		: mbAwake(false)
	{
		mKeyStates = std::make_unique<bool[]>((uint32_t)EKeyboardKey::Max);
		mHoldStates = std::make_unique<bool[]>((uint32_t)EKeyboardKey::Max);
	}

	CWin32Keyboard::~CWin32Keyboard()
	{		
	}

	EResult CWin32Keyboard::Create(IGameWindow * pWindow)
	{
		EResult lResult = EResult::Success;

		mRawDevice.usUsagePage = HID_USAGE_PAGE_GENERIC;
		mRawDevice.usUsage = HID_USAGE_GENERIC_KEYBOARD;
		mRawDevice.dwFlags = RIDEV_INPUTSINK;
		mRawDevice.hwndTarget = static_cast<CWin32GameWindow*>(pWindow)->GetHWND();

		if (!RegisterRawInputDevices(&mRawDevice, 1, sizeof(RAWINPUTDEVICE)))
		{
			LOG_DEBUG(_T("RegisterRawInputDevices() for keyboard failed"));
			lResult = EResult::CreateFailed;
		}
		else
		{
			memset(mKeyStates.get(), 0, sizeof(bool) * ((uint32_t)EKeyboardKey::Max));
			memset(mHoldStates.get(), 0, sizeof(bool) * ((uint32_t)EKeyboardKey::Max));

			pWindow->AddCallback(this);
			mbAwake = true;
		}

		return lResult;
	}

	void CWin32Keyboard::Sleep()
	{
		mbAwake = false;
		memset(mKeyStates.get(), 0, sizeof(bool) * ((uint32_t)EKeyboardKey::Max));
		memset(mHoldStates.get(), 0, sizeof(bool) * ((uint32_t)EKeyboardKey::Max));
	}

	void CWin32Keyboard::Awake()
	{
		mbAwake = true;
	}

	void CWin32Keyboard::Update()
	{
		memcpy(mHoldStates.get(), mKeyStates.get(), 
			sizeof(bool) * (uint32_t)EKeyboardKey::Max);
	}

	bool CWin32Keyboard::KeyDown(EKeyboardKey pKey)
	{
		return mKeyStates[(uint32_t)pKey];
	}

	bool CWin32Keyboard::KeyHeld(EKeyboardKey pKey)
	{
		return mKeyStates[(uint32_t)pKey] && mHoldStates[(uint32_t)pKey];
	}

	bool CWin32Keyboard::KeyTriggered(EKeyboardKey pKey)
	{
		return mKeyStates[(uint32_t)pKey] && !mHoldStates[(uint32_t)pKey];
	}

	void CWin32Keyboard::AddCallback(CKeyboardCallback * pCallback)
	{
		auto lIt = std::find(mCallbacks.begin(),
			mCallbacks.end(), pCallback);

		if (lIt == mCallbacks.end())
			mCallbacks.push_back(pCallback);
	}

	void CWin32Keyboard::RemoveCallback(CKeyboardCallback * pCallback)
	{
		auto lIt = std::find(mCallbacks.begin(),
			mCallbacks.end(), pCallback);

		if (lIt != mCallbacks.end())
			mCallbacks.erase(lIt);
	}

	void CWin32Keyboard::OnActivate(IGameWindow * pWindow, bool pbActive)
	{
		if (pbActive)		
			Awake();		
		else		
			Sleep();		
	}

	void CWin32Keyboard::OnInput(IGameWindow * pWindow, uint8_t *pInputData)
	{
		if (mbAwake)
		{
			RAWINPUT *lRawInput = reinterpret_cast<RAWINPUT*>(pInputData);

			if (lRawInput->header.dwType != RIM_TYPEKEYBOARD)
				return;

			DWORD ldwKey = static_cast<DWORD>(lRawInput->data.keyboard.VKey);
			if (ldwKey >= (uint32_t)EKeyboardKey::Max)
				return;

			bool lbNewState = !(lRawInput->data.keyboard.Flags & RI_KEY_BREAK);
			bool lbOldState = mKeyStates[ldwKey];

			mKeyStates[ldwKey] = lbNewState;

			if (lbOldState && !lbNewState)
			{
				for (const auto &lCallback : mCallbacks)
					lCallback->OnKeyUp(this, (EKeyboardKey)ldwKey);
			}
			else if (lbNewState)
			{
				for (const auto &lCallback : mCallbacks)
					lCallback->OnKeyDown(this, (EKeyboardKey)ldwKey);
			}
		}
	}
}