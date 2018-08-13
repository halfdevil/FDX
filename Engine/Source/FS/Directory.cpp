
#include "FS/Directory.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CDirectory::CDirectory(const String &pstrName)
		: mstrName(pstrName),
		mbMounted(false)
	{
	}

	CDirectory::~CDirectory()
	{
		Clear();
	}

	EResult CDirectory::GetStream(const String & pstrFileName, IFileStream ** pStream)
	{
		EResult lResult = EResult::Success;

		auto lIt = mFiles.find(pstrFileName);
		if (lIt != mFiles.end())
		{
			*pStream = (*lIt).second.get();
			return lResult;
		}

		lResult = CreateStreamInternal(pstrFileName,
			EOpenMode::Open,
			EAccessRights::Read, pStream);

		if (lResult != EResult::Success)
		{
			LOG_ERROR("Unable to get stream from internal method: %s", 
				CResultHelper::Get().GetErrorString(lResult));
		}
		else
		{
			mFiles[pstrFileName] = IFileStreamPtr(*pStream);			
		}

		return lResult;
	}

	EResult CDirectory::CreateStream(const String & pstrFileName, EAccessRights pAccessRights, IFileStream ** pStream)
	{
		EResult lResult = EResult::Success;

		auto lIt = mFiles.find(pstrFileName);
		if (lIt != mFiles.end())
		{
			*pStream = (*lIt).second.get();
			return lResult;
		}

		lResult = CreateStreamInternal(pstrFileName,
			EOpenMode::Create,
			pAccessRights, pStream);

		if (lResult != EResult::Success)
		{
			LOG_ERROR("Unable to create stream from internal method: %s",
				CResultHelper::Get().GetErrorString(lResult));
		}
		else
		{
			mFiles[pstrFileName] = IFileStreamPtr(*pStream);
		}

		return lResult;
	}

	void CDirectory::Remove(const String & pstrFileName)
	{
		auto lIt = mFiles.find(pstrFileName);

		if (lIt != mFiles.end())
		{
			mFiles.erase(lIt);
		}			
	}

	void CDirectory::Clear()
	{
		mFiles.clear();
	}
}