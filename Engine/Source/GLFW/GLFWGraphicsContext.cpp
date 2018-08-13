
#include "GLFW/GLFWGraphicsContext.h"
#include "OS/GameLogger.h"

namespace FDX
{
    CGLFWGraphicsContext::CGLFWGraphicsContext(const CContextProperties &pContextProps)
        : mContextProps(pContextProps)
    {
    }

    EResult CGLFWGraphicsContext::Create(CGLFWGameWindow *pWindow)
    {
        EResult lResult = EResult::Success;

        if (pWindow == nullptr)
        {
            LOG_DEBUG("pWindow is null");
        }
        else
        {
			glfwMakeContextCurrent(pWindow->GetHandle());
            if (gl3wInit())
            {
                LOG_DEBUG("gl3wInit() failed");
                lResult = EResult::CreateFailed;
			}
			else
			{
				mWindow = pWindow;
			}
        }

        return lResult;
    }

    EResult CGLFWGraphicsContext::Activate()
    {
        glfwMakeContextCurrent(mWindow->GetHandle());
        return EResult::Success;
    }

    EResult CGLFWGraphicsContext::Deactivate()
    {
        glfwMakeContextCurrent(nullptr);
        return EResult::Success;
    }

    EResult CGLFWGraphicsContext::SwapBuffers()
    {
        glfwSwapBuffers(mWindow->GetHandle());
        return EResult::Success;
    }
}

