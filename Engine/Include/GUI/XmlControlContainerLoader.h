#pragma once

#include "GUI/ControlContainerLoader.h"
#include "Core/XmlResource.h"

namespace FDX
{
	class FDX_EXPORT CXmlControlContainerLoader : public IControlContainerLoader
	{
	public:

		CXmlControlContainerLoader();
		~CXmlControlContainerLoader();

		EControlContainerLoaderType GetLoaderType() const;

		bool CanLoadType(EControlContainerLoaderType pLoaderType);
		bool CanLoad(const String &pstrFilePath);

		EResult Load(const String &pstrFilePath, CControlContainer **pContainer);
		
	private:

		EResult LoadControl(const tinyxml2::XMLElement *pNode, CControl *pParent,
			CControlContainer *pContainer);

		EResult LoadLayout(const tinyxml2::XMLElement *pNode, CControlContainer *pContainer);
	};
}