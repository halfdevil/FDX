#pragma once

#include "OS/GameTimer.h"
#include <Windows.h>

namespace FDX
{
	class FDX_EXPORT CWin32GameTimer : public IGameTimer
	{
	public:

		CWin32GameTimer();
		~CWin32GameTimer();

		double GetGameTime() const override;
		double GetDeltaTime() const override;

		void Start() override;
		void Stop() override;
		void Reset() override;
		void Update() override;

	private:

		double mdSecsPerCount;
		double mdDeltaTime;

		int64_t miBaseTime;
		int64_t miPausedTime;
		int64_t miStopTime;
		int64_t miPrevTime;
		int64_t miCurrTime;
		bool mbStopped;
	};
}
