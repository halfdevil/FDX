
#include "GUI/ImageControl.h"

namespace FDX
{
	CImageControl::CImageControl(const String &pstrName)
		: CControl(pstrName)
	{
	}

	CImageControl::~CImageControl()
	{
	}

	void CImageControl::SetTexture(CTexture2D * pTexture)
	{		
		CControlMaterial *lMaterial = mSkin->GetActiveMaterial();

		if (lMaterial != nullptr)
		{
			lMaterial->AddTexture(pTexture);
			lMaterial->SetCurrentTexture(0);
		}
	}
}