#pragma once

#include "Core/Core.h"
#include "Core/Result.h"

namespace FDX
{
	enum class EControlAnimationType
	{
		Loop,
		StopAtEnd,
		Oscillate,
		Random
	};

	class FDX_EXPORT IControlAnimation
	{
	public:

		virtual ~IControlAnimation()
		{
		}

		virtual EControlAnimationType GetAnimationType() const = 0;
		virtual const String& GetName() const = 0;
				
		virtual float GetFrameLength() const = 0;
		virtual uint32_t GetActiveFrame() const = 0;

		virtual void SetFrameLength(float pfLength) = 0;
		virtual void Update(float pfDelta, uint32_t puiMaxFrames) = 0;
	};

	using IControlAnimationPtr = SmartPtr<IControlAnimation>;

	class FDX_EXPORT CBaseControlAnimation : public IControlAnimation
	{
	public:

		CBaseControlAnimation(const String &pstrName);
		virtual ~CBaseControlAnimation();

		const String& GetName() const override
		{
			return mstrName;
		}

		float GetFrameLength() const override
		{
			return mfFrameLength;
		}

		uint32_t GetActiveFrame() const override
		{
			return muiActiveFrame;
		}

		void SetFrameLength(float pfLength) override;

	protected:

		String mstrName;
		uint32_t muiActiveFrame;
		float mfCurrentFrame;
		float mfFrameLength;
	};

	class FDX_EXPORT CLoopControlAnimation : public CBaseControlAnimation
	{
	public:

		CLoopControlAnimation(const String &pstrName);
		~CLoopControlAnimation();

		EControlAnimationType GetAnimationType() const override
		{
			return EControlAnimationType::Loop;
		}
				
		void Update(float pfDelta, uint32_t puiMaxFrames) override;
	};

	class FDX_EXPORT CStopAtEndControlAnimation : public CBaseControlAnimation
	{
	public:

		CStopAtEndControlAnimation(const String &pstrName);
		~CStopAtEndControlAnimation();

		EControlAnimationType GetAnimationType() const override
		{
			return EControlAnimationType::StopAtEnd;
		}

		void Update(float pfDelta, uint32_t puiMaxFrames) override;
	};

	class FDX_EXPORT COscillateControlAnimation : public CBaseControlAnimation
	{
	public:

		COscillateControlAnimation(const String &pstrName);
		~COscillateControlAnimation();

		EControlAnimationType GetAnimationType() const override
		{
			return EControlAnimationType::Oscillate;
		}
		
		void Update(float pfDelta, uint32_t puiMaxFrames) override;

	private:

		bool mbForwardAnim;
	};

	class FDX_EXPORT CRandomControlAnimation : public CBaseControlAnimation
	{
	public:

		CRandomControlAnimation(const String &pstrName);
		~CRandomControlAnimation();

		EControlAnimationType GetAnimationType() const override
		{
			return EControlAnimationType::Random;
		}

		void Update(float pfDelta, uint32_t puiMaxFrames) override;
	};
}