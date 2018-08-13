
#include "GUI/ControlEvent.h"

namespace FDX
{
	CControlEvent::CControlEvent(EControlEventType pUIEventType)
		: mControlEventType(pUIEventType),
		mPriority(EEventPriority::Normal)
	{
	}

	CControlEvent::~CControlEvent()
	{
	}

	void CControlEvent::SetEventPriority(EEventPriority pPriority)
	{
		mPriority = pPriority;
	}

	CMouseEnterControlEvent::CMouseEnterControlEvent(CMouseEvent * pMouseEvent)
		: CControlEvent(EControlEventType::MouseEnter),
		mMouseEvent(pMouseEvent)
	{
	}

	CMouseLeaveControlEvent::CMouseLeaveControlEvent(CMouseEvent * pMouseEvent)
		: CControlEvent(EControlEventType::MouseLeave),
		mMouseEvent(pMouseEvent)
	{
	}

	CMouseDownControlEvent::CMouseDownControlEvent(CMouseEvent * pMouseEvent)
		: CControlEvent(EControlEventType::MouseDown),
		mMouseEvent(pMouseEvent)
	{
	}

	CMouseUpControlEvent::CMouseUpControlEvent(CMouseEvent * pMouseEvent)
		: CControlEvent(EControlEventType::MouseUp),
		mMouseEvent(pMouseEvent)
	{
	}

	CMouseMoveControlEvent::CMouseMoveControlEvent(CMouseEvent * pMouseEvent)
		: CControlEvent(EControlEventType::MouseMove),
		mMouseEvent(pMouseEvent)
	{
	}

	CKeyDownControlEvent::CKeyDownControlEvent(CKeyboardEvent * pKeyboardEvent)
		: CControlEvent(EControlEventType::KeyDown),
		mKeyboardEvent(pKeyboardEvent)
	{
	}

	CKeyUpControlEvent::CKeyUpControlEvent(CKeyboardEvent * pKeyboardEvent)
		: CControlEvent(EControlEventType::KeyUp),
		mKeyboardEvent(pKeyboardEvent)
	{
	}
}