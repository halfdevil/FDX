#pragma once

#include "FS/MountPoint.h"

namespace FDX
{
	class FDX_EXPORT CDirectory : public IMountPoint
	{
	public:

		CDirectory(const String &pstrName);
		virtual ~CDirectory();

		CDirectory(const CDirectory&) = delete;
		CDirectory& operator=(const CDirectory&) = delete;

		const String& GetName() const override
		{
			return mstrName;
		}

		const String& GetPath() const override
		{
			return mstrPath;
		}

		bool IsMounted() const override
		{
			return mbMounted;
		}

		EResult GetStream(const String &pstrFileName,
			IFileStream **pStream) override;

		EResult CreateStream(const String &pstrFileName,
			EAccessRights pAccessRights,
			IFileStream **pStream);		

		void Remove(const String &pstrFileName);		
		void Clear() override;

	protected:

		String mstrName;
		String mstrPath;
		bool mbMounted;
		UnorderedMap<String, IFileStreamPtr> mFiles;

		virtual EResult CreateStreamInternal(const String &pstrFileName, 
			EOpenMode pOpenMode,
			EAccessRights pAccessRights,
			IFileStream **pStream) = 0;
	};
}