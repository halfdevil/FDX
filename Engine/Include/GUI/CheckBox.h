#pragma once

#include "GUI/ImageControl.h"
#include "GUI/Label.h"

namespace FDX
{
	class FDX_EXPORT CCheckBox : public CControl
	{
	public:

		CCheckBox(const String &pstrName);
		virtual ~CCheckBox();

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

	protected:

		virtual void PreDraw(CCanvas *pCanvas) override;
		virtual void PostDraw(CCanvas *pCanvas) override;
		virtual bool OnMouseUp(CMouseUpControlEvent *pEvent) override;

		virtual EResult CreateControls(CControlContainer *pContainer);

	protected:

		CImageControl * mImage;
		String mstrText;
		bool mbIsChecked;
	};
}