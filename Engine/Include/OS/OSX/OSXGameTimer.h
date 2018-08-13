#pragma once

#include "OS/GameTimer.h"

namespace FDX
{
    class FDX_EXPORT COSXGameTimer : public IGameTimer
    {
    public:

        COSXGameTimer();
        ~COSXGameTimer() override = default;

        double GetGameTime() const override;
        double GetDeltaTime() const override;

        void Start() override;
        void Stop() override;
        void Reset() override;
        void Update() override;

    private:

        double mdDeltaTime;
        int64_t miBaseTime;
        int64_t miPausedTime;
        int64_t miStopTime;
        int64_t miPrevTime;
        int64_t miCurrTime;
        bool mbStopped;
    };
}