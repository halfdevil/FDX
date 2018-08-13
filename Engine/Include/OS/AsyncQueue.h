#pragma once

#include "OS/Thread.h"
#include "OS/Task.h"

namespace FDX
{
	class FDX_EXPORT IAsyncCapsule
	{
	public:

		virtual void Invoke() = 0;
	};

	using IAsyncCapsulePtr = std::unique_ptr<IAsyncCapsule>;

	class FDX_EXPORT CAsyncQueue
	{
	public:

		CAsyncQueue();
		~CAsyncQueue();

		virtual void EnqueueCapsule(IAsyncCapsule *pCapsule);
		virtual void DemultiplexEvents();

	private:

		Array<IAsyncCapsulePtr> mQueues[2];
		Array<IAsyncCapsulePtr> *mCurrentQueue;
		uint32_t muiCurrQueueIndex;
		std::mutex mDemultiplexMutex;
	};

	using CAsyncQueuePtr = SmartPtr<CAsyncQueue>;
}
