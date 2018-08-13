#pragma once

#include "Core/Core.h"
#include <cmath>
#include <random>

# define PI				3.141592653589793f
# define TWOPI			6.2831853071795862f
# define ToRadians(x)	((x) * 0.017453292519943295f)
# define ToDegrees(x)	((x) * 57.29577951308232f)

namespace FDX
{
	class CMathUtil
	{
	public:

		template <typename T>
		static T Abs(T pVal);

		template <typename T>
		static T Min(T pA, T pB);

		template <typename T>
		static T Max(T pA, T pB);

		template <typename T>
		static T Ceil(T pVal);

		template <typename T>
		static T Floor(T pVal);
		
		template <typename T>
		static T Sqrt(T pVal);

		template <typename T>
		static T Lerp(T pA, T pB, float pfAmount);

		template <typename T>
		static T Sin(T pVal);

		template <typename T>
		static T Cos(T pfVal);

		template <typename T>
		static T Tan(T pfVal);

		template <typename T>
		static T ATan2(T pfY, T pfX);

		template <typename T>
		static T ASin(T pfVal);

		template <typename T>
		static T ACos(T pfVal);

		template <typename T>
		static T ATan(T pfVal);

		template <typename T>
		static T Pow(T pfA, T pfB);

		template <typename T>
		static T Rand(T pfL, T pfH);

		template <typename T>
		static T Clamp(T pfV, T pfA, T pfB);
	};

	template<typename T>
	inline T CMathUtil::Abs(T pVal)
	{
		return static_cast<T>(std::abs(pVal));
	}

	template <typename T>
	inline T CMathUtil::Min(T pfA, T pfB)
	{
		return pfA > pfB ? pfB : pfA;
	}

	template <typename T>
	inline T CMathUtil::Max(T pfA, T pfB)
	{
		return pfA > pfB ? pfA : pfB;
	}

	template <typename T>
	inline T CMathUtil::Ceil(T pfVal)
	{
		return static_cast<T>(std::ceil(pfVal));
	}

	template <typename T>
	inline T CMathUtil::Floor(T pfVal)
	{
		return static_cast<T>(std::floor(pfVal));
	}

	template <typename T>
	inline T CMathUtil::Sqrt(T pfVal)
	{
		return static_cast<T>(std::sqrt(pfVal));
	}

	template <typename T>
	inline T CMathUtil::Lerp(T pfA, T pfB, float pfAmount)
	{
		return pfA * pfAmount + pfB * (1.0f - pfAmount);
	}

	template <typename T>
	inline T CMathUtil::Sin(T pfVal)
	{
		return static_cast<T>(std::sin(pfVal));
	}

	template <typename T>
	inline T CMathUtil::Cos(T pfVal)
	{
		return static_cast<T>(std::cos(pfVal));
	}

	template<typename T>
	inline T CMathUtil::Tan(T pfVal)
	{
		return static_cast<T>(std::tan(pfVal));
	}

	template<typename T>
	inline T CMathUtil::ATan2(T pfY, T pfX)
	{
		return static_cast<T>(std::atan2(pfY, pfX));
	}

	template<typename T>
	inline T CMathUtil::ASin(T pfVal)
	{
		return static_cast<T>(std::asin(pfVal));
	}

	template<typename T>
	inline T CMathUtil::ACos(T pfVal)
	{
		return static_cast<T>(std::acos(pfVal));
	}

	template<typename T>
	inline T CMathUtil::ATan(T pfVal)
	{
		return static_cast<T>(std::atan(pfVal));
	}

	template<typename T>
	inline T CMathUtil::Pow(T pfA, T pfB)
	{
		return static_cast<T>(std::pow(pfA, pfB));
	}

	template<typename T>
	inline T CMathUtil::Rand(T pfL, T pfH)
	{
		std::random_device lRd;
		std::mt19937 lGenerator(lRd());
		std::uniform_real_distribution<> lDistribution(pfL, pfH);

		return static_cast<T>(lDistribution(lGenerator));
	}

	template<typename T>
	inline T CMathUtil::Clamp(T pfV, T pfA, T pfB)
	{
		return pfV < pfA ? pfA : (pfV > pfB) ? pfB : pfV;
	}
}