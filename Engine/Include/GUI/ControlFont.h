#pragma once

#include "Core/Font.h"

namespace FDX
{
	enum EFontStyle : uint32_t
	{
		Regular = 0x1,
		Italic = 0x2,
		Bold = 0x4,
		Underline = 0x8
	};

	class FDX_EXPORT CControlFont
	{
	public:

		CControlFont();
		virtual ~CControlFont();

		IFont * GetFont() const
		{
			return mFont;
		}

		const CColor4F& GetFontColor() const
		{
			return mFontColor;
		}

		uint32_t GetFontSize() const
		{
			return muiFontSize;
		}

		uint32_t GetFontStyle() const
		{
			return muiFontStyle;
		}

		virtual void SetFont(IFont *pFont);
		virtual void SetFont(const String &pstrFilePath);
		virtual void SetFontColor(const CColor4F &pColor);
		virtual void SetFontSize(uint32_t puiSize);
		virtual void SetFontStyle(uint32_t puiStyle);

	protected:

		CColor4F mFontColor;
		uint32_t muiFontSize;
		uint32_t muiFontStyle;
		IFont *mFont;
	};

	using CControlFontPtr = SmartPtr<CControlFont>;
}