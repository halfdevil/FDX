#pragma once

#include "GUI/Label.h"
#include "GUI/XmlControlLoader.h"
#include "GUI/XmlControlMaterialLoader.h"

namespace FDX
{
	class FDX_EXPORT CXmlLabelLoader : public CXmlControlLoader
	{
	public:

		CXmlLabelLoader(const String &pstrType);
		~CXmlLabelLoader();

	protected:

		virtual CControl* CreateControl(const String &pstrName) override;
		virtual EResult Parse(const tinyxml2::XMLElement *pNode,
			CControlContainer *pContainer, CControl *pControl) override;
	};
}