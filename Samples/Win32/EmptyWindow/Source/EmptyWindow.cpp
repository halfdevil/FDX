
#include "EmptyWindow.h"
#include <Windows.h>

EResult CEmptyWindow::OnUpdate()
{
	if (GetKeyboard()->KeyDown(EKeyboardKey::Escape))
		Exit();

	return EResult::Success;
}

EResult CEmptyWindow::OnDraw()
{
	EResult lResult = EResult::Success;

	GetGraphicsDevice()->Clear(EClearOptions::Color | EClearOptions::Depth,
		CColor4F(0.0f, 0.5f, 0.5f), 0.0f, 1);

	return lResult;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR pCmdLine, int nCmdShow)
{
	EResult lResult;
	CEmptyWindow lSample;

	lResult = lSample.Initialize();
	if (lResult != EResult::Success)
	{
		return -1;
	}
	else
	{
		lSample.Run();
	}

	return 0;
}