
#include "OS/OSX/OSXGameTimer.h"
#include <time.h>

namespace FDX
{
    COSXGameTimer::COSXGameTimer()
            : mdDeltaTime(0.0),
              miBaseTime(0),
              miStopTime(0),
              miPausedTime(0),
              miPrevTime(0),
              miCurrTime(0),
              mbStopped(false)
    {
    }

    double COSXGameTimer::GetGameTime() const
    {
        if (mbStopped)
            return static_cast<double>((miStopTime - miBaseTime));
        else
            return static_cast<double>((miCurrTime - miPausedTime - miBaseTime));
    }

    double COSXGameTimer::GetDeltaTime() const
    {
        return mdDeltaTime;
    }

    void COSXGameTimer::Start()
    {
        struct timespec lStartTime = {};
        clock_gettime(CLOCK_REALTIME, &lStartTime);

        if (mbStopped)
        {
            miPausedTime += (lStartTime.tv_nsec - miStopTime);

            miPrevTime = lStartTime.tv_nsec;
            miStopTime = 0;
            mbStopped = false;
        }
    }

    void COSXGameTimer::Stop()
    {
        if (!mbStopped)
        {
            struct timespec lCurrTime = {};
            clock_gettime(CLOCK_REALTIME, &lCurrTime);

            miStopTime = lCurrTime.tv_nsec;
            mbStopped = true;
        }
    }

    void COSXGameTimer::Reset()
    {
        struct timespec lCurrTime = {};
        clock_gettime(CLOCK_REALTIME, &lCurrTime);

        miBaseTime = lCurrTime.tv_nsec;
        miPrevTime = lCurrTime.tv_nsec;
        miCurrTime = lCurrTime.tv_nsec;
        miStopTime = 0;
        mbStopped = true;
    }

    void COSXGameTimer::Update()
    {
        if (!mbStopped)
        {
            struct timespec lCurrTime = {};
            clock_gettime(CLOCK_REALTIME, &lCurrTime);

            miCurrTime = lCurrTime.tv_nsec;
            mdDeltaTime = static_cast<double>((miCurrTime - miPrevTime));
            miPrevTime = miCurrTime;

            if (mdDeltaTime < 0)
                mdDeltaTime = 0.0;
        }
    }
}
