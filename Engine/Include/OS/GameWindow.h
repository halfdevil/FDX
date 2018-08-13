#pragma once

#include "Core/Core.h"
#include "Core/Result.h"
#include "Math/Vector2F.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

namespace FDX
{
	class IGameWindow;

	class FDX_EXPORT CWindowParameters
	{
	public:

		CWindowParameters()
			: muiWidth(0),
			muiHeight(0),
			mbFullscreen(false)
		{

		}

		CWindowParameters(uint32_t puiWidth,
			uint32_t puiHeight,
			String pstrTitle,
			bool pbFullscreen)
			: muiWidth(puiWidth),
			muiHeight(puiHeight),
			mstrTitle(pstrTitle),
			mbFullscreen(pbFullscreen)
		{

		}

		uint32_t GetWidth() const
		{
			return muiWidth;
		}

		uint32_t GetHeight() const
		{
			return muiHeight;
		}

		const String& GetTitle() const
		{
			return mstrTitle;
		}

		bool IsFullscreen() const
		{
			return mbFullscreen;
		}

		void SetWidth(uint32_t puiWidth)
		{
			muiWidth = puiWidth;
		}

		void SetHeight(uint32_t puiHeight)
		{
			muiHeight = puiHeight;
		}

		void SetTitle(const String &pstrTitle)
		{
			mstrTitle = pstrTitle;
		}

		void SetFullscreen(bool pbFullscreen)
		{
			mbFullscreen = pbFullscreen;
		}
		
	private:

		uint32_t muiWidth;
		uint32_t muiHeight;
		String mstrTitle;
		bool mbFullscreen;
	};

	class FDX_EXPORT CGameWindowCallback
	{
	public:

		virtual ~CGameWindowCallback() = default;

		virtual void OnCreate(IGameWindow *pWindow) {}
		virtual void OnDestroy(IGameWindow *pWindow) {}

		virtual void OnActivate(IGameWindow *pWindow, bool pbActive) {}
		virtual void OnResize(IGameWindow *pWindow, uint32_t puiWidth, uint32_t puiHeight) {}

		virtual void OnKeyDown(IGameWindow *pWindow, EKeyboardKey pKey) {}
		virtual void OnKeyUp(IGameWindow *pWindow, EKeyboardKey pKey) {}
		virtual void OnMouseDown(IGameWindow *pWindow, EMouseButton pButton) {}
		virtual void OnMouseUp(IGameWindow *pWindow, EMouseButton pButton) {}
		virtual void OnMouseMove(IGameWindow *pWindow, uint32_t puiX, uint32_t puiY) {}
		virtual void OnMouseEnter(IGameWindow *pWindow) {}
		virtual void OnMouseLeave(IGameWindow *pWindow) {}
	};

	class FDX_EXPORT IGameWindow
	{
	public:

		virtual ~IGameWindow() = default;

		virtual const CWindowParameters& GetParameters() const = 0;
		virtual const Array<CGameWindowCallback*>& GetCallbacks() const = 0;

		virtual CVector2F GetCursorPosition() const = 0;
		virtual bool IsClosed() const = 0;

		virtual EResult Show() = 0;
		virtual EResult Hide() = 0;
		virtual EResult Activate() = 0;
		virtual EResult Deactivate() = 0;
		virtual EResult Close() = 0;
		virtual EResult SetTitle(const String &pstrTitle) = 0;

		virtual void AddCallback(CGameWindowCallback *pCallback) = 0;
		virtual void RemoveCallback(CGameWindowCallback *pCallback) = 0;
	};

	using IGameWindowPtr = SmartPtr<IGameWindow>;
}
