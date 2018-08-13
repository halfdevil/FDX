#pragma once

#include "Core/ResourceFactory.h"
#include "Core/BinaryResource.h"

namespace FDX
{
	class FDX_EXPORT CBinaryResourceFactory : public IResourceFactory
	{
	public:

		CBinaryResourceFactory() = default;
		~CBinaryResourceFactory() override = default;

		EResourceType GetResourceType() const override;
		bool IsSupported(const String &pstrFilePath) override;

		EResult Build(const String &pstrFilePath,
			IResource **pResource) override;
	};
}