#pragma once

#include "GUI/Panel.h"
#include "GUI/XmlControlLoader.h"
#include "GUI/XmlControlMaterialLoader.h"

namespace FDX
{
	class FDX_EXPORT CXmlPanelLoader : public CXmlControlLoader
	{
	public:

		CXmlPanelLoader(const String &pstrType);
		~CXmlPanelLoader();

	protected:

		virtual CControl* CreateControl(const String &pstrName) override;
	};
}