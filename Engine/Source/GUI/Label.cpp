
#include "GUI/Label.h"

namespace FDX
{
	CLabel::CLabel(const String &pstrName)
		: CControl(pstrName)
	{
	}

	CLabel::~CLabel()
	{
	}

	void CLabel::SetText(const String & pstrText)
	{
		mstrText = pstrText;
	}

	void CLabel::PreDraw(CCanvas * pCanvas)
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
	}

	void CLabel::PostDraw(CCanvas * pCanvas)
	{
		CControl::PostDraw(pCanvas);
	}
}