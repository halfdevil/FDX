#pragma once

#include "Graphics/Texture.h"
#include "Core/Image.h"

namespace FDX
{
	class FDX_EXPORT CTexture2D : public ITexture
	{
	public:

		virtual ~CTexture2D()
		{
		}

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual EResult Load(IImage *pImage) = 0;
		virtual EResult Initialize(ESurfaceFormat pFormat, 
			uint32_t puiWidth, uint32_t puiHeight) = 0;
	};

	using CTexture2DPtr = SmartPtr<CTexture2D>;
}