
#include "GUI/XmlImageControlLoader.h"
#include "GUI/XmlControlSkinLoader.h"
#include "OS/GameLogger.h"
#include "OS/System.h"
#include "Graphics/TextureManager.h"

namespace FDX
{
	CXmlImageControlLoader::CXmlImageControlLoader(const String & pstrType)
		: CXmlControlLoader(pstrType)
	{
	}

	CXmlImageControlLoader::~CXmlImageControlLoader()
	{
	}

	CControl * CXmlImageControlLoader::CreateControl(const String & pstrName)
	{
		return new CImageControl(pstrName);
	}
}