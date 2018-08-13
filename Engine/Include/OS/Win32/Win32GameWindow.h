#pragma once

#include "OS/GameWindow.h"
#include <Windows.h>

namespace FDX
{
	class FDX_EXPORT CWin32GameWindow : public IGameWindow
	{
	public:

		CWin32GameWindow(const CWindowParameters &pParameters);
		~CWin32GameWindow();

		const CWindowParameters& GetParameters() const override
		{
			return mParameters;
		}

		const Array<CGameWindowCallback*>& GetCallbacks() const
		{
			return mCallbacks;
		}

		HWND GetHWND() const
		{
			return mHwnd;
		}

		EResult Create(HINSTANCE pInstance);
		CVector2F GetCursorPosition() const;

		EResult Show() override;
		EResult Hide() override;
		EResult Activate() override;
		EResult Deactivate() override;
		EResult Close() override;
		EResult SetTitle(const String &pstrTitle) override;

		void AddCallback(CGameWindowCallback *pCallback);
		void RemoveCallback(CGameWindowCallback *pCallback);

	private:

		static LRESULT CALLBACK WndProc(HWND hWnd, 
			UINT msg, 
			WPARAM wParam, 
			LPARAM lParam);

		Array<CGameWindowCallback*> mCallbacks;
		CWindowParameters mParameters;
		HWND mHwnd;
	};
}
