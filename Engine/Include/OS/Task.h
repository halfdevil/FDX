#pragma once

#include "Core/Core.h"

#include <mutex>
#include <condition_variable>

namespace FDX
{
	class FDX_EXPORT CTask
	{
	public:

		CTask()
		{
		}

		virtual ~CTask()
		{
		}

		uint32_t GetID() const
		{
			return muiID;
		}

		uint32_t GetPriority() const
		{
			return muiPriority;
		}

		bool IsPendingExit() const volatile
		{
			return mbPendingExit;
		}

		virtual void SetID(uint32_t puiID)
		{
			muiID = puiID;
		}

		virtual void SetPriority(uint32_t puiPriority)
		{
			muiPriority = puiPriority;
		}

		virtual void Exit()
		{
			mbPendingExit = true;
		}

		virtual void Run() = 0;

	private:

		uint32_t muiID;
		uint32_t muiPriority;
		std::atomic<bool> mbPendingExit;
	};

	using CTaskPtr = SmartPtr<CTask>;
}
