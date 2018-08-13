#pragma once

#include "Core/Event.h"

namespace FDX
{
	class FDX_EXPORT IEventHandler
	{
	public:

		virtual ~IEventHandler()
		{
		}

		virtual bool CanHandleEvent(uint32_t puiEventType) = 0;
		virtual bool HandleEvent(IEvent *pEvent) = 0;
	};
}