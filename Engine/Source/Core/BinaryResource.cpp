
#include "Core/BinaryResource.h"
#include "FS/FileSystem.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CBinaryResource::CBinaryResource(const String & pstrFilePath)
		: mstrFilePath(pstrFilePath),
		muiDataSize(0),
		mData(nullptr),
		mbLoaded(false)
	{
	}

	CBinaryResource::~CBinaryResource()
	{
		Unload();
	}

	EResult CBinaryResource::Load()
	{
		EResult lResult = EResult::Success;

		if (mbLoaded)
			return lResult;
		
		IFileStream *lStream = nullptr;
		lResult = IFileSystem::Get().GetStream(mstrFilePath, &lStream);

		if (lResult != EResult::Success)
		{
			LOG_ERROR("Unable to get stream %s, error: %s", 
				mstrFilePath.c_str(), CResultHelper::Get().GetErrorString(lResult));
		}
		else
		{
			muiDataSize = lStream->GetSize();
			auto *lData = new uint8_t[muiDataSize];

			lResult = lStream->Read(muiDataSize, lData);
			if (lResult != EResult::Success)
			{
				LOG_ERROR("Unable to read data from stream %s, error: %s",
					mstrFilePath.c_str(), CResultHelper::Get().GetErrorString(lResult));
			}
			else
			{
				mData = SmartPtr<uint8_t[]>(lData);
				mbLoaded = true;
			}
		}

		return lResult;
	}

	EResult CBinaryResource::Unload()
	{
		EResult lResult = EResult::Success;

		if (!mbLoaded)
			return lResult;

		mData = nullptr;
		mbLoaded = false;

		return lResult;
	}

	EResult CBinaryResource::Reload()
	{
		if (mbLoaded)
			Unload();

		return Load();
	}
}