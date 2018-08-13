
#include "Core/Bitmap.h"
#include "OS/System.h"

namespace FDX
{
	struct SBGR
	{
		uint8_t B, G, R;
	};

	struct SBGRA
	{
		uint8_t B, G, R, A;
	};

	CBitmap::CBitmap()
		: muiWidth(0),
		muiHeight(0),
		muiBitsPerPixel(0),
		muiDataSize(0),
        mFormat(ESurfaceFormat::BGR8)
	{
	}

	CColor4F CBitmap::GetPixel(uint32_t puiX, uint32_t puiY)
	{
		if (puiX >= muiWidth || puiY >= muiHeight)
			return {};

		if (mFormat == ESurfaceFormat::BGR8)
		{
			uint32_t luiOffset = (puiY * muiWidth + puiX) * 3;
			auto *lRgb = (SBGR*)&mData[luiOffset];

			return CColor4F(lRgb->R, lRgb->G, lRgb->B);
		}
		else
		{
			uint32_t luiOffset = (puiY * muiWidth + puiX) * 4;
			auto *lRgba = (SBGRA*)&mData[luiOffset];

			return CColor4F(lRgba->R, lRgba->G, lRgba->B, lRgba->A);
		}
	}

	void CBitmap::SetPixel(uint32_t puiX, uint32_t puiY, const CColor4F & pColor)
	{
		if (puiX >= muiWidth || puiY >= muiHeight)
			return;

		if (mFormat == ESurfaceFormat::BGR8)
		{
			uint32_t luiOffset = (puiY * muiWidth + puiX) * 3;
			auto *lRgb = (SBGR*)&mData[luiOffset];

			lRgb->R = static_cast<uint8_t>(pColor.R * 255);
			lRgb->G = static_cast<uint8_t>(pColor.G * 255);
			lRgb->B = static_cast<uint8_t>(pColor.B * 255);
		}
		else
		{
			uint32_t luiOffset = (puiY * muiWidth + puiX) * 4;
			auto *lRgba = (SBGRA*)&mData[luiOffset];

			lRgba->R = static_cast<uint8_t>(pColor.R * 255);
			lRgba->G = static_cast<uint8_t>(pColor.G * 255);
			lRgba->B = static_cast<uint8_t>(pColor.B * 255);
			lRgba->A = static_cast<uint8_t>(pColor.A * 255);
		}
	}

	EResult CBitmap::Create(IImage * pImage)
	{
		EResult lResult = EResult::Success;

		mFormat = pImage->GetImageFormat();
		muiWidth = pImage->GetWidth();
		muiHeight = pImage->GetHeight();

		muiBitsPerPixel = (mFormat == ESurfaceFormat::BGR8) ? 24 : 32;
		muiDataSize = muiWidth * muiHeight * (muiBitsPerPixel / 8);
		mData = std::make_unique<uint8_t[]>(muiDataSize);
		memcpy(mData.get(), pImage->GetData(), muiDataSize);

		return lResult;
	}

	EResult CBitmap::Create(uint32_t puiWidth, uint32_t puiHeight, ESurfaceFormat pFormat)
	{
		FDX_ASSERT(pFormat == ESurfaceFormat::BGR8 ||
			pFormat == ESurfaceFormat::BGRA8, "Surface format must be BGR8 or BGRA8");

		EResult lResult = EResult::Success;
		
		muiWidth = puiWidth;
		muiHeight = puiHeight;
		mFormat = pFormat;

		muiBitsPerPixel = (mFormat == ESurfaceFormat::BGR8) ? 24 : 32;
		muiDataSize = muiWidth * muiHeight * (muiBitsPerPixel / 8);
		mData = std::make_unique<uint8_t[]>(muiDataSize);
		memset(mData.get(), 0, muiDataSize);
		
		return lResult;
	}

	void CBitmap::Rescale(uint32_t puiWidth, uint32_t puiHeight)
	{
		if (mFormat == ESurfaceFormat::BGR8)
		{
		    mData = std::make_unique<uint8_t[]>(puiWidth * puiHeight * 24);
		}
		else
		{
            mData = std::make_unique<uint8_t[]>(puiWidth * puiHeight * 32);
		}

		muiWidth = puiWidth;
		muiHeight = puiHeight;
	}

	void CBitmap::Clear()
	{
		memset(mData.get(), 0, sizeof(uint8_t) * muiWidth * 
			muiHeight * muiBitsPerPixel);
	}
}