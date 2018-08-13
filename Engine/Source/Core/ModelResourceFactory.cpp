
#include "Core/ModelResourceFactory.h"

namespace FDX
{
	CModelResourceFactory::CModelResourceFactory()
	{
	}

	CModelResourceFactory::~CModelResourceFactory()
	{
		mModelLoaders.clear();
	}

	EResourceType CModelResourceFactory::GetResourceType() const
	{
		return EResourceType::Model;
	}

	bool CModelResourceFactory::IsSupported(const String &pstrFilePath)
	{
		bool lbRet = false;

		auto lIt = mModelLoaders.begin();
		while (lIt != mModelLoaders.end())
		{
			if ((*lIt)->CanLoadModel(pstrFilePath))
			{
				lbRet = true;
				break;
			}

			lIt++;
		}

		return lbRet;
	}

	EResult CModelResourceFactory::Build(const String &pstrFilePath, IResource **pResource)
	{
		EResult lResult = EResult::LoadFailed;

		auto lIt = mModelLoaders.begin();
		while (lIt != mModelLoaders.end())
		{
			if ((*lIt)->CanLoadModel(pstrFilePath))
			{
				*pResource = new CModelResource(pstrFilePath, (*lIt).get());
				lResult = EResult::Success;
				break;
			}

			lIt++;
		}

		return lResult;
	}
}