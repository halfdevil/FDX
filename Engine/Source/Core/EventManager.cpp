
#include "Core/EventManager.h"

namespace FDX
{
	CEventManager::CEventManager()
		: mEventQueue(&EventComparer),
		muiThreshold(1)
	{
	}

	CEventManager::~CEventManager()
	{
	}

	void CEventManager::RegisterHandler(IEventHandler * pHandler)
	{
		auto lIt = std::find(mEventHandlers.begin(), 
			mEventHandlers.end(), pHandler);

		if (lIt == mEventHandlers.end())
			mEventHandlers.push_back(pHandler);
	}

	void CEventManager::QueueEvent(IEventPtr pEvent)
	{
		mEventQueue.push(std::move(pEvent));
	}

	EResult CEventManager::ProcessEventImmediate(IEvent *pEvent)
	{
		EResult lResult = EResult::Success;

		for (const auto &lHandler : mEventHandlers)
		{
			if (lHandler->CanHandleEvent(pEvent->GetEventType()))
				lHandler->HandleEvent(pEvent);
		}

		return lResult;
	}

	EResult CEventManager::ProcessEvents()
	{
		EResult lResult = EResult::Success;
		uint32_t luiEventsProcessed = 0;

		while (!mEventQueue.empty() && luiEventsProcessed < muiThreshold)
		{
			IEvent *lEvent = mEventQueue.top().get();
			
			for (auto lHandler : mEventHandlers)
			{
				if (lHandler->CanHandleEvent(lEvent->GetEventType()))
					lHandler->HandleEvent(lEvent);
			}

			mEventQueue.pop();
			luiEventsProcessed++;
		}

		return lResult;
	}

	void CEventManager::SetThreshold(uint32_t puiThreshold)
	{
		muiThreshold = puiThreshold;
	}

	bool EventComparer(const IEventPtr & pLeft, const IEventPtr & pRight)
	{
		return (uint32_t)pLeft->GetEventPriority() >=
			(uint32_t)pRight->GetEventPriority();
	}
}