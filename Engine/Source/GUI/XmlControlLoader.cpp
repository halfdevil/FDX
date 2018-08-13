
#include "GUI/XmlControlLoader.h"
#include "GUI/XmlControlLoaderFactory.h"
#include "GUI/XmlControlSkinLoader.h"
#include "OS/System.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CXmlControlLoader::CXmlControlLoader(const String & pstrType)
		: mstrType(pstrType)
	{
	}

	CXmlControlLoader::~CXmlControlLoader()
	{
	}

	bool CXmlControlLoader::CanLoadType(const String & pstrType)
	{
		return mstrType == pstrType;
	}

	EResult CXmlControlLoader::Load(const tinyxml2::XMLElement * pNode, 
		CControlContainer *pContainer, CControl ** pControl)
	{
		EResult lResult = EResult::Success;
		CControl *lControl = nullptr;

		if (pNode != nullptr)
		{
			String lstrName = (pNode->Attribute("name") != nullptr) ?
				pNode->Attribute("name") : "";

			CControlSkin *lSkin = nullptr;

			auto lSkinNode = pNode->FirstChildElement("skin");
			if (lSkinNode != nullptr)
			{
				lResult = LoadSkin(lSkinNode, &lSkin);
				if (lResult != EResult::Success)
				{
					LOG_DEBUG("CXmlControlLoader::LoadSkin() failed");
				}
			}

			lControl = CreateControl(lstrName);
			lResult = Parse(pNode, pContainer, lControl);

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("CXmlControlLoader::Load() failed");
				SAFE_DELETE(lControl);				
			}
			else
			{
				lControl->SetSkin(CControlSkinPtr(lSkin));
				*pControl = lControl;
			}
		}

		return lResult;
	}

	CControl* CXmlControlLoader::CreateControl(const String & pstrName)
	{
		return new CControl(pstrName);
	}

	EResult CXmlControlLoader::LoadSkin(const tinyxml2::XMLElement * pNode, CControlSkin ** pSkin)
	{
		EResult lResult = EResult::Success;

		CXmlControlSkinLoaderPtr lSkinLoader = std::make_unique<CXmlControlSkinLoader>();
		CControlSkin *lSkin = nullptr;

		lResult = lSkinLoader->LoadSkin(pNode, &lSkin);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CXmlControlSkinLoader::LoadSkin() failed");
		}
		else
		{
			*pSkin = lSkin;
		}

		return lResult;
	}

	EResult CXmlControlLoader::Parse(const tinyxml2::XMLElement * pNode,
		CControlContainer *pContainer, CControl * pControl)
	{
		EResult lResult = EResult::Success;
		auto lAttribute = pNode->FirstAttribute();

		while (lAttribute != nullptr)
		{
			String lstrName = lAttribute->Name() != nullptr ? 
				String(lAttribute->Name()) : "";

			if (lstrName == "x")
			{
				pControl->SetX((uint32_t)lAttribute->IntValue());
			}
			else if (lstrName == "y")
			{
				pControl->SetY((uint32_t)lAttribute->IntValue());
			}
			else if (lstrName == "width")
			{
				pControl->SetWidth((uint32_t)lAttribute->IntValue());
			}
			else if (lstrName == "height")
			{
				pControl->SetHeight((uint32_t)lAttribute->IntValue());
			}
			else if (lstrName == "fraction_x")
			{
				pControl->SetParentFractionX(lAttribute->FloatValue());
			}
			else if (lstrName == "fraction_y")
			{
				pControl->SetParentFractionY(lAttribute->FloatValue());
			}
			else if (lstrName == "align_v")
			{
				pControl->SetAlignV((EAlignVertical)lAttribute->IntValue());
			}
			else if (lstrName == "align_h")
			{
				pControl->SetAlignH((EAlignHorizontal)lAttribute->IntValue());
			}
			else if (lstrName == "fill_mode")
			{
				pControl->SetFillMode((uint32_t)lAttribute->IntValue());
			}
			else if (lstrName == "enabled")
			{
				pControl->SetEnabled(lAttribute->BoolValue());
			}
			else if (lstrName == "visible")
			{
				pControl->SetVisible(lAttribute->BoolValue());
			}

			lAttribute = lAttribute->Next();
		}

		return lResult;
	}
}