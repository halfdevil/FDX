#pragma once

#include "Core/Event.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

namespace FDX
{
	enum class EInputEventType : uint32_t
	{
		KeyDownEvent,
		KeyUpEvent,
		MouseMoveEvent,
		MouseDownEvent,
		MouseUpEvent
	};

	class FDX_EXPORT CKeyboardEvent : public IEvent
	{
	public:

		CKeyboardEvent(EInputEventType pEventType);

		uint32_t GetEventType() const
		{
			return (uint32_t)mEventType;
		}

		EEventPriority GetEventPriority() const
		{
			return mPriority;
		}
		
		EKeyboardKey GetKey() const
		{
			return mKey;
		}

		virtual void SetEventPriority(EEventPriority pPriority);
		virtual void SetKey(EKeyboardKey pKey);

	protected:
		
		EInputEventType mEventType;
		EKeyboardKey mKey;
		EEventPriority mPriority;
	};

	class FDX_EXPORT CMouseEvent : public IEvent
	{
	public:

		CMouseEvent(EInputEventType pEventType);

		uint32_t GetEventType() const
		{
			return (uint32_t)mEventType;
		}

		EEventPriority GetEventPriority() const
		{
			return mPriority;
		}

		EMouseButton GetButton() const
		{
			return mButton;
		}

		const CVector2F& GetRelativePosition() const
		{
			return mRelativePosition;
		}

		const CVector2F& GetAbsolutePosition() const
		{
			return mAbsolutePosition;
		}

		virtual void SetEventPriority(EEventPriority pPriority);
		virtual void SetButton(EMouseButton pButton);
		virtual void SetRelativePosition(const CVector2F &pPosition);
		virtual void SetAbsolutePosition(const CVector2F &pPosition);

	protected:

		EInputEventType mEventType;
		CVector2F mRelativePosition;
		CVector2F mAbsolutePosition;
		EMouseButton mButton;
		EEventPriority mPriority;		
	};
}