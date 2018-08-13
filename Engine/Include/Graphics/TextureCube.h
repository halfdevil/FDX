#pragma once

#include "Graphics/Texture.h"
#include "Core/Image.h"

namespace FDX
{
	class FDX_EXPORT CTextureCube : public ITexture
	{
	public:

		virtual ~CTextureCube()
		{
		}

		virtual uint32_t GetSize() const = 0;

		virtual EResult Load(const Array<IImage*> &pImages) = 0;
		virtual EResult Initialize(ESurfaceFormat pFormat, uint32_t puiSize) = 0;
	};
}
