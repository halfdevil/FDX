
#include "GUI/XmlControlSkinLoader.h"
#include "OS/GameLogger.h"
#include "OS/System.h"
#include "Core/ResourceManager.h"
#include "Core/FontResource.h"

namespace FDX
{
	CXmlControlSkinLoader::CXmlControlSkinLoader()
	{
	}

	CXmlControlSkinLoader::~CXmlControlSkinLoader()
	{
	}

	EResult CXmlControlSkinLoader::LoadSkin(const tinyxml2::XMLElement * pNode, 
		CControlSkin ** pSkin)
	{
		EResult lResult = EResult::Success;

		String lstrName = (pNode->Attribute("name") != nullptr) ?
			pNode->Attribute("name") : "";

		if (!lstrName.empty())
		{
			CControlSkin *lSkin = new CControlSkin(lstrName);

			auto lMaterialsNode = pNode->FirstChildElement("materials");
			if (lMaterialsNode != nullptr)
			{
				auto lMaterialNode = lMaterialsNode->FirstChildElement();
				while (lMaterialNode != nullptr)
				{					
					CXmlControlMaterialLoaderPtr lLoader = std::make_unique<CXmlControlMaterialLoader>();
					CControlMaterial *lMaterial = nullptr;

					lResult = lLoader->LoadMaterial(lMaterialNode, &lMaterial);
					if (lResult != EResult::Success)
					{
						LOG_DEBUG("CXmlControlMaterialLoader::LoadMaterial() failed");
					}
					else
					{
						lSkin->AddMaterial(CControlMaterialPtr(lMaterial));
					}

					lMaterialNode = lMaterialNode->NextSiblingElement();
				}

				lSkin->SetActiveMaterial((EControlMaterialType)
					lMaterialsNode->IntAttribute("active"));
			}

			*pSkin = lSkin;
		}

		return lResult;
	}
}