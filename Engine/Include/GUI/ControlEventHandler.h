#pragma once

#include "Core/EventHandler.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "GUI/ControlEvent.h"

namespace FDX
{
	class CControl;
	using CControlEventCallback = std::function<bool(CControl*, CControlEvent*)>;

	class FDX_EXPORT CControlEventHandler
	{
	public:

		CControlEventHandler(CControl *pControl);
		virtual ~CControlEventHandler();

		CControl* GetControl() const
		{
			return mControl;
		}

		bool IsMouseOver() const
		{
			return mbIsMouseOver;
		}

		bool HasFocus() const
		{
			return mbHasFocus;
		}

		virtual bool HandleEvent(CControlEvent *pEvent);
		virtual void AddCallback(EControlEventType pType, 
			CControlEventCallback pCallback);	

		virtual void SetIsMouseOver(bool pbIsOver);
		virtual void SetHasFocus(bool pbFocus);

	protected:

		virtual bool ExecuteCallback(CControlEvent *pEvent);
		virtual bool OnGotFocus(CGotFocusControlEvent *pEvent);
		virtual bool OnLostFocus(CLostFocusControlEvent *pEvent);
		virtual bool OnMouseEnter(CMouseEnterControlEvent *pEvent);
		virtual bool OnMouseLeave(CMouseLeaveControlEvent *pEvent);
		virtual bool OnMouseDown(CMouseDownControlEvent *pEvent);
		virtual bool OnMouseUp(CMouseUpControlEvent *pEvent);
		virtual bool OnMouseMove(CMouseMoveControlEvent *pEvent);
		virtual bool OnKeyDown(CKeyDownControlEvent *pEvent);
		virtual bool OnKeyUp(CKeyUpControlEvent *pEvent);

	protected:

		CControl * mControl;		
		Map<EControlEventType, Array<CControlEventCallback>> mCallbacks;

		bool mbIsMouseOver;
		bool mbHasFocus;
	};
}