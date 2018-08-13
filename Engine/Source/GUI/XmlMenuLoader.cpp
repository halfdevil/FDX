
#include "GUI/XmlMenuLoader.h"
#include "GUI/XmlControlSkinLoader.h"
#include "Core/ResourceManager.h"
#include "Core/FontResource.h"
#include "OS/GameLogger.h"
#include "OS/System.h"

namespace FDX
{
	CXmlMenuItemLoader::CXmlMenuItemLoader(const String & pstrType)
		: CXmlButtonLoader(pstrType)
	{
	}

	CXmlMenuItemLoader::~CXmlMenuItemLoader()
	{
	}

	CControl * CXmlMenuItemLoader::CreateControl(const String & pstrName)
	{
		return new CMenuItem(pstrName);
	}

	EResult CXmlMenuItemLoader::Parse(const tinyxml2::XMLElement * pNode, 
		CControlContainer * pContainer, CControl * pControl)
	{
		EResult lResult = EResult::Success;

		lResult = CXmlControlLoader::Parse(pNode, pContainer, pControl);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CXmlControlLoader::Parse() failed");
		}
		else
		{
			String lstrMenuName = (pNode->Attribute("menu") != nullptr) ?
				pNode->Attribute("menu") : "";

			CMenuItem *lMenuItem = static_cast<CMenuItem*>(pControl);
			CMenuItem *lParentMenu = pContainer->GetControlAs<CMenuItem>(lstrMenuName);

			if (lParentMenu != nullptr)
				lParentMenu->AddMenuItem(lMenuItem);
		}

		return lResult;
	}

	CXmlMenuLoader::CXmlMenuLoader(const String & pstrType)
		: CXmlMenuItemLoader(pstrType)
	{
	}

	CXmlMenuLoader::~CXmlMenuLoader()
	{
	}

	CControl * CXmlMenuLoader::CreateControl(const String & pstrName)
	{
		return new CMenu(pstrName);
	}
}