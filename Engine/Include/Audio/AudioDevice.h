#pragma once

#include "Audio/AudioSource.h"
#include "Core/Result.h"

namespace FDX
{
	class FDX_EXPORT IAudioDevice
	{
	public:

		virtual ~IAudioDevice() = default;

		virtual EResult Create(bool pbSeparateThread) = 0;		
		virtual EResult CreateAudioSource(IAudioDataProvider *pDataProvider,
			IAudioSource **pAudioSource) = 0;
	};
}