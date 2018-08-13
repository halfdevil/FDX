
#include "GUI/Button.h"
#include "GUI/UIManager.h"
#include "GUI/ControlSkin.h"

namespace FDX
{	
	CButton::CButton(const String &pstrName)
		: CLabel(pstrName)
	{
	}

	CButton::~CButton()
	{
	}

	bool CButton::OnMouseEnter(CMouseEnterControlEvent * pEvent)
	{
		mSkin->SetActiveMaterial(EControlMaterialType::Hover);
		return true;
	}

	bool CButton::OnMouseLeave(CMouseLeaveControlEvent * pEvent)
	{
		mSkin->SetActiveMaterial(EControlMaterialType::Default);
		return true;
	}

	bool CButton::OnMouseDown(CMouseDownControlEvent * pEvent)
	{
		if (pEvent->GetButton() == EMouseButton::Left)		
			mSkin->SetActiveMaterial(EControlMaterialType::Pressed);		

		return true;
	}

	bool CButton::OnMouseUp(CMouseUpControlEvent * pEvent)
	{
		mSkin->SetActiveMaterial(EControlMaterialType::Hover);
		return false;
	}
}
