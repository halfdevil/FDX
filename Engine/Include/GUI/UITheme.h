#pragma once

#include "GUI/ControlSkin.h"
#include "GUI/ControlMaterial.h"
#include "GUI/ControlFont.h"
#include "GUI/ControlBorder.h"
#include "Core/Singleton.h"

namespace FDX
{
	class FDX_EXPORT IUITheme : public CSingleton<IUITheme>
	{
	public:

		virtual ~IUITheme()
		{
		}

		virtual CControlMaterialPtr GetButtonMaterial(EControlMaterialType pMaterialType) = 0;
		virtual CControlMaterialPtr GetLabelMaterial(EControlMaterialType pMaterialType) = 0;
		virtual CControlMaterialPtr GetPanelMaterial(EControlMaterialType pMaterialType) = 0;
		virtual CControlMaterialPtr GetImageControlMaterial(EControlMaterialType pMaterialType) = 0;
		virtual CControlMaterialPtr GetMenuItemMaterial(EControlMaterialType pMaterialType) = 0;
		virtual CControlMaterialPtr GetRadioButtonMaterial(EControlMaterialType pMaterialType) = 0;
		virtual CControlMaterialPtr GetRadioButtonImageMaterial(EControlMaterialType pMaterialType) = 0;
		virtual CControlMaterialPtr GetRadioButtonGroupMaterial(EControlMaterialType pMaterialType) = 0;
		virtual CControlMaterialPtr GetCheckBoxMaterial(EControlMaterialType pMaterialType) = 0;
		virtual CControlMaterialPtr GetCheckBoxImageMaterial(EControlMaterialType pMaterialType) = 0;
		virtual CControlMaterialPtr GetTextBoxMaterial(EControlMaterialType pMaterialType) = 0;
		virtual CControlMaterialPtr GetDialogMaterial(EControlMaterialType pMaterialType) = 0;
		virtual CControlMaterialPtr GetDialogTitleBarMaterial(EControlMaterialType pMaterialType) = 0;
		virtual CControlMaterialPtr GetDialogContentMaterial(EControlMaterialType pMaterialType) = 0;
		virtual CControlMaterialPtr GetDialogTitleBarLogoMaterial(EControlMaterialType pMaterialType) = 0;
		virtual CControlMaterialPtr GetDialogTitleBarMinimizeBoxMaterial(EControlMaterialType pMaterialType) = 0;
		virtual CControlMaterialPtr GetDialogTitleBarMaximizeBoxMaterial(EControlMaterialType pMaterialType) = 0;
		virtual CControlMaterialPtr GetDialogTitleBarCloseBoxMaterial(EControlMaterialType pMaterialType) = 0;
		
		virtual uint32_t GetRadioButtonImageWidth() = 0;
		virtual uint32_t GetRadioButtonImageHeight() = 0;
		virtual uint32_t GetRadioButtonImageMargin() = 0;
		virtual uint32_t GetCheckBoxImageWidth() = 0;
		virtual uint32_t GetCheckBoxImageHeight() = 0;
		virtual uint32_t GetCheckBoxImageMargin() = 0;
		virtual uint32_t GetTextBoxContentMargin() = 0;
		virtual uint32_t GetDialogTitleBarStyle() = 0;
		virtual uint32_t GetDialogTitleBarHeight() = 0;
		virtual uint32_t GetDialogTitleBarLogoWidth() = 0;
		virtual uint32_t GetDialogTitleBarLogoHeight() = 0;
		virtual uint32_t GetDialogTitleBarLogoMargin() = 0;
		virtual uint32_t GetDialogTitleBarMinimizeBoxWidth() = 0;
		virtual uint32_t GetDialogTitleBarMinimizeBoxHeight() = 0;
		virtual uint32_t GetDialogTitleBarMinimizeBoxMargin() = 0;
		virtual uint32_t GetDialogTitleBarMaximizeBoxWidth() = 0;
		virtual uint32_t GetDialogTitleBarMaximizeBoxHeight() = 0;
		virtual uint32_t GetDialogTitleBarMaximizeBoxMargin() = 0;
		virtual uint32_t GetDialogTitleBarCloseBoxWidth() = 0;
		virtual uint32_t GetDialogTitleBarCloseBoxHeight() = 0;
		virtual uint32_t GetDialogTitleBarCloseBoxMargin() = 0;
	};

	using IUIThemePtr = SmartPtr<IUITheme>;
}