#pragma once

#include "Core/Image.h"
#include "Math/Color4F.h"

namespace FDX
{
	class FDX_EXPORT CBitmap : public IImage
	{
	public:

		CBitmap();
		~CBitmap() override = default;

		EImageType GetImageType() const override
		{
			return EImageType::Bmp;
		}

		ESurfaceFormat GetImageFormat() const override
		{
			return mFormat;
		}

		uint32_t GetWidth() const override
		{
			return muiWidth;
		}

		uint32_t GetHeight() const override
		{
			return muiHeight;
		}

		uint32_t GetBitsPerPixel() const override
		{
			return muiBitsPerPixel;
		}

		uint32_t GetDataSize() const override
		{
			return muiDataSize;
		}

		const void* GetData() const override
		{
			return mData.get();
		}

		CColor4F GetPixel(uint32_t puiX, uint32_t puiY);
		void SetPixel(uint32_t puiX, uint32_t puiY, const CColor4F &pColor);

		EResult Create(IImage *pImage);

		EResult Create(uint32_t puiWidth, uint32_t puiHeight, 
			ESurfaceFormat pFormat);

		void Rescale(uint32_t puiWidth, uint32_t puiHeight);
		void Clear();

	private:

		ESurfaceFormat mFormat;
		uint32_t muiWidth;
		uint32_t muiHeight;
		uint32_t muiBitsPerPixel;
		uint32_t muiDataSize;
		SmartPtr<uint8_t[]> mData;
	};
}