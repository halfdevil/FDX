
#include "GUI/DefaultUITheme.h"
#include "Core/ResourceManager.h"
#include "Core/FontResource.h"
#include "GUI/Dialog.h"

namespace FDX
{
	CControlMaterialPtr CDefaultUITheme::GetButtonMaterial(EControlMaterialType pMaterialType)
	{
		CControlMaterialPtr lMaterial = std::make_unique<CControlMaterial>(pMaterialType, "default_button");
		CControlBorderPtr lBorder = std::make_unique<CControlBorder>();
		CControlFontPtr lFont = std::make_unique<CControlFont>();

		if (pMaterialType == EControlMaterialType::Default)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(2);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.7f, 0.7f, 0.7f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.7f, 0.7f, 0.7f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Hover)
		{
			lMaterial->SetColor(CColor4F(0.0f, 1.0f, 0.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(2);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.7f, 0.7f, 0.7f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.7f, 0.7f, 0.7f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Pressed)
		{
			lMaterial->SetColor(CColor4F(0.0f, 1.0f, 0.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(2);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.7f, 0.7f, 0.7f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.7f, 0.7f, 0.7f));
		}

		lFont->SetFont("/Assets/Fonts/consola.ttf");
		lFont->SetFontColor(CColor4F(1.0f, 1.0f, 1.0f));
		lFont->SetFontSize(15);

		lMaterial->SetFont(std::move(lFont));
		lMaterial->SetBorder(std::move(lBorder));

		return lMaterial;
	}

	CControlMaterialPtr CDefaultUITheme::GetLabelMaterial(EControlMaterialType pMaterialType)
	{
		CControlMaterialPtr lMaterial = std::make_unique<CControlMaterial>(pMaterialType, "default_button");
		CControlBorderPtr lBorder = std::make_unique<CControlBorder>();
		CControlFontPtr lFont = std::make_unique<CControlFont>();

		if (pMaterialType == EControlMaterialType::Default)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Hover)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Pressed)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		
		lFont->SetFont("/Assets/Fonts/consola.ttf");
		lFont->SetFontColor(CColor4F(1.0f, 1.0f, 1.0f));
		lFont->SetFontSize(15);

		lMaterial->SetFont(std::move(lFont));
		lMaterial->SetBorder(std::move(lBorder));

		return lMaterial;
	}

	CControlMaterialPtr CDefaultUITheme::GetPanelMaterial(EControlMaterialType pMaterialType)
	{
		CControlMaterialPtr lMaterial = std::make_unique<CControlMaterial>(pMaterialType, "default_button");
		CControlBorderPtr lBorder = std::make_unique<CControlBorder>();
		CControlFontPtr lFont = std::make_unique<CControlFont>();

		if (pMaterialType == EControlMaterialType::Default)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Hover)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Pressed)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}

		lFont->SetFont("/Assets/Fonts/consola.ttf");
		lFont->SetFontColor(CColor4F(1.0f, 1.0f, 1.0f));
		lFont->SetFontSize(15);

		lMaterial->SetFont(std::move(lFont));
		lMaterial->SetBorder(std::move(lBorder));

		return lMaterial;
	}

	CControlMaterialPtr CDefaultUITheme::GetImageControlMaterial(EControlMaterialType pMaterialType)
	{
		CControlMaterialPtr lMaterial = std::make_unique<CControlMaterial>(pMaterialType, "default_button");
		CControlBorderPtr lBorder = std::make_unique<CControlBorder>();
		CControlFontPtr lFont = std::make_unique<CControlFont>();

		if (pMaterialType == EControlMaterialType::Default)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Hover)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Pressed)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}

		lFont->SetFont("/Assets/Fonts/consola.ttf");
		lFont->SetFontColor(CColor4F(1.0f, 1.0f, 1.0f));
		lFont->SetFontSize(15);

		lMaterial->SetFont(std::move(lFont));
		lMaterial->SetBorder(std::move(lBorder));

		return lMaterial;
	}

	CControlMaterialPtr CDefaultUITheme::GetMenuItemMaterial(EControlMaterialType pMaterialType)
	{
		CControlMaterialPtr lMaterial = std::make_unique<CControlMaterial>(pMaterialType, "default_button");
		CControlBorderPtr lBorder = std::make_unique<CControlBorder>();
		CControlFontPtr lFont = std::make_unique<CControlFont>();

		if (pMaterialType == EControlMaterialType::Default)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Hover)
		{
			lMaterial->SetColor(CColor4F(0.0f, 1.0f, 0.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Pressed)
		{
			lMaterial->SetColor(CColor4F(0.0f, 1.0f, 0.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}

		lFont->SetFont("/Assets/Fonts/consola.ttf");
		lFont->SetFontColor(CColor4F(1.0f, 1.0f, 1.0f));
		lFont->SetFontSize(15);

		lMaterial->SetFont(std::move(lFont));
		lMaterial->SetBorder(std::move(lBorder));

		return lMaterial;
	}

	CControlMaterialPtr CDefaultUITheme::GetRadioButtonMaterial(EControlMaterialType pMaterialType)
	{
		CControlMaterialPtr lMaterial = std::make_unique<CControlMaterial>(pMaterialType, "radio_button");
		CControlBorderPtr lBorder = std::make_unique<CControlBorder>();
		CControlFontPtr lFont = std::make_unique<CControlFont>();

		if (pMaterialType == EControlMaterialType::Default)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::NoBorder);
		}
		else if (pMaterialType == EControlMaterialType::Hover)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::NoBorder);
		}
		else if (pMaterialType == EControlMaterialType::Pressed)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::NoBorder);
		}

		lFont->SetFont("/Assets/Fonts/consola.ttf");
		lFont->SetFontColor(CColor4F(1.0f, 1.0f, 1.0f));
		lFont->SetFontSize(15);

		lMaterial->SetFont(std::move(lFont));
		lMaterial->SetBorder(std::move(lBorder));

		return lMaterial;
	}

	CControlMaterialPtr CDefaultUITheme::GetRadioButtonImageMaterial(EControlMaterialType pMaterialType)
	{
		CControlMaterialPtr lMaterial = std::make_unique<CControlMaterial>(pMaterialType, "radio_button_image");
		CControlBorderPtr lBorder = std::make_unique<CControlBorder>();
		CControlFontPtr lFont = std::make_unique<CControlFont>();

		if (pMaterialType == EControlMaterialType::Default)
		{
			lMaterial->SetColor(CColor4F(1.0f, 1.0f, 0.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Hover)
		{
			lMaterial->SetColor(CColor4F(0.0f, 1.0f, 0.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Pressed)
		{
			lMaterial->SetColor(CColor4F(1.0f, 0.0f, 0.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Disabled)
		{
			lMaterial->SetColor(CColor4F(0.5f, 0.5f, 0.5f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}

		lFont->SetFont("/Assets/Fonts/consola.ttf");
		lFont->SetFontColor(CColor4F(1.0f, 1.0f, 1.0f));
		lFont->SetFontSize(15);

		lMaterial->SetFont(std::move(lFont));
		lMaterial->SetBorder(std::move(lBorder));

		return lMaterial;
	}

	CControlMaterialPtr CDefaultUITheme::GetRadioButtonGroupMaterial(EControlMaterialType pMaterialType)
	{
		CControlMaterialPtr lMaterial = std::make_unique<CControlMaterial>(pMaterialType, "radio_button_group");
		CControlBorderPtr lBorder = std::make_unique<CControlBorder>();
		CControlFontPtr lFont = std::make_unique<CControlFont>();

		if (pMaterialType == EControlMaterialType::Default)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Hover)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Pressed)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}

		lFont->SetFont("/Assets/Fonts/consola.ttf");
		lFont->SetFontColor(CColor4F(1.0f, 1.0f, 1.0f));
		lFont->SetFontSize(15);

		lMaterial->SetFont(std::move(lFont));
		lMaterial->SetBorder(std::move(lBorder));

		return lMaterial;
	}

	CControlMaterialPtr CDefaultUITheme::GetCheckBoxMaterial(EControlMaterialType pMaterialType)
	{
		CControlMaterialPtr lMaterial = std::make_unique<CControlMaterial>(pMaterialType, "checkbox");
		CControlBorderPtr lBorder = std::make_unique<CControlBorder>();
		CControlFontPtr lFont = std::make_unique<CControlFont>();

		if (pMaterialType == EControlMaterialType::Default)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Hover)
		{
			lMaterial->SetColor(CColor4F(0.0f, 1.0f, 0.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Pressed)
		{
			lMaterial->SetColor(CColor4F(0.0f, 1.0f, 0.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}

		lFont->SetFont("/Assets/Fonts/consola.ttf");
		lFont->SetFontColor(CColor4F(1.0f, 1.0f, 1.0f));
		lFont->SetFontSize(15);

		lMaterial->SetFont(std::move(lFont));
		lMaterial->SetBorder(std::move(lBorder));

		return lMaterial;
	}

	CControlMaterialPtr CDefaultUITheme::GetCheckBoxImageMaterial(EControlMaterialType pMaterialType)
	{
		CControlMaterialPtr lMaterial = std::make_unique<CControlMaterial>(pMaterialType, "checkbox_image");
		CControlBorderPtr lBorder = std::make_unique<CControlBorder>();
		CControlFontPtr lFont = std::make_unique<CControlFont>();

		if (pMaterialType == EControlMaterialType::Default)
		{
			lMaterial->SetColor(CColor4F(1.0f, 1.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Hover)
		{
			lMaterial->SetColor(CColor4F(0.0f, 1.0f, 0.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Pressed)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Disabled)
		{
			lMaterial->SetColor(CColor4F(0.5f, 0.5f, 0.5f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}

		lFont->SetFont("/Assets/Fonts/consola.ttf");
		lFont->SetFontColor(CColor4F(1.0f, 1.0f, 1.0f));
		lFont->SetFontSize(15);

		lMaterial->SetFont(std::move(lFont));
		lMaterial->SetBorder(std::move(lBorder));

		return lMaterial;
	}

	CControlMaterialPtr CDefaultUITheme::GetTextBoxMaterial(EControlMaterialType pMaterialType)
	{
		CControlMaterialPtr lMaterial = std::make_unique<CControlMaterial>(pMaterialType, "checkbox");
		CControlBorderPtr lBorder = std::make_unique<CControlBorder>();
		CControlFontPtr lFont = std::make_unique<CControlFont>();

		if (pMaterialType == EControlMaterialType::Default)
		{
			lMaterial->SetColor(CColor4F(1.0f, 1.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Hover)
		{
			lMaterial->SetColor(CColor4F(1.0f, 1.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Pressed)
		{
			lMaterial->SetColor(CColor4F(1.0f, 1.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}

		lFont->SetFont("/Assets/Fonts/consola.ttf");
		lFont->SetFontColor(CColor4F(0.0f, 0.0f, 0.0f));
		lFont->SetFontSize(15);

		lMaterial->SetFont(std::move(lFont));
		lMaterial->SetBorder(std::move(lBorder));

		return lMaterial;
	}

	CControlMaterialPtr CDefaultUITheme::GetDialogMaterial(EControlMaterialType pMaterialType)
	{
		CControlMaterialPtr lMaterial = std::make_unique<CControlMaterial>(pMaterialType, "default_button");
		CControlBorderPtr lBorder = std::make_unique<CControlBorder>();
		CControlFontPtr lFont = std::make_unique<CControlFont>();

		if (pMaterialType == EControlMaterialType::Default)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Hover)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Pressed)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}

		lFont->SetFont("/Assets/Fonts/consola.ttf");
		lFont->SetFontColor(CColor4F(1.0f, 1.0f, 1.0f));
		lFont->SetFontSize(15);

		lMaterial->SetFont(std::move(lFont));
		lMaterial->SetBorder(std::move(lBorder));

		return lMaterial;
	}

	CControlMaterialPtr CDefaultUITheme::GetDialogTitleBarMaterial(EControlMaterialType pMaterialType)
	{
		CControlMaterialPtr lMaterial = std::make_unique<CControlMaterial>(pMaterialType, "default_button");
		CControlBorderPtr lBorder = std::make_unique<CControlBorder>();
		CControlFontPtr lFont = std::make_unique<CControlFont>();

		if (pMaterialType == EControlMaterialType::Default)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Hover)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Pressed)
		{
			lMaterial->SetColor(CColor4F(0.0f, 0.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}

		lFont->SetFont("/Assets/Fonts/consola.ttf");
		lFont->SetFontColor(CColor4F(1.0f, 1.0f, 1.0f));
		lFont->SetFontSize(15);

		lMaterial->SetFont(std::move(lFont));
		lMaterial->SetBorder(std::move(lBorder));

		return lMaterial;
	}

	CControlMaterialPtr CDefaultUITheme::GetDialogContentMaterial(EControlMaterialType pMaterialType)
	{
		CControlMaterialPtr lMaterial = std::make_unique<CControlMaterial>(pMaterialType, "default_button");
		CControlBorderPtr lBorder = std::make_unique<CControlBorder>();
		CControlFontPtr lFont = std::make_unique<CControlFont>();

		if (pMaterialType == EControlMaterialType::Default)
		{
			lMaterial->SetColor(CColor4F(1.0f, 1.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Hover)
		{
			lMaterial->SetColor(CColor4F(1.0f, 1.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Pressed)
		{
			lMaterial->SetColor(CColor4F(1.0f, 1.0f, 1.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}

		lFont->SetFont("/Assets/Fonts/consola.ttf");
		lFont->SetFontColor(CColor4F(0.0f, 0.0f, 0.0f));
		lFont->SetFontSize(15);

		lMaterial->SetFont(std::move(lFont));
		lMaterial->SetBorder(std::move(lBorder));

		return lMaterial;
	}

	CControlMaterialPtr CDefaultUITheme::GetDialogTitleBarLogoMaterial(EControlMaterialType pMaterialType)
	{
		CControlMaterialPtr lMaterial = std::make_unique<CControlMaterial>(pMaterialType, "default_button");
		CControlBorderPtr lBorder = std::make_unique<CControlBorder>();
		CControlFontPtr lFont = std::make_unique<CControlFont>();

		if (pMaterialType == EControlMaterialType::Default)
		{
			lMaterial->SetColor(CColor4F(0.8f, 0.8f, 0.8f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Hover)
		{
			lMaterial->SetColor(CColor4F(0.8f, 0.8f, 0.8f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Pressed)
		{
			lMaterial->SetColor(CColor4F(0.8f, 0.8f, 0.8f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}

		lFont->SetFont("/Assets/Fonts/consola.ttf");
		lFont->SetFontColor(CColor4F(0.0f, 0.0f, 0.0f));
		lFont->SetFontSize(15);

		lMaterial->SetFont(std::move(lFont));
		lMaterial->SetBorder(std::move(lBorder));

		return lMaterial;
	}

	CControlMaterialPtr CDefaultUITheme::GetDialogTitleBarMinimizeBoxMaterial(EControlMaterialType pMaterialType)
	{
		CControlMaterialPtr lMaterial = std::make_unique<CControlMaterial>(pMaterialType, "default_button");
		CControlBorderPtr lBorder = std::make_unique<CControlBorder>();
		CControlFontPtr lFont = std::make_unique<CControlFont>();

		if (pMaterialType == EControlMaterialType::Default)
		{
			lMaterial->SetColor(CColor4F(1.0f, 1.0f, 0.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Hover)
		{
			lMaterial->SetColor(CColor4F(1.0f, 1.0f, 0.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Pressed)
		{
			lMaterial->SetColor(CColor4F(1.0f, 1.0f, 0.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}

		lFont->SetFont("/Assets/Fonts/consola.ttf");
		lFont->SetFontColor(CColor4F(1.0f, 1.0f, 1.0f));
		lFont->SetFontSize(15);

		lMaterial->SetFont(std::move(lFont));
		lMaterial->SetBorder(std::move(lBorder));

		return lMaterial;
	}

	CControlMaterialPtr CDefaultUITheme::GetDialogTitleBarMaximizeBoxMaterial(EControlMaterialType pMaterialType)
	{
		CControlMaterialPtr lMaterial = std::make_unique<CControlMaterial>(pMaterialType, "default_button");
		CControlBorderPtr lBorder = std::make_unique<CControlBorder>();
		CControlFontPtr lFont = std::make_unique<CControlFont>();

		if (pMaterialType == EControlMaterialType::Default)
		{
			lMaterial->SetColor(CColor4F(0.0f, 1.0f, 0.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Hover)
		{
			lMaterial->SetColor(CColor4F(0.0f, 1.0f, 0.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Pressed)
		{
			lMaterial->SetColor(CColor4F(0.0f, 1.0f, 0.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}

		lFont->SetFont("/Assets/Fonts/consola.ttf");
		lFont->SetFontColor(CColor4F(1.0f, 1.0f, 1.0f));
		lFont->SetFontSize(15);

		lMaterial->SetFont(std::move(lFont));
		lMaterial->SetBorder(std::move(lBorder));

		return lMaterial;
	}

	CControlMaterialPtr CDefaultUITheme::GetDialogTitleBarCloseBoxMaterial(EControlMaterialType pMaterialType)
	{
		CControlMaterialPtr lMaterial = std::make_unique<CControlMaterial>(pMaterialType, "default_button");
		CControlBorderPtr lBorder = std::make_unique<CControlBorder>();
		CControlFontPtr lFont = std::make_unique<CControlFont>();

		if (pMaterialType == EControlMaterialType::Default)
		{
			lMaterial->SetColor(CColor4F(1.0f, 0.0f, 0.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Hover)
		{
			lMaterial->SetColor(CColor4F(1.0f, 0.0f, 0.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}
		else if (pMaterialType == EControlMaterialType::Pressed)
		{
			lMaterial->SetColor(CColor4F(1.0f, 0.0f, 0.0f));
			lBorder->SetBorderType(EBorderType::AllBorder);
			lBorder->SetBorderWidth(1);
			lBorder->SetBorderColor(EBorderType::Top, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Left, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Bottom, CColor4F(0.0f, 0.0f, 0.0f));
			lBorder->SetBorderColor(EBorderType::Right, CColor4F(0.0f, 0.0f, 0.0f));
		}

		lFont->SetFont("/Assets/Fonts/consola.ttf");
		lFont->SetFontColor(CColor4F(1.0f, 1.0f, 1.0f));
		lFont->SetFontSize(15);

		lMaterial->SetFont(std::move(lFont));
		lMaterial->SetBorder(std::move(lBorder));

		return lMaterial;
	}

	uint32_t CDefaultUITheme::GetRadioButtonImageWidth()
	{
		return 16;
	}

	uint32_t CDefaultUITheme::GetRadioButtonImageHeight()
	{
		return 16;
	}

	uint32_t CDefaultUITheme::GetRadioButtonImageMargin()
	{
		return 2;
	}

	uint32_t CDefaultUITheme::GetCheckBoxImageWidth()
	{
		return 16;
	}

	uint32_t CDefaultUITheme::GetCheckBoxImageHeight()
	{
		return 16;
	}

	uint32_t CDefaultUITheme::GetCheckBoxImageMargin()
	{
		return 2;
	}

	uint32_t CDefaultUITheme::GetTextBoxContentMargin()
	{
		return 2;
	}

	uint32_t CDefaultUITheme::GetDialogTitleBarStyle()
	{
		return EDialogTitleBarStyle::Logo | EDialogTitleBarStyle::MinimizeBox |
			EDialogTitleBarStyle::MaximizeBox | EDialogTitleBarStyle::CloseBox;
	}

	uint32_t CDefaultUITheme::GetDialogTitleBarHeight()
	{
		return 28;
	}

	uint32_t CDefaultUITheme::GetDialogTitleBarLogoWidth()
	{
		return 22;
	}

	uint32_t CDefaultUITheme::GetDialogTitleBarLogoHeight()
	{
		return 22;
	}

	uint32_t CDefaultUITheme::GetDialogTitleBarLogoMargin()
	{
		return 2;
	}

	uint32_t CDefaultUITheme::GetDialogTitleBarMinimizeBoxWidth()
	{
		return 22;
	}

	uint32_t CDefaultUITheme::GetDialogTitleBarMinimizeBoxHeight()
	{
		return 22;
	}

	uint32_t CDefaultUITheme::GetDialogTitleBarMinimizeBoxMargin()
	{
		return 2;
	}

	uint32_t CDefaultUITheme::GetDialogTitleBarMaximizeBoxWidth()
	{
		return 22;
	}

	uint32_t CDefaultUITheme::GetDialogTitleBarMaximizeBoxHeight()
	{
		return 22;
	}

	uint32_t CDefaultUITheme::GetDialogTitleBarMaximizeBoxMargin()
	{
		return 2;
	}

	uint32_t CDefaultUITheme::GetDialogTitleBarCloseBoxWidth()
	{
		return 22;
	}

	uint32_t CDefaultUITheme::GetDialogTitleBarCloseBoxHeight()
	{
		return 22;
	}

	uint32_t CDefaultUITheme::GetDialogTitleBarCloseBoxMargin()
	{
		return 2;
	}
}