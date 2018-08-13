#pragma once

#include "Graphics/Canvas.h"

namespace FDX
{
	class FDX_EXPORT CControlBorder
	{
	public:

		CControlBorder();
		virtual ~CControlBorder();

		uint32_t GetBorderType() const
		{
			return muiBorderType;
		}

		uint32_t GetBorderWidth() const
		{
			return muiBorderWidth;
		}

		CColor4F GetBorderColor(EBorderType pBorderType) const
		{
			auto lIt = mBorderColors.find(pBorderType);

			if (lIt != mBorderColors.end())
				return (*lIt).second;

			return CColor4F();
		}

		UnorderedMap<EBorderType, CColor4F>& GetBorderColors()
		{
			return mBorderColors;
		}

		virtual void SetBorderType(uint32_t puiBorderType);
		virtual void SetBorderWidth(uint32_t puiBorderWidth);
		virtual void SetBorderColor(EBorderType pBorderType, const CColor4F &pColor);

	protected:

		uint32_t muiBorderType;
		uint32_t muiBorderWidth;
		UnorderedMap<EBorderType, CColor4F> mBorderColors;
	};

	using CControlBorderPtr = SmartPtr<CControlBorder>;
}