
#include "GLFW/GLFWUtil.h"
#include "OS/GameLogger.h"

namespace FDX
{
    EKeyboardKey CGLFWUtil::msKeyboardKeys[GLFW_KEY_LAST+1];
    EMouseButton CGLFWUtil::msMouseButtons[GLFW_MOUSE_BUTTON_5+1];

    EResult CGLFWUtil::Initialize()
    {
        EResult lResult = EResult::Success;

        if (!glfwInit())
        {
            LOG_DEBUG("glfwInit() failed");
            lResult = EResult::CreateFailed;
        }
        else
        {
            for (auto lIdx = 0; lIdx < GLFW_KEY_LAST; lIdx++)
                msKeyboardKeys[lIdx] = EKeyboardKey::Unknown;

            for (auto lIdx = 0; lIdx < GLFW_MOUSE_BUTTON_5 + 1; lIdx++)
                msMouseButtons[lIdx] = EMouseButton::Unknown;

            msKeyboardKeys[GLFW_KEY_SPACE] = EKeyboardKey::Space;
            msKeyboardKeys[GLFW_KEY_APOSTROPHE] = EKeyboardKey::Apostrophe;
            msKeyboardKeys[GLFW_KEY_COMMA] = EKeyboardKey::Comma;
            msKeyboardKeys[GLFW_KEY_MINUS] = EKeyboardKey::Minus;
            msKeyboardKeys[GLFW_KEY_PERIOD] = EKeyboardKey::Period;
            msKeyboardKeys[GLFW_KEY_SLASH] = EKeyboardKey::Slash;
            msKeyboardKeys[GLFW_KEY_0] = EKeyboardKey::Zero;
            msKeyboardKeys[GLFW_KEY_1] = EKeyboardKey::One;
            msKeyboardKeys[GLFW_KEY_2] = EKeyboardKey::Two;
            msKeyboardKeys[GLFW_KEY_3] = EKeyboardKey::Three;
            msKeyboardKeys[GLFW_KEY_4] = EKeyboardKey::Four;
            msKeyboardKeys[GLFW_KEY_5] = EKeyboardKey::Five;
            msKeyboardKeys[GLFW_KEY_6] = EKeyboardKey::Six;
            msKeyboardKeys[GLFW_KEY_7] = EKeyboardKey::Seven;
            msKeyboardKeys[GLFW_KEY_8] = EKeyboardKey::Eight;
            msKeyboardKeys[GLFW_KEY_9] = EKeyboardKey::Nine;
            msKeyboardKeys[GLFW_KEY_SEMICOLON] = EKeyboardKey::SemiColon;
            msKeyboardKeys[GLFW_KEY_EQUAL] = EKeyboardKey::Equal;
            msKeyboardKeys[GLFW_KEY_A] = EKeyboardKey::A;
            msKeyboardKeys[GLFW_KEY_B] = EKeyboardKey::B;
            msKeyboardKeys[GLFW_KEY_C] = EKeyboardKey::C;
            msKeyboardKeys[GLFW_KEY_D] = EKeyboardKey::D;
            msKeyboardKeys[GLFW_KEY_E] = EKeyboardKey::E;
            msKeyboardKeys[GLFW_KEY_F] = EKeyboardKey::F;
            msKeyboardKeys[GLFW_KEY_G] = EKeyboardKey::G;
            msKeyboardKeys[GLFW_KEY_H] = EKeyboardKey::H;
            msKeyboardKeys[GLFW_KEY_I] = EKeyboardKey::I;
            msKeyboardKeys[GLFW_KEY_J] = EKeyboardKey::J;
            msKeyboardKeys[GLFW_KEY_K] = EKeyboardKey::K;
            msKeyboardKeys[GLFW_KEY_L] = EKeyboardKey::L;
            msKeyboardKeys[GLFW_KEY_M] = EKeyboardKey::M;
            msKeyboardKeys[GLFW_KEY_N] = EKeyboardKey::N;
            msKeyboardKeys[GLFW_KEY_O] = EKeyboardKey::O;
            msKeyboardKeys[GLFW_KEY_P] = EKeyboardKey::P;
            msKeyboardKeys[GLFW_KEY_Q] = EKeyboardKey::Q;
            msKeyboardKeys[GLFW_KEY_R] = EKeyboardKey::R;
            msKeyboardKeys[GLFW_KEY_S] = EKeyboardKey::S;
            msKeyboardKeys[GLFW_KEY_T] = EKeyboardKey::T;
            msKeyboardKeys[GLFW_KEY_U] = EKeyboardKey::U;
            msKeyboardKeys[GLFW_KEY_V] = EKeyboardKey::V;
            msKeyboardKeys[GLFW_KEY_W] = EKeyboardKey::W;
            msKeyboardKeys[GLFW_KEY_X] = EKeyboardKey::X;
            msKeyboardKeys[GLFW_KEY_Y] = EKeyboardKey::Y;
            msKeyboardKeys[GLFW_KEY_Z] = EKeyboardKey::Z;
            msKeyboardKeys[GLFW_KEY_LEFT_BRACKET] = EKeyboardKey::LeftBracket;
            msKeyboardKeys[GLFW_KEY_BACKSLASH] = EKeyboardKey::BackSlash;
            msKeyboardKeys[GLFW_KEY_RIGHT_BRACKET] = EKeyboardKey::RightBracket;
            msKeyboardKeys[GLFW_KEY_GRAVE_ACCENT] = EKeyboardKey::GraveAccent;
            msKeyboardKeys[GLFW_KEY_WORLD_1] = EKeyboardKey::World1;
            msKeyboardKeys[GLFW_KEY_WORLD_2] = EKeyboardKey::World2;
            msKeyboardKeys[GLFW_KEY_ESCAPE] = EKeyboardKey::Escape;
            msKeyboardKeys[GLFW_KEY_ENTER] = EKeyboardKey::Enter;
            msKeyboardKeys[GLFW_KEY_TAB] = EKeyboardKey::Tab;
            msKeyboardKeys[GLFW_KEY_BACKSPACE] = EKeyboardKey::Backspace;
            msKeyboardKeys[GLFW_KEY_INSERT] = EKeyboardKey::Insert;
            msKeyboardKeys[GLFW_KEY_DELETE] = EKeyboardKey::Delete;
            msKeyboardKeys[GLFW_KEY_RIGHT] = EKeyboardKey::Right;
            msKeyboardKeys[GLFW_KEY_LEFT] = EKeyboardKey::Left;
            msKeyboardKeys[GLFW_KEY_DOWN] = EKeyboardKey::Down;
            msKeyboardKeys[GLFW_KEY_UP] = EKeyboardKey::Up;
            msKeyboardKeys[GLFW_KEY_PAGE_UP] = EKeyboardKey::PageUp;
            msKeyboardKeys[GLFW_KEY_PAGE_DOWN] = EKeyboardKey::PageDown;
            msKeyboardKeys[GLFW_KEY_HOME] = EKeyboardKey::Home;
            msKeyboardKeys[GLFW_KEY_END] = EKeyboardKey::End;
            msKeyboardKeys[GLFW_KEY_CAPS_LOCK] = EKeyboardKey::CapsLock;
            msKeyboardKeys[GLFW_KEY_SCROLL_LOCK] = EKeyboardKey::ScrollLock;
            msKeyboardKeys[GLFW_KEY_NUM_LOCK] = EKeyboardKey::NumLock;
            msKeyboardKeys[GLFW_KEY_PRINT_SCREEN] = EKeyboardKey::PrintScreen;
            msKeyboardKeys[GLFW_KEY_PAUSE] = EKeyboardKey::Pause;
            msKeyboardKeys[GLFW_KEY_F1] = EKeyboardKey::F1;
            msKeyboardKeys[GLFW_KEY_F2] = EKeyboardKey::F2;
            msKeyboardKeys[GLFW_KEY_F3] = EKeyboardKey::F3;
            msKeyboardKeys[GLFW_KEY_F4] = EKeyboardKey::F4;
            msKeyboardKeys[GLFW_KEY_F5] = EKeyboardKey::F5;
            msKeyboardKeys[GLFW_KEY_F6] = EKeyboardKey::F6;
            msKeyboardKeys[GLFW_KEY_F7] = EKeyboardKey::F7;
            msKeyboardKeys[GLFW_KEY_F8] = EKeyboardKey::F8;
            msKeyboardKeys[GLFW_KEY_F9] = EKeyboardKey::F9;
            msKeyboardKeys[GLFW_KEY_F10] = EKeyboardKey::F10;
            msKeyboardKeys[GLFW_KEY_F11] = EKeyboardKey::F11;
            msKeyboardKeys[GLFW_KEY_F12] = EKeyboardKey::F12;
            msKeyboardKeys[GLFW_KEY_F13] = EKeyboardKey::F13;
            msKeyboardKeys[GLFW_KEY_F14] = EKeyboardKey::F14;
            msKeyboardKeys[GLFW_KEY_F15] = EKeyboardKey::F15;
            msKeyboardKeys[GLFW_KEY_F16] = EKeyboardKey::F16;
            msKeyboardKeys[GLFW_KEY_F17] = EKeyboardKey::F17;
            msKeyboardKeys[GLFW_KEY_F18] = EKeyboardKey::F18;
            msKeyboardKeys[GLFW_KEY_F19] = EKeyboardKey::F19;
            msKeyboardKeys[GLFW_KEY_F20] = EKeyboardKey::F20;
            msKeyboardKeys[GLFW_KEY_F21] = EKeyboardKey::F21;
            msKeyboardKeys[GLFW_KEY_F22] = EKeyboardKey::F22;
            msKeyboardKeys[GLFW_KEY_F23] = EKeyboardKey::F23;
            msKeyboardKeys[GLFW_KEY_F24] = EKeyboardKey::F24;
            msKeyboardKeys[GLFW_KEY_F25] = EKeyboardKey::F25;
            msKeyboardKeys[GLFW_KEY_KP_0] = EKeyboardKey::KeyPadZero;
            msKeyboardKeys[GLFW_KEY_KP_1] = EKeyboardKey::KeyPadOne;
            msKeyboardKeys[GLFW_KEY_KP_2] = EKeyboardKey::KeyPadTwo;
            msKeyboardKeys[GLFW_KEY_KP_3] = EKeyboardKey::KeyPadThree;
            msKeyboardKeys[GLFW_KEY_KP_4] = EKeyboardKey::KeyPadFour;
            msKeyboardKeys[GLFW_KEY_KP_5] = EKeyboardKey::KeyPadFive;
            msKeyboardKeys[GLFW_KEY_KP_6] = EKeyboardKey::KeyPadSix;
            msKeyboardKeys[GLFW_KEY_KP_7] = EKeyboardKey::KeyPadSeven;
            msKeyboardKeys[GLFW_KEY_KP_8] = EKeyboardKey::KeyPadEight;
            msKeyboardKeys[GLFW_KEY_KP_9] = EKeyboardKey::KeyPadNine;
            msKeyboardKeys[GLFW_KEY_KP_DECIMAL] = EKeyboardKey::KeyPadDecimal;
            msKeyboardKeys[GLFW_KEY_KP_DIVIDE] = EKeyboardKey::KeyPadDivide;
            msKeyboardKeys[GLFW_KEY_KP_MULTIPLY] = EKeyboardKey::KeyPadMultiply;
            msKeyboardKeys[GLFW_KEY_KP_SUBTRACT] = EKeyboardKey::KeyPadSubtract;
            msKeyboardKeys[GLFW_KEY_KP_ADD] = EKeyboardKey::KeyPadAdd;
            msKeyboardKeys[GLFW_KEY_KP_ENTER] = EKeyboardKey::KeyPadEnter;
            msKeyboardKeys[GLFW_KEY_KP_EQUAL] = EKeyboardKey::KeyPadEqual;
            msKeyboardKeys[GLFW_KEY_LEFT_SHIFT] = EKeyboardKey::LeftShift;
            msKeyboardKeys[GLFW_KEY_LEFT_CONTROL] = EKeyboardKey::LeftControl;
            msKeyboardKeys[GLFW_KEY_LEFT_ALT] = EKeyboardKey::LeftAlt;
            msKeyboardKeys[GLFW_KEY_LEFT_SUPER] = EKeyboardKey::LeftSuper;
            msKeyboardKeys[GLFW_KEY_RIGHT_SHIFT] = EKeyboardKey::RightShift;
            msKeyboardKeys[GLFW_KEY_RIGHT_CONTROL] = EKeyboardKey::RightControl;
            msKeyboardKeys[GLFW_KEY_RIGHT_ALT] = EKeyboardKey::RightAlt;
            msKeyboardKeys[GLFW_KEY_RIGHT_SUPER] = EKeyboardKey::RightSuper;
            msKeyboardKeys[GLFW_KEY_MENU] = EKeyboardKey::Menu;

            msMouseButtons[GLFW_MOUSE_BUTTON_LEFT] = EMouseButton::Left;
            msMouseButtons[GLFW_MOUSE_BUTTON_RIGHT] = EMouseButton::Right;
            msMouseButtons[GLFW_MOUSE_BUTTON_MIDDLE] = EMouseButton::Middle;
            msMouseButtons[GLFW_MOUSE_BUTTON_4] = EMouseButton::Four;
            msMouseButtons[GLFW_MOUSE_BUTTON_5] = EMouseButton::Five;
        }

        return lResult;
    }

    EKeyboardKey CGLFWUtil::GetKeyboardKey(int32_t piGLFWKey)
    {
        if (piGLFWKey >= 0 && piGLFWKey <= GLFW_KEY_LAST)
            return msKeyboardKeys[piGLFWKey];

        return EKeyboardKey::Unknown;
    }

    EMouseButton CGLFWUtil::GetMouseButton(int32_t piGLFWButton)
    {
        if (piGLFWButton >= 0 && piGLFWButton <= GLFW_MOUSE_BUTTON_5)
            return msMouseButtons[piGLFWButton];

        return EMouseButton::Unknown;
    }
}