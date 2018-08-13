
#include "OS/WorkerThread.h"

namespace FDX
{
	CWorkerThread::CWorkerThread()
	{
	}

	CWorkerThread::~CWorkerThread()
	{
	}

	uint32_t CWorkerThread::GetSize()
	{
		std::lock_guard<std::mutex> lLock(mTaskMutex);
		return mPendingTasks.size() + (mCurrentTask ? 1 : 0);
	}

	void CWorkerThread::AddTask(CTask * pTask)
	{
		std::lock_guard<std::mutex> lLock(mTaskMutex);
		
		mPendingTasks.push_back(CTaskPtr(pTask));
		mCondition.notify_all();
	}

	void CWorkerThread::CancelTask(uint32_t puiID)
	{
		std::lock_guard<std::mutex> lLock(mTaskMutex);

		if (mCurrentTask && mCurrentTask->GetID() == puiID)
			mCurrentTask->Exit();

		for (auto &lIt : mPendingTasks)
		{
			if (lIt->GetID() == puiID)
				lIt->Exit();
		}
		
		mCondition.notify_all();
	}

	void CWorkerThread::CancelAll()
	{
		std::lock_guard<std::mutex> lLock(mTaskMutex);

		if (mCurrentTask)
			mCurrentTask->Exit();

		for (auto &lTask : mPendingTasks)
		{
			lTask->Exit();
		}

		mPendingTasks.clear();
		mCondition.notify_all();
	}

	void CWorkerThread::Run()
	{
		while (!IsPendingExit())
		{
			mCurrentTask = ExtractTask();

			if (mCurrentTask && !mCurrentTask->IsPendingExit())
				mCurrentTask->Run();

			mCurrentTask = nullptr;
		}
	}

	void CWorkerThread::NotifyExit()
	{
		mCondition.notify_all();
	}

	CTaskPtr CWorkerThread::ExtractTask()
	{
		std::unique_lock<std::mutex> lLock(mTaskMutex);

		while (mPendingTasks.empty() && !IsPendingExit())
			mCondition.wait(lLock);

		if (mPendingTasks.empty())
			return CTaskPtr();

		auto lBestTaskIt = mPendingTasks.begin();
		auto lIt = mPendingTasks.begin();

		while (lIt != mPendingTasks.end())
		{
			if ((*lIt)->GetPriority() > (*lBestTaskIt)->GetPriority())
				lBestTaskIt = lIt;

			lIt++;
		}

		CTaskPtr lBestTask = std::move(*lBestTaskIt);
		mPendingTasks.erase(lBestTaskIt);

		return lBestTask;
	}
}