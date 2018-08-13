#pragma once

#include "GUI/Control.h"

namespace FDX
{
	class FDX_EXPORT CLabel : public CControl
	{
	public:

		CLabel(const String &pstrName);
		virtual ~CLabel();

		const String& GetText() const
		{
			return mstrText;
		}

		virtual void SetText(const String &pstrText);

	protected:

		virtual void PreDraw(CCanvas *pCanvas) override;
		virtual void PostDraw(CCanvas *pCanvas) override;

	protected:

		String mstrText;
	};
}