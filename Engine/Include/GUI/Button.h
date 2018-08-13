#pragma once

#include "GUI/Label.h"

namespace FDX
{
	class FDX_EXPORT CButton : public CLabel
	{
	public:

		CButton(const String &pstrName);
		virtual ~CButton();
		
	protected:

		virtual bool OnMouseEnter(CMouseEnterControlEvent *pEvent) override;
		virtual bool OnMouseLeave(CMouseLeaveControlEvent *pEvent) override;
		virtual bool OnMouseDown(CMouseDownControlEvent *pEvent) override;
		virtual bool OnMouseUp(CMouseUpControlEvent *pEvent) override;
	};

	using CUIButtonPtr = SmartPtr<CButton>;
}