
#include "OS/Win32/Win32File.h"
#include "OS/Win32/Win32Directory.h"
#include "OS/Win32/Win32MappedFile.h"
#include "OS/Win32/Win32UnicodeAdapter.h"
#include "OS/GameLogger.h"
#include "OS/System.h"
#include <Shlwapi.h>
#include <ShlObj.h>

namespace FDX
{
	CWin32Directory::CWin32Directory(const String & pstrName, const String &pstrPath)
		: CDirectory(pstrName)
	{
		mstrPath = pstrPath;
	}

	CWin32Directory::~CWin32Directory()
	{
	}

	EResult CWin32Directory::Create(EOpenMode pOpenMode)
	{
		EResult lResult = EResult::Success;
		UTF16String lstrPath = CWin32UTF16Adapter(mstrPath).Get();

		if (pOpenMode == EOpenMode::Open)
		{
			DWORD ldwAttribs = GetFileAttributes(lstrPath.c_str());

			if (ldwAttribs == INVALID_FILE_ATTRIBUTES)
				lResult = EResult::CreateFailed;
			else if (!(ldwAttribs & FILE_ATTRIBUTE_DIRECTORY))
				lResult = EResult::CreateFailed;
		}
		else
		{
			int32_t liRet = SHCreateDirectoryEx(nullptr, lstrPath.c_str(), nullptr);
			if (liRet != ERROR_SUCCESS && liRet != ERROR_FILE_EXISTS)
			{
				LOG_DEBUG("Win32 CreateDirectory failed: %d", liRet);
				lResult = EResult::CreateFailed;
			}
		}

		return lResult;
	}

	bool CWin32Directory::IsExist(const String & pstrFileName)
	{
		String lstrFullPath;

		if (pstrFileName[pstrFileName.size() - 1] == '\\')
			lstrFullPath = mstrPath + pstrFileName;
		else
			lstrFullPath = mstrPath + '\\' + pstrFileName;

		if (!PathFileExists(CWin32UTF16Adapter(lstrFullPath).Get().c_str()))
		{
			LOG_DEBUG("PathFileExists failed for file: %s", lstrFullPath.c_str());
			return false;
		}

		return true;
	}

	EResult CWin32Directory::CreateStreamInternal(const String & pstrFileName, EOpenMode pOpenMode, 
		EAccessRights pAccessRights, IFileStream ** pStream)
	{
		EResult lResult = EResult::Success;
		IFileStream *lFile = nullptr;
		String lstrAbsouleFileName;

		if (mstrPath[mstrPath.length() - 1] == '\\')
			lstrAbsouleFileName = mstrPath + pstrFileName;
		else
			lstrAbsouleFileName = mstrPath + "\\" + pstrFileName;

		if (!PathFileExists(CWin32UTF16Adapter(lstrAbsouleFileName).Get().c_str()))
		{
			LOG_ERROR("File %s does not exist", lstrAbsouleFileName.c_str());
			lResult = EResult::CreateFailed;
		}
		else
		{
			if (pOpenMode == EOpenMode::Open && pAccessRights == EAccessRights::Read)
			{
#if defined FDX_USE_MAPPEDFILE
				lFile = new CWin32MappedFile(lstrAbsouleFileName);
#else
				lFile = new CWin32File(lstrAbsouleFileName);
#endif
			}
			else
			{
				lFile = new CWin32File(lstrAbsouleFileName);
			}

			lResult = dynamic_cast<CWin32File*>(lFile)->Create(pOpenMode, pAccessRights);
			if (lResult != EResult::Success)
			{
				LOG_ERROR("CWin32File::Create() failed for: %s", lstrAbsouleFileName.c_str());
				SAFE_DELETE(lFile);

				return lResult;
			}
			else
			{
				*pStream = lFile;
			}
		}

		return lResult;
	}
}