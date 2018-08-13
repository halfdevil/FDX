#pragma once

#include "Audio/AudioSource.h"
#include "OpenAL/al.h"
#include "OpenAL/alc.h"

namespace FDX
{
	class CALUtil
	{
	public:

		static ALenum GetALFormat(EAudioFormat pAudioFormat, 
			uint32_t puiNumChannels);
	};
}