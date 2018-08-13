
#include "OSXSample.h"

EResult COSXSample::OnUpdate()
{
    if (GetKeyboard()->KeyDown(EKeyboardKey::Escape))
        Exit();

    return EResult::Success;
}

EResult COSXSample::OnDraw()
{
    EResult lResult = EResult::Success;

    GetGraphicsDevice()->Clear(EClearOptions::Color | EClearOptions::Depth,
                               CColor4F(0.0f, 0.5f, 0.5f), 0.0f, 1);

    return lResult;
}

int main()
{
    EResult lResult;
    COSXSample lSample;

    lResult = lSample.Initialize();
    if (lResult != EResult::Success)
    {
        return -1;
    }
    else
    {
        lSample.GetWindow()->Show();

        while (!lSample.GetWindow()->IsClosed())
        {
            lSample.Update();
            lSample.Frame();

            ISystem::Get().PollEvents();
        }
    }

    return 0;
}