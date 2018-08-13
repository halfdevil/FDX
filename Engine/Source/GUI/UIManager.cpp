
#include "GUI/UIManager.h"
#include "Core/EventManager.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CUIManager::CUIManager()
		: mActiveContainer(nullptr),
		mCursor(nullptr),
		mbShowCursor(false)
	{
		CEventManager::Get().RegisterHandler(this);
	}

	CUIManager::~CUIManager()
	{
	}

	CControlContainer * CUIManager::LoadContainer(const String & pstrFilePath)
	{
		CControlContainer *lContainer = nullptr;

		for (const auto &lLoader : mControlContainerLoaders)
		{
			if (lLoader->CanLoad(pstrFilePath))
			{	
				EResult lResult = lLoader->Load(pstrFilePath, &lContainer);
				if (lResult != EResult::Success)
				{
					LOG_DEBUG("CControlContainerLoader::Load() failed for %s", 
						pstrFilePath.c_str());
				}
				else
				{
					mControlContainers.push_back(CControlContainerPtr(lContainer));
				}

				break;
			}
		}

		return lContainer;
	}

	CControlContainer * CUIManager::CreateContainer(const String &pstrName)
	{
		mControlContainers.push_back(std::make_unique<CControlContainer>(pstrName));
		return mControlContainers[mControlContainers.size() - 1].get();
	}

	CCursor * CUIManager::CreateCursor(const String &pstrName)
	{
		mCursor = std::make_unique<CCursor>(pstrName);
		mbShowCursor = true;

		return mCursor.get();
	}

	bool CUIManager::CanHandleEvent(uint32_t puiEventType)
	{
		EInputEventType lEventType = (EInputEventType)puiEventType;

		return lEventType == EInputEventType::KeyDownEvent ||
			lEventType == EInputEventType::KeyUpEvent ||
			lEventType == EInputEventType::MouseDownEvent ||
			lEventType == EInputEventType::MouseMoveEvent ||
			lEventType == EInputEventType::MouseUpEvent;
	}

	bool CUIManager::HandleEvent(IEvent * pEvent)
	{
		EInputEventType lEventType = (EInputEventType)pEvent->GetEventType();
		
		if (lEventType == EInputEventType::MouseMoveEvent)
		{
			CMouseMoveControlEvent lMouseMoveEvent(static_cast<CMouseEvent*>(pEvent));

			if (mbShowCursor && mCursor != nullptr)
				mCursor->HandleEvent(&lMouseMoveEvent);
		}
		
		if (mActiveContainer != nullptr)
			return mActiveContainer->ProcessEvent(pEvent);
		
		return false;
	}

	void CUIManager::SetTheme(IUIThemePtr pTheme)
	{
		mTheme = std::move(pTheme);
	}

	void CUIManager::SetCursorType(ECursorType pCursorType)
	{
		if (mCursor != nullptr)
		{
			mCursor->SetCursorType(pCursorType);
		}
	}

	void CUIManager::SetActiveContainer(uint32_t puiIdx)
	{
		if (puiIdx < mControlContainers.size())
			mActiveContainer = mControlContainers[puiIdx].get();
	}

	void CUIManager::Update(float pfDelta)
	{
		if (mActiveContainer != nullptr)
			mActiveContainer->Update(pfDelta);

		if (mbShowCursor && mCursor != nullptr)
			mCursor->Update(pfDelta);
	}

	void CUIManager::Draw(CCanvas * pCanvas)
	{
		if (mActiveContainer != nullptr)
			mActiveContainer->Draw(pCanvas);

		if (mbShowCursor && mCursor != nullptr)
			mCursor->Draw(pCanvas);
	}

	void CUIManager::ShowCursor(bool pbShow)
	{
		mbShowCursor = pbShow;
	}
}