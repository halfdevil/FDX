
#include "GUI/Cursor.h"

namespace FDX
{
	CCursor::CCursor(const String &pstrName)
		: CControl(pstrName)
	{
	}

	CCursor::~CCursor()
	{
	}

	void CCursor::SetCursorType(ECursorType pCursorType)
	{
		if (mSkin != nullptr)
		{
			CControlMaterial *lMaterial = mSkin->GetActiveMaterial();
			if (lMaterial != nullptr)
			{
				lMaterial->SetCurrentTexture((uint32_t)pCursorType);
			}
		}
	}

	bool CCursor::OnMouseMove(CMouseMoveControlEvent * pEvent)
	{
		CVector2F lCursorPosition = pEvent->GetAbsolutePosition();

		miScreenX = (uint32_t)lCursorPosition.X;
		miScreenY = (uint32_t)lCursorPosition.Y;

		return true;
	}
}