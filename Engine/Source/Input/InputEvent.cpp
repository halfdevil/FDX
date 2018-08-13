
#include "Input/InputEvent.h"

namespace FDX
{
	CKeyboardEvent::CKeyboardEvent(EInputEventType pEventType)
		: mEventType(pEventType)
	{
	}

	void CKeyboardEvent::SetEventPriority(EEventPriority pPriority)
	{
		mPriority = pPriority;
	}

	void CKeyboardEvent::SetKey(EKeyboardKey pKey)
	{
		mKey = pKey;
	}

	CMouseEvent::CMouseEvent(EInputEventType pEventType)
		: mEventType(pEventType)
	{
	}

	void CMouseEvent::SetEventPriority(EEventPriority pPriority)
	{
		mPriority = pPriority;
	}

	void CMouseEvent::SetButton(EMouseButton pButton)
	{
		mButton = pButton;
	}

	void CMouseEvent::SetRelativePosition(const CVector2F & pPosition)
	{
		mRelativePosition = pPosition;
	}

	void CMouseEvent::SetAbsolutePosition(const CVector2F & pPosition)
	{
		mAbsolutePosition = pPosition;
	}
}