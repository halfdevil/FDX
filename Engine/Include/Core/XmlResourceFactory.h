#pragma once

#include "Core/XmlResource.h"
#include "Core/ResourceFactory.h"

namespace FDX
{
	class FDX_EXPORT CXmlResourceFactory : public IResourceFactory
	{
	public:

		CXmlResourceFactory();
		~CXmlResourceFactory();

		EResourceType GetResourceType() const;
		bool IsSupported(const String &pstrFilePath);

		EResult Build(const String &pstrFilePath,
			IResource **pResource);
	};
}