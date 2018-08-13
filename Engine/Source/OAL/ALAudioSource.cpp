
#include "OAL/ALAudioSource.h"
#include "OAL/ALUtil.h"
#include "OS/GameLogger.h"

namespace FDX
{
	const uint32_t AL_BUFFER_SIZE = 2 * 262144;

	CALAudioSource::CALAudioSource()
		: mDataProvider(nullptr),
		muiSourceID(0),
		muiNumBuffers(0)
	{
		mBufferID[0] = 0;
		mBufferID[1] = 1;
	}

	CALAudioSource::~CALAudioSource()
	{
		Stop();

		alDeleteSources(1, &muiSourceID);
		alDeleteBuffers(muiNumBuffers, &mBufferID[0]);
	}

	bool CALAudioSource::IsPlaying()
	{
		ALint liState;
		alGetSourcei(muiSourceID, AL_SOURCE_STATE, &liState);

		return liState == AL_PLAYING;
	}

	void CALAudioSource::SetVolume(float pfVolume)
	{
		alSourcef(muiSourceID, AL_GAIN, pfVolume);
	}

	void CALAudioSource::SetPosition(const CVector3F & pPosition)
	{
		alSource3f(muiSourceID, AL_POSITION, 
			pPosition.X, pPosition.Y, pPosition.Z);
	}

	void CALAudioSource::SetVelocity(const CVector3F & pVelocity)
	{
		alSource3f(muiSourceID, AL_VELOCITY,
			pVelocity.X, pVelocity.Y, pVelocity.Z);
	}

	void CALAudioSource::Play()
	{
		if (IsPlaying())
			return;

		if (!mDataProvider)
			return;

		ALint liState;
		alGetSourcei(muiSourceID, AL_SOURCE_STATE, &liState);

		if (liState != AL_PAUSED && mDataProvider->IsStreaming())
		{
			UnqueueAll();

			StreamBuffer(mBufferID[0], AL_BUFFER_SIZE);
			StreamBuffer(mBufferID[1], AL_BUFFER_SIZE);

			alSourceQueueBuffers(muiSourceID, muiNumBuffers, &mBufferID[0]);
		}

		alSourcePlay(muiSourceID);
	}

	void CALAudioSource::Stop()
	{
		alSourceStop(muiSourceID);
	}

	void CALAudioSource::Pause()
	{
		alSourcePause(muiSourceID);
	}

	void CALAudioSource::LoopSound(bool pbLoop)
	{
		alSourcei(muiSourceID, AL_LOOPING, pbLoop ? 1 : 0);
	}

	void CALAudioSource::Update(float pfDelta)
	{
		if (!mDataProvider)
			return;

		if (!IsPlaying())
			return;

		if (mDataProvider->IsStreaming())
		{
			ALint liProcessed;
			alGetSourcei(muiSourceID, AL_PROCESSED, &liProcessed);

			while (liProcessed--)
			{
				ALuint puiBufferID;

				alSourceUnqueueBuffers(muiSourceID, 1, &puiBufferID);
				StreamBuffer(puiBufferID, AL_BUFFER_SIZE);
				alSourceQueueBuffers(muiSourceID, 1, &puiBufferID);
			}
		}
	}

	EResult CALAudioSource::Create(IAudioDataProvider * pDataProvider)
	{
		EResult lResult = EResult::Success;
		alGenSources(1, &muiSourceID);

		if (!muiSourceID)
		{
			LOG_DEBUG("alGenSources() failed");
			lResult = EResult::CreateFailed;
		}
		else
		{
			mDataProvider = pDataProvider;

			if (mDataProvider->IsStreaming())
			{
				muiNumBuffers = 2;
				alGenBuffers(muiNumBuffers, &mBufferID[0]);
			}
			else
			{
				muiNumBuffers = 1;

				alGenBuffers(muiNumBuffers, &mBufferID[0]);
				alBufferData(mBufferID[0],
					CALUtil::GetALFormat(mDataProvider->GetAudioFormat(),
						mDataProvider->GetNumChannels()),
					static_cast<ALvoid*>(mDataProvider->GetData()),
					static_cast<ALsizei>(mDataProvider->GetDataSize()),
					mDataProvider->GetSamplesPerSec());

				alSourcei(muiSourceID, AL_BUFFER, mBufferID[0]);
			}

			alSourcef(muiSourceID, AL_GAIN, 1.0f);
			alSourcei(muiSourceID, AL_LOOPING, AL_FALSE);
		}

		return lResult;
	}

	void CALAudioSource::UnqueueAll()
	{
		ALint liQueued;
		alGetSourcei(muiSourceID, AL_BUFFERS_QUEUED, &liQueued);

		if (liQueued > 0)
			alSourceUnqueueBuffers(muiSourceID, liQueued, &mBufferID[0]);
	}

	void CALAudioSource::StreamBuffer(ALuint puiBufferID, uint32_t puiSize)
	{
		mDataProvider->Stream(puiSize);

		alBufferData(puiBufferID, 
			CALUtil::GetALFormat(mDataProvider->GetAudioFormat(),
				mDataProvider->GetNumChannels()),
			static_cast<ALvoid*>(mDataProvider->GetData()),
			static_cast<ALsizei>(mDataProvider->GetDataSize()),
			mDataProvider->GetSamplesPerSec());
	}
}