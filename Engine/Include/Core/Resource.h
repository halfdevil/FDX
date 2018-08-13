#pragma once

#include "Core/Core.h"
#include "Core/Result.h"

namespace FDX
{
	enum class EResourceType
	{
		Image,
		Binary,
		Font,
		Model,
		Xml
	};

	class FDX_EXPORT IResource
	{
	public:

		virtual ~IResource()
		{
		}

		virtual EResourceType GetResourceType() const = 0;
		virtual bool IsLoaded() const = 0;

		virtual EResult Load() = 0;
		virtual EResult Unload() = 0;
		virtual EResult Reload() = 0;
	};

	using IResourcePtr = SmartPtr<IResource>;
}