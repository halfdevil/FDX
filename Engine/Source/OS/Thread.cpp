
#include "OS/Thread.h"

namespace FDX
{
	CThread::CThread()
		: mbPendingExit(false)
	{
	}

	CThread::~CThread()
	{
	}

	void CThread::Start()
	{
		mThread = std::thread{ &EntryPoint, this };
	}

	void CThread::Exit(bool pbWait)
	{
		mbPendingExit = true;
		NotifyExit();

		if (!pbWait)
			return;

		mThread.join();
	}

	void CThread::EntryPoint(CThread * pThread)
	{
		if (pThread)
			pThread->Run();
	}
}