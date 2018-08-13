#pragma once

#include "Core/Resource.h"
#include "Core/ModelLoader.h"

namespace FDX
{
	class FDX_EXPORT CModelResource : public IResource
	{
	public:

		CModelResource(const String &pstrFilePath, IModelLoader *pModelLoader);
		~CModelResource();

		const String& GetFilePath() const
		{
			return mstrFilePath;
		}

		EResourceType GetResourceType() const override
		{
			return EResourceType::Model;
		}

		bool IsLoaded() const override
		{
			return mbLoaded;
		}

		IModelLoader* GetModelLoader() const
		{
			return mModelLoader;
		}

		IModel* GetModel() const
		{
			return mModel.get();
		}

		EResult Load() override;
		EResult Unload() override;
		EResult Reload() override;

	private:

		String mstrFilePath;
		IModelLoader *mModelLoader;
		IModelPtr mModel;
		bool mbLoaded;
	};
}