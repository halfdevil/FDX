#pragma once

#include "Core/Core.h"
#include "Core/Singleton.h"

namespace FDX
{
	class FDX_EXPORT IGameTimer : public CSingleton<IGameTimer>
	{
	public:

		virtual ~IGameTimer()
		{
		}

		virtual double GetGameTime() const = 0;
		virtual double GetDeltaTime() const = 0;

		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual void Reset() = 0;
		virtual void Update() = 0;
	};

	using IGameTimerPtr = SmartPtr<IGameTimer>;
}
