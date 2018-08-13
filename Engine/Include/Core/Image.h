#pragma once

#include "Core/Core.h"
#include "Core/Result.h"

namespace FDX
{
	enum class EImageType
	{
		Png,
		Jpeg,
		Bmp,
		Tga,
		Dds
	};

	enum class ESurfaceFormat
	{
		BGR8,
		BGRA8,
		R32F,
		D24,
		D24S8
	};

	class FDX_EXPORT IImage
	{
	public:

		virtual ~IImage() = default;

		virtual EImageType GetImageType() const = 0;		
		virtual ESurfaceFormat GetImageFormat() const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetBitsPerPixel() const = 0;

		virtual uint32_t GetDataSize() const = 0;
		virtual const void* GetData() const = 0;
	};

	using IImagePtr = SmartPtr<IImage>;
}
