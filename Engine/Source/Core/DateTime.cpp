
#include "Core/DateTime.h"

namespace FDX
{
	CDateTime::CDateTime(uint64_t puiTicks)
	{
		time_t lTime = static_cast<time_t>(puiTicks);
		tm *lTm = localtime(&lTime);

		muiYear = 1900 + lTm->tm_year;
		muiMonth = lTm->tm_mon + 1;
		muiDay = lTm->tm_mday;
		muiHour = lTm->tm_hour;
		muiMinute = lTm->tm_min;
		muiSecond = lTm->tm_sec;
	}

	CDateTime::~CDateTime()
	{
	}

	CDateTime CDateTime::GetToday()
	{
		CDateTime lToday(static_cast<uint64_t>(time(0)));
		lToday.muiHour = 0;
		lToday.muiMinute = 0;
		lToday.muiSecond = 0;

		return lToday;
	}

	CDateTime CDateTime::GetNow()
	{
		return CDateTime(static_cast<uint64_t>(time(0)));
	}
}