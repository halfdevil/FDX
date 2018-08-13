#pragma once

#include "OS/GameWindow.h"
#include "Graphics/GraphicsContext.h"
#include "GLFW/GLFWUtil.h"
#include "GLFW/glfw3.h"

namespace FDX
{
    class FDX_EXPORT CGLFWGameWindow : public IGameWindow
    {
    public:

        explicit CGLFWGameWindow(const CWindowParameters &pParameters);
        ~CGLFWGameWindow() override ;

        const CWindowParameters& GetParameters() const override
        {
            return mParameters;
        }

        const Array<CGameWindowCallback*>& GetCallbacks() const override
        {
            return mCallbacks;
        }

        GLFWwindow* GetHandle() const
        {
            return mHandle;
        }

        EResult Create(const CContextProperties &pContextProps);
        EResult Create(const CContextProperties &pContextProps, IGameWindow *pWindow);

        CVector2F GetCursorPosition() const override;
        bool IsClosed() const override;

        EResult Show() override;
        EResult Hide() override;
        EResult Activate() override;
        EResult Deactivate() override;
        EResult Close() override;
        EResult SetTitle(const String &pstrTitle) override;

        void AddCallback(CGameWindowCallback *pCallback) override;
        void RemoveCallback(CGameWindowCallback *pCallback) override;

    public:

        static void PollEvents();

    private:

        static void GLFWErrorCallback(int32_t piError, const char *pstrError);
        static void GLFWSizeCallback(GLFWwindow *pWindow, int32_t piWidth, int32_t piHeight);
        static void GLFWCloseCallback(GLFWwindow *pWindow);

        static void GLFWKeyCallback(GLFWwindow *pWindow, int32_t piKey,
                                    int32_t piScanCode, int32_t piAction, int32_t piMods);

        static void GLFWMouseButtonCallback(GLFWwindow *pWindow, int32_t piButton,
                                            int32_t piAction, int32_t piMods);
        static void GLFWCursorPositionCallback(GLFWwindow *pWindow, double pdX, double pdY);
        static void GLFWCursorEnterCallback(GLFWwindow *pWindow, int piEntered);

    private:

        Array<CGameWindowCallback*> mCallbacks;
        CWindowParameters mParameters;
        GLFWwindow *mHandle;
    };
}