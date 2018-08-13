#pragma once

#include "Input/Keyboard.h"
#include "Input/Mouse.h"

namespace FDX
{
	class FDX_EXPORT CInputEventHandler : public CKeyboardCallback, public CMouseCallback
	{
	public:

		CInputEventHandler();
		~CInputEventHandler();

		bool IsEnabled() const
		{
			return mbIsEnabled;
		}

		void OnKeyDown(CKeyboard *pKeyboard, EKeyboardKey pKey) override;
		void OnKeyUp(CKeyboard *pKeyboard, EKeyboardKey pKey) override;

		void OnMouseDown(CMouse *pMouse, EMouseButton pButton) override;
		void OnMouseUp(CMouse *pMouse, EMouseButton pButton) override;
		void OnMouseMove(CMouse *pMouse, uint32_t puiX, uint32_t puiY) override;

		void SetIsEnabled(bool pbEnabled);

	private:

		bool mbIsEnabled;
	};

	using CInputEventHandlerPtr = SmartPtr<CInputEventHandler>;
}