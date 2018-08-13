#pragma once

#include "GUI/UITheme.h"

namespace FDX
{
	class FDX_EXPORT CDefaultUITheme : public IUITheme
	{
	public:

		virtual CControlMaterialPtr GetButtonMaterial(EControlMaterialType pMaterialType);
		virtual CControlMaterialPtr GetLabelMaterial(EControlMaterialType pMaterialType);
		virtual CControlMaterialPtr GetPanelMaterial(EControlMaterialType pMaterialType);
		virtual CControlMaterialPtr GetImageControlMaterial(EControlMaterialType pMaterialType);
		virtual CControlMaterialPtr GetMenuItemMaterial(EControlMaterialType pMaterialType);
		virtual CControlMaterialPtr GetRadioButtonMaterial(EControlMaterialType pMaterialType);
		virtual CControlMaterialPtr GetRadioButtonImageMaterial(EControlMaterialType pMaterialType);
		virtual CControlMaterialPtr GetRadioButtonGroupMaterial(EControlMaterialType pMaterialType);
		virtual CControlMaterialPtr GetCheckBoxMaterial(EControlMaterialType pMaterialType);
		virtual CControlMaterialPtr GetCheckBoxImageMaterial(EControlMaterialType pMaterialType);
		virtual CControlMaterialPtr GetTextBoxMaterial(EControlMaterialType pMaterialType);
		virtual CControlMaterialPtr GetDialogMaterial(EControlMaterialType pMaterialType);
		virtual CControlMaterialPtr GetDialogTitleBarMaterial(EControlMaterialType pMaterialType);
		virtual CControlMaterialPtr GetDialogContentMaterial(EControlMaterialType pMaterialType);
		virtual CControlMaterialPtr GetDialogTitleBarLogoMaterial(EControlMaterialType pMaterialType);
		virtual CControlMaterialPtr GetDialogTitleBarMinimizeBoxMaterial(EControlMaterialType pMaterialType);
		virtual CControlMaterialPtr GetDialogTitleBarMaximizeBoxMaterial(EControlMaterialType pMaterialType);
		virtual CControlMaterialPtr GetDialogTitleBarCloseBoxMaterial(EControlMaterialType pMaterialType);

		virtual uint32_t GetRadioButtonImageWidth();
		virtual uint32_t GetRadioButtonImageHeight();
		virtual uint32_t GetRadioButtonImageMargin();
		virtual uint32_t GetCheckBoxImageWidth();
		virtual uint32_t GetCheckBoxImageHeight();
		virtual uint32_t GetCheckBoxImageMargin();
		virtual uint32_t GetTextBoxContentMargin();
		virtual uint32_t GetDialogTitleBarStyle();
		virtual uint32_t GetDialogTitleBarHeight();
		virtual uint32_t GetDialogTitleBarLogoWidth();
		virtual uint32_t GetDialogTitleBarLogoHeight();
		virtual uint32_t GetDialogTitleBarLogoMargin();
		virtual uint32_t GetDialogTitleBarMinimizeBoxWidth();
		virtual uint32_t GetDialogTitleBarMinimizeBoxHeight();
		virtual uint32_t GetDialogTitleBarMinimizeBoxMargin();
		virtual uint32_t GetDialogTitleBarMaximizeBoxWidth();
		virtual uint32_t GetDialogTitleBarMaximizeBoxHeight();
		virtual uint32_t GetDialogTitleBarMaximizeBoxMargin();
		virtual uint32_t GetDialogTitleBarCloseBoxWidth();
		virtual uint32_t GetDialogTitleBarCloseBoxHeight();
		virtual uint32_t GetDialogTitleBarCloseBoxMargin();
	};
}