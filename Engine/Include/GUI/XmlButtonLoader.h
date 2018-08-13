#pragma once

#include "GUI/Button.h"
#include "GUI/XmlControlLoader.h"
#include "GUI/XmlControlMaterialLoader.h"
#include "GUI/XmlLabelLoader.h"

namespace FDX
{
	class FDX_EXPORT CXmlButtonLoader : public CXmlLabelLoader
	{
	public:

		CXmlButtonLoader(const String &pstrType);
		~CXmlButtonLoader();

	protected:

		virtual CControl* CreateControl(const String &pstrName) override;
	};

	using CXmlButtonLoaderPtr = SmartPtr<CXmlButtonLoader>;
}