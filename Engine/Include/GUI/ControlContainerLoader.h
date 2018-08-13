#pragma once

#include "GUI/ControlContainer.h"
#include "Core/Resource.h"

namespace FDX
{
	enum class EControlContainerLoaderType
	{
		Xml
	};

	class FDX_EXPORT IControlContainerLoader
	{
	public:

		virtual ~IControlContainerLoader()
		{
		}

		virtual EControlContainerLoaderType GetLoaderType() const = 0;

		virtual bool CanLoadType(EControlContainerLoaderType pLoaderType) = 0;
		virtual bool CanLoad(const String &pstrFilePath) = 0;

		virtual EResult Load(const String &pstrFilePath, CControlContainer **pContainer) = 0;
	};

	using IControlContainerLoaderPtr = SmartPtr<IControlContainerLoader>;
}