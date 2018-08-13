#pragma once

#include "Core/Singleton.h"
#include "Core/EventHandler.h"
#include "Core/Result.h"

namespace FDX
{
	bool EventComparer(const IEventPtr &left, const IEventPtr &right);

	class FDX_EXPORT CEventManager : public CSingleton<CEventManager>
	{
	public:

		CEventManager();
		virtual ~CEventManager();

		uint32_t GetThreshold() const
		{
			return muiThreshold;
		}

		CEventManager(const CEventManager&) = delete;
		CEventManager& operator=(const CEventManager&) = delete;

		virtual void RegisterHandler(IEventHandler *pHandler);
		virtual void QueueEvent(IEventPtr pEvent);

		virtual EResult ProcessEventImmediate(IEvent *pEvent);
		virtual EResult ProcessEvents();

		virtual void SetThreshold(uint32_t puiThreshold);

	protected:

		PriorityQueue<IEventPtr, Array<IEventPtr>, decltype(&EventComparer)> mEventQueue;
		Array<IEventHandler*> mEventHandlers;
		uint32_t muiThreshold;
	};

	using CEventManagerPtr = SmartPtr<CEventManager>;
}