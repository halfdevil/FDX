
#include "Core/XmlResourceFactory.h"

namespace FDX
{
	CXmlResourceFactory::CXmlResourceFactory()
	{
	}

	CXmlResourceFactory::~CXmlResourceFactory()
	{
	}

	EResourceType CXmlResourceFactory::GetResourceType() const
	{
		return EResourceType::Xml;
	}

	bool CXmlResourceFactory::IsSupported(const String & pstrFilePath)
	{
		bool lbSupported = false;
		uint32_t luiDot = pstrFilePath.find_last_of(".");

		if (luiDot != (uint32_t)String::npos)
		{
			String lstrExt = pstrFilePath.substr(luiDot);
			std::transform(lstrExt.begin(), lstrExt.end(), lstrExt.begin(), ::tolower);

			if (lstrExt == ".xml")
			{
				lbSupported = true;
			}
		}

		return lbSupported;
	}

	EResult CXmlResourceFactory::Build(const String & pstrFilePath, IResource ** pResource)
	{
		*pResource = new CXmlResource(pstrFilePath);
		return EResult::Success;
	}
}