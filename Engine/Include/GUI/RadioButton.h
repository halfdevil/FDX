#pragma once

#include "GUI/Control.h"
#include "GUI/Label.h"
#include "GUI/ImageControl.h"
#include "GUI/ControlContainer.h"

namespace FDX
{
	class FDX_EXPORT CRadioButton : public CControl
	{
	public:

		CRadioButton(const String &pstrName);
		virtual ~CRadioButton();

		const String& GetText() const
		{
			return mstrText;
		}
		
		bool IsChecked() const
		{
			return mbIsChecked;
		}

		virtual EResult Create(CControlContainer *pContainer);

		virtual void SetText(const String &pstrText);
		virtual void SetIsChecked(bool pbIsChecked);

		virtual bool IsMouseOverImage();

	protected:

		virtual void PreDraw(CCanvas *pCanvas) override;
		virtual void PostDraw(CCanvas *pCanvas) override;

		virtual EResult CreateControls(CControlContainer *pContainer);

	protected:

		CImageControl *mImage;
		String mstrText;
		bool mbIsChecked;
	};
}