#pragma once

#include "Core/Resource.h"

namespace FDX
{
	class FDX_EXPORT IResourceFactory
	{
	public:

		virtual ~IResourceFactory()
		{
		}

		virtual EResourceType GetResourceType() const = 0;

		virtual bool IsSupported(const String &pstrFilePath) = 0;
		virtual EResult Build(const String &pstrFilePath, 
			IResource **pResource) = 0;
	};

	using IResourceFactoryPtr = SmartPtr<IResourceFactory>;
}