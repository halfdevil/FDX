#pragma once

#include "Core/Bitmap.h"

namespace FDX
{
	enum class EFontType
	{
		TTF
	};

	struct SLineParameters
	{
		uint32_t Width;
		uint32_t MinY;
		uint32_t MaxY;
		uint32_t Baseline;
	};

	class FDX_EXPORT IFont
	{
	public:

		virtual ~IFont()
		{
		}

		virtual EFontType GetFontType() const = 0;

		virtual bool GetMaskMode() const = 0;
		virtual void SetMaskMode(bool pbMaskMode) = 0;

		virtual EResult DrawText(const String &pstrString, int32_t puiHeight,
			const CColor4F &pColor, bool pbLeftToRight, CBitmap **pBitmap) = 0;

		virtual EResult DrawText(const String &pstrString, int32_t puiHeight, 
			int32_t puiStartX, int32_t puiStartY,	const CColor4F &pColor, 
			bool pbLeftToRight, CBitmap *pBitmap) = 0;

		virtual EResult CalculateLineParameters(const String &pstrString, 
			uint32_t puiHeight,	SLineParameters *pParameters) = 0;
	};

	using IFontPtr = SmartPtr<IFont>;
}