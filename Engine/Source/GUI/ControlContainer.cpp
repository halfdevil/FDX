
#include "GUI/ControlContainer.h"
#include "GUI/Dialog.h"

namespace FDX
{
	CControlContainer::CControlContainer(const String &pstrName)
		: mstrName(pstrName),
		mFocusedControl(nullptr),
		mLayoutController(nullptr)
	{
	}

	CControlContainer::~CControlContainer()
	{
	}

	CControl * CControlContainer::CreateLayout(EControlLayoutType pLayoutType,
		const String &pstrName)
	{
		mLayoutController = CLayoutFactory::Get().
			CreateLayout(pLayoutType, pstrName);

		return mLayoutController.get();
	}

	void CControlContainer::AddControl(CControlPtr pControl, CControl *pParent)
	{
		if (pParent != nullptr)
			pParent->AddChild(pControl.get());
		else
			mLayoutController->AddChild(pControl.get());

		mControls.push_back(std::move(pControl));
	}

	CControlPtr CControlContainer::RemoveControl(CControl * pControl)
	{
		auto lIt = std::find_if(
			mControls.begin(),
			mControls.end(),
			[pControl](const CControlPtr &pV) {
				return pV.get() == pControl;
			}
		);

		CControlPtr lControl = std::move(*lIt);
		mControls.erase(lIt);

		return lControl;
	}

	void CControlContainer::Update(float pfDelta)
	{
		for (const auto &lControl : mControls)
			lControl->Update(pfDelta);
	}

	void CControlContainer::Draw(CCanvas * pCanvas)
	{
		mLayoutController->Draw(pCanvas);
	}

	bool CControlContainer::ProcessEvent(IEvent * pEvent)
	{
		EInputEventType lEventType = (EInputEventType)pEvent->GetEventType();

		switch (lEventType)
		{
		case EInputEventType::KeyDownEvent:
			return OnKeyDown(static_cast<CKeyboardEvent*>(pEvent));
			
		case EInputEventType::KeyUpEvent:
			return OnKeyUp(static_cast<CKeyboardEvent*>(pEvent));

		case EInputEventType::MouseMoveEvent:
			return OnMouseMove(static_cast<CMouseEvent*>(pEvent));

		case EInputEventType::MouseDownEvent:
			return OnMouseDown(static_cast<CMouseEvent*>(pEvent));

		case EInputEventType::MouseUpEvent:
			return OnMouseUp(static_cast<CMouseEvent*>(pEvent));
		}

		return false;
	}

	void CControlContainer::LayoutControls()
	{
		mLayoutController->LayoutChildControls();
		mLayoutController->UpdateScreenPosition(mLayoutController->GetX(),
			mLayoutController->GetY());
	}

	bool CControlContainer::OnMouseDown(CMouseEvent * pEvent)
	{
		bool lbRet = false;

		CControl *lOldFocusedControl = mFocusedControl;

		for (const auto &lControl : mLayoutController->GetChildControls())
		{
			lbRet = OnMouseDown(pEvent, lControl);

			if (lbRet)
				break;
		}

		if (!lbRet)
			mFocusedControl = nullptr;

		if (mFocusedControl != lOldFocusedControl)
		{
			CLostFocusControlEvent lLostFocusEvent;

			if (lOldFocusedControl)
				lOldFocusedControl->HandleEvent(&lLostFocusEvent);
		}

		return lbRet;
	}

	bool CControlContainer::OnMouseDown(CMouseEvent * pEvent, CControl *pControl)
	{
		bool lbRet = false;

		if (pControl->IsVisble() && pControl->IsMouseOver())
		{
			if (pControl != mFocusedControl)
			{
				CGotFocusControlEvent lGotFocusEvent;

				if (pControl->HandleEvent(&lGotFocusEvent))
					mFocusedControl = pControl;
			}
			else
			{
				mFocusedControl = pControl;
			}

			for (const auto &lControl : pControl->GetChildControls())
			{
				lbRet = OnMouseDown(pEvent, lControl);

				if (lbRet)
					break;
			}

			if (!lbRet)
			{
				CMouseDownControlEvent lMouseDownEvent(pEvent);
				if (pControl->HandleEvent(&lMouseDownEvent))
				{
					lbRet = true;
				}
			}
		}

		return lbRet;
	}

	bool CControlContainer::OnMouseUp(CMouseEvent * pEvent)
	{
		bool lbRet = false;

		if (mFocusedControl)
		{
			CMouseUpControlEvent lMouseUpEvent(pEvent);
			lbRet = mFocusedControl->HandleEvent(&lMouseUpEvent);
		}

		if (!lbRet)
		{
			for (const auto &lControl : mLayoutController->GetChildControls())
			{
				lbRet = OnMouseUp(pEvent, lControl);

				if (lbRet)
					break;
			}
		}

		return lbRet;
	}

	bool CControlContainer::OnMouseUp(CMouseEvent * pEvent, CControl *pControl)
	{
		bool lbRet = false;

		if (pControl->IsVisble())
		{
			if (pControl != mFocusedControl && pControl->IsMouseOver())
			{
				for (const auto &lControl : pControl->GetChildControls())
				{
					lbRet = OnMouseUp(pEvent, lControl);

					if (lbRet)
						break;
				}

				if (!lbRet)
				{
					CMouseUpControlEvent lMouseUpEvent(pEvent);

					if (pControl->HandleEvent(&lMouseUpEvent))					
						lbRet = true;
				}
			}
		}		

		return lbRet;
	}

	bool CControlContainer::OnMouseMove(CMouseEvent * pEvent)
	{
		bool lbRet = false;

		for (const auto &lControl : mLayoutController->GetChildControls())
		{		
			OnMouseLeave(pEvent, lControl, lbRet);

			if (!lbRet)
			{
				if (OnMouseEnter(pEvent, lControl))
					lbRet = true;

				if (OnMouseMove(pEvent, lControl))
					lbRet = true;
			}
		}

		return lbRet;
	}

	bool CControlContainer::OnMouseMove(CMouseEvent * pEvent, CControl *pControl)
	{
		bool lbRet = false;
		CVector2F lMousePosition = pEvent->GetAbsolutePosition();

		if (pControl->IsVisble())
		{
			if (pControl->IsMouseOver())
			{
				CMouseMoveControlEvent lMouseMoveEvent(pEvent);
				if (pControl->HandleEvent(&lMouseMoveEvent))
					lbRet = true;
			}

			for (const auto &lControl : pControl->GetChildControls())
			{
				if (OnMouseMove(pEvent, lControl))
					lbRet = true;
			}
		}
				
		return lbRet;
	}

	bool CControlContainer::OnMouseEnter(CMouseEvent * pEvent, CControl * pControl)
	{
		bool lbRet = false;
		CVector2F lMousePosition = pEvent->GetAbsolutePosition();

		if (pControl->IsVisble())
		{
			if (pControl->IsPointOver((uint32_t)lMousePosition.X, (uint32_t)lMousePosition.Y))
			{
				if (!pControl->IsMouseOver())
				{
					CMouseEnterControlEvent lMouseEnterEvent(pEvent);
					pControl->SetIsMouseOver(true);

					if (pControl->HandleEvent(&lMouseEnterEvent))
						lbRet = true;
				}

				for (const auto &lControl : pControl->GetChildControls())
				{
					if (OnMouseEnter(pEvent, lControl))
						lbRet = true;
				}
			}
		}

		return lbRet;
	}

	bool CControlContainer::OnMouseLeave(CMouseEvent * pEvent, CControl * pControl, bool pbLeaveAll)
	{
		bool lbRet = false;
		CVector2F lMousePosition = pEvent->GetAbsolutePosition();

		if (pControl->IsVisble() && pControl->IsMouseOver())
		{
			if (pbLeaveAll)
			{
				CMouseLeaveControlEvent lMouseLeaveEvent(pEvent);
				pControl->SetIsMouseOver(false);

				if (pControl->HandleEvent(&lMouseLeaveEvent))
					lbRet = true;
			}
			else if (!pControl->IsPointOver((uint32_t)lMousePosition.X, (uint32_t)lMousePosition.Y))
			{
				CMouseMoveControlEvent lMouseMoveEvent(pEvent);
				pControl->HandleEvent(&lMouseMoveEvent);

				if (!pControl->IsPointOver((uint32_t)lMousePosition.X, (uint32_t)lMousePosition.Y))
				{
					CMouseLeaveControlEvent lMouseLeaveEvent(pEvent);
					pControl->SetIsMouseOver(false);

					if (pControl->HandleEvent(&lMouseLeaveEvent))
						lbRet = true;
				}
			}
		}

		for (const auto &lControl : pControl->GetChildControls())
		{
			if (OnMouseLeave(pEvent, lControl, pbLeaveAll))
				lbRet = true;
		}

		return lbRet;
	}

	bool CControlContainer::OnKeyDown(CKeyboardEvent * pEvent)
	{
		bool lbRet = false;

		if (mFocusedControl)
		{
			CKeyDownControlEvent lKeyDownEvent(pEvent);
			lbRet = mFocusedControl->HandleEvent(&lKeyDownEvent);
		}

		if (!lbRet)
		{
			for (const auto &lControl : mLayoutController->GetChildControls())
			{
				lbRet = OnKeyDown(pEvent, lControl);

				if (lbRet)
					break;
			}
		}

		return lbRet;
	}

	bool CControlContainer::OnKeyDown(CKeyboardEvent * pEvent, CControl *pControl)
	{
		bool lbRet = false;

		if (pControl->IsVisble())
		{
			if (pControl != mFocusedControl && pControl->IsMouseOver())
			{
				for (const auto &lControl : pControl->GetChildControls())
				{
					lbRet = OnKeyDown(pEvent, lControl);
					if (lbRet)
						break;
				}

				if (!lbRet)
				{
					CKeyDownControlEvent lKeyDownEvent(pEvent);
					if (pControl->HandleEvent(&lKeyDownEvent))
						lbRet = true;
				}
			}
		}

		return lbRet;
	}

	bool CControlContainer::OnKeyUp(CKeyboardEvent * pEvent)
	{
		bool lbRet = false;

		if (mFocusedControl)
		{
			CKeyUpControlEvent lKeyUpEvent(pEvent);
			lbRet = mFocusedControl->HandleEvent(&lKeyUpEvent);
		}

		if (!lbRet)
		{
			for (const auto &lControl : mLayoutController->GetChildControls())
			{
				lbRet = OnKeyUp(pEvent, lControl);

				if (lbRet)
					break;
			}
		}

		return lbRet;
	}

	bool CControlContainer::OnKeyUp(CKeyboardEvent * pEvent, CControl *pControl)
	{
		bool lbRet = false;

		if (pControl->IsVisble())
		{
			if (pControl != mFocusedControl && pControl->IsMouseOver())
			{
				for (const auto &lControl : pControl->GetChildControls())
				{
					lbRet = OnKeyUp(pEvent, lControl);
					if (lbRet)
						break;
				}

				if (!lbRet)
				{
					CKeyUpControlEvent lKeyUpEvent(pEvent);
					if (pControl->HandleEvent(&lKeyUpEvent))
						lbRet = true;
				}
			}
		}

		return lbRet;
	}
}