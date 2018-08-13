#pragma once

#include "GUI/Control.h"

namespace FDX
{
	class FDX_EXPORT CTextBox : public CControl
	{
	public:

		CTextBox(const String &pstrName);
		virtual ~CTextBox();

		const String& GetText() const
		{
			return mstrText;
		}

		virtual EResult Create(CControlContainer *pContainer);

	protected:

		virtual void PreDraw(CCanvas *pCanvas) override;
		virtual void PostDraw(CCanvas *pCanvas) override;

		virtual bool OnMouseUp(CMouseUpControlEvent *pEvent) override;
		virtual bool OnKeyDown(CKeyDownControlEvent *pEvent) override;
		virtual bool OnKeyUp(CKeyUpControlEvent *pEvent) override;

		virtual void RepositionStart();
		virtual void RepositionEnd();

	protected:

		String mstrText;
		uint32_t muiPosition;
		uint32_t muiStartPosition;
		uint32_t muiEndPosition;
		bool mbIsFilled;
		bool mbIsShiftDown;
		float mfDeltaTime;
	};
}