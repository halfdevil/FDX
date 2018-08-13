#pragma once

#include "FS/MountPoint.h"

namespace FDX
{
	enum class EArchiveType
	{
		Zip
	};

	class FDX_EXPORT CArchive : public IMountPoint
	{
	public:

		CArchive(const String &pstrName);
		virtual ~CArchive();

		CArchive(const CArchive&) = delete;
		CArchive& operator = (const CArchive&) = delete;

		EArchiveType GetArchiveType() const
		{
			return mArchiveType;
		}

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

		void Clear() override;

	protected:

		String mstrName;
		String mstrPath;
		bool mbMounted;
		EArchiveType mArchiveType;
		UnorderedMap<String, IFileStreamPtr> mFiles;

		virtual EResult GetStreamInternal(const String &pstrFileName,
			EAccessRights pAccessRights,
			IFileStream **pStream) = 0;
	};
}
