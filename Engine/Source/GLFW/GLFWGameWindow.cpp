
#include "GLFW/GLFWGameWindow.h"
#include "OS/GameLogger.h"
#include "OS/System.h"

namespace FDX
{
    CGLFWGameWindow::CGLFWGameWindow(const CWindowParameters &pParameters)
        : mParameters(pParameters),
          mHandle(nullptr)
    {
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    CGLFWGameWindow::~CGLFWGameWindow()
    {
        if (mHandle)
            glfwDestroyWindow(mHandle);

        mHandle = nullptr;
    }

    EResult CGLFWGameWindow::Create(const CContextProperties &pContextProps)
    {
        return Create(pContextProps, nullptr);
    }

    EResult CGLFWGameWindow::Create(const CContextProperties &pContextProps, IGameWindow *pWindow)
    {
        EResult lResult = EResult::Success;
        GLFWmonitor *lMonitor = nullptr;

        if (mParameters.IsFullscreen())
            lMonitor = glfwGetPrimaryMonitor();

        glfwWindowHint(GLFW_RED_BITS, pContextProps.GetRedBits());
        glfwWindowHint(GLFW_GREEN_BITS, pContextProps.GetGreenBits());
        glfwWindowHint(GLFW_BLUE_BITS, pContextProps.GetBlueBits());
        glfwWindowHint(GLFW_ALPHA_BITS, pContextProps.GetAlphaBits());
        glfwWindowHint(GLFW_DEPTH_BITS, pContextProps.GetDepthBits());
        glfwWindowHint(GLFW_STENCIL_BITS, pContextProps.GetStencilBits());
		glfwWindowHint(GLFW_SAMPLES, pContextProps.GetMSAA());
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);        
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

#ifdef FDX_DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

#if FDX_PLATFORM == FDX_PLATFORM_MACOS
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

        GLFWwindow *lShareHandle = pWindow != nullptr ?
                dynamic_cast<CGLFWGameWindow*>(pWindow)->GetHandle() : nullptr;

        mHandle = glfwCreateWindow(mParameters.GetWidth(), mParameters.GetHeight(),
			mParameters.GetTitle().c_str(), lMonitor, lShareHandle);

        if (mHandle == nullptr)
        {
            LOG_DEBUG("glfwCreateWindow() failed");
            lResult = EResult::CreateFailed;
        }
        else
        {
            glfwSetWindowUserPointer(mHandle, this);
            glfwSetWindowSizeCallback(mHandle, GLFWSizeCallback);
            glfwSetWindowCloseCallback(mHandle, GLFWCloseCallback);
            glfwSetKeyCallback(mHandle, GLFWKeyCallback);
            glfwSetMouseButtonCallback(mHandle, GLFWMouseButtonCallback);
            glfwSetCursorPosCallback(mHandle, GLFWCursorPositionCallback);
            glfwSetCursorEnterCallback(mHandle, GLFWCursorEnterCallback);

            for (const auto &lCallback : mCallbacks)
                lCallback->OnCreate(this);
        }

        return lResult;
    }

    CVector2F CGLFWGameWindow::GetCursorPosition() const
    {
        double ldX, ldY;
        glfwGetCursorPos(mHandle, &ldX, &ldY);

        return {(float)ldX, (float)ldY};
    }

    bool CGLFWGameWindow::IsClosed() const
    {
        return (bool)glfwWindowShouldClose(mHandle);
    }

    EResult CGLFWGameWindow::Show()
    {
        FDX_ASSERT(mHandle != nullptr, "Window Handle is invalid");
        glfwShowWindow(mHandle);

        return EResult::Success;
    }

    EResult CGLFWGameWindow::Hide()
    {
        FDX_ASSERT(mHandle != nullptr, "Window Handle is invalid");
        glfwHideWindow(mHandle);

        return EResult::Success;
    }

    EResult CGLFWGameWindow::Activate()
    {
        FDX_ASSERT(mHandle != nullptr, "Window Handle is invalid");
        return EResult::Success;
    }

    EResult CGLFWGameWindow::Deactivate()
    {
        FDX_ASSERT(mHandle != nullptr, "Window Handle is invalid");
        return EResult::Success;
    }

    EResult CGLFWGameWindow::Close()
    {
        FDX_ASSERT(mHandle != nullptr, "Window Handle is invalid");
        glfwSetWindowShouldClose(mHandle, GLFW_TRUE);

        return EResult::Success;
    }

    EResult CGLFWGameWindow::SetTitle(const String &pstrTitle)
    {
        FDX_ASSERT(mHandle != nullptr, "Window Handle is invalid");
        glfwSetWindowTitle(mHandle, pstrTitle.c_str());

        return EResult::Success;
    }

    void CGLFWGameWindow::AddCallback(CGameWindowCallback *pCallback)
    {
        auto lIt = std::find(mCallbacks.begin(),
                             mCallbacks.end(), pCallback);

        if (lIt == mCallbacks.end())
            mCallbacks.push_back(pCallback);
    }

    void CGLFWGameWindow::RemoveCallback(CGameWindowCallback *pCallback)
    {
        auto lIt = std::find(mCallbacks.begin(),
                             mCallbacks.end(), pCallback);

        if (lIt != mCallbacks.end())
            mCallbacks.erase(lIt);
    }

    void CGLFWGameWindow::GLFWErrorCallback(int32_t piError, const char *pstrError)
    {
        LOG_DEBUG("GLFWGameWindow::Error() - %s", pstrError);
    }

    void CGLFWGameWindow::GLFWSizeCallback(GLFWwindow *pWindow, int32_t piWidth, int32_t piHeight)
    {
        auto lWindow = reinterpret_cast<CGLFWGameWindow*>(
                glfwGetWindowUserPointer(pWindow));

        if (lWindow != nullptr)
        {
            for (const auto &lCallback : lWindow->mCallbacks)
                lCallback->OnResize(lWindow, (uint32_t)piWidth, (uint32_t)piHeight);
        }
    }

    void CGLFWGameWindow::GLFWCloseCallback(GLFWwindow *pWindow)
    {
        auto lWindow = reinterpret_cast<CGLFWGameWindow*>(
                glfwGetWindowUserPointer(pWindow));

        if (lWindow != nullptr)
        {
            for (const auto &lCallback : lWindow->mCallbacks)
                lCallback->OnDestroy(lWindow);
        }
    }

    void CGLFWGameWindow::GLFWMouseButtonCallback(GLFWwindow *pWindow,
                                                  int32_t piButton,
                                                  int32_t piAction,
                                                  int32_t piMods)
    {
        auto lWindow = reinterpret_cast<CGLFWGameWindow*>(
                glfwGetWindowUserPointer(pWindow));

        if (lWindow != nullptr)
        {
            EMouseButton lButton = CGLFWUtil::GetMouseButton(piButton);
            if (lButton != EMouseButton::Unknown)
            {
                for (const auto &lCallback : lWindow->mCallbacks)
                {
                    if (piAction == GLFW_PRESS)
                        lCallback->OnMouseDown(lWindow, lButton);
                    else
                        lCallback->OnMouseUp(lWindow, lButton);
                }
            }
        }
    }

    void CGLFWGameWindow::GLFWCursorPositionCallback(GLFWwindow *pWindow, double pdX, double pdY)
    {
        auto lWindow = reinterpret_cast<CGLFWGameWindow*>(
                glfwGetWindowUserPointer(pWindow));

        if (lWindow != nullptr)
        {
            for (const auto &lCallback : lWindow->mCallbacks)
                lCallback->OnMouseMove(lWindow, (uint32_t)pdX, (uint32_t)pdY);
        }
    }

    void CGLFWGameWindow::GLFWCursorEnterCallback(GLFWwindow *pWindow, int piEntered)
    {
        auto lWindow = reinterpret_cast<CGLFWGameWindow*>(
                glfwGetWindowUserPointer(pWindow));

        if (lWindow != nullptr)
        {
            for (const auto &lCallback : lWindow->mCallbacks)
            {
                if (piEntered == GLFW_TRUE)
                    lCallback->OnMouseEnter(lWindow);
                else
                    lCallback->OnMouseLeave(lWindow);
            }
        }
    }

    void CGLFWGameWindow::GLFWKeyCallback(GLFWwindow *pWindow, int32_t piKey, int32_t piScanCode,
                                          int32_t piAction, int32_t piMods)
    {
        auto lWindow = reinterpret_cast<CGLFWGameWindow*>(
                glfwGetWindowUserPointer(pWindow));

        if (lWindow != nullptr)
        {
            EKeyboardKey lKey = CGLFWUtil::GetKeyboardKey(piKey);
            if (lKey != EKeyboardKey::Unknown)
            {
                for (const auto &lCallback : lWindow->mCallbacks)
                {
                    if (piAction == GLFW_PRESS || piAction == GLFW_REPEAT)
                        lCallback->OnKeyDown(lWindow, lKey);
                    else if (piAction == GLFW_RELEASE)
                        lCallback->OnKeyUp(lWindow, lKey);
                }
            }
        }
    }

    void CGLFWGameWindow::PollEvents()
    {
        glfwPollEvents();
    }
}