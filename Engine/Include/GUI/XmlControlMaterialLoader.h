#pragma once

#include "GUI/XmlControlAnimationLoader.h"
#include "GUI/ControlMaterial.h"

namespace FDX
{
	class FDX_EXPORT CXmlControlMaterialLoader
	{
	public:

		CXmlControlMaterialLoader();
		virtual ~CXmlControlMaterialLoader();

		virtual EResult LoadMaterial(const tinyxml2::XMLElement *pNode, 
			CControlMaterial **pMaterial);

	protected:

		virtual EResult Parse(const tinyxml2::XMLElement *pNode,
			CControlMaterial *pMaterial);

		virtual EResult ParseAnimations(const tinyxml2::XMLElement *pNode,
			CControlMaterial *pMaterial);

		virtual EResult ParseTextures(const tinyxml2::XMLElement *pNode,
			CControlMaterial *pMaterial);

		virtual EResult ParseMaterialParts(const tinyxml2::XMLElement *pNode,
			CControlMaterial *pMaterial);
	};

	using CXmlControlMaterialLoaderPtr = SmartPtr<CXmlControlMaterialLoader>;
}