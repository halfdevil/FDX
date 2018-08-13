#pragma once

#include "Core/Event.h"
#include "Math/Vector2F.h"
#include "Input/Mouse.h"
#include "Input/Keyboard.h"
#include "Input/InputEvent.h"

namespace FDX
{
	enum class EControlEventType : uint32_t
	{
		GotFocus,
		LostFocus,
		MouseEnter,		
		MouseLeave,
		MouseDown,
		MouseUp,
		MouseMove,
		KeyDown,
		KeyUp
	};

	class FDX_EXPORT CControlEvent : public IEvent
	{
	public:

		CControlEvent(EControlEventType pUIEventType);
		virtual ~CControlEvent();

		uint32_t GetEventType() const
		{
			return (uint32_t)mControlEventType;
		}

		EControlEventType GetControlEventType() const
		{
			return mControlEventType;
		}

		EEventPriority GetEventPriority() const
		{
			return mPriority;
		}

		virtual void SetEventPriority(EEventPriority pPriority);

	protected:

		EControlEventType mControlEventType;
		EEventPriority mPriority;
	};

	class FDX_EXPORT CGotFocusControlEvent : public CControlEvent
	{
	public:

		CGotFocusControlEvent()
			: CControlEvent(EControlEventType::GotFocus)
		{
		}
	};

	class FDX_EXPORT CLostFocusControlEvent : public CControlEvent
	{
	public:

		CLostFocusControlEvent()
			: CControlEvent(EControlEventType::LostFocus)
		{
		}
	};

	class FDX_EXPORT CMouseEnterControlEvent : public CControlEvent
	{
	public:

		CMouseEnterControlEvent(CMouseEvent *pMouseEvent);

		const CVector2F& GetAbsolutePosition() const
		{
			return mMouseEvent->GetAbsolutePosition();
		}

		const CVector2F& GetRelativePosition() const
		{
			return mMouseEvent->GetRelativePosition();
		}

	private:

		CMouseEvent * mMouseEvent;
	};

	class FDX_EXPORT CMouseLeaveControlEvent : public CControlEvent
	{
	public:

		CMouseLeaveControlEvent(CMouseEvent *pMouseEvent);

		const CVector2F& GetAbsolutePosition() const
		{
			return mMouseEvent->GetAbsolutePosition();
		}

		const CVector2F& GetRelativePosition() const
		{
			return mMouseEvent->GetRelativePosition();
		}

	private:

		CMouseEvent * mMouseEvent;
	};

	class FDX_EXPORT CMouseDownControlEvent : public CControlEvent
	{
	public:

		CMouseDownControlEvent(CMouseEvent *pMouseEvent);

		EMouseButton GetButton() const
		{
			return mMouseEvent->GetButton();
		}

		const CVector2F& GetAbsolutePosition() const
		{
			return mMouseEvent->GetAbsolutePosition();
		}

	private:

		CMouseEvent * mMouseEvent;
	};

	class FDX_EXPORT CMouseUpControlEvent : public CControlEvent
	{
	public:

		CMouseUpControlEvent(CMouseEvent *pMouseEvent);

		EMouseButton GetButton() const
		{
			return mMouseEvent->GetButton();
		}

	private:

		CMouseEvent * mMouseEvent;
	};

	class FDX_EXPORT CMouseMoveControlEvent : public CControlEvent
	{
	public:

		CMouseMoveControlEvent(CMouseEvent *pMouseEvent);

		const CVector2F& GetAbsolutePosition() const
		{
			return mMouseEvent->GetAbsolutePosition();
		}

		const CVector2F& GetRelativePosition() const
		{
			return mMouseEvent->GetRelativePosition();
		}

	private:

		CMouseEvent * mMouseEvent;
	};

	class FDX_EXPORT CKeyDownControlEvent : public CControlEvent
	{
	public:

		CKeyDownControlEvent(CKeyboardEvent *pKeyboardEvent);

		EKeyboardKey GetKey() const
		{
			return mKeyboardEvent->GetKey();
		}

	private:

		CKeyboardEvent * mKeyboardEvent;
	};

	class FDX_EXPORT CKeyUpControlEvent : public CControlEvent
	{
	public:

		CKeyUpControlEvent(CKeyboardEvent *pKeyboardEvent);

		EKeyboardKey GetKey() const
		{
			return mKeyboardEvent->GetKey();
		}

	private:

		CKeyboardEvent * mKeyboardEvent;
	};
}