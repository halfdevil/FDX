#pragma once

#include "GUI/XmlControlLoader.h"
#include "Core/Singleton.h"

namespace FDX
{
	class FDX_EXPORT CXmlControlLoaderFactory : public CSingleton<CXmlControlLoaderFactory>
	{
	public:

		CXmlControlLoaderPtr GetXmlControlLoader(const tinyxml2::XMLElement *pNode);
	};
}