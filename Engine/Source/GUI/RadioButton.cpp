
#include "GUI/RadioButton.h"
#include "GUI/UITheme.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CRadioButton::CRadioButton(const String & pstrName)
		: CControl(pstrName),
		mbIsChecked(false)
	{
	}

	CRadioButton::~CRadioButton()
	{
	}

	EResult CRadioButton::Create(CControlContainer * pContainer)
	{
		EResult lResult = EResult::Success;

		lResult = CreateControls(pContainer);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CRadioButton::CreateControls() failed");
		}
		else
		{
			CControlSkin *lSkin = new CControlSkin(mstrName + "_skin");
			lSkin->AddMaterial(IUITheme::Get().GetRadioButtonMaterial(EControlMaterialType::Default));
			lSkin->AddMaterial(IUITheme::Get().GetRadioButtonMaterial(EControlMaterialType::Disabled));
			lSkin->SetActiveMaterial(EControlMaterialType::Default);

			SetSkin(CControlSkinPtr(lSkin));
		}

		return lResult;
	}

	void CRadioButton::SetText(const String & pstrText)
	{
		mstrText = pstrText;
	}

	void CRadioButton::SetIsChecked(bool pbIsChecked)
	{
		mbIsChecked = pbIsChecked;
	}

	bool CRadioButton::IsMouseOverImage()
	{
		if (mImage != nullptr)
			return mImage->IsMouseOver();

		return false;
	}

	void CRadioButton::PreDraw(CCanvas * pCanvas)
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
				float lfOffset = 2.0f * IUITheme::Get().GetRadioButtonImageMargin() +
					IUITheme::Get().GetRadioButtonImageWidth(); 

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

	void CRadioButton::PostDraw(CCanvas * pCanvas)
	{
		CControl::PostDraw(pCanvas);
	}

	EResult CRadioButton::CreateControls(CControlContainer * pContainer)
	{
		EResult lResult = EResult::Success;

		mImage = new CImageControl(mstrName + "_image");
		mImage->SetAlignV(EAlignVertical::Center);
		mImage->SetPosition(IUITheme::Get().GetRadioButtonImageMargin(), 0);
		mImage->SetSize(IUITheme::Get().GetRadioButtonImageWidth(),
			IUITheme::Get().GetRadioButtonImageHeight());

		CControlSkin *lSkin = new CControlSkin(mImage->GetName() + "_skin");
		lSkin->AddMaterial(IUITheme::Get().GetRadioButtonImageMaterial(EControlMaterialType::Default));
		lSkin->AddMaterial(IUITheme::Get().GetRadioButtonImageMaterial(EControlMaterialType::Disabled));
		lSkin->AddMaterial(IUITheme::Get().GetRadioButtonImageMaterial(EControlMaterialType::Pressed));
		lSkin->SetActiveMaterial(EControlMaterialType::Default);

		mImage->SetSkin(CControlSkinPtr(lSkin));
		pContainer->AddControl(CControlPtr(mImage), this);

		return lResult;
	}
}