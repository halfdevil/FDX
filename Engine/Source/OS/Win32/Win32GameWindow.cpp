
#include "OS/Win32/Win32GameWindow.h"
#include "OS/Win32/Win32UnicodeAdapter.h"
#include "OS/GameLogger.h"
#include "OS/System.h"
#include <windowsx.h>

namespace FDX
{
	CWin32GameWindow::CWin32GameWindow(const CWindowParameters & pParameters)
		: mParameters(pParameters),
		mHwnd(nullptr)
	{
	}

	CWin32GameWindow::~CWin32GameWindow()
	{
		mHwnd = nullptr;
	}

	EResult CWin32GameWindow::Create(HINSTANCE pInstance)
	{
		UTF16String lstrTitle = CWin32UTF16Adapter(
			mParameters.GetTitle()).Get();

		WNDCLASSEX lWndClass =
		{
			sizeof(WNDCLASSEX),
			CS_HREDRAW | CS_VREDRAW,
			WndProc,
			0,
			sizeof(CWin32GameWindow),
			pInstance,
			LoadIcon(nullptr, IDI_APPLICATION),
			LoadCursor(nullptr, IDC_ARROW),
			nullptr,
			nullptr,
			lstrTitle.c_str(),
			LoadIcon(nullptr, IDI_WINLOGO)
		};

		if (!RegisterClassEx(&lWndClass))
		{
			LOG_ERROR("Unable to register Win32 class: %s", mParameters.GetTitle().c_str());
			return EResult::CreateFailed;
		}

		RECT lDesktopRect;
		GetWindowRect(GetDesktopWindow(), &lDesktopRect);

		uint32_t x = (lDesktopRect.right - lDesktopRect.left) / 2 - mParameters.GetWidth() / 2;
		uint32_t y = (lDesktopRect.bottom - lDesktopRect.top) / 2 - mParameters.GetHeight() / 2;

		mHwnd = CreateWindowEx(
			WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
			lstrTitle.c_str(),
			lstrTitle.c_str(),
			WS_OVERLAPPEDWINDOW,
			x,
			y,
			mParameters.GetWidth(),
			mParameters.GetHeight(),
			nullptr,
			nullptr,
			pInstance,
			this);

		if (mHwnd == nullptr)
		{
			LOG_ERROR("Unable to create win32 window: %s", mParameters.GetTitle().c_str());
			UnregisterClass(lstrTitle.c_str(), pInstance);

			return EResult::CreateFailed;
		}

		return EResult::Success;
	}

	CVector2F CWin32GameWindow::GetCursorPosition() const
	{
		POINT lCursorPos;

		GetCursorPos(&lCursorPos);
		ScreenToClient(mHwnd, &lCursorPos);

		return CVector2F((float)lCursorPos.x, (float)lCursorPos.y);
	}

	EResult CWin32GameWindow::Show()
	{
		FDX_ASSERT(mHwnd != nullptr, "Window Handle is invalid");

		return ShowWindow(mHwnd, SW_SHOW) ? 
			EResult::Success : EResult::Error;
	}

	EResult CWin32GameWindow::Hide()
	{
		FDX_ASSERT(mHwnd != nullptr, "Window Handle is invalid");

		return ShowWindow(mHwnd, SW_HIDE) ? 
			EResult::Success : EResult::Error;
	}

	EResult CWin32GameWindow::Activate()
	{
		FDX_ASSERT(mHwnd != nullptr, "Window Handle is invalid");

		return EnableWindow(mHwnd, TRUE) ? 
			EResult::Success : EResult::Error;
	}

	EResult CWin32GameWindow::Deactivate()
	{
		FDX_ASSERT(mHwnd != nullptr, "Window Handle is invalid");

		return EnableWindow(mHwnd, FALSE) ? 
			EResult::Success : EResult::Error;
	}

	EResult CWin32GameWindow::Close()
	{
		FDX_ASSERT(mHwnd != nullptr, "Window Handle is invalid");

		DestroyWindow(mHwnd);
		PostQuitMessage(0);
		
		return EResult::Success;
	}

	EResult CWin32GameWindow::SetTitle(const String & pstrTitle)
	{
		FDX_ASSERT(mHwnd != nullptr, "Window Handle is invalid");

		return SetWindowText(mHwnd, CWin32UTF16Adapter(pstrTitle).
			Get().c_str()) ?	EResult::Success : EResult::Error;
	}

	void CWin32GameWindow::AddCallback(CGameWindowCallback *pCallback)
	{
		auto lIt = std::find(mCallbacks.begin(), 
			mCallbacks.end(), pCallback);

		if (lIt == mCallbacks.end())
			mCallbacks.push_back(pCallback);
	}

	void CWin32GameWindow::RemoveCallback(CGameWindowCallback *pCallback)
	{
		auto lIt = std::find(mCallbacks.begin(),
			mCallbacks.end(), pCallback);

		if (lIt != mCallbacks.end())
			mCallbacks.erase(lIt);
	}

	LRESULT CWin32GameWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		CWin32GameWindow *lWindow = nullptr;

		if (msg == WM_CREATE)
		{
			lWindow = reinterpret_cast<CWin32GameWindow*>(((LPCREATESTRUCT)lParam)->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(lWindow));
		}
		else
		{
			lWindow = reinterpret_cast<CWin32GameWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

			if (!lWindow)
				return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		switch (msg)
		{
		case WM_CREATE:
			{
				for (const auto &lCallback : lWindow->mCallbacks)
					lCallback->OnCreate(lWindow);
			}
			break;

		case WM_SIZE:
			{
				for (const auto &lCallback : lWindow->mCallbacks)
					lCallback->OnResize(lWindow, LOWORD(lParam), HIWORD(lParam));
			}
			break;

		case WM_ACTIVATE:
			{
				for (const auto &lCallback : lWindow->mCallbacks)
					lCallback->OnActivate(lWindow, LOWORD(wParam) == WA_ACTIVE);
			}
			break;

		case WM_MOUSEMOVE:
			{
				for (const auto &lCallback : lWindow->mCallbacks)
					lCallback->OnMouseMove(lWindow, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			}
			break;

		case WM_MOUSELEAVE:
			{
				for (const auto &lCallback : lWindow->mCallbacks)
					lCallback->OnMouseLeave(lWindow);
			}
			break;

		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}