
#include "Core/XmlResource.h"
#include "FS/FileSystem.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CXmlResource::CXmlResource(const String & pstrFilePath)
		: mstrFilePath(pstrFilePath),
		mbLoaded(false)
	{
	}

	CXmlResource::~CXmlResource()
	{
		Unload();
	}

	EResult CXmlResource::Load()
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
			SmartPtr<uint8_t[]> lData = std::make_unique<uint8_t[]>(lStream->GetSize() + 1);
			memset(lData.get(), 0, lStream->GetSize());

			lResult = lStream->Read(lStream->GetSize(), lData.get());

			if (lResult != EResult::Success)
			{
				LOG_ERROR("Unable to read data from stream %s, error: %s",
					mstrFilePath.c_str(), CResultHelper::Get().GetErrorString(lResult));
			}
			else
			{				
				tinyxml2::XMLError lReturn = mDocument.Parse((const char*)lData.get(), 
					lStream->GetSize());

				if (lReturn != tinyxml2::XMLError::XML_SUCCESS)
				{
					LOG_ERROR("XML file parsing error: %d", lReturn);
					lResult = EResult::LoadFailed;
				}
			}
		}

		return lResult;
	}

	EResult CXmlResource::Unload()
	{
		EResult lResult = EResult::Success;

		if (!mbLoaded)
			return lResult;

		mDocument.Clear();

		return lResult;
	}

	EResult CXmlResource::Reload()
	{
		if (mbLoaded)
			Unload();

		return Load();
	}
}