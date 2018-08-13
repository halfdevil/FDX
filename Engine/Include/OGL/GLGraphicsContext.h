#pragma once

#include "Graphics/GraphicsContext.h"
#include "OS/Win32GameWindow.h"
#include <Windows.h>

#define OGL_MAJOR_VERSION 4
#define OGL_MINOR_VERSION 2

namespace FDX
{
	class CGLGraphicsContext : public IGraphicsContext
	{
	public:

		CGLGraphicsContext(const CContextProperties &pProperties);
		~CGLGraphicsContext();

		const CContextProperties& GetProperties() const
		{
			return mProperties;
		}

		IGameWindow* GetWindow() const
		{
			return mWindow;
		}

		EResult Create(IGameWindow *pWindow);

		EResult Activate();
		EResult Deactivate();
		EResult SwapBuffers();

		EResult Share(IGameWindow *pWindow,
			IGraphicsContext **pContext);

	private:

		CContextProperties mProperties;
		CWin32GameWindow *mWindow;
		HDC mHDC;
		HGLRC mRC;
	};

	using CGLGraphicsContextPtr = SmartPtr<CGLGraphicsContext>;
}