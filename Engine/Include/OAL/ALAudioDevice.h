#pragma once

#include "Audio/AudioDevice.h"
#include "OAL/ALAudioThread.h"

#include "OpenAL/al.h"
#include "OpenAL/alc.h"

namespace FDX
{
	class CALAudioDevice : public IAudioDevice
	{
	public:

		CALAudioDevice();
		~CALAudioDevice();

		EResult Create(bool pbSeparateThread);
		EResult CreateAudioSource(IAudioDataProvider *pDataProvider,
			IAudioSource **pAudioSource);

	private:

		ALCdevice *mDevice;
		ALCcontext *mContext;
		CALAudioThreadPtr mAudioThread;
	};
}