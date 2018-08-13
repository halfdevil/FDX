#pragma once

#include "Core/Resource.h"
#include "tinyxml2.h"

namespace FDX
{
	class FDX_EXPORT CXmlResource : public IResource
	{
	public:

		CXmlResource(const String &pstrFilePath);
		~CXmlResource();

		EResourceType GetResourceType() const override
		{
			return EResourceType::Xml;
		}

		const tinyxml2::XMLDocument& GetDocument() const
		{
			return mDocument;
		}

		bool IsLoaded() const override
		{
			return mbLoaded;
		}

		EResult Load() override;
		EResult Unload() override;
		EResult Reload() override;

	private:

		String mstrFilePath;
		tinyxml2::XMLDocument mDocument;
		bool mbLoaded;
	};
}