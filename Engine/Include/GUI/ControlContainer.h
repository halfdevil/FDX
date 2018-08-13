#pragma once

#include "GUI/Control.h"
#include "GUI/LayoutController.h"

namespace FDX
{
	class FDX_EXPORT CControlContainer 
	{
	public:

		CControlContainer(const String &pstrName);
		virtual ~CControlContainer();

		CControlContainer(const CControlContainer&) = delete;
		CControlContainer& operator=(const CControlContainer&) = delete;

		const String& GetName() const
		{
			return mstrName;
		}

		CControl* GetLayoutController() const
		{
			return mLayoutController.get();
		}

		CControl* GetControl(uint32_t puiIdx)
		{
			if (puiIdx < mControls.size())
				return mControls[puiIdx].get();

			return nullptr;
		}

		CControl* GetControl(const String &pstrName)
		{
			for (const auto &lControl : mControls)
			{
				if (lControl->GetName() == pstrName)
					return lControl.get();
			}

			return nullptr;
		}

		template <typename T>
		T* GetControlAs(uint32_t puiIdx)
		{
			if (puiIdx < mControls.size())
				return static_cast<T*>(mControls[puiIdx].get());

			return nullptr;
		}

		template <typename T>
		T* GetControlAs(const String &pstrName)
		{
			for (const auto &lControl : mControls)
			{
				if (lControl->GetName() == pstrName)
					return static_cast<T*>(lControl.get());
			}

			return nullptr;
		}

		CControl* CreateLayout(EControlLayoutType pLayoutType, 
			const String &pstrName);

		virtual void AddControl(CControlPtr pControl, CControl *pParent);
		virtual CControlPtr RemoveControl(CControl *pControl);

		virtual void Update(float pfDelta);
		virtual void Draw(CCanvas *pCanvas);
				
		virtual bool ProcessEvent(IEvent *pEvent);
		virtual void LayoutControls();

	protected:

		virtual bool OnMouseDown(CMouseEvent *pEvent);
		virtual bool OnMouseUp(CMouseEvent *pEvent);
		virtual bool OnMouseMove(CMouseEvent *pEvent);
		virtual bool OnKeyDown(CKeyboardEvent *pEvent);
		virtual bool OnKeyUp(CKeyboardEvent *pEvent);

		virtual bool OnMouseDown(CMouseEvent *pEvent, CControl *pControl);
		virtual bool OnMouseUp(CMouseEvent *pEvent, CControl *pControl);
		virtual bool OnMouseMove(CMouseEvent *pEvent, CControl *pControl);
		virtual bool OnMouseEnter(CMouseEvent *pEvent, CControl *pControl);
		virtual bool OnMouseLeave(CMouseEvent *pEvent, CControl *pControl, bool pbLeaveAll);
		virtual bool OnKeyDown(CKeyboardEvent *pEvent, CControl *pControl);
		virtual bool OnKeyUp(CKeyboardEvent *pEvent, CControl *pControl);

	protected:

		String mstrName;
		Array<CControlPtr> mControls;
		CControlPtr mLayoutController;
		CControl *mFocusedControl;
	};

	using CControlContainerPtr = SmartPtr<CControlContainer>;
}