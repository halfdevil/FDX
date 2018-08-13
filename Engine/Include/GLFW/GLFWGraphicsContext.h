#pragma once

#include "GL/gl3w.h"
#include "Graphics/GraphicsContext.h"
#include "GLFW/GLFWGameWindow.h"

namespace FDX
{
    class FDX_EXPORT CGLFWGraphicsContext : public IGraphicsContext
    {
    public:

        explicit CGLFWGraphicsContext(const CContextProperties &pContextProps);
        ~CGLFWGraphicsContext() override = default;

        const CContextProperties& GetProperties() const override
        {
            return mContextProps;
        }

        IGameWindow* GetWindow() const override
        {
            return mWindow;
        }

        EResult Create(CGLFWGameWindow *pWindow);

        EResult Activate() override;
        EResult Deactivate() override;
        EResult SwapBuffers() override;

    private:

        CContextProperties mContextProps;
        CGLFWGameWindow *mWindow;
    };
}