#pragma once

#include "Graphics/Texture.h"
#include "Core/Image.h"

namespace FDX
{
	class FDX_EXPORT CTexture2DArray : public ITexture
	{
	public:

		virtual ~CTexture2DArray()
		{
		}

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetNumLayers() const = 0;

		virtual EResult Load(const Array<IImage*> &pImages) = 0;
	};
}