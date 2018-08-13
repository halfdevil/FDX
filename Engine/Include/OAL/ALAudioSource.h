#pragma once

#include "Audio/AudioSource.h"
#include "OpenAL/al.h"
#include "OpenAL/alc.h"

namespace FDX
{
	class CALAudioSource : public IAudioSource
	{
	public:

		CALAudioSource();
		~CALAudioSource();

		IAudioDataProvider* GetDataProvider() const
		{
			return mDataProvider;
		}

		bool IsPlaying();
		
		void SetVolume(float pfVolume);
		void SetPosition(const CVector3F &pPosition);
		void SetVelocity(const CVector3F &pVelocity);

		void Play();
		void Stop();
		void Pause();

		void LoopSound(bool pbLoop);
		void Update(float pfDelta);

		EResult Create(IAudioDataProvider *pDataProvider);

	private:

		void UnqueueAll();
		void StreamBuffer(ALuint puiBufferID, uint32_t puiSize);

	private:

		IAudioDataProvider *mDataProvider;
		ALuint muiSourceID;
		std::array<ALuint, 2> mBufferID;
		uint32_t muiNumBuffers;
	};
}