
#include "GUI/XmlControlMaterialLoader.h"
#include "GUI/XmlControlAnimationLoader.h"
#include "GUI/XmlControlLoaderFactory.h"
#include "Graphics/TextureManager.h"
#include "OS/GameLogger.h"
#include "OS/System.h"

namespace FDX
{
	CXmlControlMaterialLoader::CXmlControlMaterialLoader()
	{
	}

	CXmlControlMaterialLoader::~CXmlControlMaterialLoader()
	{
	}

	EResult CXmlControlMaterialLoader::LoadMaterial(const tinyxml2::XMLElement * pNode, 
		CControlMaterial ** pMaterial)
	{
		EResult lResult = EResult::Success;
		CControlMaterial *lMaterial = nullptr;

		if (pNode)
		{
			String lstrName = (pNode->Attribute("name") != nullptr) ?
				pNode->Attribute("name") : "";

			EControlMaterialType lType = (EControlMaterialType)pNode->IntAttribute("type");

			lMaterial = new CControlMaterial(lType, lstrName);
			lResult = Parse(pNode, lMaterial);

			if (lResult != EResult::Success)
			{	
				LOG_DEBUG("CXmlControlMaterialLoader::Parse() failed");
				SAFE_DELETE(lMaterial);
			}
			else
			{
				*pMaterial = lMaterial;
			}
		}

		return lResult;
	}

	EResult CXmlControlMaterialLoader::Parse(const tinyxml2::XMLElement * pNode, CControlMaterial * pMaterial)
	{
		EResult lResult = EResult::Success;
		auto lChildNode = pNode->FirstChildElement();

		std::string lstrColorValues = pNode->Attribute("color");
		if (!lstrColorValues.empty())
		{
			std::istringstream lStringStream(lstrColorValues);
			std::string lstrFloat;

			Array<float> lColorValues;

			while (std::getline(lStringStream, lstrFloat, ' '))
				lColorValues.push_back((float)atof(lstrFloat.c_str()));

			pMaterial->SetColor(CColor4F(lColorValues[0], lColorValues[1],
				lColorValues[2], lColorValues[3]));
		}

		while (lChildNode != nullptr)
		{
			String lstrName = (lChildNode->Name() != nullptr) ?
				lChildNode->Name() : "";

			if (lstrName == "animations")
			{
				lResult = ParseAnimations(lChildNode, pMaterial);
				if (lResult != EResult::Success)
				{
					LOG_DEBUG("CXmlControlMaterialLoader::ParseAnimations() failed");
					break;
				}
			}
			else if (lstrName == "textures")
			{
				lResult = ParseTextures(lChildNode, pMaterial);
				if (lResult != EResult::Success)
				{
					LOG_DEBUG("CXmlControlMaterialLoader::ParseTextures() failed");
					break;
				}
			}
			else if (lstrName == "material_parts")
			{
				lResult = ParseMaterialParts(lChildNode, pMaterial);
				if (lResult != EResult::Success)
				{
					LOG_DEBUG("CXmlControlMaterialLoader::ParseMaterialParts() failed");
					break;
				}
			}

			lChildNode = lChildNode->NextSiblingElement();
		}

		return lResult;
	}

	EResult CXmlControlMaterialLoader::ParseAnimations(const tinyxml2::XMLElement * pNode,
		CControlMaterial * pMaterial)
	{
		EResult lResult = EResult::Success;
		auto lChildNode = pNode->FirstChildElement();

		while (lChildNode != nullptr)
		{
			String lstrName = (pNode->Name() != nullptr) ?
				pNode->Name() : "";

			if (lstrName == "animation")
			{
				CXmlControlAnimationLoaderPtr lLoader = std::make_unique<
					CXmlControlAnimationLoader>();

				IControlAnimation *lAnimation = nullptr;
				lResult = lLoader->LoadAnimation(lChildNode, &lAnimation);

				if (lResult != EResult::Success)
				{
					LOG_DEBUG("CXmlControlAnimationLoader::LoadAnimation() failed");
				}
				else
				{
					pMaterial->AddAnimation(IControlAnimationPtr(lAnimation));
				}
			}

			lChildNode = lChildNode->NextSiblingElement();
		}

		return lResult;
	}

	EResult CXmlControlMaterialLoader::ParseTextures(const tinyxml2::XMLElement * pNode, 
		CControlMaterial * pMaterial)
	{
		EResult lResult = EResult::Success;
		auto lChildNode = pNode->FirstChildElement();

		while (lChildNode != nullptr)
		{
			String lstrName = (lChildNode->Name() != nullptr) ?
				pNode->Name() : "";

			if (lstrName == "texture")
			{				
				String lstrFile = (lChildNode->Attribute("file") != nullptr) ? 
					lChildNode->Attribute("file") : "";

				if (!lstrFile.empty())
				{
					CTextureProperties lProperties;
					CTexture2D *lTexture = nullptr;
					
					CTextureManager::Get().AddTexture2D(lstrFile, lstrFile, 
						lProperties, &lTexture);

					pMaterial->AddTexture(lTexture);
				}
			}

			lChildNode = lChildNode->NextSiblingElement();
		}

		return lResult;
	}

	EResult CXmlControlMaterialLoader::ParseMaterialParts(const tinyxml2::XMLElement * pNode, 
		CControlMaterial * pMaterial)
	{
		EResult lResult = EResult::Success;
		return lResult;
	}
}