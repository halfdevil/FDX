#pragma once

#include "GUI/ControlSkin.h"
#include "GUI/XmlControlLoader.h"
#include "GUI/XmlControlMaterialLoader.h"

namespace FDX
{
	class FDX_EXPORT CXmlControlSkinLoader
	{
	public:

		CXmlControlSkinLoader();
		~CXmlControlSkinLoader();

		EResult LoadSkin(const tinyxml2::XMLElement *pNode, 
			CControlSkin **pSkin);
	};

	using CXmlControlSkinLoaderPtr = SmartPtr<CXmlControlSkinLoader>;
}