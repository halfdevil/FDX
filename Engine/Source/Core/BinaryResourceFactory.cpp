
#include "Core/BinaryResourceFactory.h"
#include "OS/GameLogger.h"

namespace FDX
{
	EResourceType CBinaryResourceFactory::GetResourceType() const
	{
		return EResourceType::Binary;
	}

	bool CBinaryResourceFactory::IsSupported(const String & pstrFilePath)
	{
		return true;
	}

	EResult CBinaryResourceFactory::Build(const String & pstrFilePath, IResource ** pResource)
	{
		*pResource = new CBinaryResource(pstrFilePath);
		return EResult::Success;
	}
}
