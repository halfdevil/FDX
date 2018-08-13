#pragma once

#include "GUI/Control.h"
#include "GUI/ControlContainer.h"
#include "tinyxml2.h"

namespace FDX
{
	class FDX_EXPORT CXmlControlLoader
	{
	public:

		CXmlControlLoader(const String &pstrType);
		virtual ~CXmlControlLoader();

		virtual bool CanLoadType(const String &pstrType);
		virtual EResult Load(const tinyxml2::XMLElement *pNode,
			CControlContainer *pContainer, CControl **pControl);

	protected:

		virtual CControl* CreateControl(const String &pstrName);

		virtual EResult LoadSkin(const tinyxml2::XMLElement *pNode, CControlSkin **pSkin);
		virtual EResult Parse(const tinyxml2::XMLElement *pNode,
			CControlContainer *pContainer, CControl *pControl);

	protected:

		String mstrType;
	};

	using CXmlControlLoaderPtr = SmartPtr<CXmlControlLoader>;
}