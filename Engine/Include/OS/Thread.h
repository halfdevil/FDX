#pragma once

#include "Core/Core.h"
#include <thread>
#include <mutex>

namespace FDX
{
	class FDX_EXPORT CThread
	{
	public:

		CThread();
		virtual ~CThread();

		bool IsPendingExit() const volatile
		{
			return mbPendingExit;
		}

		void Start();
		void Exit(bool pbWait);

	protected:

		virtual void Run() = 0;
		virtual void NotifyExit() {}

	private:

		static void EntryPoint(CThread *pThread);

		std::thread mThread;
		std::atomic<bool> mbPendingExit;
	};

	using CThreadPtr = SmartPtr<CThread>;
}
