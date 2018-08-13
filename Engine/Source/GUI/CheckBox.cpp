
#include "GUI/CheckBox.h"
#include "GUI/UITheme.h"
#include "GUI/ControlContainer.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CCheckBox::CCheckBox(const String & pstrName)
		: CControl(pstrName),
		mbIsChecked(false)
	{
	}

	CCheckBox::~CCheckBox()
	{
	}

	EResult CCheckBox::Create(CControlContainer * pContainer)
	{
		EResult lResult = EResult::Success;

		lResult = CreateControls(pContainer);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CCheckBox::CreateControls() failed");
		}
		else
		{
			CControlSkin *lSkin = new CControlSkin(mstrName + "_skin");
			lSkin->AddMaterial(IUITheme::Get().GetCheckBoxMaterial(EControlMaterialType::Default));
			lSkin->AddMaterial(IUITheme::Get().GetCheckBoxMaterial(EControlMaterialType::Disabled));
			lSkin->SetActiveMaterial(EControlMaterialType::Default);

			SetSkin(CControlSkinPtr(lSkin));
		}

		return lResult;
	}

	void CCheckBox::SetText(const String & pstrText)
	{
		mstrText = pstrText;
	}

	void CCheckBox::SetIsChecked(bool pbIsChecked)
	{
		mbIsChecked = pbIsChecked;
	}

	void CCheckBox::PreDraw(CCanvas * pCanvas)
	{
		CControl::PreDraw(pCanvas);

		if (mSkin != nullptr)
		{
			CControlMaterial *lMaterial = nullptr;

			if (!mbIsEnabled)
				lMaterial = mSkin->GetMaterial(EControlMaterialType::Disabled);
			else
				lMaterial = mSkin->GetActiveMaterial();

			if (lMaterial != nullptr)
			{
				float lfOffset = 2.0f * IUITheme::Get().GetCheckBoxImageMargin() +
					IUITheme::Get().GetCheckBoxImageWidth();

				float lfX = (float)miScreenX + lfOffset;
				float lfY = (float)miScreenY;
				float lfWidth = (float)muiWidth - lfOffset;
				float lfHeight = (float)muiHeight;

				if (!mstrText.empty())
				{
					CControlFont *lControlFont = lMaterial->GetFont();
					if (lControlFont != nullptr)
					{
						IFont *lFont = lControlFont->GetFont();

						SLineParameters lLineParams;
						lFont->CalculateLineParameters(mstrText, lControlFont->GetFontSize(), &lLineParams);

						float lfX1 = lfX + (lfWidth / 2 - lLineParams.Width / 2);
						float lfX2 = lfX + (lfWidth / 2 + lLineParams.Width / 2);
						float lfY1 = lfY + (lfHeight / 2 - (lLineParams.MinY + lLineParams.MaxY) / 2);
						float lfY2 = lfY + (lfHeight / 2 + (lLineParams.MinY + lLineParams.MaxY) / 2);

						pCanvas->DrawText(CVector2F(lfX1, lfY1), CVector2F(lfX2 - lfX1 + 1, lfY2 - lfY1 + 1),
							mstrText, lControlFont->GetFontSize(), lControlFont->GetFontColor(), lFont);
					}
				}
			}
		}

		if (mbIsChecked)
		{
			mImage->GetSkin()->SetActiveMaterial(EControlMaterialType::Pressed);
		}
		else
		{
			mImage->GetSkin()->SetActiveMaterial(EControlMaterialType::Default);
		}
	}

	void CCheckBox::PostDraw(CCanvas * pCanvas)
	{
		CControl::PostDraw(pCanvas);
	}

	bool CCheckBox::OnMouseUp(CMouseUpControlEvent * pEvent)
	{
		if (mImage != nullptr)
		{
			if (mImage->IsMouseOver())
				mbIsChecked = !mbIsChecked;
		}

		return true;
	}

	EResult CCheckBox::CreateControls(CControlContainer * pContainer)
	{
		EResult lResult = EResult::Success;

		mImage = new CImageControl(mstrName + "_image");
		mImage->SetAlignV(EAlignVertical::Center);
		mImage->SetPosition(IUITheme::Get().GetCheckBoxImageMargin(), 0);
		mImage->SetSize(IUITheme::Get().GetCheckBoxImageWidth(),
			IUITheme::Get().GetCheckBoxImageHeight());

		CControlSkin *lSkin = new CControlSkin(mImage->GetName() + "_skin");
		lSkin->AddMaterial(IUITheme::Get().GetCheckBoxImageMaterial(EControlMaterialType::Default));
		lSkin->AddMaterial(IUITheme::Get().GetCheckBoxImageMaterial(EControlMaterialType::Disabled));
		lSkin->AddMaterial(IUITheme::Get().GetCheckBoxImageMaterial(EControlMaterialType::Pressed));
		lSkin->SetActiveMaterial(EControlMaterialType::Default);

		mImage->SetSkin(CControlSkinPtr(lSkin));
		pContainer->AddControl(CControlPtr(mImage), this);

		return lResult;
	}
}