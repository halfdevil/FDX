#pragma once

#include "Input/Keyboard.h"
#include "GLFW/GLFWGameWindow.h"
#include "GLFW/glfw3.h"

namespace FDX
{
    class FDX_EXPORT CGLFWKeyboard : public CKeyboard, public CGameWindowCallback
    {
    public:

        explicit CGLFWKeyboard(CGLFWGameWindow *pWindow);
        ~CGLFWKeyboard() override = default;

        EInputType GetInputType() const override
        {
            return EInputType::Keyboard;
        }

        const Array<CKeyboardCallback*>& GetCallbacks() const override
        {
            return mCallbacks;
        }

        void Sleep() override;
        void Awake() override;
        void Update() override;

        bool KeyDown(EKeyboardKey pKey) override;
        bool KeyHeld(EKeyboardKey pKey) override;
        bool KeyTriggered(EKeyboardKey pKey) override;

        void AddCallback(CKeyboardCallback *pCallback) override;
        void RemoveCallback(CKeyboardCallback *pCallback) override;

        void OnKeyDown(IGameWindow *pWindow, EKeyboardKey pKey) override;
        void OnKeyUp(IGameWindow *pWindow, EKeyboardKey pKey) override;

    private:

        bool mbAwake;
        CGLFWGameWindow *mWindow;
        Array<CKeyboardCallback*> mCallbacks;
        SmartPtr<bool[]> mKeyStates;
        SmartPtr<bool[]> mHoldStates;
    };
}