#pragma once

#include "Graphics/Texture2DArray.h"
#include <GL/gl3w.h>

namespace FDX
{
	class CGLTexture2DArray : public CTexture2DArray
	{
	public:

		CGLTexture2DArray(const String &pstrName);
		~CGLTexture2DArray();

		ETextureType GetType() const;

		const String& GetName() const;
		ETextureUsage GetUsage() const;

		bool HasMipMaps() const;

		const CTextureProperties& GetProperties() const;
		void SetProperties(const CTextureProperties &pProperties);

		uint32_t GetWidth() const;
		uint32_t GetHeight() const;
		uint32_t GetNumLayers() const;

		EResult Load(const Array<IImage*> &pImages);
		EResult Create(ETextureUsage pUsage);

		void Activate(uint32_t puiUnit);

	private:

		String mstrName;
		ETextureUsage mUsage;
		bool mbHasMipMaps;
		CTextureProperties mProperties;
		uint32_t muiWidth;
		uint32_t muiHeight;
		uint32_t muiNumLayers;
		ESurfaceFormat mFormat;
		GLuint muiHandle;
	};
}