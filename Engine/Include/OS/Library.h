#pragma once

#include "Core/Core.h"

namespace FDX
{
	class FDX_EXPORT ILibrary
	{
	public:

		virtual ~ILibrary()
		{
		}

		virtual const String& GetFileName() const = 0;
		virtual void* GetFunctionAddress(const char *pstrFuncName) = 0;
	};

	using ILibraryPtr = SmartPtr<ILibrary>;
}