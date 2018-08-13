#pragma once

#include "Graphics/Texture2D.h"
#include "Graphics/TextureCube.h"
#include "Graphics/Texture2DArray.h"
#include "Graphics/GraphicsDevice.h"
#include "Core/Singleton.h"

namespace FDX
{
	class FDX_EXPORT CTextureManager : public CSingleton<CTextureManager>
	{
	public:

		CTextureManager(IGraphicsDevice *pGraphicsDevice);
		~CTextureManager();

		CTextureManager(const CTextureManager&) = delete;
		CTextureManager& operator=(const CTextureManager&) = delete;

		IGraphicsDevice* GetGraphicsDevice() const
		{
			return mGraphicsDevice;
		}

		EResult GetTexture(const String &pstrName, ITexture **pTexture);

		EResult AddTexture2D(const String &pstrName, const String &pstrFilePath,
			const CTextureProperties &pProperties, CTexture2D **pTexture);

		EResult AddTextureCube(const String &pstrName, const Array<String> &pstrFilePaths,
			const CTextureProperties &pProperties, CTextureCube **pTexture);

		EResult AddTexture2DArray(const String &pstrName, const Array<String> &pstrFilePaths,
			const CTextureProperties &pProperties, CTexture2DArray **pTexture);

		EResult RemoveTexture(const String &pstrName);
		void RemoveAll();

	private:

		IGraphicsDevice *mGraphicsDevice;
		UnorderedMap<String, ITexturePtr> mTextures;
	};

	using CTextureManagerPtr = SmartPtr<CTextureManager>;
}
