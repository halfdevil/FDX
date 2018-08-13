#pragma once

#include "Audio/AudioDataProvider.h"
#include "Math/Vector3F.h"

namespace FDX
{
	class FDX_EXPORT IAudioSource
	{
	public:

		virtual ~IAudioSource() = default;

		virtual IAudioDataProvider* GetDataProvider() const = 0;
		virtual bool IsPlaying() = 0;

		virtual void SetVolume(float pfVolume) = 0;
		virtual void SetPosition(const CVector3F &pPosition) = 0;
		virtual void SetVelocity(const CVector3F &pVelocity) = 0;

		virtual void Play() = 0;
		virtual void Stop() = 0;
		virtual void Pause() = 0;

		virtual void LoopSound(bool pbLoop) = 0;
		virtual void Update(float pfDelta) = 0;
	};

	using IAudioSourcePtr = SmartPtr<IAudioSource>;
}