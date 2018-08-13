#pragma once

#include "Core/Resource.h"

namespace FDX
{
	class FDX_EXPORT CBinaryResource : public IResource
	{
	public:

		explicit CBinaryResource(const String &pstrFilePath);
		~CBinaryResource() override;

		EResourceType GetResourceType() const override
		{
			return EResourceType::Binary;
		}

		bool IsLoaded() const override
		{
			return mbLoaded;
		}

		uint32_t GetSize() const
		{
			return muiDataSize;
		}

		uint8_t* GetData() const
		{
			return mData.get();
		}

		EResult Load() override;
		EResult Unload() override;
		EResult Reload() override;

	private:

		String mstrFilePath;
		uint32_t muiDataSize;
		SmartPtr<uint8_t[]> mData;			
		bool mbLoaded;
	};

	using CBinaryResourcePtr = SmartPtr<CBinaryResource>;
}
