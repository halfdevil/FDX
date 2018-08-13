#pragma once

#include "Core/Core.h"
#include "Core/Result.h"

namespace FDX
{
	enum class EInputType
	{
		Keyboard,
		Mouse
	};

	class FDX_EXPORT IInputDevice
	{
	public:

		virtual ~IInputDevice() = default;
		virtual EInputType GetInputType() const = 0;

		virtual void Sleep() = 0;
		virtual void Awake() = 0;
		virtual void Update() = 0;
	};

	using IInputDevicePtr = SmartPtr<IInputDevice>;
}