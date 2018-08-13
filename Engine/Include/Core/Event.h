#pragma once

#include "Core/Core.h"

namespace FDX
{
	enum class EEventPriority : uint32_t
	{
		Low = 0,
		Normal,
		High,
		VeryHigh
	};

	class FDX_EXPORT IEvent
	{
	public:

		virtual ~IEvent()
		{
		}

		virtual uint32_t GetEventType() const = 0;
		virtual EEventPriority GetEventPriority() const = 0;

		virtual void SetEventPriority(EEventPriority pPriority) = 0;
	};

	using IEventPtr = SmartPtr<IEvent>;
}