
#include "GUI/TextBox.h"
#include "GUI/UITheme.h"
#include "OS/GameTimer.h"

namespace FDX
{
	CTextBox::CTextBox(const String & pstrName)
		: CControl(pstrName),
		muiPosition(0),
		muiStartPosition(0),
		muiEndPosition(0),
		mbIsFilled(false),
		mfDeltaTime(0.0f),
		mbIsShiftDown(false)
	{
	}

	CTextBox::~CTextBox()
	{
	}

	EResult CTextBox::Create(CControlContainer * pContainer)
	{
		EResult lResult = EResult::Success;

		CControlSkin *lSkin = new CControlSkin(mstrName + "_skin");
		lSkin->AddMaterial(IUITheme::Get().GetTextBoxMaterial(EControlMaterialType::Default));
		lSkin->AddMaterial(IUITheme::Get().GetTextBoxMaterial(EControlMaterialType::Disabled));
		lSkin->SetActiveMaterial(EControlMaterialType::Default);

		SetSkin(CControlSkinPtr(lSkin));

		return lResult;
	}

	void CTextBox::PreDraw(CCanvas * pCanvas)
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
				float lfX = (float)miScreenX;
				float lfY = (float)miScreenY;
				float lfWidth = (float)muiWidth;
				float lfHeight = (float)muiHeight;
				float lfCursorPosX = (float)miScreenX + IUITheme::Get().GetTextBoxContentMargin();
				float lfCursorHeight = (float)muiHeight - 6.0f;

				if (!mstrText.empty())
				{					
					CControlFont *lControlFont = lMaterial->GetFont();
					if (lControlFont != nullptr)
					{
						IFont *lFont = lControlFont->GetFont();	
						String lstrText = mstrText.substr(muiStartPosition, muiEndPosition - muiStartPosition);

						SLineParameters lLineParams;
						lFont->CalculateLineParameters(lstrText, lControlFont->GetFontSize(), &lLineParams);
												
						float lfTextWidth = (float)lLineParams.Width;
						float lfContentWidth = lfWidth - 2 * IUITheme::Get().GetTextBoxContentMargin();
						float lfActualWidth = (lfTextWidth > lfContentWidth) ? lfContentWidth : lfTextWidth;

						float lfX1 = lfX + IUITheme::Get().GetTextBoxContentMargin();
						float lfX2 = lfX1 + lfActualWidth;
						float lfY1 = lfY + (lfHeight / 2 - (lLineParams.MinY + lLineParams.MaxY) / 2);
						float lfY2 = lfY + (lfHeight / 2 + (lLineParams.MinY + lLineParams.MaxY) / 2);
						float lfSX1 = lfTextWidth > lfActualWidth ? lfTextWidth - lfActualWidth : 0;
						
						pCanvas->DrawText(CVector2F(lfX1, lfY1), CVector2F(lfSX1, 0.0f), CVector2F(lfX2 - lfX1, lfY2 - lfY1 + 1), lstrText,
							lControlFont->GetFontSize(), lControlFont->GetFontColor(), lControlFont->GetFont());
						
						if (muiPosition > 0)
						{
							lstrText = mstrText.substr(muiStartPosition, muiPosition - muiStartPosition);
							lFont->CalculateLineParameters(lstrText, lControlFont->GetFontSize(), &lLineParams);

							lfCursorPosX += (float)lLineParams.Width;
						}
					}
				}

				mfDeltaTime += (float)IGameTimer::Get().GetDeltaTime();

				if (mfDeltaTime >= 0.5f && mfDeltaTime <= 1.0f)
				{
					pCanvas->DrawRect(CVector2F(lfCursorPosX,
						lfY + 3.0f), CVector2F(1.0f, lfCursorHeight), CColor4F(0.0f, 0.0f, 0.0f));
				}

				if (mfDeltaTime > 1.0f)
					mfDeltaTime = 0.0f;
			}
		}
	}

	void CTextBox::PostDraw(CCanvas * pCanvas)
	{
		CControl::PostDraw(pCanvas);
	}

	bool CTextBox::OnMouseUp(CMouseUpControlEvent * pEvent)
	{
		return false;
	}

	bool CTextBox::OnKeyDown(CKeyDownControlEvent * pEvent)
	{
		uint32_t luiKey = (uint32_t)pEvent->GetKey();

		if (pEvent->GetKey() == EKeyboardKey::LeftShift)
			mbIsShiftDown = true;

		if (luiKey >= 65 && luiKey <= 90)
		{			
			if (mbIsShiftDown)
				luiKey += 32;

			mstrText.insert(muiPosition++, 1, (char)luiKey);
			muiEndPosition++;

			RepositionEnd();
		}

		if (pEvent->GetKey() == EKeyboardKey::Left)
		{
			if (muiPosition > 0)
			{
				if (muiPosition == muiStartPosition)
				{
					if (muiStartPosition > 0)
						muiStartPosition--;
				}

				muiPosition--;
			}

			RepositionStart();
		}
		else if (pEvent->GetKey() == EKeyboardKey::Right)
		{			
			if (muiPosition < mstrText.length())
			{
				if (muiPosition == muiEndPosition)
				{	
					if (muiEndPosition < mstrText.length())					
						muiEndPosition++;					
				}
					
				muiPosition++;
			}

			RepositionEnd();
		}

		return true;
	}

	bool CTextBox::OnKeyUp(CKeyUpControlEvent * pEvent)
	{
		if (pEvent->GetKey() == EKeyboardKey::LeftShift)
			mbIsShiftDown = false;

		return true;
	}

	void CTextBox::RepositionStart()
	{
		CControlMaterial *lMaterial = nullptr;

		if (!mbIsEnabled)
			lMaterial = mSkin->GetMaterial(EControlMaterialType::Disabled);
		else
			lMaterial = mSkin->GetActiveMaterial();

		CControlFont *lControlFont = lMaterial->GetFont();
		if (lControlFont != nullptr)
		{
			mbIsFilled = false;
						
			IFont *lFont = lControlFont->GetFont();
			String lstrText = mstrText.substr(muiStartPosition, muiEndPosition - muiStartPosition);
			uint32_t luiContentWidth = muiWidth - 2 * IUITheme::Get().GetTextBoxContentMargin();

			SLineParameters lLineParams;
			lFont->CalculateLineParameters(lstrText, lControlFont->GetFontSize(), &lLineParams);
						
			while (lLineParams.Width > luiContentWidth)
			{
				muiEndPosition--;
				mbIsFilled = true;

				lstrText = mstrText.substr(muiStartPosition, muiEndPosition - muiStartPosition);
				lFont->CalculateLineParameters(lstrText, lControlFont->GetFontSize(), &lLineParams);
			}

			if (mbIsFilled && lLineParams.Width < luiContentWidth)
				muiEndPosition++;
		}
	}

	void CTextBox::RepositionEnd()
	{
		CControlMaterial *lMaterial = nullptr;

		if (!mbIsEnabled)
			lMaterial = mSkin->GetMaterial(EControlMaterialType::Disabled);
		else
			lMaterial = mSkin->GetActiveMaterial();

		CControlFont *lControlFont = lMaterial->GetFont();
		if (lControlFont != nullptr)
		{
			mbIsFilled = false;

			IFont *lFont = lControlFont->GetFont();
			String lstrText = mstrText.substr(muiStartPosition, muiEndPosition - muiStartPosition);
			uint32_t luiContentWidth = muiWidth - 2 * IUITheme::Get().GetTextBoxContentMargin();

			SLineParameters lLineParams;
			lFont->CalculateLineParameters(lstrText, lControlFont->GetFontSize(), &lLineParams);

			while (lLineParams.Width >= luiContentWidth)
			{
				muiStartPosition++;
				mbIsFilled = true;

				lstrText = mstrText.substr(muiStartPosition, muiEndPosition - muiStartPosition);
				lFont->CalculateLineParameters(lstrText, lControlFont->GetFontSize(), &lLineParams);
			}

			if (mbIsFilled && lLineParams.Width < luiContentWidth)
				muiStartPosition--;
		}
	}
}