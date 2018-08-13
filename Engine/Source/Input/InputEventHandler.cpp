
#include "Input/InputEventHandler.h"
#include "Core/EventManager.h"
#include "Input/InputEvent.h"

namespace FDX
{
	CInputEventHandler::CInputEventHandler()
		: mbIsEnabled(true)
	{
	}

	CInputEventHandler::~CInputEventHandler()
	{
	}

	void CInputEventHandler::OnKeyDown(CKeyboard * pKeyboard, EKeyboardKey pKey)
	{
		if (!mbIsEnabled)
			return;

		CKeyboardEvent lKeyDown(EInputEventType::KeyDownEvent);
		lKeyDown.SetKey(pKey);

		CEventManager::Get().ProcessEventImmediate(&lKeyDown);
	}

	void CInputEventHandler::OnKeyUp(CKeyboard * pKeyboard, EKeyboardKey pKey)
	{
		if (!mbIsEnabled)
			return;

		CKeyboardEvent lKeyUp(EInputEventType::KeyUpEvent);
		lKeyUp.SetKey(pKey);

		CEventManager::Get().ProcessEventImmediate(&lKeyUp);
	}

	void CInputEventHandler::OnMouseDown(CMouse * pMouse, EMouseButton pButton)
	{
		if (!mbIsEnabled)
			return;

		CMouseEvent lMouseDown(EInputEventType::MouseDownEvent);
		lMouseDown.SetAbsolutePosition(pMouse->GetAbsolutePosition());
		lMouseDown.SetButton(pButton);

		CEventManager::Get().ProcessEventImmediate(&lMouseDown);
	}

	void CInputEventHandler::OnMouseUp(CMouse * pMouse, EMouseButton pButton)
	{
		if (!mbIsEnabled)
			return;

		CMouseEvent lMouseUp(EInputEventType::MouseUpEvent);
		lMouseUp.SetButton(pButton);

		CEventManager::Get().ProcessEventImmediate(&lMouseUp);
	}

	void CInputEventHandler::OnMouseMove(CMouse * pMouse, uint32_t puiX, uint32_t puiY)
	{
		if (!mbIsEnabled)
			return;

		CMouseEvent lMouseMove(EInputEventType::MouseMoveEvent);
		lMouseMove.SetAbsolutePosition(pMouse->GetAbsolutePosition());
		lMouseMove.SetRelativePosition(pMouse->GetRelativePosition());

		CEventManager::Get().ProcessEventImmediate(&lMouseMove);
	}

	void CInputEventHandler::SetIsEnabled(bool pbEnabled)
	{
		mbIsEnabled = pbEnabled;
	}
}