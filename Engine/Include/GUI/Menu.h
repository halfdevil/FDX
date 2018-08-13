#pragma once

#include "GUI/Control.h"
#include "GUI/Button.h"

namespace FDX
{
	class FDX_EXPORT CMenuItem : public CButton
	{
	public:

		CMenuItem(const String &pstrName);
		virtual ~CMenuItem();

		virtual void UpdatePosition(uint32_t puiX, uint32_t puiY);

		virtual void AddMenuItem(CMenuItem *pMenuItem);
		virtual void RemoveMenuItem(CMenuItem *pMenuItem);

		virtual void Close();
		virtual void Open();

	protected:
		
		virtual void PreDraw(CCanvas *pCanvas) override;
		virtual void PostDraw(CCanvas *pCanvas) override;

		virtual bool OnLostFocus(CLostFocusControlEvent *pEvent) override;
		virtual bool OnMouseEnter(CMouseEnterControlEvent *pEvent) override;
		virtual bool OnMouseLeave(CMouseLeaveControlEvent *pEvent) override;
		virtual bool OnMouseDown(CMouseDownControlEvent *pEvent) override;
		virtual bool OnMouseUp(CMouseUpControlEvent *pEvent) override;

	protected:

		bool mbIsMenuOpen;
		Array<CMenuItem*> mMenuItems;
	};

	class FDX_EXPORT CMenu : public CMenuItem
	{
	public:

		CMenu(const String &pstrName);
		virtual ~CMenu();

		virtual void LayoutChildControls() override;

		virtual void PreDraw(CCanvas *pCanvas) override;
		virtual void PostDraw(CCanvas *pCanvas) override;

		virtual bool OnMouseEnter(CMouseEnterControlEvent *pEvent) override;
		virtual bool OnMouseLeave(CMouseLeaveControlEvent *pEvent) override;
		virtual bool OnMouseUp(CMouseUpControlEvent *pEvent) override;
	};
}