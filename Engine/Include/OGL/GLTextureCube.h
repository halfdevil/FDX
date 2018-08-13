#pragma once

#include "Graphics/TextureCube.h"
#include <GL/gl3w.h>

namespace FDX
{
	class CGLTextureCube : public CTextureCube
	{
	public:

		CGLTextureCube(const String &pstrName);
		~CGLTextureCube();

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

		uint32_t GetSize() const;

		EResult Load(const Array<IImage*> &pImages);
		EResult Create(ETextureUsage pUsage);

		EResult Initialize(ESurfaceFormat pFormat, uint32_t puiSize) override;
		void Activate(uint32_t puiUnit);

	private:

		void GenerateMipMaps();

	private:

		String mstrName;
		ETextureUsage mUsage;
		bool mbHasMipMaps;
		CTextureProperties mProperties;
		uint32_t muiSize;
		ESurfaceFormat mFormat;
		GLuint muiHandle;
	};
}