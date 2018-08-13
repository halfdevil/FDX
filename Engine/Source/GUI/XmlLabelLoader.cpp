
#include "GUI/XmlLabelLoader.h"
#include "GUI/XmlControlSkinLoader.h"
#include "OS/GameLogger.h"
#include "OS/System.h"
#include "Core/ResourceManager.h"
#include "Core/FontResource.h"

namespace FDX
{
	CXmlLabelLoader::CXmlLabelLoader(const String & pstrType)
		: CXmlControlLoader(pstrType)
	{
	}

	CXmlLabelLoader::~CXmlLabelLoader()
	{
	}

	CControl* CXmlLabelLoader::CreateControl(const String & pstrName)
	{
		return new CLabel(pstrName);
	}

	EResult CXmlLabelLoader::Parse(const tinyxml2::XMLElement * pNode, 
		CControlContainer *pContainer, CControl * pControl)
	{
		EResult lResult = EResult::Success;

		lResult = CXmlControlLoader::Parse(pNode, pContainer, pControl);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CXmlControlLoader::Parse() failed");
		}
		else
		{
			CLabel *lLabel = static_cast<CLabel*>(pControl);
			String lstrText = (pNode->Attribute("text") != nullptr) ?
				pNode->Attribute("text") : "";

			if (!lstrText.empty())
				lLabel->SetText(lstrText);
		}

		return lResult;
	}
}