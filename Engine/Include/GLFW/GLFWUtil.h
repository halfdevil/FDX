#pragma once

#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "Core/Result.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

namespace FDX
{
    class FDX_EXPORT CGLFWUtil
    {
    public:

        static EResult Initialize();
        static EKeyboardKey GetKeyboardKey(int32_t piGLFWKey);
        static EMouseButton GetMouseButton(int32_t piGLFWButton);

    private:

        static EKeyboardKey msKeyboardKeys[GLFW_KEY_LAST+1];
        static EMouseButton msMouseButtons[GLFW_MOUSE_BUTTON_5+1];
    };
}