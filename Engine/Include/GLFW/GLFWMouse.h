#pragma once

#include "Input/Mouse.h"
#include "OS/GameWindow.h"
#include "GLFW/GLFWGameWindow.h"

namespace FDX
{
    class FDX_EXPORT CGLFWMouse : public CMouse, public CGameWindowCallback
    {
    public:

        explicit CGLFWMouse(CGLFWGameWindow *pWindow);
        ~CGLFWMouse() override = default;

        EInputType GetInputType() const override
        {
            return EInputType::Mouse;
        }

        const Array<CMouseCallback*>& GetCallbacks() const override
        {
            return mCallbacks;
        }

        void Sleep() override;
        void Awake() override;
        void Update() override;

        bool ButtonDown(EMouseButton pButton) override;
        bool BottonHeld(EMouseButton pButton) override;
        bool DoubleClicked(EMouseButton pButton) override;

        CVector2F GetRelativePosition() override;
        CVector2F GetAbsolutePosition() override;

        bool IsCustomCursor() const override;
        bool WheelMoved() override;
        int GetWheelMovement() override;

        void SetMouseSensivity(float pfAmount) override;
        void SetAbsolutePosition(int32_t piX, int32_t piY) override;
        void SetAbsolutePositionBounds(int32_t piX, int32_t piY) override;

        void SetDoubleClickLimit(float pfMilliSec) override;
        void UpdateDoubleClick(float pfMilliSec) override;

        void EnableCustomCursor(bool pbEnable) override;

        void AddCallback(CMouseCallback *pCallback) override;
        void RemoveCallback(CMouseCallback *pCallback) override;

        void OnMouseDown(IGameWindow *pWindow, EMouseButton pButton) override;
        void OnMouseUp(IGameWindow *pWindow, EMouseButton pButton) override;
        void OnMouseMove(IGameWindow *pWindow, uint32_t puiX, uint32_t puiY) override;
        void OnMouseEnter(IGameWindow *pWindow) override;
        void OnMouseLeave(IGameWindow *pWindow) override;

    private:

        bool mbAwake;
        bool mbCustomCursor;

        CGLFWGameWindow *mWindow;
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