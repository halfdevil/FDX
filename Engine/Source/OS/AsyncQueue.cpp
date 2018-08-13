
#include "OS/AsyncQueue.h"

namespace FDX
{
	CAsyncQueue::CAsyncQueue()
		: muiCurrQueueIndex(0),
		mCurrentQueue(&mQueues[0])
	{
	}

	CAsyncQueue::~CAsyncQueue()
	{
	}

	void CAsyncQueue::EnqueueCapsule(IAsyncCapsule * pCapsule)
	{
		std::lock_guard<std::mutex> lLock(mDemultiplexMutex);
		mCurrentQueue->push_back(IAsyncCapsulePtr(pCapsule));
	}

	void CAsyncQueue::DemultiplexEvents()
	{
		Array<IAsyncCapsulePtr> &lQueue = mQueues[muiCurrQueueIndex];
		{
			std::lock_guard<std::mutex> lLock(mDemultiplexMutex);
			muiCurrQueueIndex = (muiCurrQueueIndex + 1) % 2;
			mCurrentQueue = &mQueues[muiCurrQueueIndex];
		}

		for (auto &lCapsule : lQueue)
		{
			lCapsule->Invoke();
		}

		lQueue.clear();
	}
}