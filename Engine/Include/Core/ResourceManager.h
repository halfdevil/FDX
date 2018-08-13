#pragma once

#include "Core/Singleton.h"
#include "Core/Resource.h"
#include "Core/ResourceFactory.h"
#include "OS/GameLogger.h"
#include "OS/AsyncQueue.h"
#include "OS/WorkerThread.h"

namespace FDX
{
	class CResourceManager;
	using CResourceManagerPtr = SmartPtr<CResourceManager>;

	class FDX_EXPORT CResourceLoadCompleteCapsule : public IAsyncCapsule
	{
	public:

		CResourceLoadCompleteCapsule(CResourceManager *pManager);
		~CResourceLoadCompleteCapsule();

		void Invoke() override;

	private:

		CResourceManager *mResourceManager;
	};

	class FDX_EXPORT CResourceLoadTask : public CTask
	{
	public:

		CResourceLoadTask(CResourceManager *pResourceManager);
		~CResourceLoadTask();

		void Run() override;
		void Exit() override;

	private:

		CResourceManager *mResourceManager;
		std::mutex mExitingMutex;
	};

	class FDX_EXPORT CResourceManager : public CSingleton<CResourceManager>
	{
	public:

		CResourceManager(CAsyncQueue *pAsyncQueue);
		~CResourceManager();

		CResourceManager(const CResourceManager&) = delete;
		CResourceManager& operator = (const CResourceManager&) = delete;

		uint32_t GetNumResourcesToLoad() const;
		uint32_t GetNumResourcesLoaded() const;

		bool IsResourceLoaded(const String &pstrFilePath);

		template <typename T>
		EResult GetFactory(EResourceType puiResourceType, T **pFactory);

		template <typename T>
		EResult RegisterFactory(EResourceType puiResourceType, T **pFactory);
		EResult UnregisterFactory(EResourceType puiResourceType);

		template <typename T>
		EResult GetResource(const String &pstrFilePath,
			T **pResource);

		template <typename T>
		EResult LoadResource(EResourceType puiResourceType,
			const String &pstrFilePath,
			T **pResource);
		
		EResult LoadAllResources();
		EResult LoadResourceAsync(EResourceType puiResourceType,
			const String &pstrFilePath);

		EResult RemoveResource(const String &pstrFilePath);

		void StartLoading();
		void StopLoading();

		void CompleteLoading();
		void UnloadAll();

		void OnLoadingCompleted();

	private:
		
		Map<EResourceType, IResourceFactoryPtr> mResorceFactoryMap;
		Map<String, IResourcePtr> mResourcesToLoadMap;
		Map<String, IResourcePtr> mResourcesLoaded;

		CWorkerThreadPtr mThread;
		CAsyncQueue *mAsyncQueue;
		mutable std::mutex mResourceMutex;
	};

	template<typename T>
	inline EResult CResourceManager::GetFactory(EResourceType puiResourceType, T ** pFactory)
	{
		EResult lResult = EResult::Success;

		auto lIt = mResorceFactoryMap.find(puiResourceType);
		if (lIt != mResorceFactoryMap.end())
		{			
			*pFactory = static_cast<T*>((*lIt).second.get());
		}
		else
		{
			lResult = EResult::NotFound;
		}

		return lResult;
	}

	template<typename T>
	inline EResult CResourceManager::RegisterFactory(EResourceType puiResourceType, T **pFactory)
	{
		EResult lResult = EResult::Success;
		auto lIt = mResorceFactoryMap.find(puiResourceType);

		if (lIt != mResorceFactoryMap.end())
		{
			LOG_DEBUG("Resource factory of type %d already exist", puiResourceType);
			lResult = EResult::RegisterFailed;
		}
		else
		{
			*pFactory = new T();
			mResorceFactoryMap[puiResourceType] = IResourceFactoryPtr(*pFactory);
		}

		return lResult;
	}

	template<typename T>
	inline EResult CResourceManager::GetResource(const String & pstrFilePath, T ** pResource)
	{
		EResult lResult = EResult::Success;
		auto lIt = mResourcesLoaded.find(pstrFilePath);

		if (lIt == mResourcesLoaded.end())
		{
			LOG_ERROR("Resource %s not loaded", pstrFilePath.c_str());
			lResult = EResult::NotFound;
		}
		else
		{
			*pResource = static_cast<T*>((*lIt).second.get());			

			if (*pResource == nullptr)
			{
				LOG_DEBUG("GetResource() - invalid casting");
				lResult = EResult::LoadFailed;
			}
		}

		return lResult;
	}

	template<typename T>
	inline EResult CResourceManager::LoadResource(EResourceType puiResourceType,
		const String & pstrFilePath, T ** pResource)
	{
		EResult lResult = EResult::Success;
		auto lIt = mResourcesLoaded.find(pstrFilePath);

		if (lIt != mResourcesLoaded.end())
		{
			LOG_DEBUG("Resource %s already loaded", pstrFilePath.c_str());
			*pResource = static_cast<T*>((*lIt).second.get());
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
					lResult = lResource->Load();
					if (lResult != EResult::Success)
					{
						LOG_ERROR("Unable to load resource %s", pstrFilePath.c_str());
					}
					else
					{
						*pResource = static_cast<T*>(lResource);
						mResourcesLoaded[pstrFilePath] = IResourcePtr(lResource);						
					}					
				}
			}

			return lResult;
		}

		return lResult;
	}
}