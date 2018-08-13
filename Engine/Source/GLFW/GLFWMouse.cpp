
#include "GLFW/GLFWMouse.h"
#include "GLFW/GLFWUtil.h"

namespace FDX
{
    CGLFWMouse::CGLFWMouse(CGLFWGameWindow *pWindow)
        : mWindow(pWindow),
          mbAwake(false),
          miLastWheel(0),
          miFrameWheel(0),
          mfClickLimit(0.0f),
          mfSensitivity(0.5f),
          mbCustomCursor(false)
    {
        mWindow->AddCallback(this);

        mButtons = std::make_unique<bool[]>((uint32_t)EMouseButton::Max);
        mHoldButtons = std::make_unique<bool[]>((uint32_t)EMouseButton::Max);
        mDoubleClicks = std::make_unique<bool[]>((uint32_t)EMouseButton::Max);
        mLastClickTime = std::make_unique<float[]>((uint32_t)EMouseButton::Max);
    }

    void CGLFWMouse::Sleep()
    {
        mbAwake = false;
        memset(mButtons.get(), 0, sizeof(bool) * ((uint32_t)EMouseButton::Max));
        memset(mHoldButtons.get(), 0, sizeof(bool) * ((uint32_t)EMouseButton::Max));
        memset(mDoubleClicks.get(), 0, sizeof(bool) * ((uint32_t)EMouseButton::Max));
        memset(mLastClickTime.get(), 0, sizeof(float) * ((uint32_t)EMouseButton::Max));
    }

    void CGLFWMouse::Awake()
    {
        mbAwake = true;
    }

    void CGLFWMouse::Update()
    {
        memcpy(mHoldButtons.get(), mButtons.get(),
               sizeof(bool) * ((uint32_t)EMouseButton::Max));

        mRelativePosition.X = 0;
        mRelativePosition.Y = 0;
        miFrameWheel = 0;
    }

    bool CGLFWMouse::ButtonDown(EMouseButton pButton)
    {
        return mButtons[(uint32_t)pButton];
    }

    bool CGLFWMouse::BottonHeld(EMouseButton pButton)
    {
        return mButtons[(uint32_t)pButton] && mHoldButtons[(uint32_t)pButton];
    }

    bool CGLFWMouse::DoubleClicked(EMouseButton pButton)
    {
        return mButtons[(uint32_t)pButton] && mDoubleClicks[(uint32_t)pButton];
    }

    CVector2F CGLFWMouse::GetRelativePosition()
    {
        return mRelativePosition;
    }

    CVector2F CGLFWMouse::GetAbsolutePosition()
    {
        return mAbsolutePosition;
    }

    bool CGLFWMouse::IsCustomCursor() const
    {
        return mbCustomCursor;
    }

    bool CGLFWMouse::WheelMoved()
    {
        return miFrameWheel != 0;
    }

    int CGLFWMouse::GetWheelMovement()
    {
        return miFrameWheel;
    }

    void CGLFWMouse::SetMouseSensivity(float pfAmount)
    {
        if (pfAmount == 0.0f)
            pfAmount = 1.0f;

        mfSensitivity = pfAmount;
    }

    void CGLFWMouse::SetAbsolutePosition(int32_t piX, int32_t piY)
    {
        mAbsolutePosition.X = (float)piX;
        mAbsolutePosition.Y = (float)piY;
    }

    void CGLFWMouse::SetAbsolutePositionBounds(int32_t piX, int32_t piY)
    {
        mAbsolutePositionBounds.X = (float)piX;
        mAbsolutePositionBounds.Y = (float)piY;
    }

    void CGLFWMouse::SetDoubleClickLimit(float pfMilliSec)
    {
        mfClickLimit = pfMilliSec;
    }

    void CGLFWMouse::UpdateDoubleClick(float pfMilliSec)
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

    void CGLFWMouse::EnableCustomCursor(bool pbEnable)
    {
        if (pbEnable)
            glfwSetInputMode(mWindow->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(mWindow->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        mbCustomCursor = pbEnable;
    }

    void CGLFWMouse::AddCallback(CMouseCallback * pCallback)
    {
        auto lIt = std::find(mCallbacks.begin(),
                             mCallbacks.end(), pCallback);

        if (lIt == mCallbacks.end())
            mCallbacks.push_back(pCallback);
    }

    void CGLFWMouse::RemoveCallback(CMouseCallback * pCallback)
    {
        auto lIt = std::find(mCallbacks.begin(),
                             mCallbacks.end(), pCallback);

        if (lIt != mCallbacks.end())
            mCallbacks.erase(lIt);
    }

    void CGLFWMouse::OnMouseDown(IGameWindow *pWindow, EMouseButton pButton)
    {
        auto lButtonIdx = (uint32_t)pButton;
        mButtons[lButtonIdx] = true;

        if (mLastClickTime[lButtonIdx] > 0)
            mDoubleClicks[lButtonIdx] = true;

        mLastClickTime[lButtonIdx] = mfClickLimit;

        for (const auto &lCallback : mCallbacks)
            lCallback->OnMouseDown(this, pButton);
    }

    void CGLFWMouse::OnMouseUp(IGameWindow *pWindow, EMouseButton pButton)
    {
        auto lButtonIdx = (uint32_t)pButton;
        auto lOldState = mButtons[lButtonIdx];

        mButtons[lButtonIdx] = false;
        mHoldButtons[lButtonIdx] = false;

        mLastClickTime[lButtonIdx] = mfClickLimit;

        if (lOldState)
        {
            for (const auto &lCallback : mCallbacks)
                lCallback->OnMouseUp(this, pButton);
        }
    }

    void CGLFWMouse::OnMouseMove(IGameWindow *pWindow, uint32_t puiX, uint32_t puiY)
    {
        CVector2F lOldPosition = mAbsolutePosition;

		mAbsolutePosition.X = CMathUtil::Clamp((float)puiX, 0.0f, mAbsolutePositionBounds.X);
		mAbsolutePosition.Y = CMathUtil::Clamp((float)puiY, 0.0f, mAbsolutePositionBounds.Y);

        mRelativePosition = mAbsolutePosition - lOldPosition;

        for (const auto &lCallback : mCallbacks)
            lCallback->OnMouseMove(this, puiX, puiY);
    }

    void CGLFWMouse::OnMouseEnter(IGameWindow *pWindow)
    {
        Awake();
    }

    void CGLFWMouse::OnMouseLeave(IGameWindow *pWindow)
    {
        Sleep();
    }
}