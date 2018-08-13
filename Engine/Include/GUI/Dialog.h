#pragma once

#include "GUI/Control.h"
#include "GUI/Button.h"
#include "GUI/ImageControl.h"
#include "GUI/Panel.h"

namespace FDX
{
	class CDialog;

	enum EDialogTitleBarStyle : uint32_t
	{
		Logo = 0x1,
		MinimizeBox = 0x2,
		MaximizeBox = 0x4,
		CloseBox = 0x8
	};

	class FDX_EXPORT CDialogTitleBar : public CControl
	{
	public:

		CDialogTitleBar(const String &pstrName, CDialog *pDialog);
		virtual ~CDialogTitleBar();

		uint32_t GetStyle() const
		{
			return mStyle;
		}

		CDialog* GetDialog() const
		{
			return mDialog;
		}

		CControl* GetLeftControl() const
		{
			return mLeftControl;
		}

		CControl* GetRightControl() const
		{
			return mRightControl;
		}

		virtual EResult Create(CControlContainer *pContainer);
		virtual void SetStyle(uint32_t pStyle);

	protected:

		virtual bool OnMouseDown(CMouseDownControlEvent *pEvent) override;
		virtual bool OnMouseUp(CMouseUpControlEvent *pEvent) override;
		virtual bool OnMouseMove(CMouseMoveControlEvent *pEvent) override;

		virtual EResult OnInitialize(CControlContainer *pContainer);
		virtual EResult CreateControls(CControlContainer *pContainer);

	protected:

		uint32_t mStyle;
		CDialog *mDialog;
		CControl *mLeftControl;
		CControl *mRightControl;
		CImageControl *mLogo;
		CLabel *mCaption;
		CButton *mMinimizeBox;
		CButton *mMaximizeBox;
		CButton *mCloseBox;
	};

	class FDX_EXPORT CDialog : public CControl
	{
	public:

		CDialog(const String &pstrName);
		virtual ~CDialog();

		CDialogTitleBar* GetTitleBar() const
		{
			return mTitleBar;
		}

		bool IsDragging() const
		{
			return mbIsDragging;
		}

		uint32_t GetTitleBarStyle() const
		{
			return muiTitleBarStyle;
		}
		
		virtual EResult Create(CControlContainer *pContainer) override;
		virtual void SetTitleBarStyle(uint32_t puiTitleBarStyle);

	protected:

		virtual EResult OnInitialize(CControlContainer *pContainer);
		virtual EResult CreateControls(CControlContainer *pContainer);

		virtual bool OnGotFocus(CGotFocusControlEvent *pEvent) override;
		virtual bool OnMouseDown(CMouseDownControlEvent *pEvent) override;
		virtual bool OnMouseUp(CMouseUpControlEvent *pEvent) override;
		virtual bool OnMouseMove(CMouseMoveControlEvent *pEvent) override;
		virtual bool OnMouseLeave(CMouseLeaveControlEvent *pEvent) override;

	protected:

		CDialogTitleBar *mTitleBar;
		CPanel *mContent;
		bool mbIsDragging;
		CVector2F mDragStart;
		uint32_t muiTitleBarStyle;
	};
}