
#include "OAL/ALAudioDevice.h"
#include "OAL/ALAudioSource.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CALAudioDevice::CALAudioDevice()
		: mDevice(nullptr),
		mContext(nullptr),
		mAudioThread(nullptr)
	{
	}

	CALAudioDevice::~CALAudioDevice()
	{
		if (mAudioThread != nullptr)
			mAudioThread->Exit(true);
	}

	EResult CALAudioDevice::Create(bool pbSeparateThread)
	{
		EResult lResult = EResult::Success;

		mDevice = alcOpenDevice(nullptr);
		if (!mDevice)
		{
			LOG_DEBUG("alcOpenDevice() failed");
			lResult = EResult::CreateFailed;
		}
		else
		{
			mContext = alcCreateContext(mDevice, nullptr);
			if (!mContext)
			{
				LOG_DEBUG("alcCreateContext() failed");
				lResult = EResult::CreateFailed;
			}
			else
			{
				if (pbSeparateThread)
				{
					mAudioThread = std::make_unique<CALAudioThread>(mContext);
					mAudioThread->Run();
				}
				else
				{
					alcMakeContextCurrent(mContext);
				}
			}
		}

		return lResult;
	}

	EResult CALAudioDevice::CreateAudioSource(IAudioDataProvider * pDataProvider, IAudioSource ** pAudioSource)
	{
		EResult lResult = EResult::Success;

		CALAudioSource *lAudioSource = new CALAudioSource();
		lResult = lAudioSource->Create(pDataProvider);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CALAudioSource::Create() failed");
			SAFE_DELETE(lAudioSource);
		}
		else
		{
			*pAudioSource = lAudioSource;
		}

		return lResult;
	}
}