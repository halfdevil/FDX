
#include "GUI/XmlControlContainerLoader.h"
#include "GUI/XmlControlLoaderFactory.h"
#include "GUI/UIManager.h"
#include "Core/ResourceManager.h"
#include "Core/XmlResource.h"
#include "OS/System.h"

namespace FDX
{
	CXmlControlContainerLoader::CXmlControlContainerLoader()
	{
	}

	CXmlControlContainerLoader::~CXmlControlContainerLoader()
	{
	}

	EControlContainerLoaderType CXmlControlContainerLoader::GetLoaderType() const
	{
		return EControlContainerLoaderType::Xml;
	}

	bool CXmlControlContainerLoader::CanLoadType(EControlContainerLoaderType pLoaderType)
	{
		return pLoaderType == EControlContainerLoaderType::Xml;
	}

	bool CXmlControlContainerLoader::CanLoad(const String & pstrFilePath)
	{
		bool lbCanLoad = false;
		uint32_t luiDot = pstrFilePath.find_last_of(".");

		if (luiDot != (uint32_t)String::npos)
		{
			String lstrExt = pstrFilePath.substr(luiDot);
			std::transform(lstrExt.begin(), lstrExt.end(), lstrExt.begin(), ::tolower);

			if (lstrExt == ".fvc")
			{
				lbCanLoad = true;
			}
		}

		return lbCanLoad;
	}

	EResult CXmlControlContainerLoader::Load(const String & pstrFilePath, CControlContainer ** pContainer)
	{
		EResult lResult = EResult::Success;
		CXmlResource *lResource = nullptr;		

		lResult = CResourceManager::Get().GetResource(pstrFilePath, &lResource);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CResourceManager::GetResource() failed for %s", pstrFilePath.c_str());
		}
		else
		{
			const auto &lDocument = lResource->GetDocument();

			const auto lContainerNode = lDocument.FirstChildElement("view_container");
			auto lChildNode = lContainerNode->FirstChildElement();

			String lstrName = (lContainerNode->Attribute("name") != nullptr) ?
				lContainerNode->Attribute("name") : "";

			CControlContainer *lContainer = new CControlContainer(lstrName);

			while (lChildNode != nullptr)
			{
				lstrName = (lChildNode->Name() != nullptr) ? 
					String(lChildNode->Name()) : "";

				if (lstrName == "view_layout")
				{
					LoadLayout(lChildNode, lContainer);
				}
				else
				{
					lResult = LoadControl(lChildNode, nullptr, lContainer);
					if (lResult != EResult::Success)
					{
						LOG_DEBUG("LoadControl() failed for %s", pstrFilePath.c_str());
						break;
					}
				}

				lChildNode = lChildNode->NextSiblingElement();
			}

			*pContainer = lContainer;
		}

		return lResult;
	}

	EResult CXmlControlContainerLoader::LoadControl(const tinyxml2::XMLElement * pNode, CControl *pParent, 
		CControlContainer * pContainer)
	{
		EResult lResult = EResult::Success;

		CXmlControlLoaderPtr lControlLoader = CXmlControlLoaderFactory::Get().
			GetXmlControlLoader(pNode);

		if (lControlLoader != nullptr)
		{
			CControl *lControl = nullptr;

			lResult = lControlLoader->Load(pNode, pContainer, &lControl);
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("CXmlControlLoader::Load() failed");
			}
			else
			{
				auto lChildControl = pNode->FirstChildElement();				
				while (lChildControl != nullptr)
				{
					String lstrName = (pNode->Name() != nullptr) ?
						pNode->Name() : "";

					if (lstrName != "skin")
					{
						lResult = LoadControl(lChildControl, lControl, pContainer);

						if (lResult != EResult::Success)
						{
							LOG_DEBUG("LoadControl() failed");
							break;
						}
					}

					lChildControl = lChildControl->NextSiblingElement();
				}

				pContainer->AddControl(CControlPtr(lControl), pParent);
			}
		}

		return lResult;
	}

	EResult CXmlControlContainerLoader::LoadLayout(const tinyxml2::XMLElement * pNode, CControlContainer *pContainer)
	{
		EResult lResult = EResult::Success;

		String lstrName = (pNode->Attribute("name") != nullptr) ?
			pNode->Attribute("name") : "";

		EControlLayoutType lLayoutType = (EControlLayoutType)pNode->IntAttribute("type");
		CControl *lLayout = pContainer->CreateLayout(lLayoutType, lstrName);

		if (lLayout != nullptr)
		{
			lLayout->SetX((uint32_t)pNode->IntAttribute("x"));
			lLayout->SetY((uint32_t)pNode->IntAttribute("y"));
			lLayout->SetWidth((uint32_t)pNode->IntAttribute("width"));
			lLayout->SetHeight((uint32_t)pNode->IntAttribute("height"));
		}

		return lResult;
	}
}