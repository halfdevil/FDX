
#include "FS/Archive.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CArchive::CArchive(const String & pstrName)
		: mstrName(pstrName),
		mbMounted(false)
	{
	}

	CArchive::~CArchive()
	{
		Clear();
	}

	EResult CArchive::GetStream(const String & pstrFileName, IFileStream ** pStream)
	{
		EResult lResult = EResult::Success;

		auto lIt = mFiles.find(pstrFileName);
		if (lIt != mFiles.end())
		{
			*pStream = (*lIt).second.get();
			return lResult;
		}

		lResult = GetStreamInternal(pstrFileName,
			EAccessRights::Read, pStream);

		if (lResult != EResult::Success)
		{
			LOG_ERROR("Unable to get internal stream");
		}
		else
		{
			mFiles[pstrFileName] = IFileStreamPtr(*pStream);
		}

		return lResult;
	}

	void CArchive::Clear()
	{
		mFiles.clear();
	}
}