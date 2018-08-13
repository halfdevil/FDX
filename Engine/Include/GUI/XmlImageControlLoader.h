#pragma once

#include "GUI/ImageControl.h"
#include "GUI/XmlControlLoader.h"
#include "GUI/XmlControlMaterialLoader.h"

namespace FDX
{	
	class FDX_EXPORT CXmlImageControlLoader : public CXmlControlLoader
	{
	public:

		CXmlImageControlLoader(const String &pstrType);
		~CXmlImageControlLoader();

	protected:

		virtual CControl* CreateControl(const String &pstrName) override;
	};
}