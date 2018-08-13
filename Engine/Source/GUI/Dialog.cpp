
#include "GUI/Dialog.h"
#include "GUI/ControlSkin.h"
#include "GUI/UITheme.h"
#include "GUI/ControlContainer.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CDialogTitleBar::CDialogTitleBar(const String & pstrName, CDialog * pDialog)
		: CControl(pstrName),
		mDialog(pDialog)
	{
	}

	CDialogTitleBar::~CDialogTitleBar()
	{
	}

	EResult CDialogTitleBar::Create(CControlContainer * pContainer)
	{
		EResult lResult = EResult::Success;

		lResult = OnInitialize(pContainer);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CDialogTitleBar::OnInitialize() failed");
		}
		else
		{
			CControlSkinPtr lSkin = std::make_unique<CControlSkin>(mstrName);
			lSkin->AddMaterial(IUITheme::Get().GetDialogTitleBarMaterial(EControlMaterialType::Default));
			lSkin->SetActiveMaterial(EControlMaterialType::Default);

			SetSkin(std::move(lSkin));

			lResult = CreateControls(pContainer);
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("CDialogTitleBar::CreateControls() failed");
			}
		}

		return lResult;
	}

	void CDialogTitleBar::SetStyle(uint32_t pStyle)
	{
		mStyle = pStyle;
	}

	bool CDialogTitleBar::OnMouseDown(CMouseDownControlEvent * pEvent)
	{
		return false;
	}

	bool CDialogTitleBar::OnMouseUp(CMouseUpControlEvent * pEvent)
	{
		return false;
	}

	bool CDialogTitleBar::OnMouseMove(CMouseMoveControlEvent * pEvent)
	{
		return false;
	}

	EResult CDialogTitleBar::OnInitialize(CControlContainer * pContainer)
	{
		EResult lResult = EResult::Success;

		SetAlignV(EAlignVertical::Top);
		SetFillMode(EControlFillMode::Horizontal);
		SetSize(0, IUITheme::Get().GetDialogTitleBarHeight());

		return lResult;
	}

	EResult CDialogTitleBar::CreateControls(CControlContainer * pContainer)
	{
		EResult lResult = EResult::Success;

		if (mStyle & EDialogTitleBarStyle::Logo)
		{
			mLeftControl = new CControl(mstrName + "_left");
			mLeftControl->SetAlignH(EAlignHorizontal::Left);
			mLeftControl->SetFillMode(EControlFillMode::Vertical);
			mLeftControl->SetWidth(2 * IUITheme::Get().GetDialogTitleBarLogoMargin() +
				IUITheme::Get().GetDialogTitleBarLogoWidth());

			mLogo = new CImageControl(mstrName + "_logo");
			mLogo->SetWidth(IUITheme::Get().GetDialogTitleBarLogoWidth());
			mLogo->SetHeight(IUITheme::Get().GetDialogTitleBarLogoHeight());
			mLogo->SetAlignH(EAlignHorizontal::Center);
			mLogo->SetAlignV(EAlignVertical::Center);

			CControlSkinPtr lSkin = std::make_unique<CControlSkin>(mstrName);
			lSkin->AddMaterial(IUITheme::Get().GetDialogTitleBarLogoMaterial(EControlMaterialType::Default));
			lSkin->SetActiveMaterial(EControlMaterialType::Default);
			mLogo->SetSkin(std::move(lSkin));

			pContainer->AddControl(CControlPtr(mLeftControl), this);
			pContainer->AddControl(CControlPtr(mLogo), mLeftControl);
		}

		uint32_t luiRightControlWidth = 0;
		
		if (mStyle & EDialogTitleBarStyle::MinimizeBox)
		{			
			mMinimizeBox = new CButton(mstrName + "_minimize");
			mMinimizeBox->SetAlignV(EAlignVertical::Center);
			mMinimizeBox->SetWidth(IUITheme::Get().GetDialogTitleBarMinimizeBoxWidth());
			mMinimizeBox->SetHeight(IUITheme::Get().GetDialogTitleBarMinimizeBoxHeight());
			mMinimizeBox->SetX(luiRightControlWidth + IUITheme::Get().
				GetDialogTitleBarMinimizeBoxMargin());

			luiRightControlWidth += 2 * IUITheme::Get().GetDialogTitleBarMinimizeBoxMargin() +
				IUITheme::Get().GetDialogTitleBarMinimizeBoxWidth();		

			CControlMaterialPtr lDefaultMaterial = IUITheme::Get().
				GetDialogTitleBarMinimizeBoxMaterial(EControlMaterialType::Default);
			CControlMaterialPtr lHoverMaterial = IUITheme::Get().
				GetDialogTitleBarMinimizeBoxMaterial(EControlMaterialType::Hover);
			CControlMaterialPtr lPressedMaterial = IUITheme::Get().
				GetDialogTitleBarMinimizeBoxMaterial(EControlMaterialType::Pressed);

			CControlSkinPtr lSkin = std::make_unique<CControlSkin>(mstrName);
			lSkin->AddMaterial(std::move(lDefaultMaterial));
			lSkin->AddMaterial(std::move(lHoverMaterial));
			lSkin->AddMaterial(std::move(lPressedMaterial));
			lSkin->SetActiveMaterial(EControlMaterialType::Default);
			mMinimizeBox->SetSkin(std::move(lSkin));
		}

		if (mStyle & EDialogTitleBarStyle::MaximizeBox)
		{
			mMaximizeBox = new CButton(mstrName + "_maximize");
			mMaximizeBox->SetAlignV(EAlignVertical::Center);
			mMaximizeBox->SetWidth(IUITheme::Get().GetDialogTitleBarMaximizeBoxWidth());
			mMaximizeBox->SetHeight(IUITheme::Get().GetDialogTitleBarMaximizeBoxHeight());
			mMaximizeBox->SetX(luiRightControlWidth + IUITheme::Get().
				GetDialogTitleBarMaximizeBoxMargin());

			luiRightControlWidth += 2 * IUITheme::Get().GetDialogTitleBarMaximizeBoxMargin() +
				IUITheme::Get().GetDialogTitleBarMaximizeBoxWidth();

			CControlMaterialPtr lDefaultMaterial = IUITheme::Get().
				GetDialogTitleBarMaximizeBoxMaterial(EControlMaterialType::Default);
			CControlMaterialPtr lHoverMaterial = IUITheme::Get().
				GetDialogTitleBarMaximizeBoxMaterial(EControlMaterialType::Hover);
			CControlMaterialPtr lPressedMaterial = IUITheme::Get().
				GetDialogTitleBarMaximizeBoxMaterial(EControlMaterialType::Pressed);
			
			CControlSkinPtr lSkin = std::make_unique<CControlSkin>(mstrName);
			lSkin->AddMaterial(std::move(lDefaultMaterial));
			lSkin->AddMaterial(std::move(lHoverMaterial));
			lSkin->AddMaterial(std::move(lPressedMaterial));
			lSkin->SetActiveMaterial(EControlMaterialType::Default);
			mMaximizeBox->SetSkin(std::move(lSkin));
		}

		if (mStyle & EDialogTitleBarStyle::CloseBox)
		{
			mCloseBox = new CButton(mstrName + "_close");
			mCloseBox->SetAlignV(EAlignVertical::Center);
			mCloseBox->SetWidth(IUITheme::Get().GetDialogTitleBarCloseBoxWidth());
			mCloseBox->SetHeight(IUITheme::Get().GetDialogTitleBarCloseBoxHeight());
			mCloseBox->SetX(luiRightControlWidth + IUITheme::Get().
				GetDialogTitleBarCloseBoxMargin());

			luiRightControlWidth += 2 * IUITheme::Get().GetDialogTitleBarCloseBoxMargin() +
				IUITheme::Get().GetDialogTitleBarCloseBoxWidth();

			CControlMaterialPtr lDefaultMaterial = IUITheme::Get().
				GetDialogTitleBarCloseBoxMaterial(EControlMaterialType::Default);
			CControlMaterialPtr lHoverMaterial = IUITheme::Get().
				GetDialogTitleBarCloseBoxMaterial(EControlMaterialType::Hover);
			CControlMaterialPtr lPressedMaterial = IUITheme::Get().
				GetDialogTitleBarCloseBoxMaterial(EControlMaterialType::Pressed);
			
			CControlSkinPtr lSkin = std::make_unique<CControlSkin>(mstrName);
			lSkin->AddMaterial(std::move(lDefaultMaterial));
			lSkin->AddMaterial(std::move(lHoverMaterial));
			lSkin->AddMaterial(std::move(lPressedMaterial));
			lSkin->SetActiveMaterial(EControlMaterialType::Default);
			mCloseBox->SetSkin(std::move(lSkin));
		}

		if (luiRightControlWidth > 0)
		{
			mRightControl = new CControl(mstrName + "_right");
			mRightControl->SetAlignH(EAlignHorizontal::Right);
			mRightControl->SetFillMode(EControlFillMode::Vertical);
			mRightControl->SetWidth(luiRightControlWidth);

			if (mStyle & EDialogTitleBarStyle::MinimizeBox)
				pContainer->AddControl(CControlPtr(mMinimizeBox), mRightControl);

			if (mStyle & EDialogTitleBarStyle::MaximizeBox)
				pContainer->AddControl(CControlPtr(mMaximizeBox), mRightControl);

			if (mStyle & EDialogTitleBarStyle::CloseBox)
				pContainer->AddControl(CControlPtr(mCloseBox), mRightControl);

			pContainer->AddControl(CControlPtr(mRightControl), this);
		}

		return lResult;
	}

	CDialog::CDialog(const String & pstrName)
		: CControl(pstrName),
		mbIsDragging(false),
		muiTitleBarStyle(IUITheme::Get().GetDialogTitleBarStyle())
	{
	}

	CDialog::~CDialog()
	{
	}

	void CDialog::SetTitleBarStyle(uint32_t puiTitleBarStyle)
	{
		muiTitleBarStyle = puiTitleBarStyle;
	}

	EResult CDialog::Create(CControlContainer * pContainer)
	{
		EResult lResult = EResult::Success;

		lResult = OnInitialize(pContainer);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CDialogTitleBar::OnInitialize() failed");
		}
		else
		{
			CControlMaterialPtr lDefaultMaterial = IUITheme::Get().
				GetDialogMaterial(EControlMaterialType::Default);

			CControlSkinPtr lSkin = std::make_unique<CControlSkin>(mstrName);
			lSkin->AddMaterial(std::move(lDefaultMaterial));
			lSkin->SetActiveMaterial(EControlMaterialType::Default);

			SetSkin(std::move(lSkin));

			lResult = CreateControls(pContainer);
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("CDialog::CreateControls() failed");
			}
		}

		return lResult;
	}

	EResult CDialog::OnInitialize(CControlContainer * pContainer)
	{
		EResult lResult = EResult::Success;		
		return lResult;
	}

	EResult CDialog::CreateControls(CControlContainer * pContainer)
	{
		EResult lResult = EResult::Success;

		mTitleBar = new CDialogTitleBar(mstrName + "_titlebar", this);
		mTitleBar->SetStyle(muiTitleBarStyle);
		mTitleBar->Create(pContainer);

		mContent = new CPanel(mstrName + "_content");
		mContent->SetAlignV(EAlignVertical::Bottom);
		mContent->SetFillMode(EControlFillMode::Horizontal);
		mContent->SetHeight(GetHeight() - mTitleBar->GetHeight());
		mContent->SetY(mTitleBar->GetHeight());

		CControlMaterialPtr lDefaultMaterial = IUITheme::Get().
			GetDialogContentMaterial(EControlMaterialType::Default);
		
		CControlSkinPtr lSkin = std::make_unique<CControlSkin>(mstrName);
		lSkin->AddMaterial(std::move(lDefaultMaterial));
		lSkin->SetActiveMaterial(EControlMaterialType::Default);
		mContent->SetSkin(std::move(lSkin));

		pContainer->AddControl(CControlPtr(mTitleBar), this);
		pContainer->AddControl(CControlPtr(mContent), this);

		return lResult;
	}

	bool CDialog::OnGotFocus(CGotFocusControlEvent * pEvent)
	{
		BringToFront();
		return true;
	}

	bool CDialog::OnMouseDown(CMouseDownControlEvent * pEvent)
	{
		if (mTitleBar->IsMouseOver())
		{
			if (!mTitleBar->GetLeftControl()->IsMouseOver() &&
				!mTitleBar->GetRightControl()->IsMouseOver())
			{
				mDragStart = pEvent->GetAbsolutePosition();
				mbIsDragging = true;
			}
		}

		return true;
	}

	bool CDialog::OnMouseUp(CMouseUpControlEvent * pEvent)
	{
		mbIsDragging = false;
		return true;
	}

	bool CDialog::OnMouseMove(CMouseMoveControlEvent * pEvent)
	{
		if (mbIsDragging)
		{
			CVector2F lDiff = mDragStart - pEvent->GetAbsolutePosition();
			mDragStart = pEvent->GetAbsolutePosition();

			SetX(GetX() + (int32_t)-lDiff.X);
			SetY(GetY() + (int32_t)-lDiff.Y);

			if (mParent != nullptr)
				UpdateScreenPosition(mParent->GetScreenX(), mParent->GetScreenY());
			else
				UpdateScreenPosition(0, 0);
		}

		return true;
	}

	bool CDialog::OnMouseLeave(CMouseLeaveControlEvent * pEvent)
	{
		return true;
	}
}