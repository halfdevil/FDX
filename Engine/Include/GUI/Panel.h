#pragma once

#include "GUI/Control.h"

namespace FDX
{
	class FDX_EXPORT CPanel : public CControl
	{
	public:

		CPanel(const String &pstrName);
		virtual ~CPanel();

	protected:

		virtual void PreDraw(CCanvas *pCanvas) override;
		virtual void PostDraw(CCanvas *pCanvas) override;
	};

	using CPanelPtr = SmartPtr<CPanel>;
}