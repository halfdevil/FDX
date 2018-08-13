#pragma once

#include "GUI/ControlContainer.h"
#include "GUI/ControlContainerLoader.h"
#include "Core/Singleton.h"
#include "GUI/Cursor.h"
#include "GUI/UITheme.h"

namespace FDX
{
	class FDX_EXPORT CUIManager : public IEventHandler, public CSingleton<CUIManager>
	{
	public:

		CUIManager();
		~CUIManager();

		CUIManager(const CUIManager&) = delete;
		CUIManager& operator=(const CUIManager&) = delete;

		CControlContainer* GetActiveContainer() const
		{
			return mActiveContainer;
		}

		CControlContainer* GetContainer(uint32_t puiIdx) const
		{
			if (puiIdx < mControlContainers.size())
				return mControlContainers[puiIdx].get();

			return nullptr;
		}

		bool GetShowCursor() const
		{
			return mbShowCursor;
		}

		CCursor* GetCursor() const
		{
			return mCursor.get();
		}

		IUITheme* GetTheme() const
		{
			return mTheme.get();
		}

		CControlContainer* LoadContainer(const String &pstrFilePath);
		CControlContainer* CreateContainer(const String &pstrName);

		CCursor* CreateCursor(const String &pstrName);

		bool CanHandleEvent(uint32_t puiEventType);
		bool HandleEvent(IEvent *pEvent);

		void SetTheme(IUIThemePtr pTheme);
		void SetCursorType(ECursorType pCursorType);
		void SetActiveContainer(uint32_t puiIdx);

		void Update(float pfDelta);
		void Draw(CCanvas *pCanvas);
		void ShowCursor(bool pbShow);	

		template <typename T>
		EResult RegisterLoader(EControlContainerLoaderType pLoaderType, T **pLoader);

	private:
						
		IUIThemePtr mTheme;
		CControlContainer *mActiveContainer;
		Array<CControlContainerPtr> mControlContainers;
		Array<IControlContainerLoaderPtr> mControlContainerLoaders;

		bool mbShowCursor;
		CCursorPtr mCursor;
	};

	using CUIManagerPtr = SmartPtr<CUIManager>;

	template<typename T>
	inline EResult CUIManager::RegisterLoader(EControlContainerLoaderType pLoaderType, T ** pLoader)
	{
		EResult lResult = EResult::Success;

		for (const auto &lLoader : mControlContainerLoaders)
		{
			if (lLoader->GetLoaderType() == pLoaderType)
			{
				lResult = EResult::AddFailed;
				break;
			}
		}

		if (lResult == EResult::Success)
		{
			*pLoader = new T();
			mControlContainerLoaders.push_back(IControlContainerLoaderPtr(*pLoader));
		}

		return lResult;
	}
}
