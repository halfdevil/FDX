#pragma once

#include "GUI/RadioButton.h"

namespace FDX
{
	class FDX_EXPORT CRadioButtonGroup : public CControl
	{
	public:

		CRadioButtonGroup(const String &pstrName);
		virtual ~CRadioButtonGroup();

		const Array<CRadioButton*>& GetButtons() const
		{
			return mButtons;
		}

		virtual EResult Create(CControlContainer *pContainer) override;
		virtual void AddButton(CRadioButton *pButton);

	protected:

		virtual bool OnMouseUp(CMouseUpControlEvent *pEvent) override;
		
	protected:

		Array<CRadioButton*> mButtons;
	};
}