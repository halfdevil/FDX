
#include "Core/ResourceManager.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CResourceManager::CResourceManager(CAsyncQueue *pAsyncQueue)
		: mAsyncQueue(pAsyncQueue)
	{
	}

	CResourceManager::~CResourceManager()
	{
		UnloadAll();
	}

	uint32_t CResourceManager::GetNumResourcesToLoad() const
	{
		std::lock_guard<std::mutex> lLock(mResourceMutex);
		return mResourcesToLoadMap.size();
	}

	uint32_t CResourceManager::GetNumResourcesLoaded() const
	{
		std::lock_guard<std::mutex> lLock(mResourceMutex);
		return mResourcesLoaded.size();
	}

	bool CResourceManager::IsResourceLoaded(const String & pstrFilePath)
	{
		std::lock_guard<std::mutex> lLock(mResourceMutex);
		return mResourcesLoaded.find(pstrFilePath) != mResourcesLoaded.end();
	}

	EResult CResourceManager::UnregisterFactory(EResourceType puiResourceType)
	{
		EResult lResult = EResult::Success;
		auto lIt = mResorceFactoryMap.find(puiResourceType);

		if (lIt == mResorceFactoryMap.end())
		{
			LOG_DEBUG("Resource factory of type %d does not exist", puiResourceType);
			lResult = EResult::NotFound;
		}
		else
		{			
			mResorceFactoryMap.erase(lIt);
		}

		return lResult;
	}

	EResult CResourceManager::LoadAllResources()
	{
		EResult lResult = EResult::Success;
		
		std::lock_guard<std::mutex> lLock(mResourceMutex);		
		auto lIt = mResourcesToLoadMap.begin();

		while (lIt != mResourcesToLoadMap.end())
		{
			lResult = (*lIt).second->Load();
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("Resource load failed for %s", (*lIt).first.c_str());
			}
			else
			{
				mResourcesLoaded[(*lIt).first] = std::move((*lIt).second);
				mResourcesToLoadMap.erase(lIt);
			}
			lIt++;
		}
		
		return lResult;
	}

	EResult CResourceManager::LoadResourceAsync(EResourceType puiResourceType, const String & pstrFilePath)
	{
		EResult lResult = EResult::Success;
		auto lIt = mResourcesLoaded.find(pstrFilePath);

		if (lIt != mResourcesLoaded.end())
		{
			LOG_DEBUG("Resource %s already loaded", pstrFilePath.c_str());
			lResult = EResult::LoadFailed;
		}
		else
		{
			auto lFactoryIt = mResorceFactoryMap.find(puiResourceType);

			if (lFactoryIt == mResorceFactoryMap.end())
			{
				LOG_DEBUG("Resource factory of type %d not found", puiResourceType);
				lResult = EResult::LoadFailed;
			}
			else
			{
				IResource *lResource;
				lResult = (*lFactoryIt).second->Build(pstrFilePath, &lResource);

				if (lResult != EResult::Success)
				{
					LOG_ERROR("Unable to build resource %s from resource factory", 
						pstrFilePath.c_str());
				}
				else
				{
					mResourcesToLoadMap[pstrFilePath] = IResourcePtr(lResource);
				}
			} 
		}

		return lResult;
	}

	EResult CResourceManager::RemoveResource(const String & pstrFilePath)
	{
		EResult lResult = EResult::Success;
		auto lIt = mResourcesLoaded.find(pstrFilePath);

		if (lIt == mResourcesLoaded.end())
			lResult = EResult::RemoveFailed;
		else
		{
			mResourcesLoaded.erase(lIt);
		}

		return lResult;
	}

	void CResourceManager::StartLoading()
	{
		EResult lResult = EResult::Success;

		if (mResourcesToLoadMap.size() == 0)
		{
			LOG_DEBUG("No resources to load");
			return;
		}

		mThread = CWorkerThreadPtr(new CWorkerThread());
		mThread->Start();

		mThread->AddTask(new CResourceLoadTask(this));
	}

	void CResourceManager::StopLoading()
	{
		mThread->Exit(true);
	}

	void CResourceManager::CompleteLoading()
	{
		mAsyncQueue->EnqueueCapsule(new CResourceLoadCompleteCapsule(this));
	}

	void CResourceManager::UnloadAll()
	{
		mResourcesLoaded.clear();
		mResourcesToLoadMap.clear();
		mResorceFactoryMap.clear();
	}

	void CResourceManager::OnLoadingCompleted()
	{
		mThread->Exit(true);
	}
	
	CResourceLoadTask::CResourceLoadTask(CResourceManager * pResourceManager)
		: mResourceManager(pResourceManager)
	{
	}

	CResourceLoadTask::~CResourceLoadTask()
	{
	}

	void CResourceLoadTask::Run()
	{
		std::lock_guard<std::mutex> lLock(mExitingMutex);

		mResourceManager->LoadAllResources();
		mResourceManager->CompleteLoading();
	}

	void CResourceLoadTask::Exit()
	{
		std::lock_guard<std::mutex> lLock(mExitingMutex);
		CTask::Exit();
	}

	CResourceLoadCompleteCapsule::CResourceLoadCompleteCapsule(CResourceManager * pManager)
		: mResourceManager(pManager)
	{
	}

	CResourceLoadCompleteCapsule::~CResourceLoadCompleteCapsule()
	{
	}

	void CResourceLoadCompleteCapsule::Invoke()
	{
		mResourceManager->OnLoadingCompleted();
	}
}