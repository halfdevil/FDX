
#include "GUI/XmlButtonLoader.h"
#include "GUI/XmlControlSkinLoader.h"
#include "Core/ResourceManager.h"
#include "Core/FontResource.h"
#include "OS/GameLogger.h"
#include "OS/System.h"

namespace FDX
{
	CXmlButtonLoader::CXmlButtonLoader(const String & pstrType)
		: CXmlLabelLoader(pstrType)
	{
	}

	CXmlButtonLoader::~CXmlButtonLoader()
	{
	}

	CControl * CXmlButtonLoader::CreateControl(const String & pstrName)
	{
		return new CButton(pstrName);
	}
}