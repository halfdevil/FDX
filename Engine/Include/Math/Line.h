#pragma once

#include "Math/Vector3F.h"

namespace FDX
{
	class FDX_EXPORT CLine
	{
	public:

		inline CLine();
		inline CLine(const CVector3F &pStart, const CVector3F &pEnd);

		inline float GetLength() const;
		inline float GetLengthSqr() const;

	public:

		CVector3F Start;
		CVector3F End;
	};

	inline CLine::CLine()
	{
	}

	inline CLine::CLine(const CVector3F & pStart, const CVector3F & pEnd)
		: Start(pStart),
		End(pEnd)
	{
	}

	inline float CLine::GetLength() const
	{
		return (End - Start).GetLength();
	}

	inline float CLine::GetLengthSqr() const
	{
		return (End - Start).GetLengthSqr();
	}
}