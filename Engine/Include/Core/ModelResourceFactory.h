#pragma once

#include "Core/ResourceFactory.h"
#include "Core/ModelResource.h"

namespace FDX
{
	class FDX_EXPORT CModelResourceFactory : public IResourceFactory
	{
	public:

		CModelResourceFactory();
		~CModelResourceFactory();

		CModelResourceFactory(const CModelResourceFactory&) = delete;
		CModelResourceFactory& operator=(const CModelResourceFactory&) = delete;

		EResourceType GetResourceType() const;

		bool IsSupported(const String &pstrFilePath);
		EResult Build(const String &pstrFilePath,
			IResource **pResource);

		template <typename T>
		EResult RegisterLoader(EModelLoaderType puiModelLoaderType, T **pLoader);

	private:

		Array<IModelLoaderPtr> mModelLoaders;
	};

	template<typename T>
	inline EResult CModelResourceFactory::RegisterLoader(EModelLoaderType puiModelLoaderType, T ** pLoader)
	{
		EResult lResult = EResult::Success;

		auto lIt = mModelLoaders.begin();
		while (lIt != mModelLoaders.end())
		{
			if ((*lIt)->GetModelLoaderType() == puiModelLoaderType)
			{
				lResult = EResult::AddFailed;
				break;
			}

			lIt++;
		}

		if (lIt == mModelLoaders.end())
		{
			*pLoader = new T();
			mModelLoaders.push_back(IModelLoaderPtr(*pLoader));
		}

		return lResult;
	}
}