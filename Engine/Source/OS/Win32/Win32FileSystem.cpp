
#include "OS/Win32/Win32FileSystem.h"
#include "OS/Win32/Win32Directory.h"
#include "OS/Win32/Win32System.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CWin32FileSystem::CWin32FileSystem()
	{
	}

	EResult CWin32FileSystem::OpenDirectory(EOpenMode pOpenMode, const String & pstrName, 
		const String & pstrPath, CDirectory ** pDirectory)
	{
		FDX_ASSERT(pDirectory != nullptr, "Passed parameter pDirectory cannot be null");

		EResult lResult = EResult::Success;
		CDirectory *lDirectory = nullptr;

		lDirectory = new CWin32Directory(pstrName, pstrPath);
		lResult = dynamic_cast<CWin32Directory*>(lDirectory)->Create(pOpenMode);

		if (lResult != EResult::Success)
		{
			LOG_ERROR("Win32 CreateDirectory failed: %s", 
				CResultHelper::Get().GetErrorString(lResult));
		}
		else
		{
			*pDirectory = lDirectory;
		}

		return lResult;
	}

	EResult CWin32FileSystem::OpenArchive(const String & pstrName, const String & pstrPath, 
		EArchiveType pArchiveType, CArchive ** pArchive)
	{
		return EResult();
	}

	EResult CWin32FileSystem::AddMountPoint(const String & pstrAlias,
		IMountPoint * pMountPoint)
	{
		FDX_ASSERT(pMountPoint != nullptr, "Passed pointer pMountPoint cannot be null");

		EResult lResult = EResult::Success;
		auto lIt = mMountPoints.find(pstrAlias);

		if (lIt != mMountPoints.end())
			lResult = EResult::AddFailed;
		else
		{
			mMountPoints[pstrAlias] = IMountPointPtr(pMountPoint);
		}

		return lResult;
	}

	EResult CWin32FileSystem::RemoveMountPoint(const String & pstrAlias)
	{
		EResult lResult = EResult::Success;
		auto lIt = mMountPoints.find(pstrAlias);

		if (lIt == mMountPoints.end())
		{
			lResult = EResult::RemoveFailed;
		}
		else
		{
			mMountPoints.erase(lIt);
		}

		return lResult;
	}

	EResult CWin32FileSystem::GetMountPoint(const String & pstrAlias, 
		IMountPoint ** pMountPoint)
	{
		EResult lResult = EResult::Success;
		auto lIt = mMountPoints.find(pstrAlias);

		if (lIt == mMountPoints.end())
			lResult = EResult::Error;
		else
		{
			*pMountPoint = ((*lIt).second).get();
		}

		return lResult;
	}

	EResult CWin32FileSystem::GetStream(const String & pstrFileName,
		IFileStream ** pStream)
	{
		EResult lResult = EResult::Success;
		IMountPoint *lMountPoint = nullptr;
		IFileStream *lStream = nullptr;
		String lstrActualPath;

		auto lIt = mMountPoints.begin();
		while (lIt != mMountPoints.end())
		{
			String lstrAlias = (*lIt).first;
			if (pstrFileName.compare(0, lstrAlias.length(), lstrAlias) == 0)
			{
				lMountPoint = (*lIt).second.get();
				lstrActualPath = pstrFileName.substr(lstrAlias.size());

				break;
			}

			lIt++;
		}

		if (lMountPoint == nullptr)
		{
			LOG_ERROR("No mount point found for the path: %s", pstrFileName.c_str());
			lResult = EResult::NotFound;
		}
		else
		{
			lResult = lMountPoint->GetStream(lstrActualPath, &lStream);
			if (lResult != EResult::Success)
			{
				LOG_ERROR("Unable to get stream %s from mountpoint: %s", 
					pstrFileName.c_str(),
					lMountPoint->GetName().c_str());

				lResult = EResult::NotFound;
			}
			else
			{
				*pStream = lStream;
			}
		}

		return lResult;
	}
	
	bool CWin32FileSystem::IsExist(const String & pstrFileName)
	{
		IMountPoint *lMountPoint = nullptr;
		String lstrActualPath;

		auto lIt = mMountPoints.begin();
		while (lIt != mMountPoints.end())
		{
			String lstrAlias = (*lIt).first;
			if (pstrFileName.compare(0, lstrAlias.length(), lstrAlias) == 0)
			{
				lMountPoint = (*lIt).second.get();
				lstrActualPath = pstrFileName.substr(lstrAlias.size());

				break;
			}

			lIt++;
		}

		if (lMountPoint == nullptr)
		{
			LOG_DEBUG("MountPoints::Find() failed for: %s", pstrFileName.c_str());
			return false;
		}

		return lMountPoint->IsExist(lstrActualPath);
	}

	char CWin32FileSystem::GetPathSeperator() const
	{
		return '\\';
	}
}