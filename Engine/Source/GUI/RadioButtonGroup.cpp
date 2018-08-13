
#include "GUI/RadioButtonGroup.h"
#include "GUI/UITheme.h"

namespace FDX
{
	CRadioButtonGroup::CRadioButtonGroup(const String & pstrName)
		: CControl(pstrName)
	{
	}

	CRadioButtonGroup::~CRadioButtonGroup()
	{
	}

	EResult CRadioButtonGroup::Create(CControlContainer * pContainer)
	{
		EResult lResult = EResult::Success;

		CControlSkin *lSkin = new CControlSkin(mstrName + "_skin");
		lSkin->AddMaterial(IUITheme::Get().GetRadioButtonGroupMaterial(EControlMaterialType::Default));
		lSkin->AddMaterial(IUITheme::Get().GetRadioButtonGroupMaterial(EControlMaterialType::Disabled));
		lSkin->SetActiveMaterial(EControlMaterialType::Default);

		SetSkin(CControlSkinPtr(lSkin));

		return lResult;
	}

	void CRadioButtonGroup::AddButton(CRadioButton * pButton)
	{
		mButtons.push_back(pButton);
	}

	bool CRadioButtonGroup::OnMouseUp(CMouseUpControlEvent * pEvent)
	{
		int32_t liCheckedIdx = -1;

		for (uint32_t lIdx = 0; lIdx < mButtons.size(); lIdx++)
		{
			if (!mButtons[lIdx]->IsChecked() && mButtons[lIdx]->IsMouseOverImage())
			{
				liCheckedIdx = (int32_t)lIdx;
				break;
			}
		}

		if (liCheckedIdx != -1)
		{
			for (uint32_t lIdx = 0; lIdx < mButtons.size(); lIdx++)
			{
				if (liCheckedIdx == (int32_t)lIdx)
				{
					mButtons[lIdx]->SetIsChecked(true);
				}
				else
				{
					mButtons[lIdx]->SetIsChecked(false);
				}
			}
		}

		return true;
	}
}
