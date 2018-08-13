
#include "OAL/ALAudioThread.h"
#include "OS/System.h"

namespace FDX
{
	CALAudioThread::CALAudioThread(ALCcontext * pContext)
		: mContext(pContext),
		mbInitialized(false)
	{
	}

	CALAudioThread::~CALAudioThread()
	{
	}

	void CALAudioThread::Run()
	{
		alcMakeContextCurrent(mContext);
		mbInitialized = true;

		while (!IsPendingExit())
			ISystem::Get().Sleep(100);

		alcMakeContextCurrent(nullptr);
	}

	void CALAudioThread::WaitForInit()
	{
		while (!mbInitialized)
		{
		}
	}
}