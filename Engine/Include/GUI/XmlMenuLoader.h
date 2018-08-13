#pragma once

#include "GUI/Menu.h"
#include "GUI/XmlControlLoader.h"
#include "GUI/XmlControlMaterialLoader.h"
#include "GUI/XmlButtonLoader.h"

namespace FDX
{	
	class FDX_EXPORT CXmlMenuItemLoader : public CXmlButtonLoader
	{
	public:

		CXmlMenuItemLoader(const String &pstrType);
		~CXmlMenuItemLoader();

	protected:

		virtual CControl* CreateControl(const String &pstrName) override;
		virtual EResult Parse(const tinyxml2::XMLElement *pNode, 
			CControlContainer *pContainer, CControl *pControl) override;
	};

	using CXmlMenuItemLoaderPtr = SmartPtr<CXmlMenuItemLoader>;

	class FDX_EXPORT CXmlMenuLoader : public CXmlMenuItemLoader
	{
	public:

		CXmlMenuLoader(const String &pstrType);
		~CXmlMenuLoader();

	protected:

		virtual CControl* CreateControl(const String &pstrName) override;
	};

	using CXmlMenuItemLoaderPtr = SmartPtr<CXmlMenuItemLoader>;
}