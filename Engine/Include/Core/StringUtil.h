#pragma once

#include "Core/Core.h"

namespace FDX
{
	class FDX_EXPORT CStringUtil
	{
	public:

		static void LTrim(String &pstrString);
		static void RTrim(String &pstrString);
		static void Trim(String &pstrString);
	};
}