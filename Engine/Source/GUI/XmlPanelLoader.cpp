
#include "GUI/XmlPanelLoader.h"
#include "GUI/XmlControlSkinLoader.h"
#include "GUI/XmlControlMaterialLoader.h"
#include "OS/GameLogger.h"
#include "OS/System.h"

namespace FDX
{
	CXmlPanelLoader::CXmlPanelLoader(const String &pstrType)
		: CXmlControlLoader(pstrType)
	{
	}

	CXmlPanelLoader::~CXmlPanelLoader()
	{
	}

	CControl * CXmlPanelLoader::CreateControl(const String & pstrName)
	{
		return new CPanel(pstrName);
	}
}