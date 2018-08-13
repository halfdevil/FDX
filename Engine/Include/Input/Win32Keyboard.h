#pragma once

#include "Input/Keyboard.h"
#include "OS/GameWindow.h"
#include <Windows.h>

namespace FDX
{
	class FDX_EXPORT CWin32Keyboard : public CKeyboard, public CGameWindowCallback
	{
	public:

		CWin32Keyboard();
		~CWin32Keyboard();

		EInputType GetInputType() const
		{
			return EInputType::Keyboard;
		}

		const Array<CKeyboardCallback*>& GetCallbacks() const
		{
			return mCallbacks;
		}

		EResult Create(IGameWindow *pWindow);

		void Sleep();
		void Awake();
		void Update();

		bool KeyDown(EKeyboardKey pKey);
		bool KeyHeld(EKeyboardKey pKey);
		bool KeyTriggered(EKeyboardKey pKey);

		void AddCallback(CKeyboardCallback *pCallback);
		void RemoveCallback(CKeyboardCallback *pCallback);

		void OnActivate(IGameWindow *pWindow, bool pbActive) override;
		void OnInput(IGameWindow *pWindow, uint8_t *pInputData) override;

	private:

		bool mbAwake;
		RAWINPUTDEVICE mRawDevice;

		SmartPtr<bool[]> mKeyStates;
		SmartPtr<bool[]> mHoldStates;
		Array<CKeyboardCallback*> mCallbacks;
	};
}