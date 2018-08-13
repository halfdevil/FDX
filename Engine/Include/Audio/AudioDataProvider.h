#pragma once

#include "Core/Core.h"
#include "Core/Result.h"

namespace FDX
{
	enum class EAudioFormat
	{
		Stereo,
		Mono
	};

	class FDX_EXPORT IAudioDataProvider
	{
	public:

		virtual ~IAudioDataProvider() = default;

		virtual EAudioFormat GetAudioFormat() const = 0;

		virtual uint8_t* GetData() const = 0;
		virtual uint32_t GetDataSize() const = 0;

		virtual uint32_t GetNumChannels() const = 0;
		virtual uint32_t GetSamplesPerSec() const = 0;
		virtual uint32_t GetNumBitsPerSample() const = 0;

		virtual bool IsStreaming() const = 0;

		virtual uint32_t Stream(uint32_t puiSize) = 0;
		virtual void Seek(float pfTime) = 0;
	};

	using IAudioDataProviderPtr = SmartPtr<IAudioDataProvider>;
}