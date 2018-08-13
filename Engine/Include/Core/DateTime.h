#pragma once

#include "Core/Core.h"
#include <time.h>

namespace FDX
{
	class FDX_EXPORT CDateTime
	{
	public:

		CDateTime(uint64_t puiTicks);
		~CDateTime();

		uint32_t GetYear() const
		{
			return muiYear;
		}

		uint32_t GetMonth() const
		{
			return muiMonth;
		}

		uint32_t GetDay() const
		{
			return muiDay;
		}

		uint32_t GetHour() const
		{
			return muiHour;
		}

		uint32_t GetMinute() const
		{
			return muiMinute;
		}

		uint32_t GetSecond() const
		{
			return muiSecond;
		}

		uint32_t GetMillisecond() const
		{
			return muiMillisecond;
		}

	public:

		static CDateTime GetToday();
		static CDateTime GetNow();

	private:

		uint32_t muiYear;
		uint32_t muiMonth;
		uint32_t muiDay;
		uint32_t muiHour;
		uint32_t muiMinute;
		uint32_t muiSecond;
		uint32_t muiMillisecond;
	};
}
