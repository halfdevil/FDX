#pragma once

#include "OS/Thread.h"
#include "OS/Task.h"

namespace FDX
{
	class FDX_EXPORT CWorkerThread : public CThread
	{
	public:

		CWorkerThread();
		~CWorkerThread();

		uint32_t GetSize();

		void AddTask(CTask *pTask);
		void CancelTask(uint32_t puiID);
		void CancelAll();		

	protected:

		virtual void Run() override;
		virtual void NotifyExit() override;

		CTaskPtr ExtractTask();

	private:
				
		List<CTaskPtr> mPendingTasks;
		CTaskPtr mCurrentTask;
		std::mutex mTaskMutex;
		std::condition_variable mCondition;
	};

	using CWorkerThreadPtr = std::unique_ptr<CWorkerThread>;
}
