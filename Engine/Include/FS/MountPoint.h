#pragma once

#include "FS/FileStream.h"

namespace FDX
{
	class FDX_EXPORT IMountPoint
	{
	public:

		virtual ~IMountPoint()
		{
		}

		virtual const String& GetName() const = 0;
		virtual const String& GetPath() const = 0;

		virtual EResult GetStream(const String &pstrFileName,
			IFileStream **pStream) = 0;

		virtual bool IsMounted() const = 0;
		virtual void Clear() = 0;

		virtual bool IsExist(const String &pstrFileName) = 0;
	};

	using IMountPointPtr = SmartPtr<IMountPoint>;
}