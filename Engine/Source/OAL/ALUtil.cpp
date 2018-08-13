
#include "OAL/ALUtil.h"

namespace FDX
{
	ALenum CALUtil::GetALFormat(EAudioFormat pAudioFormat, uint32_t puiNumChannels)
	{
		if (puiNumChannels == 1)
		{
			switch (pAudioFormat)
			{
			case EAudioFormat::Mono:
				return AL_FORMAT_MONO8;

			case EAudioFormat::Stereo:
				return AL_FORMAT_STEREO8;
			}
		}
		else
		{
			switch (pAudioFormat)
			{
			case EAudioFormat::Mono:
				return AL_FORMAT_MONO16;

			case EAudioFormat::Stereo:
				return AL_FORMAT_STEREO16;
			}
		}

		return 0;
	}
}