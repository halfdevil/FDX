
#include "GLFW/GLFWKeyboard.h"

namespace FDX
{
    CGLFWKeyboard::CGLFWKeyboard(CGLFWGameWindow *pWindow)
        : mbAwake(true),
          mWindow(pWindow)
    {
        pWindow->AddCallback(this);

        mKeyStates = std::make_unique<bool[]>((uint32_t)EKeyboardKey::Max);
        mHoldStates = std::make_unique<bool[]>((uint32_t)EKeyboardKey::Max);
    }

    void CGLFWKeyboard::Sleep()
    {
        mbAwake = false;
        memset(mKeyStates.get(), 0, sizeof(bool) * ((uint32_t)EKeyboardKey::Max));
        memset(mHoldStates.get(), 0, sizeof(bool) * ((uint32_t)EKeyboardKey::Max));
    }

    void CGLFWKeyboard::Awake()
    {
        mbAwake = true;
    }

    void CGLFWKeyboard::Update()
    {
        memcpy(mHoldStates.get(), mKeyStates.get(),
               sizeof(bool) * (uint32_t)EKeyboardKey::Max);
    }

    bool CGLFWKeyboard::KeyDown(EKeyboardKey pKey)
    {
        return mKeyStates[(uint32_t)pKey];
    }

    bool CGLFWKeyboard::KeyHeld(EKeyboardKey pKey)
    {
        return mKeyStates[(uint32_t)pKey] && mHoldStates[(uint32_t)pKey];
    }

    bool CGLFWKeyboard::KeyTriggered(EKeyboardKey pKey)
    {
        return mKeyStates[(uint32_t)pKey] && !mHoldStates[(uint32_t)pKey];
    }

    void CGLFWKeyboard::AddCallback(CKeyboardCallback * pCallback)
    {
        auto lIt = std::find(mCallbacks.begin(),
                             mCallbacks.end(), pCallback);

        if (lIt == mCallbacks.end())
            mCallbacks.push_back(pCallback);
    }

    void CGLFWKeyboard::RemoveCallback(CKeyboardCallback * pCallback)
    {
        auto lIt = std::find(mCallbacks.begin(),
                             mCallbacks.end(), pCallback);

        if (lIt != mCallbacks.end())
            mCallbacks.erase(lIt);
    }

    void CGLFWKeyboard::OnKeyDown(IGameWindow *pWindow, EKeyboardKey pKey)
    {
        if (pKey != EKeyboardKey::Unknown)
        {
            mKeyStates[(uint32_t) pKey] = true;

            if (!mHoldStates[(uint32_t)pKey])
            {
                for (const auto &lCallback : mCallbacks)
                    lCallback->OnKeyDown(this, pKey);
            }
        }
    }

    void CGLFWKeyboard::OnKeyUp(FDX::IGameWindow *pWindow, EKeyboardKey pKey)
    {
        if (pKey != EKeyboardKey::Unknown)
        {
            mKeyStates[(uint32_t) pKey] = false;

            if (mHoldStates[(uint32_t)pKey])
            {
                for (const auto &lCallback : mCallbacks)
                    lCallback->OnKeyUp(this, pKey);
            }
        }
    }
}