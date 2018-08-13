
#include "OS/Win32/Win32GameTimer.h"

namespace FDX
{
	CWin32GameTimer::CWin32GameTimer()
		: mdSecsPerCount(0.0),
		mdDeltaTime(0.0),
		miBaseTime(0),
		miStopTime(0),
		miPausedTime(0),
		miPrevTime(0),
		miCurrTime(0),
		mbStopped(true)
	{
		LARGE_INTEGER liCountsPerSec;
		QueryPerformanceFrequency(&liCountsPerSec);

		mdSecsPerCount = 1.0 / static_cast<double>(liCountsPerSec.QuadPart);
	}

	CWin32GameTimer::~CWin32GameTimer()
	{
	}

	double CWin32GameTimer::GetGameTime() const
	{
		if (mbStopped)
			return static_cast<double>((miStopTime - miBaseTime) * mdSecsPerCount);
		else
			return static_cast<double>((miCurrTime - miPausedTime - miBaseTime) * mdSecsPerCount);
	}

	double CWin32GameTimer::GetDeltaTime() const
	{
		return mdDeltaTime;
	}

	void CWin32GameTimer::Start()
	{
		LARGE_INTEGER liStartTime;
		QueryPerformanceCounter(&liStartTime);

		if (mbStopped)
		{
			miPausedTime += (liStartTime.QuadPart - miStopTime);

			miPrevTime = liStartTime.QuadPart;
			miStopTime = 0;
			mbStopped = false;
		}
	}

	void CWin32GameTimer::Stop()
	{
		if (!mbStopped)
		{
			LARGE_INTEGER liCurrTime;
			QueryPerformanceCounter(&liCurrTime);

			miStopTime = liCurrTime.QuadPart;
			mbStopped = true;
		}
	}

	void CWin32GameTimer::Reset()
	{
		LARGE_INTEGER liCurrTime;
		QueryPerformanceCounter(&liCurrTime);

		miBaseTime = liCurrTime.QuadPart;
		miPrevTime = liCurrTime.QuadPart;
		miCurrTime = liCurrTime.QuadPart;
		miStopTime = 0;
		mbStopped = true;
	}

	void CWin32GameTimer::Update()
	{
		if (!mbStopped)
		{
			LARGE_INTEGER liCurrTime;
			QueryPerformanceCounter(&liCurrTime);

			miCurrTime = liCurrTime.QuadPart;
			mdDeltaTime = static_cast<double>((miCurrTime - miPrevTime) * mdSecsPerCount);
			miPrevTime = miCurrTime;

			if (mdDeltaTime < 0)
				mdDeltaTime = 0.0;
		}
	}
}