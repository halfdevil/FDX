#pragma once

#include "Graphics/Texture2D.h"
#include <GL/gl3w.h>

namespace FDX
{
	class CGLTexture2D : public CTexture2D
	{
	public:

		CGLTexture2D(const String &pstrName);
		~CGLTexture2D();

		GLuint GetHandle() const
		{
			return muiHandle;
		}

		ETextureType GetType() const;

		const String& GetName() const;
		ETextureUsage GetUsage() const;

		bool HasMipMaps() const;

		const CTextureProperties& GetProperties() const;
		void SetProperties(const CTextureProperties &pProperties);

		uint32_t GetWidth() const;
		uint32_t GetHeight() const;

		EResult Load(IImage *pImage);
		EResult Create(ETextureUsage pUsage);

		EResult Initialize(ESurfaceFormat pFormat,
			uint32_t puiWidth, uint32_t puiHeight) override;

		void Activate(uint32_t puiUnit);

	private:

		void GenerateMipMaps();

	private:
				
		String mstrName;
		ETextureUsage mUsage;
		bool mbHasMipMaps;
		CTextureProperties mProperties;
		uint32_t muiWidth;
		uint32_t muiHeight;
		ESurfaceFormat mFormat;
		GLuint muiHandle;
	};
}