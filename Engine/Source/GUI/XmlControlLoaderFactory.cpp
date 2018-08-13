
#include "GUI/XmlControlLoaderFactory.h"
#include "GUI/XmlPanelLoader.h"
#include "GUI/XmlButtonLoader.h"
#include "GUI/XmlMenuLoader.h"
#include "GUI/XmlImageControlLoader.h"
#include "GUI/XmlLabelLoader.h"

namespace FDX
{
	CXmlControlLoaderPtr CXmlControlLoaderFactory::GetXmlControlLoader(const tinyxml2::XMLElement * pNode)
	{
		String lstrName = (pNode->Name() != nullptr) ? 
			String(pNode->Name()) : "";

		if (lstrName == "view")
		{
			return std::make_unique<CXmlControlLoader>("view");
		}
		else if (lstrName == "panel")
		{
			return std::make_unique<CXmlPanelLoader>("panel");
		}
		else if (lstrName == "button")
		{
			return std::make_unique<CXmlButtonLoader>("button");
		}
		else if (lstrName == "menuitem")
		{
			return std::make_unique<CXmlMenuItemLoader>("menuitem");
		}
		else if (lstrName == "menu")
		{
			return std::make_unique<CXmlMenuLoader>("menu");
		}
		else if (lstrName == "imageview")
		{
			return std::make_unique<CXmlImageControlLoader>("imageview");
		}
		else if (lstrName == "label")
		{
			return std::make_unique<CXmlLabelLoader>("label");
		}

		return nullptr;
	}
}