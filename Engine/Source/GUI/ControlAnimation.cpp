
#include "GUI/ControlAnimation.h"
#include "Math/MathUtil.h"

namespace FDX
{
	CBaseControlAnimation::CBaseControlAnimation(const String & pstrName)
		: mstrName(pstrName),
		mfCurrentFrame(0.0f),
		muiActiveFrame(0),
		mfFrameLength(0.0f)
	{
	}

	CBaseControlAnimation::~CBaseControlAnimation()
	{
	}

	void CBaseControlAnimation::SetFrameLength(float pfLength)
	{
		mfFrameLength = pfLength;
	}

	CLoopControlAnimation::CLoopControlAnimation(const String & pstrName)
		: CBaseControlAnimation(pstrName)
	{
	}

	CLoopControlAnimation::~CLoopControlAnimation()
	{
	}

	void CLoopControlAnimation::Update(float pfDelta, uint32_t puiMaxFrames)
	{
		mfCurrentFrame += pfDelta * (1.0f / mfFrameLength);

		if ((uint32_t)mfCurrentFrame >= puiMaxFrames)
		{
			muiActiveFrame = 0;
			mfCurrentFrame = 0.0f;
		}
	}

	CStopAtEndControlAnimation::CStopAtEndControlAnimation(const String & pstrName)
		: CBaseControlAnimation(pstrName)
	{
	}

	CStopAtEndControlAnimation::~CStopAtEndControlAnimation()
	{
	}

	void CStopAtEndControlAnimation::Update(float pfDelta, uint32_t puiMaxFrames)
	{
		mfCurrentFrame += pfDelta * (1.0f / mfFrameLength);

		if ((uint32_t)mfCurrentFrame >= puiMaxFrames)
		{	
			muiActiveFrame = puiMaxFrames - 1;
			mfCurrentFrame = (float)muiActiveFrame;			
		}
	}

	COscillateControlAnimation::COscillateControlAnimation(const String & pstrName)
		: CBaseControlAnimation(pstrName),
		mbForwardAnim(true)
	{
	}

	COscillateControlAnimation::~COscillateControlAnimation()
	{
	}

	void COscillateControlAnimation::Update(float pfDelta, uint32_t puiMaxFrames)
	{
		mfCurrentFrame += pfDelta * (1.0f / mfFrameLength);

		if ((uint32_t)mfCurrentFrame >= puiMaxFrames)
		{
			muiActiveFrame = 1;
			mfCurrentFrame = 1.0f;
			mbForwardAnim = !mbForwardAnim;
		}

		if (!mbForwardAnim)
			muiActiveFrame = puiMaxFrames - muiActiveFrame;
	}

	CRandomControlAnimation::CRandomControlAnimation(const String & pstrName)
		: CBaseControlAnimation(pstrName)
	{
	}

	CRandomControlAnimation::~CRandomControlAnimation()
	{
	}

	void CRandomControlAnimation::Update(float pfDelta, uint32_t puiMaxFrames)
	{
		if (puiMaxFrames > 1)
		{
			float lfPrevFrame = mfCurrentFrame;
			
			mfCurrentFrame += pfDelta * (1.0f / mfFrameLength);
			muiActiveFrame = (uint32_t)mfCurrentFrame;

			if (muiActiveFrame != (uint32_t)lfPrevFrame)
			{
				uint32_t luiPrevFrame = (uint32_t)lfPrevFrame;

				do 
				{
					muiActiveFrame = CMathUtil::Rand((uint32_t)0, puiMaxFrames);
				} while (muiActiveFrame == luiPrevFrame);

				mfCurrentFrame = (float)muiActiveFrame;
			}
		}
	}	
}