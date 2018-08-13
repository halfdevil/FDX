
#include "Core/ModelResource.h"
#include "OS/GameLogger.h"
#include "OS/System.h"

namespace FDX
{
	CModelResource::CModelResource(const String & pstrFilePath, IModelLoader * pModelLoader)
		: mstrFilePath(pstrFilePath),
		mModelLoader(pModelLoader),
		mModel(nullptr),
		mbLoaded(false)
	{
	}

	CModelResource::~CModelResource()
	{
		Unload();
	}

	EResult CModelResource::Load()
	{
		FDX_ASSERT(mModelLoader != nullptr, "mModelLoader cannot be null");
		EResult lResult = EResult::Success;

		if (mbLoaded)
			return lResult;

		if (!mModelLoader->CanLoadModel(mstrFilePath))
		{
			LOG_ERROR("ModelLoader cannot load model: %s", mstrFilePath.c_str());
			lResult = EResult::LoadFailed;
		}
		else
		{
			IModel *lModel;
			lResult = mModelLoader->Load(mstrFilePath, &lModel);

			if (lResult != EResult::Success)
			{
				LOG_ERROR("ModelLoader cannot load model: %s", mstrFilePath.c_str());
				lResult = EResult::LoadFailed;
			}
			else
			{
				mModel = IModelPtr(lModel);
				mbLoaded = true;
			}
		}

		return lResult;
	}

	EResult CModelResource::Unload()
	{
		EResult lResult = EResult::Success;

		if (!mbLoaded)
			return lResult;

		mModel = nullptr;
		mbLoaded = false;

		return lResult;
	}

	EResult CModelResource::Reload()
	{
		if (mbLoaded)
			Unload();

		return Load();
	}
}