
#include "GUI/ControlEventHandler.h"
#include "GUI/Control.h"

namespace FDX
{
	CControlEventHandler::CControlEventHandler(CControl * pControl)
		: mControl(pControl),
		mbIsMouseOver(false),
		mbHasFocus(false)
	{
	}

	CControlEventHandler::~CControlEventHandler()
	{
	}

	bool CControlEventHandler::HandleEvent(CControlEvent * pEvent)
	{
		bool lbReturn = false;

		if (!mControl->IsEnabled())
			return lbReturn;

		switch (pEvent->GetControlEventType())
		{
		case EControlEventType::GotFocus:
			lbReturn = OnGotFocus(static_cast<CGotFocusControlEvent*>(pEvent));
			break;

		case EControlEventType::LostFocus:
			lbReturn = OnLostFocus(static_cast<CLostFocusControlEvent*>(pEvent));
			break;

		case EControlEventType::MouseEnter:
			lbReturn = OnMouseEnter(static_cast<CMouseEnterControlEvent*>(pEvent));
			break;

		case EControlEventType::MouseLeave:
			lbReturn = OnMouseLeave(static_cast<CMouseLeaveControlEvent*>(pEvent));
			break;

		case EControlEventType::MouseDown:
			lbReturn = OnMouseDown(static_cast<CMouseDownControlEvent*>(pEvent));
			break;

		case EControlEventType::MouseUp:
			lbReturn = OnMouseUp(static_cast<CMouseUpControlEvent*>(pEvent));
			break; 
		
		case EControlEventType::MouseMove:
			lbReturn = OnMouseMove(static_cast<CMouseMoveControlEvent*>(pEvent));
			break;

		case EControlEventType::KeyDown:
			lbReturn = OnKeyDown(static_cast<CKeyDownControlEvent*>(pEvent));
			break;

		case EControlEventType::KeyUp:
			lbReturn = OnKeyUp(static_cast<CKeyUpControlEvent*>(pEvent));
			
		default:
			break;
		}

		if (ExecuteCallback(pEvent))
			lbReturn = true;

		return lbReturn;
	}

	void CControlEventHandler::AddCallback(EControlEventType pType, CControlEventCallback pCallback)
	{
		mCallbacks[pType].push_back(pCallback);
	}

	void CControlEventHandler::SetIsMouseOver(bool pbIsOver)
	{
		mbIsMouseOver = pbIsOver;
	}

	void CControlEventHandler::SetHasFocus(bool pbFocus)
	{
		mbHasFocus = pbFocus;
	}

	bool CControlEventHandler::ExecuteCallback(CControlEvent * pEvent)
	{
		bool lbReturn = false;
		auto lIt = mCallbacks.find(pEvent->GetControlEventType());

		if (lIt != mCallbacks.end())
		{
			auto &lCallbacks = (*lIt).second;

			for (const auto &lCallback : lCallbacks)
			{
				if (lCallback(mControl, pEvent))
					lbReturn = true;
			}				
		}

		return lbReturn;
	}

	bool CControlEventHandler::OnGotFocus(CGotFocusControlEvent * pEvent)
	{
		return mControl->IsEnabled();
	}

	bool CControlEventHandler::OnLostFocus(CLostFocusControlEvent * pEvent)
	{
		return false;
	}

	bool CControlEventHandler::OnMouseEnter(CMouseEnterControlEvent * pEvent)
	{
		return false;
	}

	bool CControlEventHandler::OnMouseLeave(CMouseLeaveControlEvent * pEvent)
	{
		return false;
	}

	bool CControlEventHandler::OnMouseDown(CMouseDownControlEvent * pEvent)
	{
		return false;
	}

	bool CControlEventHandler::OnMouseUp(CMouseUpControlEvent * pEvent)
	{
		return false;
	}

	bool CControlEventHandler::OnMouseMove(CMouseMoveControlEvent * pEvent)
	{
		return false;
	}

	bool CControlEventHandler::OnKeyDown(CKeyDownControlEvent * pEvent)
	{
		return false;
	}

	bool CControlEventHandler::OnKeyUp(CKeyUpControlEvent * pEvent)
	{
		return false;
	}
}