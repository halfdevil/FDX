#pragma once

#include "OS/Thread.h"
#include "OpenAL/alc.h"

namespace FDX
{
	class CALAudioThread : public CThread
	{
	public:

		CALAudioThread(ALCcontext *pContext);
		~CALAudioThread();

		void Run() override;
		void WaitForInit();

	private:

		ALCcontext *mContext;
		std::atomic<bool> mbInitialized;
	};

	using CALAudioThreadPtr = SmartPtr<CALAudioThread>;
}